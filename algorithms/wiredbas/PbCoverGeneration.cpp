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
#include "PbCoverGeneration.h"

namespace cosi {

namespace algorithms {

namespace wiredbas {

using namespace std;
using namespace cosi::commstruct::wiredbas;
using namespace cosi::platforms::wiredbas;
using namespace cosi::io::bas;

PbCoverGeneration::PbCoverGeneration(Specification* pSpec,
		list<Implementation*> pChains, set<int> Routers, BusPlatform* pPlt) {
	mChains = pChains;
	mPlt = pPlt;
	mSpec = pSpec;
	mRouters = Routers;
	mBusBased = true ;
}


PbCoverGeneration::PbCoverGeneration(Specification* pSpec,
		list<Implementation*> pChains, set<int> Routers, NetworkPlatform* pPlt) {
	mChains = pChains;
	mNetPlt = pPlt;
	mSpec = pSpec;
	mRouters = Routers;
	mBusBased = false;
}


PbCoverGeneration::~PbCoverGeneration() {

}

bool PbCoverGeneration::Run() {
	//Variables of the optimization problem
	//m : number of chains
	//n : number of nodes
	//zi : a binary variable that is one if the i-th chain is
	//     part of the cover
	//xij : a binary variable that is 1 if sensor i is assigned to
	//      chain j. Notice that the same node belongs to a certain number of
	//      chains and we need to decide to which one that sensor belogns to.
	//      Also, the same node does not need to belong to all chains.
	//      Therefore we  need to set to zero all those variables xij such that
	//      node i is not in chain j
	//ykj : a binary variable that is 1 if router in position k serves chain j.
	//      In general a router serves many chains and we must be carefull not to
	//      include two chains that are served by the same router.
	//      A router does not necessarely serve all chains.
	//0...m-1 : correspond to zj
	//m...2m-1 : correspond to x0j
	//m + i*m + j : correspond to xij
	//(i+1)*m + l : correspond to y0j
	//(i+1)*m + k*m +j : correspond to ykj


	PbCodeGen* PbSolver = new PbCodeGen(Configuration->GetTmp() + "/covering.opb");

	//Add variables to the solver
	SetUpVariables(PbSolver);

	//Cost function
	list<Implementation*>::iterator It;
	for (It = mChains.begin(); It!= mChains.end(); It++) {
		int Vindex = GetChainIndex(*It);
		double Cost = 0;
		if (mBusBased)
			Cost = mPlt->GetBusCost("Bus", *It);
		else
			Cost = mNetPlt->GetNetCost("Net",*It) ;
		//cout << "Cost "<< Cost << endl ;
		int IntCost = Cost ;
		PbSolver->AddToCostFunction(IntCost, Vindex);
	}

	//A node is assigned to a chain if and only if
	//that chain is selected
	//A router serves a chain if and only if
	//that chain is selected
	for (It = mChains.begin(); It!= mChains.end(); It++) {
		Implementation* Chain = *It;
		IdGraph::v_iterator V;
		for (V = Chain->v_begin(); V != Chain->v_end(); V++) {
			if ((*Chain)[*V].PRJ(Type)GetValue() == "Router") {
				map<int,int> VarList;
				int Rindex = GetRouterToChainIndex(*V, Chain);
				int Cindex = GetChainIndex(Chain);
				VarList[Cindex] = 1;
				VarList[Rindex] = -1;
				PbSolver->AddEqualityConstraint(VarList, 0);
			} else {
				map<int,int> VarList;
				int Vindex = GetVertexToChainIndex(*V, Chain);
				int Cindex = GetChainIndex(Chain);
				VarList[Cindex] = 1;
				VarList[Vindex] = -1;
				PbSolver->AddEqualityConstraint(VarList, 0);
			}
		}
	}

	//A node must belong to one and only one chain.
	{
		IdGraph::v_iterator U;
		for (U = mSpec->v_begin(); U != mSpec->v_end(); U++) {
			map<int,int> VarList;
			for (It = mChains.begin(); It!= mChains.end(); It++) {
				if ((*It)->InV(*U) ) {
					int Vindex = GetVertexToChainIndex(*U, *It);
					VarList[Vindex] = 1;
				}
			}
			if (VarList.size() != 0)
			PbSolver->AddEqualityConstraint(VarList, 1);
		}
	}

	//A router can serve at most one or 4 (for multi-port) chians
	{
		set<int>::iterator Rit;
		for (Rit = mRouters.begin(); Rit != mRouters.end(); Rit++) {
			map<int,int> VarList;
			for (It = mChains.begin(); It!= mChains.end(); It++) {
				if ((*It)->InV(*Rit) ) {
					int Vindex = GetRouterToChainIndex(*Rit, *It);
					VarList[Vindex] = 1;
				}
			}
			if (VarList.size() != 0)
			PbSolver->AddLessThanOrEqualToConstraint(VarList, 4);
		}
	}

	//Exlusion due to delay violation
	//He we should also be able to put threshoulds on the
	//minimum delay slack
	//	{
	//		IdGraph::v_iterator U, V;
	//
	//		list<Implementation*>::iterator It1;
	//
	//		for (U = mSpec->v_begin() ; U != mSpec->v_end() ; U++) {
	//			for (V = mSpec->v_begin() ; V != mSpec->v_end() ; V++) {
	//				if (mSpec->InE(*U, *V)) {
	//
	//					for (It = mChains.begin(); It!= mChains.end(); It++) {
	//						for (It1 = mChains.begin(); It1!= mChains.end(); It1++) {
	//							if (It1 != It ) {
	//								if ( ((*It1)->InV(*U) && (*It)->InV(*V)) ||((*It)->InV(*U) && (*It1)->InV(*V))) {
	//									//sum up the two token rotation times
	//									double Delay = mPlt->BusAccessLatencyValue(
	//											"Bus", *It1) ;
	//									Delay += mPlt->BusAccessLatencyValue("Bus",
	//											*It) ;
	//									if (Delay > (*mSpec)[Edge(*U,*V)].PRJ(RealLatency)GetValue() ) {
	//										map<int,int> VarList;
	//										int C1Index = GetChainIndex(*It) ;
	//										int C2Index = GetChainIndex(*It1) ;
	//										VarList[C1Index] = 1;
	//										VarList[C2Index] = 1;
	//										PbSolver->AddEqualityConstraint(
	//												VarList, 1) ;
	//									}
	//								}
	//							}
	//						}
	//					}
	//
	//				}
	//			}
	//		}
	//	}

	PbSolver->Run() ;
	for (It = mChains.begin(); It!= mChains.end(); It++) {
		int Index = GetChainIndex(*It) ;
		if (PbSolver->GetValue(Index) )
			mCover.push_back(*It) ;
	}
	return true;
}

void PbCoverGeneration::SetUpVariables(cosi::io::bas::PbCodeGen* pSolver) {
	int VarIndex = 0;
	//set up all the chains first;
	list<Implementation*>::iterator It;
	for (It = mChains.begin(); It!= mChains.end(); It++) {
		mI2Index[*It] = VarIndex ;
		mIndex2I[VarIndex] = *It ;
		pSolver->AddVariable(VarIndex) ;
		VarIndex++;
	}

	//set up vertex to chain
	IdGraph::v_iterator V;
	for (V = mSpec->v_begin() ; V!= mSpec->v_end() ; V++) {
		for (It = mChains.begin(); It!= mChains.end(); It++) {
			if ((*It)->InV(*V) ) {
				int Cindex = mI2Index[*It];
				mVertexChain2Index[pair<int,int>(*V,Cindex)] = VarIndex ;
				mIndex2VertexChain[VarIndex] = pair<int, int>(*V, Cindex) ;
				pSolver->AddVariable(VarIndex) ;
				VarIndex ++;
			}
		}
	}

	//set up router to chain
	set<int>::iterator Rit;
	for (Rit = mRouters.begin() ; Rit != mRouters.end() ; Rit++) {
		for (It = mChains.begin(); It!= mChains.end(); It++) {
			if ((*It)->InV(*Rit) ) {
				int Cindex = mI2Index[*It];
				mRouterChain2Index[pair<int,int>(*Rit,Cindex)] = VarIndex ;
				mIndex2RouterChain[VarIndex] = pair<int, int>(*Rit, Cindex) ;
				pSolver->AddVariable(VarIndex) ;
				VarIndex ++;
			}
		}
	}
}

int PbCoverGeneration::GetChainIndex(
		cosi::commstruct::wiredbas::Implementation* pI) {
	return mI2Index[pI];
}

int PbCoverGeneration::GetVertexToChainIndex(int pV,
		cosi::commstruct::wiredbas::Implementation* pI) {
	int Index = GetChainIndex(pI) ;
	return mVertexChain2Index[pair<int,int>(pV,Index)];
}

int PbCoverGeneration::GetRouterToChainIndex(int pV,
		cosi::commstruct::wiredbas::Implementation* pI) {
	int Index = GetChainIndex(pI) ;
	return mRouterChain2Index[pair<int,int>(pV,Index)];
}

std::list<cosi::commstruct::wiredbas::Implementation*> PbCoverGeneration::GetCover() {
	return mCover;
}

}

}

}
