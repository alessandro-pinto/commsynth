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
#include "StarTopologyGeneration.h"

namespace cosi {

namespace algorithms {

namespace onchipcommunication {

using namespace cosi::commstruct::onchipcommunication;
using namespace cosi::platforms::onchipcommunication;
using namespace cosi::library::onchipcommunication;
using namespace cosi::io::onchipcommunication;
using namespace std;

StarTopologyGeneration::StarTopologyGeneration(Specification* Spec,
		Platform* Plt) {
	mPlt = Plt ;
	mSpec = Spec ;
}

StarTopologyGeneration::~StarTopologyGeneration() {
}

bool StarTopologyGeneration::Run() {

	//Connect a router in the center of all ips

	CosiLog << mSpec->Print() << endl ;

	mI = new Implementation("");

	//Add all the IP cores from the specification
	IdGraph::v_iterator U, V;
	for (U = mSpec->v_begin() ; U != mSpec->v_end() ; U++) {
		//Set the component name. The component name should be
		//availalbe in the specificaiton for each IP core
		Name ComponentName = (*mSpec)[*U].PRJ(Name)Get();
		//Ask the platform to provide an instance of the core with the same
		//id of the specification.
		Implementation* N = mPlt->GetImplementationInstance(ComponentName, *U,
				(*mSpec)[*U].PRJ(Position)Get(), TransferTable());
		N->SetName(*U, mSpec->GetName(*U)) ;
		(*N)[*U].PRJ(IpGeometry)Set((*mSpec)[*U].PRJ(IpGeometry)Get()) ;
		//Add the core to the implementation
		(*mI) = (*mI) + (*N);
		delete N;
	}

	//Add the router to the implementation.
	//The id of the router is the first one tha is not used
	//by the specification.
	Implementation* N = mPlt->GetImplementationInstance(Type("Router"),
			mSpec->GetLastId()+1, Position(), TransferTable()) ;
	(*mI) = (*mI) + (*N);
	delete N;
	int RouterId = mSpec->GetLastId()+1;
	mI->SetName(RouterId, "CentralSwitch") ;

	CosiLog << mI->Print() << endl ;

	//Add inputs and outputs: a core may be connected to multiple
	//destination but we don't care, routing will take care of this
	for (U = mSpec->v_begin() ; U != mSpec->v_end() ; U++) {
		for (V = mSpec->v_begin() ; V != mSpec->v_end() ; V++) {
			//For all constraints
			if (mSpec->InE(*U, *V) ) {
				CosiLog << "Constraint "<< mSpec->GetName(*U) << " to "
						<< mSpec->GetName(*V) << endl ;
				//If there isn't a link between the source of the
				//constraint and the router, then add one
				if ( !mI->InE(*U, RouterId) ) {

					//PortU is the output port of the IP core
					//PortRin is the input port of the router
					//PortRout is the output port of the router
					//PortV is the input port of the destination IP core
					string PortU, PortRin;

					//Get the output port of the IP core that is not bound
					//to any link
					vector<string> OutU = (*mI)[*U].PRJ(Ports)GetNotBoundOut();
					//IF there are not free ports then return
					if (OutU.size() == 0) {
						CosiLog << "Core "<< mI->GetName(*U)
								<< " does not have an available output port "
								<< endl ;
						return false;
					} else {
						PortU = OutU[0];
					}
					//Get the router input port that is not bound to any link
					vector<string> InR = (*mI)[RouterId].PRJ(Ports)GetNotBoundIn();
					//If there are no available ports then return
					if (InR.size() == 0) {
						CosiLog << "Router "<< mI->GetName(RouterId)
								<< " does not have an available input port "
								<< endl ;
						CosiLog << (*mI)[RouterId].PRJ(Ports)Get() ;
						return false;
					} else {
						PortRin = InR[0];
					}
					//Instantiate the link between the source and the router
					Implementation* L = mPlt->GetImplementationInstance(
							Type("Link"), *U, PortU, RouterId, PortRin,
							WiringPath());
					//Add the link to the implementation
					(*mI) = (*mI) + (*L);
					delete L;
				}

				//Add the commodity to the link between the source IP core
				//and the router. Here we can also set up the routing table.

				//Get all commodities on this constraint
				CommoditySet Cset = (*mSpec)[Edge(*U,*V)].PRJ(CommoditySet)Get();
				//Add all commodities in the set to the commodity of the
				//link between the source and the router.
				CommoditySet::iterator It;
				for (It = Cset.Begin() ; It != Cset.End() ; It++) {
					(*mI)[Edge(*U,RouterId)].PRJ(CommoditySet)Add(*It);
				}

				//Do the same for the link between the router
				//and the destination IP core.
				if ( !mI->InE(RouterId, *V)) {
					string PortRout, PortV;
					vector<string> OutR = (*mI)[RouterId].PRJ(Ports)GetNotBoundOut();
					if (OutR.size() == 0) {
						CosiLog << "Router "<< mI->GetName(RouterId)
								<< " does not have an available output port "
								<< endl ;
						CosiLog << (*mI)[RouterId].PRJ(Ports)Get() ;
						return false;
					} else {
						PortRout = OutR[0];
					}
					vector<string> InV = (*mI)[*V].PRJ(Ports)GetNotBoundIn();
					if (InV.size() == 0) {
						CosiLog << "Core "<< mI->GetName(*V)
								<< " does not have an available input port "
								<< endl ;
						return false;
					} else {
						PortV = InV[0];
					}
					Implementation* L = mPlt->GetImplementationInstance(
							Type("Link"), RouterId, PortRout, *V, PortV,
							WiringPath());
					(*mI) = (*mI) + (*L);
					delete L;
				}
				//Add the commodity to the link between the router
				//and the destination IP core. Here we can also set up the routing table.
				for (It = Cset.Begin() ; It != Cset.End() ; It++) {
					(*mI)[Edge(RouterId,*V)].PRJ(CommoditySet)Add(*It);
				}

			}
		}
	}

	//Simply print out the implementation
	CosiLog << mI->Print() << endl ;

	//Generate the floorplan information
	ParquetInputGen(Configuration->GetTmp() + "/Soc.blocks",
			Configuration->GetTmp() + "/Soc.nets", Configuration->GetTmp()
					+ "/Soc.wts", Configuration->GetTmp() + "/Soc.pl", mI ) ;
	string ParquetCmdLine;
	ParquetCmdLine = "export LD_LIBRARY_PATH=";
	ParquetCmdLine+= Configuration->GetParquetRoot();
	ParquetCmdLine+=";";
	ParquetCmdLine+= Configuration->GetParquetRoot();
	ParquetCmdLine+="/" + Configuration->GetParquetExec();
	ParquetCmdLine += " -f " + Configuration->GetTmp() + "/Soc";
	ParquetCmdLine+=" -minWL -takePl -compact "+ Configuration->GetTmp()
			+ "/Soc.pl -soft -savePl "+ Configuration->GetTmp()
			+ "/starsoc > parquetlog.txt";
	system(ParquetCmdLine.c_str() );

	mPlacement = new PlacementParser( Configuration->GetTmp( ) + "/starsoc.pl" , 0.25 );
	mPlacement->Parse();

	//Get the geometry of all ips

	for( U = mI->v_begin() ; U != mI->v_end() ; U++ ) {
		PlacedIpCore C = mPlacement->GetIpCore(mI->GetName(*U)) ;
		IpGeometry G ;
		G.SetXl(RealQuantity(C.GetBottomLeftX()));
		G.SetXr(RealQuantity(C.GetTopRightX()));
		G.SetYt(RealQuantity(C.GetTopRightY()));
		G.SetYb(RealQuantity(C.GetBottomLeftY())) ;
		G.SetArea(RealQuantity(C.GetArea()));
		Position P(C.GetBottomLeftX(),C.GetBottomLeftY(),0);
		(*mI)[*U].PRJ(Position)Set(P);
		(*mI)[*U].PRJ(IpGeometry)Set(G);
	}
	CosiLog << "Start topology generation done " << endl ;
	CosiLog << mI->Print() ;
	return true;
}

PlacementParser* StarTopologyGeneration::GetPlacement() {
	return mPlacement ;
}

Implementation* StarTopologyGeneration::GetImplementation() {
	return mI ;
}



}

}

}
