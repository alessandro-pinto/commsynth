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
#include "SecondLevelNetwork.h"

namespace cosi {

namespace algorithms {

namespace wiredbas {

SecondLevelNetwork::SecondLevelNetwork(
		std::list<cosi::commstruct::wiredbas::Implementation*> SubNets,
		cosi::platforms::wiredbas::BusPlatform* Plt) {
	mBusBased = true;
	mSubNets = SubNets ;
	mPlt = Plt ;

}

SecondLevelNetwork::SecondLevelNetwork(
		std::list<cosi::commstruct::wiredbas::Implementation*> SubNets,
		cosi::platforms::wiredbas::NetworkPlatform* Plt) {
	mBusBased = false;
	mSubNets = SubNets ;
	mNetPlt = Plt ;
}

SecondLevelNetwork::~SecondLevelNetwork() {

}

bool SecondLevelNetwork::Run() {

	if (mBusBased)
		return RunBusBased();
	else
		return RunNetBased();

}

bool SecondLevelNetwork::RunBusBased() {

	mI = new cosi::commstruct::wiredbas::Implementation("");

	{ //Collect all routers from all subnets
		std::list<cosi::commstruct::wiredbas::Implementation*>::iterator It;
		for (It = mSubNets.begin() ; It != mSubNets.end() ; It++) {
			cosi::commstruct::wiredbas::Implementation* SubNet = *It ;
			IdGraph::v_iterator U;
			for (U = SubNet->v_begin() ; U != SubNet->v_end() ; U++) {
				if ((*SubNet)[*U].PRJ(Type)GetValue() == "Router") {
					mI->AddVertex(*U) ;
					(*mI)[*U] = (*SubNet)[*U];
				}
			}
		}
	}

	{//Build a chain connecting all routers (using Tsp as usual)
		cosi::commstruct::graph::U_RealWeightedGraph
				* G = new cosi::commstruct::graph::U_RealWeightedGraph;
		IdGraph::v_iterator U, V;
		for (U = mI->v_begin() ; U != mI->v_end() ; U++) {
			V = U ;
			V++;
			for (; V != mI->v_end() ; V++) {
				if ( !G->InV(*U)) {
					G->AddVertex(*U) ;
				}
				if ( !G->InV(*V)) {
					G->AddVertex(*V) ;
				}
				G->AddEdge(*U, *V) ;
				//compute wire layout and distance
				Position Pstart, Pend;
				Pstart = (*mI)[*U].PRJ(Position)Get() ;
				Pend = (*mI)[*V].PRJ(Position)Get() ;
				WiringPath P = mPlt->WireLayout(Pstart, Pend) ;
				(*G)[Edge(*U,*V)].PRJ(RealQuantity)SetValue(P.Length() );
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
		std::list<int> Chain;
		for (int i = 0; i < Tour.size() ; i++) {
			Chain.push_back(*First) ;
			First++;
			if (First == Tour.end() ) {
				First = Tour.begin();
			}
		}

		delete T;

		//instantiate the links
		cosi::commstruct::wiredbas::Implementation* Link;

		std::list<int>::iterator CurrentNode, NextNode;
		CurrentNode = Chain.begin();
		NextNode = Chain.begin() ;
		NextNode ++;

		while (NextNode != Chain.end() ) {
			//link
			WiringPath Path;
			Path = mPlt->WireLayout((*mI)[*CurrentNode].PRJ(Position)Get(), (*mI)[*NextNode].PRJ(Position)Get());
			Link = mPlt->GetImplementationInstance("Wire", *CurrentNode, "P3",
					*NextNode, "P4", Path);
			(*mI) = (*mI) + (*Link);
			delete Link;
			CurrentNode = NextNode;
			NextNode++;
		}
	}
	return true ;
}

bool SecondLevelNetwork::RunNetBased() {

	//add all the switches
	mI = new cosi::commstruct::wiredbas::Implementation("");

	std::map<cosi::commstruct::wiredbas::Implementation*,int> mRouterMap;

	{ //Collect all routers from all subnets
		std::list<cosi::commstruct::wiredbas::Implementation*>::iterator It;
		for (It = mSubNets.begin() ; It != mSubNets.end() ; It++) {
			cosi::commstruct::wiredbas::Implementation* SubNet = *It ;
			IdGraph::v_iterator U;
			for (U = SubNet->v_begin() ; U != SubNet->v_end() ; U++) {
				if ((*SubNet)[*U].PRJ(Type)GetValue() == "Router") {
					mI->AddVertex(*U) ;
					(*mI)[*U] = (*SubNet)[*U];
					mRouterMap[SubNet] = *U ;
				}
			}
		}
	}

	//connect the switches
	{
		std::list<cosi::commstruct::wiredbas::Implementation*>::iterator It1,
				It2;
		for (It1 = mSubNets.begin() ; It1 != mSubNets.end() ; It1++) {
			cosi::commstruct::wiredbas::Implementation* SubNet = *It1 ;
			IdGraph::v_iterator U;
			for (U = SubNet->v_begin() ; U != SubNet->v_end() ; U++) {
				if ( *U != mRouterMap[SubNet]) {
					//for each destination
					ThreadSet T = (*SubNet)[*U].PRJ(ThreadSet)Get() ;
					ThreadSet::iterator It;
					for (It = T.ThreadSetBegin() ; It != T.ThreadSetEnd() ; It++) {
						int DestId = It->GetDestinationNodeId() ;
						//find the subnet that contains the destination node
						for (It2 = mSubNets.begin() ; It2 != mSubNets.end() ; It2++) {
							if ((*It2)->InV(DestId) ) {
								//make sure there is a link between the two routers
								int SrcR = mRouterMap[*It1];
								int DestR = mRouterMap[*It2];
								if ( SrcR != DestR && !mI->InE(SrcR, DestR)) {
									//Instantiate a link
									string Pout, Pin;
									if (((*mI)[SrcR].PRJ(Ports)GetNotBoundOut()).size() == 0) {
										//need to add an output port
										std::stringstream s;
										s << "To_"<< DestR ;
										Pout = s.str() ;
										(*mI)[SrcR].PRJ(Ports)AddPort(Pout, "Eth",
												cosi::PortInterface::OUT) ;
									} else {
										Pout = (*mI)[SrcR].PRJ(Ports)GetOutputPortNotBound();
									}
									if (((*mI)[DestR].PRJ(Ports)GetNotBoundIn()).size() == 0) {
										//need to add an input port
										std::stringstream s;
										s << "From_"<< SrcR ;
										Pin = s.str() ;
										(*mI)[DestR].PRJ(Ports)AddPort(Pin, "Eth",
												cosi::PortInterface::IN) ;
									} else {
										Pin = (*mI)[DestR].PRJ(Ports)GetInputPortNotBound();
									}

									cosi::commstruct::wiredbas::Implementation* Link;
									Position Pstart = (*mI)[SrcR].PRJ(Position)Get();
									Position Pend = (*mI)[DestR].PRJ(Position)Get();
									WiringPath Path = mNetPlt->WireLayout(Pstart,Pend) ;
									Link = mNetPlt->GetImplementationInstance("Wire",SrcR,Pout,DestR,Pin,Path);
									(*mI) = (*mI) + (*Link) ;
									delete Link ;
								}
								break;
							}
						}

					}
				}
			}
		}

	}
	return true ;
}

cosi::commstruct::wiredbas::Implementation* SecondLevelNetwork::Get() {
	return mI ;
}

}

}

}
