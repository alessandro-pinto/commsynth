/*
 Copyright (c) 2007-2008 The Regents of the University of California.
 All rights reserved.

 Permission is hereby granted, without written agreement and without
 license or royalty fees, to use, copy, modify, and distribute this
 software and its documentation for any purpose, provided that the
 above copyright notice and the following two paragraphs appear in all
 copies of this software and that appropriate acknowledgments are made
 to the research of the COSI group.

 IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO ANY PARTY
 FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
 ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF
 THE UNIVERSITY OF CALIFORNIA HAS BEEN ADVISED OF THE POSSIBILITY OF
 SUCH DAMAGE.

 THE UNIVERSITY OF CALIFORNIA SPECIFICALLY DISCLAIMS ANY WARRANTIES,
 INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. THE SOFTWARE
 PROVIDED HEREUNDER IS ON AN "AS IS" BASIS, AND THE UNIVERSITY OF
 CALIFORNIA HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
 ENHANCEMENTS, OR MODIFICATIONS.

 Author : Alessandro Pinto <apinto@eecs.berkeley.edu>
 University of California, Berkeley
 545 Cory Hall, Berkeley, CA 94720
 */


#include "ChainGenerator.h"

namespace cosi {

namespace algorithms {

namespace wiredbas {

ChainGenerator::ChainGenerator() {

}

void ChainGenerator::Initialize(
		cosi::commstruct::wiredbas::Specification* pSpec,
		cosi::platforms::wiredbas::BusPlatform* pPlt) {
	mSpecification = pSpec;
	mPlt = pPlt;
}

void ChainGenerator::Run() {

	std::cout << "(A) -- Running the chain generation algorithm (cg)..." << endl ;

	std::list<Position> mBusRouterPosition = mPlt->GetGatewayPosition();

	//build complete graph with length on the edges
	std::cout << "(cg) Generating complete wiring..." << std::endl ;
	cosi::commstruct::wiredbas::Implementation
			* CompleteWiring = GenerateCompleteWiring();
	//Abstract the wiring information to an undirected graph with
	//real weights. The weights are equal to the distance.
	std::cout << "(cg) Generating complete distance..." << std::endl ;
	cosi::commstruct::graph::U_RealWeightedGraph
			* CompleteDistance = GenerateCompleteDistance(CompleteWiring);

	std::list<Position>::iterator R;
	int RouterId = mSpecification->GetLastId() + 1;
	for (R = mBusRouterPosition.begin(); R != mBusRouterPosition.end(); R++) {
		std::cout << "(cg) Adding router : " << RouterId << " in position " << *R << std::endl ;
		AddRouter(RouterId , *R, CompleteWiring, CompleteDistance);
		mRouters.insert(RouterId) ;
		std::cout << "(cg) Finding tour ... " << std::endl ;
		std::list<int> Tour = FindTour(CompleteDistance);
		std::cout << "(cg) Generating chains ... " << std::endl ;
		GenerateChains(Tour, RouterId, *R, CompleteWiring);
		std::cout << "(cg) Removing router ... " << std::endl ;
		RemoveRouter(RouterId, CompleteWiring);
		RemoveRouter(RouterId, CompleteDistance);
		RouterId++;
	}
	delete CompleteDistance;
	delete CompleteWiring;
	std::cout << "(A) -- Chain generation done" << endl ;
}

cosi::commstruct::wiredbas::Implementation* ChainGenerator::GenerateCompleteWiring() {
	cosi::commstruct::wiredbas::Implementation
			* CompleteWiring = new cosi::commstruct::wiredbas::Implementation("");
	IdGraph::v_iterator U, V;
	//For all end-to-end requirements in the specification
	for (U = mSpecification->v_begin(); U != mSpecification->v_end(); U++) {
		for (V = mSpecification->v_begin(); V != mSpecification->v_end(); V++) {
			//Add the source vertex to the commstruct and set its position
			if ( !CompleteWiring->InV(*U)) {
				CompleteWiring->AddVertex(*U);
				(*CompleteWiring)[*U].PRJ(Position)Set((*mSpecification)[*U].PRJ(Position)Get());
			}
			//Add the destination vertex to the commstruct and set its position
			if ( !CompleteWiring->InV(*V)) {
				CompleteWiring->AddVertex(*V);
				(*CompleteWiring)[*V].PRJ(Position)Set((*mSpecification)[*V].PRJ(Position)Get());
			}
			//Add the edge between the two and set the wire layout
			if ( !CompleteWiring->InE(*U, *V)) {
				WiringPath Path;
				Path = mPlt->WireLayout((*CompleteWiring)[*U].PRJ(Position)Get(), (*CompleteWiring)[*V].PRJ(Position)Get());
				CompleteWiring->AddEdge(*U, *V);
				(*CompleteWiring)[Edge(*U,*V)].PRJ(WiringPath)Set(Path);
			}
		}
	}
	return CompleteWiring;
}

cosi::commstruct::graph::U_RealWeightedGraph* ChainGenerator::GenerateCompleteDistance(
		cosi::commstruct::wiredbas::Implementation* pW) {
	cosi::commstruct::graph::U_RealWeightedGraph
			* CompleteDistance = new cosi::commstruct::graph::U_RealWeightedGraph;
	IdGraph::v_iterator U, V;
	for (U = mSpecification->v_begin(); U != mSpecification->v_end(); U++) {
		for (V = mSpecification->v_begin(); V != mSpecification->v_end(); V++) {
			if ( !CompleteDistance->InV(*U)) {
				CompleteDistance->AddVertex(*U);
			}
			if ( !CompleteDistance->InV(*V)) {
				CompleteDistance->AddVertex(*V);
			}
			if ( !CompleteDistance->InE(*U, *V)) {
				CompleteDistance->AddEdge(*U, *V);
				(*CompleteDistance)[Edge(*U,*V)].PRJ(RealQuantity)SetValue((*pW)[Edge(*U,*V)].PRJ(WiringPath)Length());
			}
		}
	}
	return CompleteDistance;
}

void ChainGenerator::AddRouter(int RouterId , Position pP,
		cosi::commstruct::wiredbas::Implementation* pW,
		cosi::commstruct::graph::U_RealWeightedGraph* pD) {

	//Add the router to both the wiring and and distance graphs
	pW->AddVertex(RouterId);
	pD->AddVertex(RouterId);
	(*pW)[RouterId].PRJ(Position)Set(pP);

	//Add the wiring information to all the links
	//between the router and the other nodes.
	IdGraph::v_iterator U;
	for (U = pW->v_begin(); U != pW->v_end(); U ++) {
		if ( *U != RouterId) {
			WiringPath Path;
			Path = mPlt->WireLayout((*pW)[RouterId].PRJ(Position)Get(), (*pW)[*U].PRJ(Position)Get());
			pW->AddEdge(RouterId, *U);
			pW->AddEdge(*U, RouterId);
			pD->AddEdge(RouterId, *U);
			(*pW)[Edge(RouterId,*U)].PRJ(WiringPath)Set(Path);
			(*pW)[Edge(*U,RouterId)].PRJ(WiringPath)Set(Path);
			(*pD)[Edge(RouterId,*U)].PRJ(RealQuantity)SetValue(Path.Length());
		}
	}
}

std::list<int> ChainGenerator::FindTour(
		cosi::commstruct::graph::U_RealWeightedGraph* pD) {
	//call tsp
	cosi::algorithms::graph::Tsp* T = new cosi::algorithms::graph::Tsp(pD);
	T->Run();
	//get the tour
	std::list<int> Tour = T->Get();
	delete T;
	return Tour;
}

void ChainGenerator::GenerateChains(std::list<int> pTour, int pRouterId,
		Position P, cosi::commstruct::wiredbas::Implementation* pW) {

	//std::copy(pTour.begin(),pTour.end(),std::ostream_iterator<int>(cerr," ")) ;
	//std::cerr << std::endl ;


	//Tit pointer to the nodes in the tour
	std::list<int>::iterator Tit ;
	//T contains the tour in a vector data structure
	std::vector<int> T(pTour.size()) ;
	//RouterIndex is the index of the vector element that contains the router
	int RouterIndex;
	int Index = 0 ;
	cout << "Tour : " ;
	for( Tit = pTour.begin() ; Tit != pTour.end() ; Tit++) {
		T[Index] = *Tit ;
		cout << *Tit << " " ;
		if ( T[Index] == pRouterId ) {
			RouterIndex = Index ;
		}
		Index++ ;
	}
	cout <<endl ;
	//Length containts the number of element in the chain
	int Length = 1;
	bool ChainFound = true;
	while (ChainFound && ( Length < T.size() ) ) {
		ChainFound = false;
		//This loop find all the chains containing the router and that
		//extend for a maximum of Length nodes.
		for (int Left = Length; Left >=0; Left--) {
			//SubTour contains the nodes of part of the tour of length Length
			std::list<int> SubTour ;
			//cout << "SubTour : " ;
			for(Index = RouterIndex-Left ; Index <= RouterIndex+Length-Left; Index++ ) {
				if ( Index < 0 ) {
					SubTour.push_back(T[T.size() + Index ]) ;
					//cout << T[T.size() + Index ] << " " ;
				} else if ( Index >= T.size() ) {
					SubTour.push_back(T[Index - T.size()]) ;
					//cout << T[Index - T.size()] << " " ;
				} else {
					SubTour.push_back(T[Index]) ;
					//cout << T[Index] << " " ;
				}

			} ;
			//cout <<endl ;
			//std::copy(SubTour.begin(),SubTour.end(),std::ostream_iterator<int>(cerr," ")) ;
			//std::cerr << std::endl ;
			//Generates a bus for this tour
			cosi::commstruct::wiredbas::Implementation
					* Chain = GenerateBusInstance(SubTour, pRouterId, P , pW);
			//std::cout << Chain->Print() <<std::endl ;
			if (mPlt->Satisfies("Bus",Chain) ) {
				if (CheckLatency(Chain) ) {
					//std::cout << "Bus order " << Chain->Order() <<std::endl ;
					mChains.push_back(Chain);
					std::cout << "\r Number of chains : " << mChains.size() << "\t" ;
					ChainFound = true;
				}
			}
		}
		Length++ ;
	}
	cout << endl ;
}

cosi::commstruct::wiredbas::Implementation* ChainGenerator::GenerateBusInstance(
		std::list<int> pSubTour, int RouterId, Position P,
		cosi::commstruct::wiredbas::Implementation* pW) {

	cosi::commstruct::wiredbas::Implementation
			* RetVal = new cosi::commstruct::wiredbas::Implementation("");
	cosi::commstruct::wiredbas::Implementation* Node;
	cosi::commstruct::wiredbas::Implementation* Link;

	std::list<int>::iterator CurrentNode, NextNode;
	CurrentNode = pSubTour.begin();

	if (*CurrentNode == RouterId ) {
		Node = mPlt->GetImplementationInstance("Router", RouterId, P) ;
	} else {
		Type T = (*mSpecification)[*CurrentNode].PRJ(Type)Get() ;
		//cout << "Node " << *CurrentNode << " " << T << endl ;
		Position P = (*mSpecification)[*CurrentNode].PRJ(Position)Get() ;
		ThreadSet Th = (*mSpecification)[*CurrentNode].PRJ(ThreadSet)Get() ;
		Node = mPlt->GetImplementationInstance(T.GetValue(), *CurrentNode, P, Th) ;
	}

	(*RetVal) = (*RetVal) + (*Node);

	delete Node;

	NextNode = pSubTour.begin() ;
	NextNode ++;

	while (NextNode != pSubTour.end() ) {
		if (*NextNode == RouterId ) {
			Node = mPlt->GetImplementationInstance("Router", RouterId, P) ;
		} else {
			Type T = (*mSpecification)[*NextNode].PRJ(Type)Get() ;
			//cout << "Node " << *NextNode << " " << T << endl ;
			Position P = (*mSpecification)[*NextNode].PRJ(Position)Get() ;
			ThreadSet Th = (*mSpecification)[*NextNode].PRJ(ThreadSet)Get() ;
			Node = mPlt->GetImplementationInstance(T.GetValue(), *NextNode, P, Th) ;
		}
		(*RetVal) = (*RetVal) + (*Node);
		delete Node;
		//link
		WiringPath Path;
		Path = (*pW)[Edge(*CurrentNode,*NextNode)].PRJ(WiringPath)Get() ;
		Link = mPlt->GetImplementationInstance("Wire", *CurrentNode, "P1",
				*NextNode, "P2", Path);
		(*RetVal) = (*RetVal) + (*Link);
		delete Link;
		CurrentNode = NextNode ;
		NextNode++ ;
	}

	//Add threads to the router
	IdGraph::v_iterator U ;
	IdGraph::list_iterator V ;
	for( U = RetVal->v_begin() ; U != RetVal->v_end() ; U++ ) {
		if ( *U != RouterId  ) {
			//if the the node receives some messages from other nodes
			for( V = RetVal->in_begin(*U) ; V != RetVal->in_end(*U) ; V++) {
				//Generate a thread and add it to the router
				ThreadSet::iterator It ;
				for( It = (*RetVal)[*V].PRJ(ThreadSet)ThreadSetBegin() ; It != (*RetVal)[*V].PRJ(ThreadSet)ThreadSetEnd() ; It++ ) {
					if ( It->GetDestinationNodeId() == *U ) {
						//Add the thread to the node
						(*RetVal)[RouterId].PRJ(ThreadSet)AddThread(*It) ;
					}
				}
			}
		}
	}
	return RetVal ;

}
void ChainGenerator::RemoveRouter(int pRouterId,
		cosi::commstruct::wiredbas::Implementation* pW) {
	pW->DeleteVertex(pRouterId);
}
void ChainGenerator::RemoveRouter(int pRouterId,
		cosi::commstruct::graph::U_RealWeightedGraph* pD) {
	pD->DeleteVertex(pRouterId);
}

bool ChainGenerator::CheckLatency(
		cosi::commstruct::wiredbas::Implementation* pBus) {
	IdGraph::v_iterator V;
	for (V = mSpecification->v_begin(); V != mSpecification->v_end(); V++) {
		if (pBus->InV(*V)) {
			//for each destination
			IdGraph::list_iterator Dest;
			for (Dest = mSpecification->out_begin(*V); Dest
					!= mSpecification->out_end(*V); Dest++) {
				double MaxLatency = (*mSpecification)[Edge(*V,*Dest)].PRJ(RealLatency)GetValue();
				int Nthreads = (*mSpecification)[*V].PRJ(ThreadSet)Size();
				if (Nthreads * mPlt->BusAccessLatencyValue("Bus",pBus) >= MaxLatency ){
					//std::cout << "Latency violation : Maximum latency from " << *V << " to " << *Dest << " is " << MaxLatency << std::endl ;
					//std::cout << "Number of threads on " << *V << " is " << Nthreads <<  " access latency is " << mPlt->BusAccessLatencyValue("Bus",pBus) << " total latency is " << Nthreads * mPlt->BusAccessLatencyValue("Bus",pBus) << std::endl ;
					return false;
				}
			}
		}
	}
	return true;
}

std::list< cosi::commstruct::wiredbas::Implementation* > ChainGenerator::GetChains(){
	return mChains ;
}

std::set<int> ChainGenerator::GetRouters() {
	return mRouters ;
}

}
}
}
