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
#include "HierarchicalNetwork.h"

namespace cosi {

namespace algorithms {

namespace onchipcommunication {

using namespace cosi::commstruct::onchipcommunication;
using namespace cosi::platforms::onchipcommunication;
using namespace std;

HierarchicalNetwork::HierarchicalNetwork(Specification* Spec, Platform* Plt) {
	mPlt = Plt;
	mSpec = Spec;
}

HierarchicalNetwork::~HierarchicalNetwork() {
}

cosi::io::onchipcommunication::PlacementParser* HierarchicalNetwork::GetPlacement() {
	return mPlacement;
}
cosi::commstruct::onchipcommunication::Implementation* HierarchicalNetwork::GetImplementation() {
	return mI;
}

bool HierarchicalNetwork::Run() {

	vector<set<int> > Partitions = GenerateHyperedges(mSpec) ;
	vector<Implementation* > Level1Net(Partitions.size());
	vector<int> Level1Centers(Partitions.size());
	int CenterId = mSpec->GetLastId() + 1;
	for (unsigned int i = 0; i < Partitions.size() ; i++) {
		CosiLog <<"Generating star with center "<< CenterId << endl;
		Implementation* I = StarInstance(Partitions[i], CenterId,
				Partitions[i].size()+Partitions.size()-1) ;
		Level1Net[i] = I;
		Level1Centers[i] = CenterId;
		CenterId++;
	}

	CosiLog << "Generating the complete graph "<< endl;
	Implementation* Net = CompleteGraph(Level1Net, Level1Centers);
	CosiLog <<"Finding the routes "<< endl;
	FindRoutes(Net, mSpec) ;
	CosiLog << "Cleaning Up "<< endl;
	mI = CleanUp(Net) ;

	Floorplan(mI) ;

	//release memory
	for (unsigned int i = 0; i < Partitions.size() ; i++) {
		delete Level1Net[i];
	}
	delete Net;

}

std::vector<std::set<int> > HierarchicalNetwork::GenerateHyperedges(
		Specification* Spec) {
	int Ncores = Spec->Order() ;
	int Nparts = Ncores/6+ 1;
	return cosi::io::onchipcommunication::ShmetisPartition(Spec, Nparts, 1) ;

}
Implementation* HierarchicalNetwork::StarInstance(std::set<int> P,
		int CenterId, int Nports) {

	//Instantiate all the cores
	set<int>::iterator Core;
	Implementation* RetVal = new Implementation("");
	for (Core = P.begin() ; Core != P.end() ; Core++) {
		Name ComponentName = (*mSpec)[*Core].PRJ(Name)Get();
		Implementation* N = mPlt->GetImplementationInstance(ComponentName,
				*Core, (*mSpec)[*Core].PRJ(Position)Get(), TransferTable());
		N->SetName(*Core, mSpec->GetName(*Core));
		(*N)[*Core].PRJ(IpGeometry)Set((*mSpec)[*Core].PRJ(IpGeometry)Get());
		//Add the core to the implementation
		(*RetVal) = (*RetVal) + (*N);
		delete N;
	}
	//Instantiate the central router
	{
		Implementation* N = mPlt->GetImplementationInstance(Nports, Nports,
				CenterId, Position(), TransferTable());
		(*RetVal) = (*RetVal) + (*N);
		stringstream s;
		s << "Router_"<< CenterId;
		RetVal->SetName(CenterId, s.str());
		delete N;
	}
	//link the Ip to the central router
	for (Core = P.begin() ; Core != P.end() ; Core++) {
		//Add the links
		string Out = (*RetVal)[*Core].PRJ(Ports)GetOutputPortNotBound();
		string In = (*RetVal)[CenterId].PRJ(Ports)GetInputPortNotBound();
		Implementation* L = mPlt->GetImplementationInstance(Type("Link"),
				*Core, Out, CenterId, In, WiringPath());
		(*RetVal) = (*RetVal) + (*L);
		delete L;
		Out = (*RetVal)[CenterId].PRJ(Ports)GetOutputPortNotBound();
		In = (*RetVal)[*Core].PRJ(Ports)GetInputPortNotBound();
		L = mPlt->GetImplementationInstance(Type("Link"), CenterId, Out, *Core,
				In, WiringPath());
		(*RetVal) = (*RetVal) + (*L);
		delete L;
	}

	return RetVal;

}
Implementation* HierarchicalNetwork::CompleteGraph(vector<Implementation* > I,
		vector<int> C) {

	Implementation* RetVal = new Implementation("");
	for (int i = 0; i < I.size() ; i++) {
		CosiLog << "Adding star "<< i << endl;
		(*RetVal) = (*RetVal) + (*I[i] );
	}

	CosiLog << RetVal->Print() ;

	//need to generate a complete graph on the centers
	for (int i = 0; i < C.size() ; i++) {
		for (int j = 0; j < C.size() ; j++) {
			if (i != j) {
				CosiLog << "("<< C[i]<< ","<< C[j]<< ") ";
				string Out = (*RetVal)[C[i]].PRJ(Ports)GetOutputPortNotBound();
				string In = (*RetVal)[C[j]].PRJ(Ports)GetInputPortNotBound();
				Implementation* L = mPlt->GetImplementationInstance(
						Type("Link"), C[i], Out, C[j], In, WiringPath());
				(*RetVal) = (*RetVal) + (*L);
				delete L;
			}
		}
		CosiLog <<endl;
	}
	return RetVal;

}

Implementation* HierarchicalNetwork::Ring(vector<Implementation* > I,
		vector<int> C) {

	Implementation* RetVal = new Implementation("");
	for (int i = 0; i < I.size() ; i++) {
		CosiLog << "Adding star "<< i << endl;
		(*RetVal) = (*RetVal) + (*I[i] );
	}

	CosiLog << RetVal->Print() ;

	//need to generate a complete graph on the centers
	for (int i = 0; i < C.size() ; i++) {
		for (int j = 0; j < C.size() ; j++) {
			if (i != j) {
				CosiLog << "("<< C[i]<< ","<< C[j]<< ") ";
				string Out = (*RetVal)[C[i]].PRJ(Ports)GetOutputPortNotBound();
				string In = (*RetVal)[C[j]].PRJ(Ports)GetInputPortNotBound();
				Implementation* L = mPlt->GetImplementationInstance(
						Type("Link"), C[i], Out, C[j], In, WiringPath());
				(*RetVal) = (*RetVal) + (*L);
				delete L;
			}
		}
		CosiLog <<endl;
	}
	return RetVal;

}

void HierarchicalNetwork::FindRoutes(Implementation* I, Specification* Spec) {

	WiringPath P;
	P.AppendPosition(Position(0, 0, 0)) ;
	P.AppendPosition(Position(1, 0, 0)) ;
	Commodity C;
	C.SetValue("", "", 0, 0, 1, 1);
	CommoditySet Cs;
	Cs.Add(C) ;
	double InstallationCost = mPlt->GetStaticPower(Name("OCP-Link"), P);
	double UtilizationCost = mPlt->GetDynamicPower(Name("OCP-Link"), P, Cs);

	//build the abstract graph
	cosi::commstruct::graph::RealWeightedGraph * G =
			new cosi::commstruct::graph::RealWeightedGraph;

	IdGraph::v_iterator U, V;
	for (U = I->v_begin() ; U != I->v_end() ; U++) {
		G->AddVertex(*U) ;
	}

	for (U = I->v_begin() ; U != I->v_end() ; U++) {
		for (V = I->v_begin() ; V != I->v_end() ; V++) {
			if (I->InE(*U, *V)) {
				G->AddEdge(*U, *V) ;
			}
		}
	}

	//for all constraints in  order find a route
	set<pair<double, pair<int,int > > > CommodityQueue;
	for (U = mSpec->v_begin() ; U != mSpec->v_end() ; U++) {
		for (V = mSpec->v_begin() ; V != mSpec->v_end() ; V++) {
			if (mSpec->InE(*U, *V)) {
				double W = -(*mSpec)[Edge(*U,*V)].PRJ(CommoditySet)GetAggregateBandwidthValue() ;
				CommodityQueue.insert(pair<double, pair<int,int> >(W, pair<int,
						int>(*U, *V)));
			}
		}
	}

	set<pair<double,pair<int,int> > >::iterator Cit;
	for (Cit = CommodityQueue.begin() ; Cit != CommodityQueue.end() ; Cit++) {
		double W = -(*Cit).first;
		int S = (*Cit).second.first;
		int D = (*Cit).second.second;

		//set up the edge weights of G
		for (U = I->v_begin() ; U != I->v_end() ; U++) {
			for (V = I->v_begin() ; V != I->v_end() ; V++) {
				if (I->InE(*U, *V)) {
					Bandwidth B = (*I)[Edge(*U,*V)].PRJ(CommoditySet)GetAggregateBandwidth() ;
					if (B.GetValue() == 0) {
						(*G)[Edge(*U,*V)].PRJ(RealQuantity)SetValue(InstallationCost
								+ UtilizationCost *W);
					} else {
						(*G)[Edge(*U,*V)].PRJ(RealQuantity)SetValue(UtilizationCost*W);
					}
				}
			}
		}
		//find the shortest path
		cout << "Finding route from "<< S << " to "<< D << endl;
		cosi::algorithms::graph::ShortestPath * Sp =
				new cosi::algorithms::graph::ShortestPath(G);
		Sp->Run(S, D) ;
		list<Vertex> Path = Sp->Get() ;

		if (Path.size() == 0) {
			cout << "WARNING : Returning a path of length 0"<< endl;
		}

		delete Sp;

		//Add the path to the implementation
		CommoditySet Commodities = (*mSpec)[Edge(S,D)].PRJ(CommoditySet)Get() ;
		list<Vertex>::iterator Current, Next;
		Current = Path.begin();
		Next = Current;
		while ( *Current != D) {

			Next++;

			//add commodities to the edge
			CommoditySet::iterator It;
			for (It = Commodities.Begin() ; It != Commodities.End() ; It++) {
				(*I)[Edge(*Current,*Next)].PRJ(CommoditySet)Add(*It) ;
			}
			string P = (*I)[*Current].PRJ(Ports)IsBoundTo(Edge(*Current, *Next)) ;
			(*I)[*Current].PRJ(TransferTable)AddEntry(mSpec->GetName(S),
					mSpec->GetName(D) , P);
			Current = Next;
		}

	}
	delete G;
}
cosi::commstruct::onchipcommunication::Implementation* HierarchicalNetwork::CleanUp(
		cosi::commstruct::onchipcommunication::Implementation* Net) {
	//remove all the links that do not carry any commodity
	IdGraph::v_iterator U, V;
	for (U = Net->v_begin() ; U != Net->v_end() ; U++) {
		for (V = Net->v_begin() ; V != Net->v_end() ; V++) {
			if (Net->InE(*U, *V)) {
				if ((*Net)[Edge(*U,*V)].PRJ(CommoditySet)GetAggregateBandwidthValue() == 0) {
					//remove the edge
					Net->DeleteEdge(*U, *V) ;
					string P = (*Net)[*U].PRJ(Ports)IsBoundTo(Edge(*U, *V)) ;
					(*Net)[*U].PRJ(Ports)UnBound(P) ;
					cout << "Unbound " << P << " on " << *U << endl;
					P = (*Net)[*V].PRJ(Ports)IsBoundTo(Edge(*U, *V), Interface::IN) ;
					(*Net)[*V].PRJ(Ports)UnBound(P) ;
					cout << "Unbound " << P << " on " << *V << endl;
				}
			}
		}
	}
	//remove isolated nodes
	for (U = Net->v_begin() ; U != Net->v_end() ; U++) {
		if (Net->InDegree(*U) == 0&& Net->OutDegree(*U) == 0) {
			if (mSpec->InV(*U)) {
				cout << "WARNING : Isolated core"<< endl;
			} else {
				Net->DeleteVertex(*U) ;
			}
		}
	}

	//Need to install components with the right number of inputs and outputs
	//Simply copy the IP cores and install routers with the
	//right input and output degree
	Implementation* Clean = new Implementation("");
	for (U = Net->v_begin() ; U != Net->v_end() ; U++) {
		if (mSpec->InV(*U)) {
			Clean->AddVertex(*U) ;
			(*Clean)[*U] = (*Net)[*U];
			Clean->SetName(*U, Net->GetName(*U)) ;
		} else {
			int Nin = Net->InDegree(*U) ;
			int Nout = Net->OutDegree(*U) ;
			Implementation* N = mPlt->GetImplementationInstance(Nin, Nout, *U,
					Position(), TransferTable()) ;
			(*Clean) = (*Clean) + (*N);
			Clean->SetName(*U, Net->GetName(*U)) ;
			delete N;
		}
	}
	//Need to add edges
	//Be careful to the port binding
	//Need to change port binding
	//and regenerate transfer tables
	for (U = Net->v_begin() ; U != Net->v_end() ; U++) {
		for (V = Net->v_begin() ; V != Net->v_end() ; V++) {
			if (Net->InE(*U, *V)) {
				//Copy the edge
				Clean->AddEdge(*U, *V) ;
				(*Clean)[Edge(*U,*V)] = (*Net)[Edge(*U,*V)];
				//bind ports to the edge
				if ( !mSpec->InV(*U) ) {
					string NewOut = (*Clean)[*U].PRJ(Ports)GetOutputPortNotBound() ;
					(*Clean)[*U].PRJ(Ports)BoundTo(NewOut, Edge(*U, *V));
					string OldOut = (*Net)[*U].PRJ(Ports)IsBoundTo(Edge(*U, *V));
					TransferTable T = (*Net)[*U].PRJ(TransferTable)Get();
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

	CosiLog << Clean->Print() ;

	return Clean;
}

void HierarchicalNetwork::Floorplan(Implementation* I) {
	//run the floorplanner
	//Generate the floorplan information
	ParquetInputGen(Configuration->GetTmp() + "/Soc.blocks",
			Configuration->GetTmp() + "/Soc.nets", Configuration->GetTmp()
					+ "/Soc.wts", Configuration->GetTmp() + "/Soc.pl", I) ;
	string ParquetCmdLine;
	ParquetCmdLine = "export LD_LIBRARY_PATH=";
	ParquetCmdLine+= Configuration->GetParquetRoot();
	ParquetCmdLine+=";";
	ParquetCmdLine+= Configuration->GetParquetRoot();
	ParquetCmdLine+="/" + Configuration->GetParquetExec();
	ParquetCmdLine += " -f " + Configuration->GetTmp() + "/Soc";
	ParquetCmdLine+=" -minWL -takePl -compact "+ Configuration->GetTmp()
			+ "/Soc.pl -soft -savePl "+ Configuration->GetTmp()
			+ "/hsoc -plot > parquetlog.txt";
	system(ParquetCmdLine.c_str() );

	mPlacement = new PlacementParser( Configuration->GetTmp( ) + "/hsoc.pl" , 0.15 );
	mPlacement->Parse();

	//Get the geometry of all ips
	IdGraph::v_iterator U, V;
	for (U = I->v_begin() ; U != I->v_end() ; U++) {
		PlacedIpCore C = mPlacement->GetIpCore(I->GetName(*U)) ;
		IpGeometry G;
		G.SetXl(RealQuantity(C.GetBottomLeftX()));
		G.SetXr(RealQuantity(C.GetTopRightX()));
		G.SetYt(RealQuantity(C.GetTopRightY()));
		G.SetYb(RealQuantity(C.GetBottomLeftY())) ;
		G.SetArea(RealQuantity(C.GetArea()));
		Position P(C.GetBottomLeftX(), C.GetBottomLeftY(), 0);
		(*I)[*U].PRJ(Position)Set(P);
		(*I)[*U].PRJ(IpGeometry)Set(G);

	}
	for (U = I->v_begin() ; U != I->v_end() ; U++) {
		for (V = I->v_begin() ; V != I->v_end() ; V++) {
			if (I->InE(*U, *V)) {
				WiringPath P;
				P.AppendPosition((*I)[*U].PRJ(Position)Get()) ;
				P.AppendPosition((*I)[*V].PRJ(Position)Get()) ;
				(*I)[Edge(*U,*V)].PRJ(WiringPath)Set(P) ;
			}
		}
	}

}

}

}

}
