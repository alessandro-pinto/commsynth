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
#include "HeuristicChainGenerator.h"

namespace cosi {

namespace algorithms {

namespace wiredbas {

HeuristicChainGenerator::HeuristicChainGenerator(
		cosi::commstruct::wiredbas::Specification* pSpec,
		cosi::platforms::wiredbas::BusPlatform* pPlt) {
	mSpecification = pSpec ;
	mPlt = pPlt ;

}

HeuristicChainGenerator::~HeuristicChainGenerator() {
}

void HeuristicChainGenerator::Run() {
	//Get the set of routing points
	list<Position> RoutingPoints = mPlt->GetGatewayPosition() ;
	list< Position >::iterator Rit;
	map<int,Position> Routers;
	int LastId;
	LastId = mSpecification->GetLastId() ;
	for (Rit = RoutingPoints.begin() ; Rit != RoutingPoints.end() ; Rit++) {
		LastId++;
		Routers[LastId] = *Rit ;
		mRouters.insert(LastId) ;
	}

	cosi::commstruct::wiredbas::Implementation
			* mCompleteWiring = GenerateCompleteWiring(Routers);

	/////////////////////////////////////////////////////////////////////////////
	//
	// MAIN LOOP. FOR ALL ROUTERS FIND ALL POSSIBLE CHAINS
	//
	/////////////////////////////////////////////////////////////////////////////
	map<int,Position>::iterator RouterIt;

	for (RouterIt = Routers.begin() ; RouterIt != Routers.end() ; RouterIt++) {
		//Compute delay
		mAssigned.clear() ;
		int RouterId = RouterIt->first;
		Position RouterPoint = RouterIt->second;

		bool NodeAdded = true;
		Chain C(RouterId);
		C.Cost = mPlt->GetNodeCost("Router") ;
		C.Delay = 0;
		C.Length = 0;
		C.Bandwidth = 0;
		mChains[RouterId].insert(C);

		cout << "Routing point "<< RouterPoint << endl ;

		while (NodeAdded && (mAssigned.size() != mSpecification->Order() - 1)) {
			NodeAdded = false;
			// FIND THE CLOSES NODE TO THE CHAIN
			double Distance;
			double MinDistance= DBL_MAX;
			int U, V;
			IdGraph::v_iterator Vit;
			for (Vit = mSpecification->v_begin() ; Vit
					!= mSpecification->v_end() ; Vit++) {

				if (mAssigned.find( *Vit ) == mAssigned.end() ) { // the node has not been found yet

					int Left = C.GetLeft() ;
					int Right = C.GetRight() ;

					// CHECK LEFT NODE

					if (Left == RouterId) {
						Distance = (*mCompleteWiring)[Edge(RouterId,*Vit)].PRJ(WiringPath)Length() ;
						if (Distance < MinDistance ) {
							MinDistance = Distance ;
							V = *Vit ;
							U = RouterId;
						}
					} else {
						Distance = (*mCompleteWiring)[Edge(Left,*Vit)].PRJ(WiringPath)Length() ;
						if (Distance < MinDistance ) {
							MinDistance = Distance ;
							V = *Vit ;
							U = Left ;
						}
					}

					//CHECK RIGHT NODE

					if (Right == RouterId) {
						Distance = (*mCompleteWiring)[Edge(RouterId,*Vit)].PRJ(WiringPath)Length() ;
						if (Distance < MinDistance ) {
							MinDistance = Distance ;
							V = *Vit ;
							U = RouterId;
						}
					} else {
						Distance = (*mCompleteWiring)[Edge(Right,*Vit)].PRJ(WiringPath)Length() ;
						if (Distance < MinDistance ) {
							MinDistance = Distance ;
							V = *Vit ;
							U = Right ;
						}
					}

				}
			} // END OF COLOSEST VERTEX COMPUTATION


			//Add to C


			//       cout << "Computed minimum distance " << MinDistance << " between " << U << " and " << V << endl ;
			bool Right;
			if (C.IsRight(U ) ) {
				// 	cout << "Add " << V <<  " to the right " << endl ;
				C.AddRight(V ) ;
				Right = true;
			} else {
				// 	cout << "Add " << V <<  " to the left " << endl ;
				C.AddLeft(V ) ;
				Right = false;
			}

			// EXTEND ALL CHAINS CONTAINING U IF THAT CAN BE DONE
			set< Chain >::iterator Cit;
			set< Chain > ChainsToAdd;
			for (Cit = mChains[RouterId].begin() ; Cit
					!= mChains[ RouterId ].end() ; Cit++) {
				Chain ToExtend(0);
				ToExtend = *Cit;
				// 	cout << " U " << U << "  Left " << ToExtend.GetLeft( ) << "  Right " << ToExtend.GetRight( ) << endl ;
				if (ToExtend.Contains(U ) && ToExtend.IsRight(U ) && Right ) {
					//Extend the chain to the right
					Chain ToAdd(0);
					ToAdd.C = ToExtend.C;
					ToAdd.AddRight(V ) ;
					if (IsChainExtensible(ToExtend , RouterId, RouterPoint ) ) {
						// 	    cout << "Extending to the right" << endl ;
						//UPDATE CHAIN FIELDS
						string NodeType;
						NodeType = (*mSpecification)[V].PRJ(Type)GetValue() ;
						ToAdd.Cost = ToExtend.Cost
								+ mPlt->GetNodeCost(NodeType )
								+ mPlt->GetLinkCost("Wire", (*mCompleteWiring)[Edge(U,V)].PRJ(WiringPath)Get());
						ChainsToAdd.insert(ToAdd ) ;
						NodeAdded = true;
						mAssigned.insert(V ) ;
					}
				} else if (ToExtend.Contains(U ) && ToExtend.IsLeft(U )
						&& !Right ) {
					//Extend the chain to the left
					Chain ToAdd(0);
					ToAdd.C = ToExtend.C;
					ToAdd.AddLeft(V ) ;
					if (IsChainExtensible(ToExtend , RouterId, RouterPoint ) ) {
						// 	    cout << "Extending to the left" << endl ;
						string NodeType;
						NodeType = (*mSpecification)[V].PRJ(Type)GetValue() ;
						ToAdd.Cost = ToExtend.Cost
								+ mPlt->GetNodeCost(NodeType )
								+ mPlt->GetLinkCost("Wire", (*mCompleteWiring)[Edge(U,V)].PRJ(WiringPath)Get());
						ChainsToAdd.insert(ToAdd ) ;
						NodeAdded = true;
						mAssigned.insert(V ) ;
					}
				}
			}

			//       cout << "Number chains " << mChains[RouterName].size( ) << "  Number of new chains " << ChainsToAdd.size( ) << endl ;
			set< Chain >::iterator ToInsert;
			for (ToInsert = ChainsToAdd.begin() ; ToInsert != ChainsToAdd.end() ; ToInsert++) {
				mChains[RouterId].insert( *ToInsert ) ;
			}
			//       cout << "Post insertion size " << mChains[RouterName].size( ) << endl ;


		} // END OF ROUTER CHAINS COMPUTATION

		//print all chains
		//     cout << "Chains of router " << RouterName << endl ;
		//     set< Chain >::iterator Cit ;
		//     for( Cit = mChains[RouterName].begin( ) ; Cit != mChains[ RouterName ].end( ) ; Cit++ ) {
		//        list< int >::iterator Lit ;
		//        Chain Lc = *Cit ;
		//        for( Lit = Lc.C.begin( ) ; Lit != Lc.C.end( ) ; Lit++ ) {
		// 	 cout << *Lit << " " ;
		//        }
		//        cout << endl ;
		//     }


	} // END OF FOR ALL ROUTERS

	//Generate the chains
	map<int,set<Chain> >::iterator RpIt;
	for (RpIt = mChains.begin() ; RpIt != mChains.end() ; RpIt++) {
		set<Chain>::iterator It;
		for (It = (RpIt->second).begin() ; It != (RpIt->second).end() ; It++) {
			mChainImplementation.push_back(GenerateBusInstance(*It,
					RpIt->first, Routers[RpIt->first]) ) ;
		}
	}

}

cosi::commstruct::wiredbas::Implementation* HeuristicChainGenerator::GenerateCompleteWiring(
		map<int,Position> pRouters) {
	cosi::commstruct::wiredbas::Implementation
			* CompleteWiring = new cosi::commstruct::wiredbas::Implementation("");

	IdGraph::v_iterator U, V;
	for (U = mSpecification->v_begin(); U != mSpecification->v_end(); U++) {
		for (V = mSpecification->v_begin(); V != mSpecification->v_end(); V++) {
			if ( !CompleteWiring->InV(*U)) {
				CompleteWiring->AddVertex(*U);
				(*CompleteWiring)[*U].PRJ(Position)Set((*mSpecification)[*U].PRJ(Position)Get());
			}
			if ( !CompleteWiring->InV(*V)) {
				CompleteWiring->AddVertex(*V);
				(*CompleteWiring)[*V].PRJ(Position)Set((*mSpecification)[*V].PRJ(Position)Get());
			}
			if ( !CompleteWiring->InE(*U, *V)) {
				WiringPath Path;
				Path = mPlt->WireLayout((*CompleteWiring)[*U].PRJ(Position)Get(), (*CompleteWiring)[*V].PRJ(Position)Get());
				CompleteWiring->AddEdge(*U, *V);
				(*CompleteWiring)[Edge(*U,*V)].PRJ(WiringPath)Set(Path);
			}
		}
	}

	map<int,Position>::iterator Rit;
	for (Rit = pRouters.begin() ; Rit != pRouters.end() ; Rit++) {
		CompleteWiring->AddVertex(Rit->first) ;
		(*CompleteWiring)[Rit->first].PRJ(Position)Set(Rit->second) ;
		for (U = mSpecification->v_begin(); U != mSpecification->v_end(); U++) {
			WiringPath Path;
			Path = mPlt->WireLayout((*CompleteWiring)[*U].PRJ(Position)Get(), Rit->second);
			CompleteWiring->AddEdge(Rit->first, *U) ;
			CompleteWiring->AddEdge(*U, Rit->first) ;
			(*CompleteWiring)[Edge(Rit->first,*U)].PRJ(WiringPath)Set(Path) ;
			(*CompleteWiring)[Edge(*U,Rit->first)].PRJ(WiringPath)Set(Path) ;
		}
	}

	return CompleteWiring;
}

bool HeuristicChainGenerator::IsChainExtensible(Chain C, int RouterId,
		Position RouterPosition) {

	//generate the bus instance
	cosi::commstruct::wiredbas::Implementation* RetVal = GenerateBusInstance(C,
			RouterId, RouterPosition) ;

	//check rules
	if (mPlt->Satisfies("Bus", RetVal) ) {
		if (CheckLatency(RetVal) ) {
			//std::cout << "Bus order " << Chain->Order() <<std::endl ;
			delete RetVal;
			return true;
		}
	}
	delete RetVal;
	return false;
}

cosi::commstruct::wiredbas::Implementation* HeuristicChainGenerator::GenerateBusInstance(
		Chain C, int RouterId, Position RouterPosition) {

	//generate the bus instance
	cosi::commstruct::wiredbas::Implementation
			* RetVal = new cosi::commstruct::wiredbas::Implementation("");
	cosi::commstruct::wiredbas::Implementation* Node;
	cosi::commstruct::wiredbas::Implementation* Link;

	std::list<int>::iterator CurrentNode, NextNode;
	CurrentNode = C.C.begin();

	if ( *CurrentNode == RouterId ) {
		Node = mPlt->GetImplementationInstance("Router", RouterId,
				RouterPosition ) ;
	} else {
		Type T = (*mSpecification)[*CurrentNode].PRJ(Type)Get() ;
		//cout << "Node " << *CurrentNode << " " << T << endl ;
		Position P = (*mSpecification)[*CurrentNode].PRJ(Position)Get() ;
		ThreadSet Th = (*mSpecification)[*CurrentNode].PRJ(ThreadSet)Get() ;
		Node = mPlt->GetImplementationInstance(T.GetValue(), *CurrentNode, P,
				Th) ;
	}

	(*RetVal) = (*RetVal) + (*Node);

	delete Node;

	NextNode = C.C.begin() ;
	NextNode ++;

	while (NextNode != C.C.end() ) {
		if (*NextNode == RouterId ) {
			Node = mPlt->GetImplementationInstance("Router", RouterId,
					RouterPosition) ;
		} else {
			Type T = (*mSpecification)[*NextNode].PRJ(Type)Get() ;
			//cout << "Node " << *NextNode << " " << T << endl ;
			Position P = (*mSpecification)[*NextNode].PRJ(Position)Get() ;
			ThreadSet Th = (*mSpecification)[*NextNode].PRJ(ThreadSet)Get() ;
			Node = mPlt->GetImplementationInstance(T.GetValue(), *NextNode, P,
					Th) ;
		}
		(*RetVal) = (*RetVal) + (*Node);
		delete Node;
		//link
		WiringPath Path;
		Path = mPlt->WireLayout((*RetVal)[*CurrentNode].PRJ(Position)Get(),(*RetVal)[*NextNode].PRJ(Position)Get());
		Link = mPlt->GetImplementationInstance("Wire", *CurrentNode, "P1",
				*NextNode, "P2", Path);
		(*RetVal) = (*RetVal) + (*Link);
		delete Link;
		CurrentNode = NextNode ;
		NextNode++;
	}

	//Add threads to the router
	IdGraph::v_iterator U;
	IdGraph::list_iterator V;
	for (U = RetVal->v_begin() ; U != RetVal->v_end() ; U++) {
		if ( *U != RouterId ) {
			//if the the node receives some messages from other nodes
			for (V = RetVal->in_begin(*U) ; V != RetVal->in_end(*U) ; V++) {
				//Generate a thread and add it to the router
				ThreadSet::iterator It;
				for (It = (*RetVal)[*V].PRJ(ThreadSet)ThreadSetBegin() ; It != (*RetVal)[*V].PRJ(ThreadSet)ThreadSetEnd() ; It++) {
					if (It->GetDestinationNodeId() == *U ) {
						//Add the thread to the node
						(*RetVal)[RouterId].PRJ(ThreadSet)AddThread(*It) ;
					}
				}
			}
		}
	}

	return RetVal ;
}

bool HeuristicChainGenerator::CheckLatency(
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
				if (Nthreads * mPlt->BusAccessLatencyValue("Bus", pBus)
						>= MaxLatency ) {
					//std::cout << "Latency violation : Maximum latency from " << *V << " to " << *Dest << " is " << MaxLatency << std::endl ;
					//std::cout << "Number of threads on " << *V << " is " << Nthreads <<  " access latency is " << mPlt->BusAccessLatencyValue("Bus",pBus) << " total latency is " << Nthreads * mPlt->BusAccessLatencyValue("Bus",pBus) << std::endl ;
					return false;
				}
			}
		}
	}
	return true;
}

