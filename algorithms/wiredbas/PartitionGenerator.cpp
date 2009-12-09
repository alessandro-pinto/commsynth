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
#include "PartitionGenerator.h"

namespace cosi {

namespace algorithms {

namespace wiredbas {

PartitionGenerator::PartitionGenerator(
		cosi::commstruct::wiredbas::Specification* pSpec,
		cosi::platforms::wiredbas::BusPlatform* pPlt) {
	mSpecification = pSpec;
	mPlt = pPlt;
	mBandwidthWeight = 0;
	mLatencyWeight = 0;
	mDistanceWeight = 1000;
	mBusBased = true;
}

PartitionGenerator::PartitionGenerator(
		cosi::commstruct::wiredbas::Specification* pSpec,
		cosi::platforms::wiredbas::NetworkPlatform* pPlt) {
	mSpecification = pSpec;
	mNetPlt = pPlt;
	mBandwidthWeight = 0;
	mLatencyWeight = 0;
	mDistanceWeight = 1000;
	mBusBased = false;
}

PartitionGenerator::~PartitionGenerator() {

}

void PartitionGenerator::SetBandwidthWeight(double W) {
	mBandwidthWeight = W;
}
void PartitionGenerator::SetLatencyWeight(double W) {
	mLatencyWeight = W;
}
void PartitionGenerator::SetDistanceWeight(double W) {
	mDistanceWeight = W;
}

void PartitionGenerator::Run() {

	std::list<Position> BusRouterPosition;
	if (mBusBased )
		BusRouterPosition= mPlt->GetGatewayPosition();
	else
		BusRouterPosition = mNetPlt->GetGatewayPosition();
	std::list<Position>::iterator It;
	int RouterId = mSpecification->GetLastId() + 1;
	for (It = BusRouterPosition.begin() ; It != BusRouterPosition.end() ; It++) {
		mRouters[RouterId] = *It;
		RouterId++;
	}

	//Generate the wigthed graph
	cosi::commstruct::graph::RealWeightedGraph *G = GenerateWeightedGraph() ;

	int MaxParts = 0;
	if (mRouters.size() < mSpecification->Order() )
		MaxParts = mRouters.size() ;
	else
		MaxParts = mSpecification->Order();

	//For all number of partitions
	for (int Nparts = MaxParts; Nparts >=2; Nparts--) {
		//generate the partition
		std::vector<std::set<int> >Partitions =cosi::io::bas::ShmetisPartition(
				G, Nparts, 1) ;

		//std::vector<std::set<int> >Partitions =cosi::io::bas::PatohPartition(G,
			//Nparts) ;

		if (mBusBased) {
			std::list<cosi::commstruct::wiredbas::Implementation* >
					Chains =GenerateChains(Partitions) ;
			std::list<cosi::commstruct::wiredbas::Implementation* >::iterator i;
			for (i = Chains.begin() ; i != Chains.end() ; i++) {
				cosi::commstruct::wiredbas::Implementation
						* Tmp =new cosi::commstruct::wiredbas::Implementation("");
				*Tmp = *(*i);
				//std::cout << Tmp->Print() << std::endl ;
				delete *i;
				if (Tmp->Size() > 0)
					mChains.push_back(Tmp) ;
			}
			std::cout << "Number of chains "<< mChains.size() << std::endl;
		} else {
			std::list<cosi::commstruct::wiredbas::Implementation* >
					Chains =GenerateNets(Partitions) ;
			std::list<cosi::commstruct::wiredbas::Implementation* >::iterator i;
			for (i = Chains.begin() ; i != Chains.end() ; i++) {
				cosi::commstruct::wiredbas::Implementation
						* Tmp =new cosi::commstruct::wiredbas::Implementation("");
				*Tmp = *(*i);
				//std::cout << Tmp->Print() << std::endl ;
				delete *i;
				if (Tmp->Size() > 0)
					mChains.push_back(Tmp) ;
			}
			std::cout << "Number of sub-networks "<< mChains.size()<< std::endl;
		}
	}

	if (mBusBased) {

		//single partition
		std::set<int> Partition;
		IdGraph::v_iterator U;
		for (U = mSpecification->v_begin() ; U!= mSpecification->v_end() ; U++) {
			Partition.insert( *U) ;
		}
		std::vector<std::set<int> > Partitions;
		Partitions.push_back(Partition) ;
		std::list<cosi::commstruct::wiredbas::Implementation* >
				Chains =GenerateChains(Partitions) ;
		std::list<cosi::commstruct::wiredbas::Implementation* >::iterator i;
		for (i = Chains.begin() ; i != Chains.end() ; i++) {
			cosi::commstruct::wiredbas::Implementation
					* Tmp =new cosi::commstruct::wiredbas::Implementation("");
			*Tmp = *(*i);
			//std::cout << Tmp->Print() << std::endl ;
			delete *i;
			if (Tmp->Size() > 0)
				mChains.push_back(Tmp) ;
		}
		std::cout << "Number of chains "<< mChains.size() << std::endl;
	} else {
		//single partition
		std::set<int> Partition;
		IdGraph::v_iterator U;
		for (U = mSpecification->v_begin() ; U!= mSpecification->v_end() ; U++) {
			Partition.insert( *U) ;
		}
		std::vector<std::set<int> > Partitions;
		Partitions.push_back(Partition) ;
		std::list<cosi::commstruct::wiredbas::Implementation* >
				Chains =GenerateNets(Partitions) ;
		std::list<cosi::commstruct::wiredbas::Implementation* >::iterator i;
		for (i = Chains.begin() ; i != Chains.end() ; i++) {
			cosi::commstruct::wiredbas::Implementation
					* Tmp =new cosi::commstruct::wiredbas::Implementation("");
			*Tmp = *(*i);
			//std::cout << Tmp->Print() << std::endl ;
			delete *i;
			if (Tmp->Size() > 0)
				mChains.push_back(Tmp) ;
		}
		std::cout << "Number of sub-networks "<< mChains.size() << std::endl;
	}
}

cosi::commstruct::graph::RealWeightedGraph* PartitionGenerator::GenerateWeightedGraph() {
	cosi::commstruct::graph::RealWeightedGraph
			* RetVal =new cosi::commstruct::graph::RealWeightedGraph;
	IdGraph::v_iterator U, V;
	for (U = mSpecification->v_begin() ; U != mSpecification->v_end() ; U++) {
		for (V = mSpecification->v_begin() ; V != mSpecification->v_end() ; V++) {
			if ( !RetVal->InV(*U)) {
				RetVal->AddVertex(*U) ;
			}
			if ( !RetVal->InV(*V)) {
				RetVal->AddVertex(*V) ;
			}
			if ( *U != *V ) {
				RetVal->AddEdge(*U, *V) ;
				if (mSpecification->InE(*U, *V)) {
					double Weight = 1;
					Weight += mBandwidthWeight* ComputeAverageBandwidth(*U, *V) ;
					//Position Pstart = (*mSpecification)[*U].PRJ(Position)Get();
					//Position Pend = (*mSpecification)[*V].PRJ(Position)Get();
					//WiringPath P = mPlt->WireLayout(Pstart, Pend) ;
					//Weight += mDistanceWeight / Norm1(Pstart, Pend) ;
					//Weight += mDistanceWeight / P.Length() ;
					Weight += mLatencyWeight * (*mSpecification)[Edge(*U,*V)].PRJ(RealLatency)GetValue() ;
					(*RetVal)[Edge(*U,*V)].PRJ(RealQuantity)SetValue(Weight) ;
				}
				//Add the distance weight
				double Weight = 0;
				Position Pstart = (*mSpecification)[*U].PRJ(Position)Get();
				Position Pend = (*mSpecification)[*V].PRJ(Position)Get();
				WiringPath P;
				if (mBusBased )
					P = mPlt->WireLayout(Pstart, Pend) ;
				else
					P = mNetPlt->WireLayout(Pstart, Pend) ;
				Weight += mDistanceWeight / P.Length() ;
				if ((*RetVal)[Edge(*U,*V)].PRJ(RealQuantity)IsBot() ) {
					(*RetVal)[Edge(*U,*V)].PRJ(RealQuantity)SetValue(Weight) ;
				} else {
					Weight +=(*RetVal)[Edge(*U,*V)].PRJ(RealQuantity)GetValue() ;
					(*RetVal)[Edge(*U,*V)].PRJ(RealQuantity)SetValue(Weight) ;
				}
			}
		}
	}

	return RetVal;
}

std::list<cosi::commstruct::wiredbas::Implementation* > PartitionGenerator::GenerateChains(
		std::vector<std::set<int> > Partitions) {

	std::list<cosi::commstruct::wiredbas::Implementation* > RetVal;

	for (unsigned int i = 0; i < Partitions.size() ; i++) {

		//try each router position and retian the minimum found.
		std::map<int,Position>::iterator It;
		double MinCost= DBL_MAX;
		cosi::commstruct::wiredbas::Implementation
				* Optimal =new cosi::commstruct::wiredbas::Implementation("");
		for (It = mRouters.begin() ; It != mRouters.end() ; It++) {
			//generate the optimal chain with this router
			std::list<int> Chain = GenerateChain(Partitions[i], It->first);
			cosi::commstruct::wiredbas::Implementation* I =GenerateBusInstance(
					Chain, It->first) ;
			//std::cout << "Generated bus " << std::endl ;
			//std::cout << I->Print() << std::endl ;
			//check that rules are satified
			if (mPlt->Satisfies("Bus", I) ) {
				if (CheckLatency(I) ) {
					double Cost = mPlt->GetBusCost("Bus", I) ;
					if (Cost < MinCost) {
						//std::cout << "Cost decreses from "<< MinCost << " to "
						//		<< Cost << std::endl ;
						MinCost = Cost;
						*Optimal = *I;
						delete I;
					} else {
						delete I;
					}
				} else {
					delete I;
				}
			} else {
				delete I;
			}

		}
		cosi::commstruct::wiredbas::Implementation
				* Tmp =new cosi::commstruct::wiredbas::Implementation("");
		*Tmp = *Optimal;
		//std::cout << "Chain to store " << std::endl ;
		//std::cout << Tmp->Print() << endl ;
		RetVal.push_back(Tmp) ;
		delete Optimal;
	}

	return RetVal;
}

std::list<cosi::commstruct::wiredbas::Implementation* > PartitionGenerator::GenerateNets(
		std::vector<std::set<int> > Partitions) {

	std::list<cosi::commstruct::wiredbas::Implementation* > RetVal;

	for (unsigned int i = 0; i < Partitions.size() ; i++) {

		//try each router position and retian the minimum found.
		std::map<int,Position>::iterator It;
		double MinCost= DBL_MAX;
		cosi::commstruct::wiredbas::Implementation
				* Optimal =new cosi::commstruct::wiredbas::Implementation("");
		for (It = mRouters.begin() ; It != mRouters.end() ; It++) {
			//generate the optimal chain with this router
			cosi::commstruct::wiredbas::Implementation* I =GenerateNetInstance(
					Partitions[i], It->first);
			//std::cout << "Generated bus " << std::endl ;
			//std::cout << I->Print() << std::endl ;
			//check that rules are satified
			if (mNetPlt->Satisfies("Net", I, mSpecification) ) {
				if (CheckNetLatency(I) ) {
					double Cost = mNetPlt->GetNetCost("Net", I) ;
					if (Cost < MinCost) {
						//std::cout << "Cost decreses from "<< MinCost << " to "
						//		<< Cost << std::endl ;
						MinCost = Cost;
						*Optimal = *I;
						delete I;
					} else {
						delete I;
					}
				} else {
					delete I;
				}
			} else {
				delete I;
			}

		}
		cosi::commstruct::wiredbas::Implementation
				* Tmp =new cosi::commstruct::wiredbas::Implementation("");
		*Tmp = *Optimal;
		//std::cout << "Chain to store " << std::endl ;
		//std::cout << Tmp->Print() << endl ;
		RetVal.push_back(Tmp) ;
		delete Optimal;
	}
	return RetVal;
}

cosi::commstruct::wiredbas::Implementation* PartitionGenerator::GenerateNetInstance(
		std::set<int> Nodes, int RouterId) {

	cosi::commstruct::wiredbas::Implementation
			* RetVal =mNetPlt->StartNew("Net") ;

	//adding the router
	Position P = mRouters[RouterId];
	ThreadSet Tset;
	TransferTable Tb;
	mNetPlt->AddNode("Net", "Router", RouterId, P, Tset, Tb) ;
	//adding the nodes and links
	std::set<int>::iterator NodeIt;
	for (NodeIt = Nodes.begin() ; NodeIt != Nodes.end() ; NodeIt++) {
		string TypeName = (*mSpecification)[*NodeIt].PRJ(Type)GetValue() ;
		Position P = (*mSpecification)[*NodeIt].PRJ(Position)Get() ;
		ThreadSet Tset = (*mSpecification)[*NodeIt].PRJ(ThreadSet)Get();
		TransferTable Tb;
		mNetPlt->AddNode("Net", TypeName, *NodeIt, P, Tset, Tb) ;
		RetVal->SetName(*NodeIt, mSpecification->GetName(*NodeIt)) ;
		WiringPath Path = mNetPlt->WireLayout(P, mRouters[RouterId]) ;
		mNetPlt->AddLink("Net", "Wire", *NodeIt, RouterId, Path) ;

		//get the port name
		string Pname = (*RetVal)[RouterId].PRJ(Ports)IsBoundTo(Edge(*NodeIt, RouterId), PortInterface::INOUT);

		IdGraph::list_iterator In;
		for (In = mSpecification->in_begin(*NodeIt) ; In
				!= mSpecification->in_end(*NodeIt) ; In++) {
			//cout << "Adding entry " << mSpecification->GetName(*In) << " " << mSpecification->GetName(*NodeIt) << " " << Pname << endl ;
			Tb.AddEntry(mSpecification->GetName(*In),
					mSpecification->GetName(*NodeIt), Pname) ;
		}
		(*RetVal)[RouterId].PRJ(TransferTable)Set(Tb) ;
	}

	//finish up the routing tables
	string Pname = (*RetVal)[RouterId].PRJ(Ports)GetInputOutputPortNotBound();
	for (NodeIt = Nodes.begin() ; NodeIt != Nodes.end() ; NodeIt++) {
		IdGraph::list_iterator Out;
		TransferTable NodeTb;
		string NodePname = (*RetVal)[*NodeIt].PRJ(Ports)IsBoundTo(Edge(*NodeIt, RouterId), PortInterface::INOUT);
		for (Out = mSpecification->out_begin(*NodeIt) ; Out
				!= mSpecification->out_end(*NodeIt) ; Out++) {
			if ( !RetVal->InV(*Out)) {
				//cout << "Adding entry " << mSpecification->GetName(*NodeIt) << " " << mSpecification->GetName(*Out) << " " << Pname << endl ;
				(*RetVal)[RouterId].PRJ(TransferTable)AddEntry(
						mSpecification->GetName(*NodeIt),
						mSpecification->GetName(*Out), Pname);
			}
			NodeTb.AddEntry(mSpecification->GetName(*NodeIt),
					mSpecification->GetName(*Out), NodePname);
		}
		(*RetVal)[*NodeIt].PRJ(TransferTable)Set(NodeTb) ;
	}

	//cout << RetVal->Print() << endl ;
	//char c;
	//cin >> c ;

	return RetVal ;
}

std::list<cosi::commstruct::wiredbas::Implementation* > PartitionGenerator::GenerateTrees(
		std::vector<std::set<int> > Partitions) {

	std::list<cosi::commstruct::wiredbas::Implementation* > RetVal;

	for (unsigned int i = 0; i < Partitions.size() ; i++) {

		//try each router position and retian the minimum found.
		std::map<int,Position>::iterator It;
		double MinCost= DBL_MAX;
		cosi::commstruct::wiredbas::Implementation
				* Optimal =new cosi::commstruct::wiredbas::Implementation("");
		for (It = mRouters.begin() ; It != mRouters.end() ; It++) {
			//generate the optimal chain with this router
			cosi::commstruct::graph::U_RealWeightedGraph* T = GenerateTree(
					Partitions[i], It->first);
			cosi::commstruct::wiredbas::Implementation* I =GenerateBusInstance(
					T, It->first) ;
			//std::cout << "Generated bus " << std::endl ;
			//std::cout << I->Print() << std::endl ;
			//check that rules are satified

			if (mPlt->Satisfies("Bus", I) ) {
				if (CheckLatency(I) ) {
					double Cost = mPlt->GetBusCost("Bus", I) ;
					if (Cost < MinCost) {
						//std::cout << "Cost decreses from "<< MinCost << " to "
						//		<< Cost << std::endl ;
						MinCost = Cost;
						*Optimal = *I;
						delete I;
					} else {
						delete I;
					}
				} else {
					delete I;
				}
			} else {
				delete I;
			}

		}
		cosi::commstruct::wiredbas::Implementation
				* Tmp =new cosi::commstruct::wiredbas::Implementation("");
		*Tmp = *Optimal;
		//std::cout << "Chain to store " << std::endl ;
		//std::cout << Tmp->Print() << endl ;
		RetVal.push_back(Tmp) ;
		delete Optimal;
	}
	return RetVal;
}

double PartitionGenerator::ComputeAverageBandwidth(int U, int V) {
	//analysize all threads in U
	ThreadSet::iterator It;
	ThreadSet T = (*mSpecification)[U].PRJ(ThreadSet)Get();
	double B = 0;
	for (It = T.ThreadSetBegin() ; It != T.ThreadSetEnd() ; It++) {
		if (It->GetDestinationNodeId() == V) {
			B+= It->GetMessageLength() / It->GetPeriod() ;
		}
	}
	return B;
}

std::list<int> PartitionGenerator::GenerateChain(std::set<int> Nodes,
		int RouterId) {
	//need to find the best chain that connects all the nodes
	//use spanning tree by now

	//Generate the graph for the spanning tree
	cosi::commstruct::graph::U_RealWeightedGraph
			* G =new cosi::commstruct::graph::U_RealWeightedGraph;
	std::set<int> AllNodes = Nodes;
	AllNodes.insert(RouterId) ;
	std::set<int>::iterator It1, It2;
	for (It1 = AllNodes.begin() ; It1 != AllNodes.end() ; It1++) {
		It2 = It1;
		It2++;
		for (; It2 != AllNodes.end() ; It2++) {
			if ( !G->InV(*It1)) {
				G->AddVertex(*It1) ;
			}
			if ( !G->InV(*It2)) {
				G->AddVertex(*It2) ;
			}
			G->AddEdge(*It1, *It2) ;
			//compute wire layout and distance
			Position Pstart, Pend;
			if ( *It1 == RouterId) {
				Pstart = mRouters[RouterId];
			} else {
				Pstart = (*mSpecification)[*It1].PRJ(Position)Get() ;
			}
			if ( *It2 == RouterId) {
				Pend = mRouters[RouterId];
			} else {
				Pend = (*mSpecification)[*It2].PRJ(Position)Get() ;
			}
			WiringPath P = mPlt->WireLayout(Pstart, Pend) ;
			(*G)[Edge(*It1,*It2)].PRJ(RealQuantity)SetValue(P.Length() );
		}
	}

	cosi::algorithms::graph::Tsp* T = new cosi::algorithms::graph::Tsp(G);
	T->Run() ;
	std::list<int> Tour = T->Get() ;

	//cut the longest edge
	std::list<int>::iterator Current, Next, First;
	Current = Tour.begin() ;
	Next = Tour.begin() ;
	Next++;
	First = Tour.begin() ;
	double MaxDist = 0;
	while (Next != Tour.end() ) {
		double Dist = (*G)[Edge(*Current,*Next)].PRJ(RealQuantity)GetValue() ;
		if (Dist > MaxDist) {
			MaxDist = Dist;
			First = Next;
		}
		Current++;
		Next++;
	}

	//build the list starting from First
	std::list<int> RetVal;
	for (int i = 0; i < Tour.size() ; i++) {
		RetVal.push_back(*First) ;
		First++;
		if (First == Tour.end() ) {
			First = Tour.begin();
		}
	}

	delete T;
	return RetVal;
}

cosi::commstruct::graph::U_RealWeightedGraph* PartitionGenerator::GenerateTree(
		std::set<int> Nodes, int RouterId) {

	//Generate the graph for the spanning tree
	cosi::commstruct::graph::U_RealWeightedGraph
			* G =new cosi::commstruct::graph::U_RealWeightedGraph;
	std::set<int> AllNodes = Nodes;
	AllNodes.insert(RouterId) ;
	std::set<int>::iterator It1, It2;
	for (It1 = AllNodes.begin() ; It1 != AllNodes.end() ; It1++) {
		It2 = It1;
		It2++;
		for (; It2 != AllNodes.end() ; It2++) {
			if ( !G->InV(*It1)) {
				G->AddVertex(*It1) ;
			}
			if ( !G->InV(*It2)) {
				G->AddVertex(*It2) ;
			}
			G->AddEdge(*It1, *It2) ;
			//compute wire layout and distance
			Position Pstart, Pend;
			if ( *It1 == RouterId) {
				Pstart = mRouters[RouterId];
			} else {
				Pstart = (*mSpecification)[*It1].PRJ(Position)Get() ;
			}
			if ( *It2 == RouterId) {
				Pend = mRouters[RouterId];
			} else {
				Pend = (*mSpecification)[*It2].PRJ(Position)Get() ;
			}
			WiringPath P = mPlt->WireLayout(Pstart, Pend) ;
			(*G)[Edge(*It1,*It2)].PRJ(RealQuantity)SetValue(P.Length() );
		}
	}

	cosi::algorithms::graph::MinimumSpanningTree
			* Mst =new cosi::algorithms::graph::MinimumSpanningTree(G);
	Mst->Run(RouterId) ;
	return Mst->Get() ;
}

std::list<int> PartitionGenerator::GenerateShortestChain(std::set<int> Nodes,
		int RouterId) {
	//need to find the best chain that connects all the nodes

	//Generate a graph with weight
	cosi::commstruct::graph::U_RealWeightedGraph
			* G =new cosi::commstruct::graph::U_RealWeightedGraph;
	std::set<int> AllNodes = Nodes;
	AllNodes.insert(RouterId) ;
	std::set<int>::iterator It1, It2;
	double MinLength= DBL_MAX;
	Edge MinLengthEdge;
	for (It1 = AllNodes.begin() ; It1 != AllNodes.end() ; It1++) {
		It2 = It1;
		It2++;
		for (; It2 != AllNodes.end() ; It2++) {
			if ( !G->InV(*It1)) {
				G->AddVertex(*It1) ;
			}
			if ( !G->InV(*It2)) {
				G->AddVertex(*It2) ;
			}
			G->AddEdge(*It1, *It2) ;
			//compute wire layout and distance
			Position Pstart, Pend;
			if ( *It1 == RouterId) {
				Pstart = mRouters[RouterId];
			} else {
				Pstart = (*mSpecification)[*It1].PRJ(Position)Get() ;
			}
			if ( *It2 == RouterId) {
				Pend = mRouters[RouterId];
			} else {
				Pend = (*mSpecification)[*It2].PRJ(Position)Get() ;
			}
			WiringPath P = mPlt->WireLayout(Pstart, Pend) ;
			(*G)[Edge(*It1,*It2)].PRJ(RealQuantity)SetValue(P.Length() );
			if (P.Length() < MinLength) {
				MinLength = P.Length() ;
				MinLengthEdge.U(*It1) ;
				MinLengthEdge.V(*It2) ;
			}
		}
	}

	//Start from the minimum length edge
	std::list<int> RetVal;
	RetVal.push_back(MinLengthEdge.U()) ;
	RetVal.push_back(MinLengthEdge.V()) ;
	AllNodes.erase(MinLengthEdge.U()) ;
	AllNodes.erase(MinLengthEdge.V()) ;
	while (AllNodes.size() > 0) {
		//find the closest node to the left and to the right and add that
		MinLength = DBL_MAX;
		int ToAdd = 0;
		int ToCompare = 0;
		bool Front = false;
		for (It1 = AllNodes.begin() ; It1 != AllNodes.end() ; It1++) {
			ToCompare = RetVal.front() ;
			double Length = (*G)[Edge(ToCompare,*It1)].PRJ(RealQuantity)GetValue();
			if (Length < MinLength) {
				MinLength = Length;
				Front = true;
				ToAdd = *It1;
			}
			ToCompare = RetVal.back() ;
			Length = (*G)[Edge(ToCompare,*It1)].PRJ(RealQuantity)GetValue();
			if (Length < MinLength) {
				MinLength = Length;
				Front = false;
				ToAdd = *It1;
			}
		}
		if (Front) {
			RetVal.push_front(ToAdd);
		} else {
			RetVal.push_back(ToAdd) ;
		}
		AllNodes.erase(ToAdd) ;
	}
	return RetVal;
}

cosi::commstruct::wiredbas::Implementation* PartitionGenerator::GenerateBusInstance(
		std::list<int> Chain, int RouterId) {

	//generate the bus instance
	cosi::commstruct::wiredbas::Implementation
			* RetVal = new cosi::commstruct::wiredbas::Implementation("");

	cosi::commstruct::wiredbas::Implementation* N;
	cosi::commstruct::wiredbas::Implementation* L;

	std::list<int>::iterator CurrentNode, NextNode;
	CurrentNode = Chain.begin();

	if ( *CurrentNode == RouterId) {
		N = mPlt->GetImplementationInstance("Router", RouterId,
				mRouters[RouterId]) ;

	} else {
		Type T = (*mSpecification)[*CurrentNode].PRJ(Type)Get() ;
		//cout << "Node " << *CurrentNode << " " << T << endl ;
		Position P = (*mSpecification)[*CurrentNode].PRJ(Position)Get() ;
		ThreadSet Th = (*mSpecification)[*CurrentNode].PRJ(ThreadSet)Get() ;
		N = mPlt->GetImplementationInstance(T.GetValue(), *CurrentNode, P, Th) ;

	}

	(*RetVal) = (*RetVal) + (*N);

	delete N;

	NextNode = Chain.begin() ;
	NextNode ++;

	while (NextNode != Chain.end() ) {
		if (*NextNode == RouterId) {
			N = mPlt->GetImplementationInstance("Router", RouterId,
					mRouters[RouterId]) ;

		} else {
			Type T = (*mSpecification)[*NextNode].PRJ(Type)Get() ;
			//cout << "Node " << *NextNode << " " << T << endl ;
			Position P = (*mSpecification)[*NextNode].PRJ(Position)Get() ;
			ThreadSet Th = (*mSpecification)[*NextNode].PRJ(ThreadSet)Get() ;
			N = mPlt->GetImplementationInstance(T.GetValue(), *NextNode, P, Th) ;

		}

		(*RetVal) = (*RetVal) + (*N);


		delete N;
		//link
		WiringPath Path;
		Path = mPlt->WireLayout((*RetVal)[*CurrentNode].PRJ(Position)Get(), (*RetVal)[*NextNode].PRJ(Position)Get());
		L = mPlt->GetImplementationInstance("Wire", *CurrentNode, "P1",
				*NextNode, "P2", Path);
		(*RetVal) = (*RetVal) + (*L);
		delete L;
		CurrentNode = NextNode;
		NextNode++;
	}

	//Add threads to the router
	IdGraph::v_iterator U;
	IdGraph::list_iterator V;
	for (U = RetVal->v_begin() ; U != RetVal->v_end() ; U++) {
		if ( *U != RouterId) {
			//if the the node receives some messages from other nodes
			for (V = RetVal->in_begin(*U) ; V != RetVal->in_end(*U) ; V++) {
				//Generate a thread and add it to the router
				ThreadSet::iterator It;
				for (It = (*RetVal)[*V].PRJ(ThreadSet)ThreadSetBegin() ; It != (*RetVal)[*V].PRJ(ThreadSet)ThreadSetEnd() ; It++) {
					if (It->GetDestinationNodeId() == *U) {
						//Add the thread to the node
						(*RetVal)[RouterId].PRJ(ThreadSet)AddThread(*It) ;
					}
				}
			}
		}
	}
	return RetVal;
}

cosi::commstruct::wiredbas::Implementation* PartitionGenerator::GenerateBusInstance(
		cosi::commstruct::graph::U_RealWeightedGraph* G, int RouterId) {

	//generate the bus instance
	cosi::commstruct::wiredbas::Implementation
			* RetVal =new cosi::commstruct::wiredbas::Implementation("");
	cosi::commstruct::wiredbas::Implementation* Node;
	cosi::commstruct::wiredbas::Implementation* Link;

	IdGraph::v_iterator U, V;
	for (U = G->v_begin() ; U != G->v_end() ; U++) {
		if ( *U == RouterId) {
			Node = mPlt->GetImplementationInstance("Router", RouterId,
					mRouters[RouterId]) ;
		} else {
			Type T = (*mSpecification)[*U].PRJ(Type)Get() ;
			//cout << "Node " << *CurrentNode << " " << T << endl ;
			Position P = (*mSpecification)[*U].PRJ(Position)Get() ;
			ThreadSet Th = (*mSpecification)[*U].PRJ(ThreadSet)Get() ;
			Node = mPlt->GetImplementationInstance(T.GetValue(), *U, P, Th) ;
		}
		(*RetVal) = (*RetVal) + (*Node);
		delete Node;
	}

	for (U = G->v_begin() ; U != G->v_end() ; U++) {
		V = U;
		V++;
		for (; V!= G->v_end() ; V++) {
			if (G->InE(*U, *V)) {

				WiringPath Path;
				Path = mPlt->WireLayout((*RetVal)[*U].PRJ(Position)Get(), (*RetVal)[*V].PRJ(Position)Get());
				Link = mPlt->GetImplementationInstance("Wire", *U, "P1", *V,
						"P2", Path);
				(*RetVal) = (*RetVal) + (*Link);
				delete Link;

			}
		}
	}

	{
		//Add threads to the router
		IdGraph::v_iterator U;
		IdGraph::list_iterator V;
		for (U = RetVal->v_begin() ; U != RetVal->v_end() ; U++) {
			if ( *U != RouterId) {
				//if the the node receives some messages from other nodes
				for (V = RetVal->in_begin(*U) ; V != RetVal->in_end(*U) ; V++) {
					//Generate a thread and add it to the router
					ThreadSet::iterator It;
					for (It = (*RetVal)[*V].PRJ(ThreadSet)ThreadSetBegin() ; It != (*RetVal)[*V].PRJ(ThreadSet)ThreadSetEnd() ; It++) {
						if (It->GetDestinationNodeId() == *U) {
							//Add the thread to the node
							(*RetVal)[RouterId].PRJ(ThreadSet)AddThread(*It) ;
						}
					}
				}
			}
		}
	}
	return RetVal;
}

bool PartitionGenerator::CheckLatency(
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
						>= MaxLatency) {
					std::cout << "Latency violation :"<< " max "<< MaxLatency
							<< " actual "<< Nthreads
							* mPlt->BusAccessLatencyValue("Bus", pBus)
							<< std::endl;
					return false;
				}
			}
		}
	}
	return true;
}

bool PartitionGenerator::CheckNetLatency(
		cosi::commstruct::wiredbas::Implementation* pNet) {
	return true;
}

std::list< cosi::commstruct::wiredbas::Implementation* > PartitionGenerator::GetChains() {
	return mChains;
}

std::set<int> PartitionGenerator::GetRouters() {
	std::map<int,Position>::iterator It;
	std::set<int> RetVal;
	for (It = mRouters.begin() ; It != mRouters.end() ; It++) {
		RetVal.insert(It->first);
	}
	return RetVal;
}

}

}

}
