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
#include "MeshTopology.h"

namespace cosi {

namespace algorithms {

namespace onchipcommunication {

using namespace cosi::commstruct::onchipcommunication;
using namespace cosi::platforms::onchipcommunication;
using namespace cosi::library::onchipcommunication;
using namespace cosi::io::onchipcommunication;
using namespace std;

MeshTopology::MeshTopology(Specification* Spec, Platform* Plt) {
	mSpec = Spec;
	mPlt = Plt;

}

MeshTopology::~MeshTopology() {

}

bool MeshTopology::Run() {

	CosiLog << "Mapping on a mesh topology: "<< endl;

	mI = new Implementation("" );
	CosiLog << "\t"<< "Setting un the mesh network "<< endl;
	if (MeshSetup() ) {
		int FirstNode = GetCommunicationCenter();
		CosiLog << "\t"<< "First node to map "<< mSpec->GetName(FirstNode)
				<< " with id "<< FirstNode << endl;
		AddCommunicationCenter(FirstNode);
		while (mCoreToRouter.size() < mSpec->Order() ) {
			int NextNode = GetNextNode();
			CosiLog << "\t"<< "Next node to map "<< mSpec->GetName(NextNode)
					<< " with id "<< NextNode << endl;
			AddNextNode(NextNode);
		}
		CosiLog << "\t"<< "Computing the optimal routing "<< endl;
		mRoutingCost = 0;
		if (mI->InDegree(3) == 0) {
			cout << "Node not connected " << endl;
		}

		FindRoutesIncremental();


		//Save best solution
		mMinCost = mRoutingCost;
		mBestCoreToRouter = mCoreToRouter;
		mBestRouterToCore = mRouterToCore;
		mBestI = new Implementation("");
		(*mBestI) = (*mI);



		//final swap
		IdGraph::v_iterator U, V;
		for (U = mSpec->v_begin() ; U != mSpec->v_end() ; U++) {
			V = U;
			V++;
			for (; V != mSpec->v_end() ; V++) {
				cout << "Swapping "<< *U << " with "<< *V << endl;
				Swap(*U, *V) ;
				mRoutingCost = 0;
				FindRoutesIncremental();
				if (mRoutingCost < mMinCost) {
					//cout << "Swapping "<< *U << " with "<< *V << endl ;
					cout << "Minimum cost = "<< mMinCost
							<< "  --  Cost after swapping " << *U << " with "
							<< *V << " = "<< mRoutingCost << endl;
					mMinCost = mRoutingCost;
					mBestCoreToRouter = mCoreToRouter;
					mBestRouterToCore = mRouterToCore;
					delete mBestI;
					mBestI = new Implementation("");
					(*mBestI) = (*mI);
					//cout << "Move accepted and recorded "<< endl ;
				} else {
					//cout << "Move not accepted, swapping back "<< endl ;
				}
				Swap(*V, *U) ;
			}
			delete mI;
			mI = new Implementation("");
			(*mI) = (*mBestI);
			mCoreToRouter = mBestCoreToRouter;
			mRouterToCore = mBestRouterToCore;
		}
	}

	delete mI;
	mI = new Implementation("");
	(*mI) = (*mBestI);
	mCoreToRouter = mBestCoreToRouter;
	mRouterToCore = mBestRouterToCore;
	delete mBestI;
	cout << "Final cleanup " << endl;
	CleanUp();
	CosiLog << "\t"<< "Floorplanning "<< endl;
	cout << "Floor-Planning " << endl;
	Floorplan();
	CosiLog << mI->Print() ;
}

bool MeshTopology::MeshSetup() {
	//Build the mesh network
	//here we need to fin the closes square to the number of ip cores in the spec.
	int Ncores = mSpec->Order();
	CosiLog << "\t"<< "Number of cores "<< Ncores << endl;
	mMeshSize = ceil(sqrt(Ncores));
	mIdMatrix.resize(mMeshSize);
	CosiLog << "\t"<< "Mesh size "<< mMeshSize << "x"<< mMeshSize << endl;
	for (unsigned int i = 0; i < mMeshSize; i++) {
		mIdMatrix[i].resize(mMeshSize);
	}
	//We need to start from the last id of the specification
	int Id = mSpec->GetLastId() + 1;
	for (int Row = 0; Row < mMeshSize; Row++) {
		for (int Column = 0; Column < mMeshSize; Column++) {
			//Instantate one router for each entry of this matrix
			Implementation* N = mPlt->GetImplementationInstance(5, 5, Id,
					Position(), TransferTable());
			(*mI) = (*mI) + (*N);
			delete N;
			mIdMatrix[Row][Column] = Id;
			mRouterCoordinates[Id] = pair<int, int>(Row, Column);
			//Give it a name. This is inportant for the floor-planner
			stringstream s;
			s << "Router_"<< Id;
			mI->SetName(Id, s.str());
			Id++;
		}
	}

	//Add all the edges of the mesh
	//The row first and then the columns
	for (int Row = 0; Row < mMeshSize; Row++) {
		for (int Column = 0; Column < mMeshSize - 1; Column++) {
			//Get a output port not bound to any edge form Column
			string Out = (*mI)[mIdMatrix[Row][Column]].PRJ(Ports)GetOutputPortNotBound();
			//Get an input port not bound to any edge from Column+1
			string In = (*mI)[mIdMatrix[Row][Column+1]].PRJ(Ports)GetInputPortNotBound();
			//Instantiate the link
			Implementation* L = mPlt->GetImplementationInstance(Type("Link"),
					mIdMatrix[Row][Column], Out, mIdMatrix[Row][Column+1], In,
					WiringPath());
			//Add the link to the implementation
			(*mI) = (*mI) + (*L);
			delete L;

			//Add a link between Column+1 and Column
			//Get a output port not bound to any edge form Column+1
			Out = (*mI)[mIdMatrix[Row][Column+1]].PRJ(Ports)GetOutputPortNotBound();
			//Get an input port not bound to any edge from Column
			In = (*mI)[mIdMatrix[Row][Column]].PRJ(Ports)GetInputPortNotBound();
			//Instantiate the link b
			L = mPlt->GetImplementationInstance(Type("Link"),
					mIdMatrix[Row][Column+1], Out, mIdMatrix[Row][Column], In,
					WiringPath());
			//Add the link to the implementation
			(*mI) = (*mI) + (*L);
			delete L;
		}
	}

	for (int Column = 0; Column < mMeshSize; Column++) {
		for (int Row = 0; Row < mMeshSize - 1; Row++) {
			string Out = (*mI)[mIdMatrix[Row][Column]].PRJ(Ports)GetOutputPortNotBound();
			string In = (*mI)[mIdMatrix[Row+1][Column]].PRJ(Ports)GetInputPortNotBound();
			//Instantiate the link
			Implementation* L = mPlt->GetImplementationInstance(Type("Link"),
					mIdMatrix[Row][Column], Out, mIdMatrix[Row+1][Column], In,
					WiringPath());
			//Add the link to the implementation
			(*mI) = (*mI) + (*L);
			delete L;

			//Add a link between Column+1 and Column
			//Get a output port not bound to any edge form Column+1
			Out = (*mI)[mIdMatrix[Row+1][Column]].PRJ(Ports)GetOutputPortNotBound();
			//Get an input port not bound to any edge from Column
			In = (*mI)[mIdMatrix[Row][Column]].PRJ(Ports)GetInputPortNotBound();
			//Instantiate the link b
			L = mPlt->GetImplementationInstance(Type("Link"),
					mIdMatrix[Row+1][Column], Out, mIdMatrix[Row][Column], In,
					WiringPath());
			//Add the link to the implementation
			(*mI) = (*mI) + (*L);
			delete L;
		}
	}
	if (mI->Order() == mMeshSize*mMeshSize) {
		return true;
	}
	return false;
}

int MeshTopology::GetCommunicationCenter() {

	IdGraph::v_iterator U, V;

	int MaxNc = 0;
	int MaxNcId = 0;
	double MaxRate = 0;
	int MaxRateId = 0;
	//For all Ip cores
	for (U = mSpec->v_begin(); U != mSpec->v_end(); U++) {
		int Nc = 0;
		double Rate = 0;
		for (V = mSpec->v_begin(); V != mSpec->v_end(); V++) {
			if (mSpec->InE(*U, *V)) {
				Nc ++;
				Rate += (*mSpec)[Edge(*U,*V)].PRJ(CommoditySet)GetAggregateBandwidthValue();
			}
			if (mSpec->InE(*V, *U)) {
				Nc ++;
				Rate += (*mSpec)[Edge(*V,*U)].PRJ(CommoditySet)GetAggregateBandwidthValue();
			}
		}
		if (Nc >= MaxNc) {
			MaxNc = Nc;
			MaxNcId = *U;
		}
		if (Rate >= MaxRate) {
			MaxRate = Rate;
			MaxRateId = *U;
		}
	}
	return MaxRateId;
}

void MeshTopology::AddCommunicationCenter(int FirstNode) {

	//Add the ip core to the center of the mesh
	int Row = mMeshSize/2;
	int Column = mMeshSize/2;
	Name ComponentName = (*mSpec)[FirstNode].PRJ(Name)Get();
	Implementation* N = mPlt->GetImplementationInstance(ComponentName,
			FirstNode, (*mSpec)[FirstNode].PRJ(Position)Get(), TransferTable());
	N->SetName(FirstNode, mSpec->GetName(FirstNode));
	(*N)[FirstNode].PRJ(IpGeometry)Set((*mSpec)[FirstNode].PRJ(IpGeometry)Get());
	//Add the core to the implementation
	(*mI) = (*mI) + (*N);
	delete N;
	//Add the links
	string Out = (*mI)[FirstNode].PRJ(Ports)GetOutputPortNotBound();
	string In = (*mI)[mIdMatrix[Row][Column]].PRJ(Ports)GetInputPortNotBound();
	Implementation* L = mPlt->GetImplementationInstance(Type("Link"),
			FirstNode, Out, mIdMatrix[Row][Column], In, WiringPath());
	(*mI) = (*mI) + (*L);
	delete L;
	Out = (*mI)[mIdMatrix[Row][Column]].PRJ(Ports)GetOutputPortNotBound();
	In = (*mI)[FirstNode].PRJ(Ports)GetInputPortNotBound();
	L = mPlt->GetImplementationInstance(Type("Link"), mIdMatrix[Row][Column],
			Out, FirstNode, In, WiringPath());
	(*mI) = (*mI) + (*L);
	delete L;

	mRouterToCore[mIdMatrix[Row][Column]] = FirstNode;
	mCoreToRouter[FirstNode] = mIdMatrix[Row][Column];
	CosiLog << "\t"<< "Node "<< mSpec->GetName(FirstNode) << " mapped to ("
			<< Row<< ","<< Column << ")"<< endl;
}

int MeshTopology::GetNextNode() {

	//Get the node that communication the most with the one already
	//instantiated
	IdGraph::v_iterator U;
	int MaxNc = 0;
	int MaxNcId = 0;
	double MaxRate = 0;
	int MaxRateId = 0;
	bool NextNodeFound = false;
	map<int,int>::iterator It;
	for (U = mSpec->v_begin(); U != mSpec->v_end(); U++) {
		if (mCoreToRouter.find(*U) == mCoreToRouter.end() ) {
			double Rate = 0;
			int Nc = 0;
			for (It = mCoreToRouter.begin() ; It != mCoreToRouter.end() ; It++) {
				if (mSpec->InE(*U, It->first)) {
					Nc ++;
					Rate += (*mSpec)[Edge(*U,It->first)].PRJ(CommoditySet)GetAggregateBandwidthValue();
				}
				if (mSpec->InE(It->first, *U)) {
					Nc ++;
					Rate += (*mSpec)[Edge(It->first,*U)].PRJ(CommoditySet)GetAggregateBandwidthValue();
				}
			}
			if (Nc >= MaxNc) {
				MaxNc = Nc;
				MaxNcId = *U;
			}
			if (Rate >= MaxRate) {
				MaxRate = Rate;
				MaxRateId = *U;
				NextNodeFound = true;
			}
		}
	}

	if ( !NextNodeFound) {
		CosiLog << "A new node to map could not be found "<< endl;
	}

	return MaxRateId;
}

void MeshTopology::AddNextNode(int NextNode) {

	int Row;
	int Column;
	double MinCost= DBL_MAX;
	bool RouterFound = false;
	IdGraph::v_iterator V;
	//For each router check the cost and get the least cost
	for (int i = 0; i < mMeshSize; i ++) {
		for (int j = 0; j < mMeshSize; j ++) {
			if (mRouterToCore.find(mIdMatrix[i][j]) == mRouterToCore.end()) {
				double Cost = 0;
				for (V = mSpec->v_begin(); V != mSpec->v_end(); V++) {
					if (mCoreToRouter.find(*V) != mCoreToRouter.end() && *V
							!= NextNode) {
						if (mSpec->InE(NextNode, *V) ) {
							int
									R =
											mRouterCoordinates[ mCoreToRouter[*V] ].first;
							int
									C =
											mRouterCoordinates[ mCoreToRouter[*V] ].second;
							Cost += (*mSpec)[Edge(NextNode,*V)].PRJ(CommoditySet)GetAggregateBandwidthValue()
									*(fabs(i-R) + fabs(j-C));
						}
						if (mSpec->InE(*V, NextNode) ) {
							int
									R =
											mRouterCoordinates[ mCoreToRouter[*V] ].first;
							int
									C =
											mRouterCoordinates[ mCoreToRouter[*V] ].second;
							Cost += (*mSpec)[Edge(*V,NextNode)].PRJ(CommoditySet)GetAggregateBandwidthValue()
									*(fabs(i-R) + fabs(j-C));
						}
					}
				}
				if (Cost <= MinCost) {
					RouterFound = true;
					MinCost = Cost;
					Row = i;
					Column = j;
				}
			}
		}
	}

	if ( !RouterFound) {
		CosiLog << "A rotuer for "<< mSpec->GetName(NextNode)
				<< " coult not be found "<< endl;
	}

	Name ComponentName = (*mSpec)[NextNode].PRJ(Name)Get();
	Implementation* N = mPlt->GetImplementationInstance(ComponentName,
			NextNode, (*mSpec)[NextNode].PRJ(Position)Get(), TransferTable());
	N->SetName(NextNode, mSpec->GetName(NextNode));
	(*N)[NextNode].PRJ(IpGeometry)Set((*mSpec)[NextNode].PRJ(IpGeometry)Get());
	//Add the core to the implementation
	(*mI) = (*mI) + (*N);
	delete N;
	//Add the links
	string Out = (*mI)[NextNode].PRJ(Ports)GetOutputPortNotBound();
	if (Out == "") {
		CosiLog << "Node "<< NextNode
				<< " does not have an availalbe output port "<< endl;
	}
	string In = (*mI)[mIdMatrix[Row][Column]].PRJ(Ports)GetInputPortNotBound();
	if (In == "") {
		CosiLog << "Node "<< mIdMatrix[Row][Column]
				<< " does not have an availalbe input port "<< endl;
	}
	Implementation* L = mPlt->GetImplementationInstance(Type("Link"), NextNode,
			Out, mIdMatrix[Row][Column], In, WiringPath());
	(*mI) = (*mI) + (*L);
	delete L;
	Out = (*mI)[mIdMatrix[Row][Column]].PRJ(Ports)GetOutputPortNotBound();
	In = (*mI)[NextNode].PRJ(Ports)GetInputPortNotBound();
	if (Out == "") {
		CosiLog << "Node "<< mIdMatrix[Row][Column]
				<< " does not have an availalbe output port "<< endl;
	}
	if (In == "") {
		CosiLog << "Node "<< NextNode
				<< " does not have an availalbe input port "<< endl;
	}
	L = mPlt->GetImplementationInstance(Type("Link"), mIdMatrix[Row][Column],
			Out, NextNode, In, WiringPath());
	(*mI) = (*mI) + (*L);
	delete L;

	mRouterToCore[mIdMatrix[Row][Column]] = NextNode;
	mCoreToRouter[NextNode] = mIdMatrix[Row][Column];

	CosiLog << "Node "<< mSpec->GetName(NextNode) << " mapped to ("<< Row<< ","
			<< Column << ")"<< endl;

}
bool MeshTopology::FindRoutes() {

	IdGraph::v_iterator S, D;
	for (S = mSpec->v_begin() ; S!= mSpec->v_end() ; S++) {
		for (D = mSpec->v_begin() ; D!= mSpec->v_end() ; D++) {
			//find quadrant graph for each source destination pair
			//build a graph with weights
			if (mSpec->InE(*S, *D) ) {
				int Sr = mCoreToRouter[*S];
				int Rs = mRouterCoordinates[Sr].first;
				int Cs = mRouterCoordinates[Sr].second;
				int Dr = mCoreToRouter[*D];
				int Rd = mRouterCoordinates[Dr].first;
				int Cd = mRouterCoordinates[Dr].second;
				int RowL, RowR, ColT, ColB;
				if (Rs < Rd) {
					RowL = Rs;
					RowR = Rd;
				} else {
					RowL = Rd;
					RowR = Rs;
				}
				if (Cs < Cd) {
					ColT = Cd;
					ColB = Cs;
				} else {
					ColT = Cs;
					ColB = Cd;
				}
				cosi::commstruct::graph::RealWeightedGraph * G =
						new cosi::commstruct::graph::RealWeightedGraph;
				for (int i = RowL; i <= RowR; i ++) {
					for (int j = ColB; j <= ColT; j++) {
						//add the core if the router is mapped
						//if (mRouterToCore.find(mIdMatrix[i][j])
						//		!= mRouterToCore.end())
						//	G->AddVertex(mRouterToCore[mIdMatrix[i][j]]) ;
						//add the router
						G->AddVertex(mIdMatrix[i][j]);
						//connect core and router
						//G->AddEdge(mRouterToCore[mIdMatrix[i][j]], mIdMatrix[i][j]);
						//G->AddEdge(mIdMatrix[i][j], mRouterToCore[mIdMatrix[i][j]]);
					}
				}
				//Add the column links
				for (int i = RowL; i <= RowR; i ++) {
					for (int j = ColB; j < ColT; j++) {
						G->AddEdge(mIdMatrix[i][j], mIdMatrix[i][j+1]);
						Bandwidth B = (*mI)[Edge(mIdMatrix[i][j], mIdMatrix[i][j+1])].PRJ(CommoditySet)GetAggregateBandwidth() ;
						if (B.GetValue() == 0) {
							(*G)[Edge(mIdMatrix[i][j], mIdMatrix[i][j+1])].PRJ(RealQuantity)SetValue(1);
						} else {
							(*G)[Edge(mIdMatrix[i][j], mIdMatrix[i][j+1])].PRJ(RealQuantity)SetValue(B.GetValue());
						}
						B = (*mI)[Edge(mIdMatrix[i][j+1], mIdMatrix[i][j])].PRJ(CommoditySet)GetAggregateBandwidth() ;
						G->AddEdge(mIdMatrix[i][j+1], mIdMatrix[i][j]);
						if (B.GetValue() == 0) {
							(*G)[Edge(mIdMatrix[i][j+1], mIdMatrix[i][j])].PRJ(RealQuantity)SetValue(1);
						} else {
							(*G)[Edge(mIdMatrix[i][j+1], mIdMatrix[i][j])].PRJ(RealQuantity)SetValue(B.GetValue());
						}
					}
				}
				//Add the row links
				for (int j = ColB; j <= ColT; j ++) {
					for (int i = RowL; i < RowR; i++) {
						G->AddEdge(mIdMatrix[i][j], mIdMatrix[i+1][j]);
						Bandwidth B = (*mI)[Edge(mIdMatrix[i][j], mIdMatrix[i+1][j])].PRJ(CommoditySet)GetAggregateBandwidth() ;
						if (B.GetValue() == 0) {
							(*G)[Edge(mIdMatrix[i][j], mIdMatrix[i+1][j])].PRJ(RealQuantity)SetValue(1);
						} else {
							(*G)[Edge(mIdMatrix[i][j], mIdMatrix[i+1][j])].PRJ(RealQuantity)SetValue(B.GetValue());
						}
						B = (*mI)[Edge(mIdMatrix[i+1][j], mIdMatrix[i][j])].PRJ(CommoditySet)GetAggregateBandwidth() ;
						G->AddEdge(mIdMatrix[i+1][j], mIdMatrix[i][j]);
						if (B.GetValue() == 0) {
							(*G)[Edge(mIdMatrix[i+1][j], mIdMatrix[i][j])].PRJ(RealQuantity)SetValue(1);
						} else {
							(*G)[Edge(mIdMatrix[i+1][j], mIdMatrix[i][j])].PRJ(RealQuantity)SetValue(B.GetValue());
						}
					}
				}

				//add the source and destination
				G->AddVertex(*S) ;
				G->AddEdge(*S, mCoreToRouter[*S]) ;
				(*G)[Edge(*S,mCoreToRouter[*S])].PRJ(RealQuantity)SetValue(1);
				G->AddVertex(*D) ;
				G->AddEdge(mCoreToRouter[*D], *D);
				(*G)[Edge(mCoreToRouter[*D],*D)].PRJ(RealQuantity)SetValue(1);

				//find the shortest path
				cosi::algorithms::graph::ShortestPath * Sp =
						new cosi::algorithms::graph::ShortestPath(G);
				Sp->Run(*S, *D) ;
				list<Vertex> Path = Sp->Get() ;

				mRoutingCost += (*mSpec)[Edge(*S,*D)].PRJ(CommoditySet)GetAggregateBandwidthValue() * Path.size() ;

				delete Sp;
				delete G;

				//Add the path to the implementation
				CommoditySet Commodities = (*mSpec)[Edge(*S,*D)].PRJ(CommoditySet)Get() ;
				list<Vertex>::iterator Current, Next;
				Current = Path.begin();
				Next = Current;
				while ( *Current != *D) {

					Next ++;

					//add commodities to the edge
					CommoditySet::iterator It;
					for (It = Commodities.Begin() ; It != Commodities.End() ; It++) {
						(*mI)[Edge(*Current,*Next)].PRJ(CommoditySet)Add(*It) ;
					}
					string P = (*mI)[*Current].PRJ(Ports)IsBoundTo(Edge(*Current, *Next)) ;
					(*mI)[*Current].PRJ(TransferTable)AddEntry(mSpec->GetName(*S),
							mSpec->GetName(*D) , P);
					Current = Next;
				}
			}
		}
	}

	return true;
}

bool MeshTopology::FindRoutesIncremental() {

	WiringPath P;
	P.AppendPosition(Position(0, 0, 0)) ;
	P.AppendPosition(Position(1, 0, 0)) ;
	Commodity C;
	C.SetValue("", "", 0, 0, 1, 1);
	CommoditySet Cs;
	Cs.Add(C) ;
	double InstallationCost = mPlt->GetStaticPower(Name("OCP-Link"), P);
	double UtilizationCost = mPlt->GetDynamicPower(Name("OCP-Link"), P, Cs);

	IdGraph::v_iterator S, D;
	for (S = mSpec->v_begin() ; S!= mSpec->v_end() ; S++) {
		for (D = mSpec->v_begin() ; D!= mSpec->v_end() ; D++) {
			//find quadrant graph for each source destination pair
			//build a graph with weights
			if (mSpec->InE(*S, *D) ) {
				int Sr = mCoreToRouter[*S];
				int Rs = mRouterCoordinates[Sr].first;
				int Cs = mRouterCoordinates[Sr].second;
				int Dr = mCoreToRouter[*D];
				int Rd = mRouterCoordinates[Dr].first;
				int Cd = mRouterCoordinates[Dr].second;
				int RowL, RowR, ColT, ColB;
				if (Rs < Rd) {
					RowL = Rs;
					RowR = Rd;
				} else {
					RowL = Rd;
					RowR = Rs;
				}
				if (Cs < Cd) {
					ColT = Cd;
					ColB = Cs;
				} else {
					ColT = Cs;
					ColB = Cd;
				}
				cosi::commstruct::graph::RealWeightedGraph * G =
						new cosi::commstruct::graph::RealWeightedGraph;
				for (int i = RowL; i <= RowR; i ++) {
					for (int j = ColB; j <= ColT; j++) {
						//add the core if the router is mapped
						//if (mRouterToCore.find(mIdMatrix[i][j])
						//		!= mRouterToCore.end())
						//	G->AddVertex(mRouterToCore[mIdMatrix[i][j]]) ;
						//add the router
						G->AddVertex(mIdMatrix[i][j]);
						//connect core and router
						//G->AddEdge(mRouterToCore[mIdMatrix[i][j]], mIdMatrix[i][j]);
						//G->AddEdge(mIdMatrix[i][j], mRouterToCore[mIdMatrix[i][j]]);
					}
				}
				//Add the column links
				for (int i = RowL; i <= RowR; i ++) {
					for (int j = ColB; j < ColT; j++) {
						G->AddEdge(mIdMatrix[i][j], mIdMatrix[i][j+1]);
						Bandwidth B = (*mI)[Edge(mIdMatrix[i][j], mIdMatrix[i][j+1])].PRJ(CommoditySet)GetAggregateBandwidth() ;
						if (B.GetValue() == 0) {
							(*G)[Edge(mIdMatrix[i][j], mIdMatrix[i][j+1])].PRJ(RealQuantity)SetValue(InstallationCost
									+ UtilizationCost
											*(*mSpec)[Edge(*S,*D)].PRJ(CommoditySet)GetAggregateBandwidthValue());
						} else {
							(*G)[Edge(mIdMatrix[i][j], mIdMatrix[i][j+1])].PRJ(RealQuantity)SetValue(UtilizationCost*(*mSpec)[Edge(*S,*D)].PRJ(CommoditySet)GetAggregateBandwidthValue());
						}
						B = (*mI)[Edge(mIdMatrix[i][j+1], mIdMatrix[i][j])].PRJ(CommoditySet)GetAggregateBandwidth() ;
						G->AddEdge(mIdMatrix[i][j+1], mIdMatrix[i][j]);
						if (B.GetValue() == 0) {
							(*G)[Edge(mIdMatrix[i][j+1], mIdMatrix[i][j])].PRJ(RealQuantity)SetValue(InstallationCost
									+ UtilizationCost
											*(*mSpec)[Edge(*S,*D)].PRJ(CommoditySet)GetAggregateBandwidthValue());
						} else {
							(*G)[Edge(mIdMatrix[i][j+1], mIdMatrix[i][j])].PRJ(RealQuantity)SetValue(UtilizationCost*(*mSpec)[Edge(*S,*D)].PRJ(CommoditySet)GetAggregateBandwidthValue());
						}
					}
				}
				//Add the row links
				for (int j = ColB; j <= ColT; j ++) {
					for (int i = RowL; i < RowR; i++) {
						G->AddEdge(mIdMatrix[i][j], mIdMatrix[i+1][j]);
						Bandwidth B = (*mI)[Edge(mIdMatrix[i][j], mIdMatrix[i+1][j])].PRJ(CommoditySet)GetAggregateBandwidth() ;
						if (B.GetValue() == 0) {
							(*G)[Edge(mIdMatrix[i][j], mIdMatrix[i+1][j])].PRJ(RealQuantity)SetValue(InstallationCost
									+ UtilizationCost
											*(*mSpec)[Edge(*S,*D)].PRJ(CommoditySet)GetAggregateBandwidthValue());
						} else {
							(*G)[Edge(mIdMatrix[i][j], mIdMatrix[i+1][j])].PRJ(RealQuantity)SetValue(UtilizationCost*(*mSpec)[Edge(*S,*D)].PRJ(CommoditySet)GetAggregateBandwidthValue());
						}
						B = (*mI)[Edge(mIdMatrix[i+1][j], mIdMatrix[i][j])].PRJ(CommoditySet)GetAggregateBandwidth() ;
						G->AddEdge(mIdMatrix[i+1][j], mIdMatrix[i][j]);
						if (B.GetValue() == 0) {
							(*G)[Edge(mIdMatrix[i+1][j], mIdMatrix[i][j])].PRJ(RealQuantity)SetValue(InstallationCost
									+ UtilizationCost
											*(*mSpec)[Edge(*S,*D)].PRJ(CommoditySet)GetAggregateBandwidthValue());
						} else {
							(*G)[Edge(mIdMatrix[i+1][j], mIdMatrix[i][j])].PRJ(RealQuantity)SetValue(UtilizationCost*(*mSpec)[Edge(*S,*D)].PRJ(CommoditySet)GetAggregateBandwidthValue());
						}
					}
				}

				//add the source and destination
				G->AddVertex(*S) ;
				G->AddEdge(*S, mCoreToRouter[*S]) ;
				(*G)[Edge(*S,mCoreToRouter[*S])].PRJ(RealQuantity)SetValue(1);
				G->AddVertex(*D) ;
				G->AddEdge(mCoreToRouter[*D], *D);
				(*G)[Edge(mCoreToRouter[*D],*D)].PRJ(RealQuantity)SetValue(1);

				//find the shortest path
				cosi::algorithms::graph::ShortestPath * Sp =
						new cosi::algorithms::graph::ShortestPath(G);
				Sp->Run(*S, *D) ;
				list<Vertex> Path = Sp->Get() ;

				if (Path.size() == 0) {
					cout << "WARNING : Returning a path of length 0"<< endl;
				}

				delete Sp;
				delete G;

				//Add the path to the implementation
				CommoditySet Commodities = (*mSpec)[Edge(*S,*D)].PRJ(CommoditySet)Get() ;
				list<Vertex>::iterator Current, Next;
				Current = Path.begin();
				Next = Current;
				while ( *Current != *D) {

					Next++;

					//add commodities to the edge
					CommoditySet::iterator It;
					for (It = Commodities.Begin() ; It != Commodities.End() ; It++) {
						(*mI)[Edge(*Current,*Next)].PRJ(CommoditySet)Add(*It) ;
					}
					string P = (*mI)[*Current].PRJ(Ports)IsBoundTo(Edge(*Current, *Next)) ;
					(*mI)[*Current].PRJ(TransferTable)AddEntry(mSpec->GetName(*S),
							mSpec->GetName(*D) , P);
					Current = Next;
				}
			}
		}
	}

	//ComputeCost
	for (S = mI->v_begin() ; S != mI->v_end() ; S++) {
		for (D = mI->v_begin() ; D != mI->v_end() ; D++) {
			if (mI->InE(*S, *D)) {
				mRoutingCost += InstallationCost + UtilizationCost * (*mI)[Edge(*S,*D)].PRJ(CommoditySet)GetAggregateBandwidthValue() ;
			}
		}
	}
	return true;
}

bool MeshTopology::CleanUp() {
	//remove all the links that do not carry any commodity
	IdGraph::v_iterator U, V;
	for (U = mI->v_begin() ; U != mI->v_end() ; U++) {
		for (V = mI->v_begin() ; V != mI->v_end() ; V++) {
			if (mI->InE(*U, *V)) {
				cout << "Checking " << *U << " " << *V << endl;
				cout << (*mI)[Edge(*U,*V)].PRJ(CommoditySet)Get() << endl;
				cout << "TotalBandwidth " << (*mI)[Edge(*U,*V)].PRJ(CommoditySet)GetAggregateBandwidthValue() << endl;
				if ((*mI)[Edge(*U,*V)].PRJ(CommoditySet)GetAggregateBandwidthValue() == 0) {
					//remove the edge
					mI->DeleteEdge(*U, *V) ;
					string P = (*mI)[*U].PRJ(Ports)IsBoundTo(Edge(*U, *V)) ;
					(*mI)[*U].PRJ(Ports)UnBound(P) ;
					cout << "Unbound " << P << " on " << *U << endl;
					P = (*mI)[*V].PRJ(Ports)IsBoundTo(Edge(*U, *V),Interface::IN) ;
					(*mI)[*V].PRJ(Ports)UnBound(P) ;
					cout << "Unbound " << P << " on " << *V << endl;
				}
			}
		}
	}
	//remove isolated nodes
	for (U = mI->v_begin() ; U != mI->v_end() ; U++) {
		if (mI->InDegree(*U) == 0&& mI->OutDegree(*U) == 0) {
			if (mSpec->InV(*U)) {
				cout << "WARNING : Isolated core"<< endl;
			} else {
				mI->DeleteVertex(*U) ;
			}
		}
	}

	//Need to install components with the right number of inputs and outputs
	//Simply copy the IP cores and install routers with the
	//right input and output degree
	Implementation* Clean = new Implementation("");
	for (U = mI->v_begin() ; U != mI->v_end() ; U++) {
		if (mSpec->InV(*U)) {
			Clean->AddVertex(*U) ;
			(*Clean)[*U] = (*mI)[*U];
			Clean->SetName(*U, mI->GetName(*U)) ;
		} else {
			int Nin = mI->InDegree(*U) ;
			int Nout = mI->OutDegree(*U) ;
			Implementation* N = mPlt->GetImplementationInstance(Nin, Nout, *U,
					Position(), TransferTable()) ;
			(*Clean) = (*Clean) + (*N);
			Clean->SetName(*U, mI->GetName(*U)) ;
			delete N;
		}
	}
	//Need to add edges
	//Be careful to the port binding
	//Need to change port binding
	//and regenerate transfer tables
	for (U = mI->v_begin() ; U != mI->v_end() ; U++) {
		for (V = mI->v_begin() ; V != mI->v_end() ; V++) {
			if (mI->InE(*U, *V)) {
				//Copy the edge
				Clean->AddEdge(*U, *V) ;
				(*Clean)[Edge(*U,*V)] = (*mI)[Edge(*U,*V)];
				//bind ports to the edge
				if ( !mSpec->InV(*U) ) {
					string NewOut = (*Clean)[*U].PRJ(Ports)GetOutputPortNotBound() ;
					(*Clean)[*U].PRJ(Ports)BoundTo(NewOut, Edge(*U, *V));
					string OldOut = (*mI)[*U].PRJ(Ports)IsBoundTo(Edge(*U, *V));
					TransferTable T = (*mI)[*U].PRJ(TransferTable)Get();
					TransferTable::iterator It;
					for (It = T.TransferTableBegin() ; It
							!= T.TransferTableEnd() ; It++) {
						if (It->second == OldOut) {
							(*Clean)[*U].PRJ(TransferTable)AddEntry((It->first).first, (It->first).second, NewOut);
						}
					}
				}
				if ( !mSpec->InV(*V)) {
					string NewIn = (*Clean)[*V].PRJ(Ports)GetInputPortNotBound() ;
					(*Clean)[*V].PRJ(Ports)BoundTo(NewIn, Edge(*U, *V));
				}
			}
		}
	}

	delete mI;
	mI = new Implementation("");
	(*mI) = (*Clean);
	delete Clean;
	return true;
}

bool MeshTopology::Floorplan() {
	//run the floorplanner
	//Generate the floorplan information
	ParquetInputGen(Configuration->GetTmp() + "/Soc.blocks",
			Configuration->GetTmp() + "/Soc.nets", Configuration->GetTmp()
					+ "/Soc.wts", Configuration->GetTmp() + "/Soc.pl", mI) ;
	string ParquetCmdLine;
	ParquetCmdLine = "export LD_LIBRARY_PATH=";
	ParquetCmdLine+= Configuration->GetParquetRoot();
	ParquetCmdLine+=";";
	ParquetCmdLine+= Configuration->GetParquetRoot();
	ParquetCmdLine+="/" + Configuration->GetParquetExec();
	ParquetCmdLine += " -f " + Configuration->GetTmp() + "/Soc";
	ParquetCmdLine+=" -minWL -takePl -compact "+ Configuration->GetTmp()
			+ "/Soc.pl -soft -savePl "+ Configuration->GetTmp()
			+ "/meshsoc -plot > parquetlog.txt";
	system(ParquetCmdLine.c_str() );

	mPlacement = new PlacementParser( Configuration->GetTmp( ) + "/meshsoc.pl" , 0.15 );
	mPlacement->Parse();

	//Get the geometry of all ips
	IdGraph::v_iterator U, V;
	for (U = mI->v_begin() ; U != mI->v_end() ; U++) {
		PlacedIpCore C = mPlacement->GetIpCore(mI->GetName(*U)) ;
		IpGeometry G;
		G.SetXl(RealQuantity(C.GetBottomLeftX()));
		G.SetXr(RealQuantity(C.GetTopRightX()));
		G.SetYt(RealQuantity(C.GetTopRightY()));
		G.SetYb(RealQuantity(C.GetBottomLeftY())) ;
		G.SetArea(RealQuantity(C.GetArea()));
		Position P(C.GetBottomLeftX(), C.GetBottomLeftY(), 0);
		(*mI)[*U].PRJ(Position)Set(P);
		(*mI)[*U].PRJ(IpGeometry)Set(G);

	}
	for (U = mI->v_begin() ; U != mI->v_end() ; U++) {
		for (V = mI->v_begin() ; V != mI->v_end() ; V++) {
			if (mI->InE(*U, *V)) {
				WiringPath P;
				P.AppendPosition((*mI)[*U].PRJ(Position)Get()) ;
				P.AppendPosition((*mI)[*V].PRJ(Position)Get()) ;
				(*mI)[Edge(*U,*V)].PRJ(WiringPath)Set(P) ;
			}
		}
	}

	return true;
}

void MeshTopology::Swap(int U, int V) {

	Implementation* TmpI = new Implementation("");
	(*TmpI) = (*mI);
	delete mI;
	mI = new Implementation("" );
	IdGraph::v_iterator i, j;
	for (i = TmpI->v_begin() ; i != TmpI->v_end() ; i++) {
		if ( *i == U) {
			mI->AddVertex(V) ;
			(*mI)[V] = (*TmpI)[U];
			mI->SetName(V, TmpI->GetName(V)) ;
			(*mI)[V].PRJ(TransferTable)Set(TransferTable()) ;
		} else if ( *i == V) {
			mI->AddVertex(U) ;
			(*mI)[U] = (*TmpI)[V];
			mI->SetName(U, TmpI->GetName(U)) ;
			(*mI)[U].PRJ(TransferTable)Set(TransferTable()) ;
		} else {
			mI->AddVertex(*i) ;
			(*mI)[*i] = (*TmpI)[*i];
			mI->SetName(*i, TmpI->GetName(*i)) ;
			(*mI)[*i].PRJ(TransferTable)Set(TransferTable()) ;
		}
	}
	for (i = TmpI->v_begin() ; i != TmpI->v_end() ; i++) {
		for (j = TmpI->v_begin() ; j != TmpI->v_end() ; j++) {
			if (TmpI->InE(*i, *j)) {
				Edge E;
				if ( *i == U) {
					E.U(V) ;
				} else if ( *i == V) {
					E.U(U) ;
				} else {
					E.U(*i) ;
				}
				if ( *j == U) {
					E.V(V) ;
				} else if ( *j == V) {
					E.V(U) ;
				} else {
					E.V(*j) ;
				}
				mI->AddEdge(E.U(), E.V()) ;
				(*mI)[E] = (*TmpI)[Edge(*i,*j)];
				(*mI)[E].PRJ(CommoditySet)Set(CommoditySet()) ;
				string P = (*TmpI)[*i].PRJ(Ports)IsBoundTo(Edge(*i, *j));
				if (P.size() == 0) {
					cout << "There are no ports bound to " << *i << " " << *j << " on " << *i << endl;
				}
				(*mI)[E.U()].PRJ(Ports)BoundTo(P, E);
				P = (*TmpI)[*j].PRJ(Ports)IsBoundTo(Edge(*i, *j),Interface::IN);
				if (P.size() == 0) {
					cout << "There are no ports bound to " << *i << " " << *j << " on " << *j << endl;
				}
				(*mI)[E.V()].PRJ(Ports)BoundTo(P, E);
			}
		}
	}

	delete TmpI;

	//CoreToRouter
	int RouterU = mCoreToRouter[U];
	int RouterV = mCoreToRouter[V];
	mCoreToRouter[U] = RouterV;
	mCoreToRouter[V] = RouterU;
	//RouterToCore
	mRouterToCore[RouterU] = V;
	mRouterToCore[RouterV] = U;

}

double MeshTopology::ComputeCost() {
	//this is simply the communication cost that can be obtained by the network cost
	//function
	NetworkCost* Nc = new NetworkCost;
	Nc->Initialize(mI, 0x0, mPlt);
	Nc->Run();
	double RetVal = Nc->GetTotalPower();
	delete Nc;
	return RetVal;
}

cosi::io::onchipcommunication::PlacementParser* MeshTopology::GetPlacement() {
	return mPlacement;
}

cosi::commstruct::onchipcommunication::Implementation* MeshTopology::GetImplementation() {
	return mI;
}

}

}

}