Chain::Chain(int Id) {
	C.push_back(Id) ;
}

void Chain::AddLeft(int pV) {
	C.push_front(pV ) ;
}

void Chain::AddRight(int pV) {
	C.push_back(pV ) ;
}

int Chain::GetLeft() {
	return C.front() ;
}

int Chain::GetRight() {
	return C.back() ;
}

bool Chain::IsLeft(int pV) {
	return (pV == C.front() );
}

bool Chain::IsRight(int pV) {
	return (pV == C.back() );
}

bool Chain::Contains(int pV) {
	list<int>::iterator elem;
	elem = find(C.begin() , C.end() , pV ) ;
	if (elem != C.end() )
		return true;
	else
		return false;
}

int Chain::size() const {
	return C.size() ;
}

bool Chain::operator <( const Chain & C1 ) const
{
	return ( this->C < C1.C );
}

set< int > Chain::NodeSet() {
	set< int > RetVal;
	list< int >::iterator It;
	for (It = C.begin(); It != C.end(); It++) {
		if ( *It != -1)
		RetVal.insert( *It );
	}
	return RetVal;
}

std::list< cosi::commstruct::wiredbas::Implementation* > HeuristicChainGenerator::GetChains() {
	return mChainImplementation;
}

std::set< int > HeuristicChainGenerator::GetRouters( ) {
	return mRouters;
}

}

}

}
