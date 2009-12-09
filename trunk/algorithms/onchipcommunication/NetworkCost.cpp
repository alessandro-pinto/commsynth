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
#include "NetworkCost.h"

namespace cosi {

namespace algorithms {

namespace onchipcommunication {

NetworkCost::NetworkCost() {
}

NetworkCost::~NetworkCost() {
}

void NetworkCost::Initialize(
		cosi::commstruct::onchipcommunication::Implementation* I,
		cosi::platforms::onchipcommunication::NocPlatformInterface* PltIf,
		cosi::platforms::onchipcommunication::Platform* Plt) {
	mI = I ;
	mPlt = Plt ;
	mPltIf = PltIf ;

}

void NetworkCost::RunApprox() {

	int Rindex, Cindex;

	//Analysis of edges
	mWireStaticPower = 0.0;
	mWireDynamicPower = 0.0;
	mRouterDynamicPower = 0.0;
	mRouterStaticPower = 0.0;
	mWireArea = 0.0;
	mWireDeviceArea = 0.0;
	mWireWiringArea = 0.0;
	mRouterArea = 0.0;

	mInterfacePower = 0.0;
	mInterfaceArea = 0.0;

	mNnodes = 0;
	mNlinks = 0;

	mMaxDist = 0.0;
	mMinDist = DBL_MAX;
	mTotalDist = 0.0;
	mAverageDist = 0.0;

	mMinInDegree = INT_MAX ;
	mMaxInDegree = 0;
	mAverageInDegree = 0;

	mMinOutDegree = INT_MAX ;
	mMaxOutDegree = 0;
	mAverageOutDegree = 0;

	double Dist;
	IdGraph::v_iterator U, V;
	for (U = mI->v_begin() ; U != mI->v_end() ; U++) {
		for (V = mI->v_begin() ; V != mI->v_end() ; V++) {
			if (mI->InE( *U , *V ) ) {
				Dist = cosi::Norm1((*mI)[*U].PRJ(Position)Get() , (*mI)[*V].PRJ(Position)Get());
				if (Dist > mMaxDist )
					mMaxDist = Dist ;
				if (Dist < mMinDist )
					mMinDist = Dist ;
				mTotalDist += Dist ;
				mWireStaticPower = mWireStaticPower + mPltIf->FlitWidth()
						*mPltIf->WireLeakage()*Dist ;
				mWireDynamicPower = mWireDynamicPower
						+ mPltIf->SwitchingFactor() * mPltIf->WireEnergy() * ((*mI)[Edge(*U,*V)].PRJ(CommoditySet)GetAggregateBandwidthValue() )
								* Dist ;
				mWireArea = mWireArea + mPltIf->FlitWidth()
						*mPltIf->WireArea(Dist ) ;
				mWireDeviceArea = mWireDeviceArea + mPltIf->FlitWidth()
						*mPltIf->WireDeviceArea(Dist ) ;
				mWireWiringArea = mWireWiringArea + mPltIf->FlitWidth()
						*mPltIf->WireWiringArea(Dist ) ;
				cout << "Area "<< mWireArea << " Power "<< mWireDynamicPower
						<< " Leakage "<< mWireStaticPower << endl ;
				mNlinks ++;
			}
		}
	}

	mAverageDist = mTotalDist / mNlinks ;

	// Nodes
	double Rate;
	for (U = mI->v_begin() ; U != mI->v_end() ; U++) {
		//if ((*mI)[*U].PRJ(Name)GetValue() == mPltIf->GetNodeName() ) {
			if (mI->InDegree( *U ) > mMaxInDegree )
				mMaxInDegree = mI->InDegree( *U ) ;
			if (mI->InDegree( *U ) < mMinInDegree )
				mMinInDegree = mI->InDegree( *U ) ;
			if (mI->OutDegree( *U ) > mMaxOutDegree )
				mMaxOutDegree = mI->OutDegree( *U ) ;
			if (mI->OutDegree( *U ) < mMinOutDegree )
				mMinOutDegree = mI->OutDegree( *U ) ;

			mAverageInDegree += mI->InDegree( *U ) ;
			mAverageOutDegree += mI->OutDegree( *U ) ;

			map < string , CommoditySet > PortCommodities;
			cosi::quantity::onchipcommunication::Ports::iterator Pit;
			Rate = 0.0;
			for (Pit = (*mI)[*U].PRJ(Ports)Begin() ; Pit != (*mI)[*U].PRJ(Ports)End() ; Pit++) {
				Edge E = (*mI)[*U].PRJ(Ports)BoundTo(Pit->first);
				Rate += (*mI)[E].PRJ(CommoditySet)GetAggregateBandwidthValue() ;
				PortCommodities[Pit->first] = (*mI)[E].PRJ(CommoditySet)Get() ;
			}
			mRouterDynamicPower = mRouterDynamicPower
					+ mPltIf->SwitchingFactor()* mPltIf->RouterEnergy(
							mI->InDegree( *U ) , mI->OutDegree( *U ) ) * Rate
							/ (double)mPltIf->FlitWidth() ;
			mRouterStaticPower = mRouterStaticPower + mPltIf->RouterLeakage(
					mI->InDegree( *U ) , mI->OutDegree( *U ) )
					*mPltIf->OperatingFrequency() ;
			mRouterArea = mRouterArea + mPltIf->RouterArea(mI->InDegree(*U) ,
					mI->OutDegree( *U ) ) ;
			cout << "Area "<< mRouterArea << " Power "<< mRouterDynamicPower
					<< " Leakage "<< mRouterStaticPower << endl ;
			mNnodes++;
		//}
	}

	mAverageInDegree = ( (double) mAverageInDegree ) / (double) mNnodes ;
	mAverageOutDegree = ( (double) mAverageOutDegree ) / ( double ) mNnodes ;

}

void NetworkCost::Run() {

	int Rindex, Cindex;

	//Analysis of edges
	mWireStaticPower = 0.0;
	mWireDynamicPower = 0.0;
	mRouterDynamicPower = 0.0;
	mRouterStaticPower = 0.0;
	mWireArea = 0.0;
	mWireDeviceArea = 0.0;
	mWireWiringArea = 0.0;
	mRouterArea = 0.0;

	mInterfacePower = 0.0;
	mInterfaceArea = 0.0;

	mNnodes = 0;
	mNlinks = 0;

	mMaxDist = 0.0;
	mMinDist = DBL_MAX;
	mTotalDist = 0.0;
	mAverageDist = 0.0;

	mMinInDegree = INT_MAX ;
	mMaxInDegree = 0;
	mAverageInDegree = 0;

	mMinOutDegree = INT_MAX ;
	mMaxOutDegree = 0;
	mAverageOutDegree = 0;

	double Dist;
	IdGraph::v_iterator U, V;
	for (U = mI->v_begin() ; U != mI->v_end() ; U++) {
		for (V = mI->v_begin() ; V != mI->v_end() ; V++) {
			if (mI->InE( *U , *V ) ) {
				Dist = cosi::Norm1((*mI)[*U].PRJ(Position)Get() , (*mI)[*V].PRJ(Position)Get());
				if (Dist > mMaxDist )
					mMaxDist = Dist ;
				if (Dist < mMinDist )
					mMinDist = Dist ;
				mTotalDist += Dist ;
				mWireStaticPower = mWireStaticPower + mPlt->GetStaticPower((*mI)[Edge(*U,*V)].PRJ(Name)Get() , (*mI)[Edge(*U,*V)].PRJ(WiringPath)Get() ) ;
				mWireDynamicPower = mWireDynamicPower + mPlt->GetDynamicPower((*mI)[Edge(*U,*V)].PRJ(Name)Get() , (*mI)[Edge(*U,*V)].PRJ(WiringPath)Get() , (*mI)[Edge(*U,*V)].PRJ(CommoditySet)Get() ) ;
				mWireDeviceArea = mWireDeviceArea + mPlt->GetDeviceArea((*mI)[Edge(*U,*V)].PRJ(Name)Get() , (*mI)[Edge(*U,*V)].PRJ(WiringPath)Get() ) ;
				mWireWiringArea = mWireWiringArea + mPlt->GetWiringArea((*mI)[Edge(*U,*V)].PRJ(Name)Get() , (*mI)[Edge(*U,*V)].PRJ(WiringPath)Get() ) ;
				mNlinks ++;
			}
		}
	}
	mWireArea = mWireDeviceArea + mWireWiringArea ;
	mAverageDist = mTotalDist / mNlinks ;

	// Nodes
	double Rate;
	for (U = mI->v_begin() ; U != mI->v_end() ; U++) {
		if ((*mI)[*U].PRJ(Type)GetValue() == "Router") {
			if (mI->InDegree( *U ) > mMaxInDegree )
				mMaxInDegree = mI->InDegree( *U ) ;
			if (mI->InDegree( *U ) < mMinInDegree )
				mMinInDegree = mI->InDegree( *U ) ;
			if (mI->OutDegree( *U ) > mMaxOutDegree )
				mMaxOutDegree = mI->OutDegree( *U ) ;
			if (mI->OutDegree( *U ) < mMinOutDegree )
				mMinOutDegree = mI->OutDegree( *U ) ;

			mAverageInDegree += mI->InDegree( *U ) ;
			mAverageOutDegree += mI->OutDegree( *U ) ;

			map < string , CommoditySet > PortCommodities;
			cosi::quantity::onchipcommunication::Ports::iterator Pit;
			for (Pit = (*mI)[*U].PRJ(Ports)Begin() ; Pit != (*mI)[*U].PRJ(Ports)End() ; Pit++) {
				Edge E = (*mI)[*U].PRJ(Ports)BoundTo(Pit->first);
				PortCommodities[Pit->first] = (*mI)[E].PRJ(CommoditySet)Get() ;
			}
			mRouterDynamicPower = mRouterDynamicPower + mPlt->GetDynamicPower((*mI)[*U].PRJ(Name)Get(), PortCommodities);
			mRouterStaticPower = mRouterStaticPower + mPlt->GetStaticPower((*mI)[*U].PRJ(Name)Get());
			mRouterArea = mRouterArea + mPlt->GetArea((*mI)[*U].PRJ(Name)Get());
			mNnodes++;
		}
	}

	mAverageInDegree = ( (double) mAverageInDegree ) / (double) mNnodes ;
	mAverageOutDegree = ( (double) mAverageOutDegree ) / ( double ) mNnodes ;

}

//bool NetworkCost::Verify(cosi::commstruct::onchipcommunication::Specification* Spec ) {
//
//	cout << "Network verification ... "<< endl ;
//	//Verify that there is no clock violation and no degree violation
//	double Rate;
//	int Rindex, Cindex;
//	int Nhops;
//	mHopAvg = 0;
//	mMinHop = INT_MAX ;
//	mMaxHop = 0;
//	map< pair<int,int> , double > Commodities;
//	map< pair<int,int> , double >::iterator Commodity;
//	for (Rindex = 0; Rindex < mG->Order() ; Rindex++) {
//		for (Cindex = 0; Cindex < mG->Order() ; Cindex++) {
//			if (mG->IsEdge(Rindex , Cindex) ) {
//				Commodities = mG->GetCommodities(Rindex , Cindex ) ;
//				double TotalBandwidth = 0;
//				for (Commodity = Commodities.begin() ; Commodity
//						!= Commodities.end() ; Commodity++) {
//					TotalBandwidth = TotalBandwidth + Commodity->second;
//				}
//				if (mG->GetAggregateFlow(Rindex , Cindex) != TotalBandwidth ) {
//					cout
//							<< "Aggregate flow is different from the total bandwidth on edge ("
//							<< Rindex << ","<< Cindex
//							<< ") make sure that this is becasue of exclusion constraints "
//							<< endl ;
//				}
//				if (mG->GetAggregateFlow(Rindex , Cindex) > mPlt->RouterMaxBw()
//						* mPlt->FlitWidth() ) {
//					cout << "Capacity Violation on ("<< Rindex << ","<< Cindex
//							<< ")"<< endl ;
//					cout << "Aggregate flow = "<< mG->GetAggregateFlow(Rindex ,
//							Cindex) << "  Maximum flow = "
//							<< mPlt->RouterMaxBw() * mPlt->FlitWidth() << endl ;
//				}
//
//			}
//		}
//
//		if (Rindex < mG->Ns() ) {
//
//			if (mG->OutDegree(Rindex ) > mPlt->SourceOutDegree() ) {
//				cout << "Degree violation at Source "<< Rindex << endl ;
//				cout << "MaxOutDegree = "<< mPlt->MaxOutDegree()
//						<< "  OutDegree = "<< mG->OutDegree(Rindex ) ;
//			}
//			if (mG->InDegree(Rindex) !=0) {
//				cout << "Degree violation at Source "<< Rindex
//						<< " the input degree is not zero "<< endl ;
//
//			}
//
//		} else if (Rindex < mG->Ns() + mG->Nd() ) {
//
//			if (mG->InDegree(Rindex ) > mPlt->DestInDegree() ) {
//				cout << "Degree violation at Destination "<< Rindex << endl ;
//				cout << "MaxInDegree = "<< mPlt->DestInDegree()
//						<< "  InDegree = "<< mG->InDegree(Rindex ) ;
//			}
//			if (mG->OutDegree(Rindex) !=0) {
//				cout << "Degree violation at Destination "<< Rindex
//						<< " the output degree is not zero "<< endl ;
//
//			}
//		} else {
//
//			if (mG->InDegree(Rindex ) > mPlt->MaxInDegree() ) {
//				cout << "Degree violation at router "<< Rindex << endl ;
//				cout << "MaxInDegree = "<< mPlt->MaxInDegree()
//						<< "  InDegree = "<< mG->InDegree(Rindex ) ;
//			}
//			if (mG->OutDegree(Rindex ) > mPlt->MaxOutDegree() ) {
//				cout << "Degree violation at router "<< Rindex << endl ;
//				cout << "MaxOutDegree = "<< mPlt->MaxOutDegree()
//						<< "  OutDegree = "<< mG->OutDegree(Rindex ) ;
//			}
//		}
//	}
//
//	cout << "Maximum bandwidth constraints are satisfied !"<< endl ;
//	cout << "Degree constraints are satisfied ! "<< endl ;
//
//	//Verify that there are true paths between source and destination
//	int CurrentV, NextV, OutPort;
//	map< pair<int,int> ,int> RoutingTable;
//	map< pair<int,int> , int >::iterator Entry;
//	ChannelDependencyGraph CDG("CDG");
//	int Nc;
//	Nc = 0;
//	for (int S = 0; S < PtP->Ns() ; S++) {
//		//forall sources
//		for (int D = PtP->Ns() ; D < PtP->Ns() + PtP->Nd() ; D++) {
//			if (PtP->IsEdge(S , D ) ) {
//				//verify that there is a path to destination
//				Nc++;
//				Nhops = 0;
//				CurrentV = S ;
//				//Get routing table
//				while (CurrentV != D ) {
//					RoutingTable = mG->GetRoutingTable(CurrentV) ;
//					Entry = RoutingTable.find(pair<int, int>(S, D) );
//					if (Entry == RoutingTable.end() ) {
//						cout << "Coul not find entry "<< D
//								<< " in routing table of "<< CurrentV << endl ;
//						return false;
//					}
//					OutPort = Entry->second;
//					//from all edges leaving CurrentV, find the one with that outport
//					int OutV;
//					for (OutV = 0; OutV < mG->Order() ; OutV++) {
//						if (mG->IsEdge(CurrentV , OutV ) ) {
//							if (mG->GetOutputPort(CurrentV, OutV) == OutPort ) {
//								break;
//							}
//						}
//					}
//
//					//verify that there is such edge and that
//					//it carries this commodity
//					if ( !mG->IsEdge(CurrentV , OutV ) ) {
//						cout << "There should be an edge ("<< CurrentV << ","
//								<< OutV << ") but there isn't"<< endl ;
//						return false;
//					}
//					Commodity = (mG->GetCommodities( CurrentV , OutV )).find(pair<int, int>(S,
//							D)) ;
//					if (Commodity == (mG->GetCommodities( CurrentV , OutV )).end() ) {
//						cout << "Edge ("<< CurrentV << ","<< OutV
//								<< ") should carry commodity ("<< S << ","<< D
//								<< ") but it does not"<< endl ;
//						return false;
//					} else if ((mG->GetCommodities( CurrentV , OutV ))[pair<int,int>(S,D)] != PtP->GetFlow(S, D ) ) {
//						cout << "Edge ("<< CurrentV << ","<< OutV
//								<< ") should carry "<< PtP->GetFlow(S, D )
//								<< " for commodity ("<< S << ","<< D
//								<< ") but it carried"<< Commodity->second
//								<< endl ;
//					}
//
//					cout << "("<< CurrentV << ","<< OutV << ")"<< " ";
//
//					CurrentV = OutV ;
//					Nhops++;
//				}
//				cout << endl ;
//
//				if (Nhops > mMaxHop ) {
//					mMaxHop = Nhops ;
//					mMaxHopPathSource = S ;
//					mMaxHopPathDest = D ;
//				}
//				if (Nhops < mMinHop )
//					mMinHop = Nhops ;
//				mHopAvg += Nhops ;
//			}
//
//		}
//	}
//	mHopAvg = mHopAvg / (double)Nc ;
//
//	cout << "There exist a path for each point-to-point constraint !"<< endl ;
//
//	//Check cyclic dependencies
//	for (Rindex = mG->Ns() + mG->Nd() ; Rindex < mG->Order() ; Rindex++) {
//		//Add a dependencies from inputs to outputs
//		for (Cindex = 0; Cindex < mG->Order() ; Cindex++) {
//			if (mG->IsEdge(Cindex , Rindex ) ) { //input edge
//
//				set< pair<int,int> > InputFlows;
//				set< pair<int,int> >::iterator Flow;
//				map< pair<int,int> , int > RoutingTable;
//				map< pair<int,int> , int >::iterator Entry;
//				InputFlows = mG->InFlow(Rindex , mG->GetInputPort(Cindex ,
//						Rindex ) ) ;
//				RoutingTable = mG->GetRoutingTable(Rindex ) ;
//				for (Flow = InputFlows.begin() ; Flow != InputFlows.end() ; Flow++) {
//					//find the output
//					int Output;
//					Entry = RoutingTable.find( *Flow ) ;
//					for (int Out = 0; Out < mG->Order() ; Out++) {
//						if (mG->IsEdge(Rindex , Out ) && (mG->GetOutputPort(
//								Rindex , Out ) == Entry->second)) {
//							CDG.AddDependency(pair<int, int>(Cindex, Rindex) ,
//									pair<int, int>(Rindex, Out) ) ;
//							break;
//						}
//					}
//				}
//
//			}
//		}
//	}
//
//	if (CDG.Cyclic() ) {
//		cout
//				<< "There could be a deadlock (the SystemC simulation might deadlock)"
//				<< endl ;
//		return false;
//	}
//
//	cout << "The channel dependency graph is acyclic ! "<< endl ;
//
//	return true;
//}

bool NetworkCost::Verify(
		cosi::commstruct::onchipcommunication::Specification* Spec) {

	cout << "Network verification ... "<< endl ;
	//Verify that there is no clock violation and no degree violation
	double Rate;
	int Rindex, Cindex;
	int Nhops;
	mHopAvg = 0;
	mMinHop = INT_MAX ;
	mMaxHop = 0;
	map< pair<int,int> , double > Commodities;
	map< pair<int,int> , double >::iterator Commodity;
	IdGraph::v_iterator U, V;
	for (U = mI->v_begin() ; U != mI->v_end() ; U++) {
		for (V= mI->v_begin() ; V != mI->v_end() ; V++) {
			if (mI->InE( *U , *V ) ) {
				if ((*mI)[Edge(*U,*V)].PRJ(CommoditySet)GetAggregateBandwidthValue()
						> mPltIf->RouterMaxBw()* mPltIf->FlitWidth() ) {
					cout << "Capacity Violation on ("<< *U << ","<< *V<< ")"
							<< endl ;
					cout << "Aggregate flow = "<< (*mI)[Edge(*U,*V)].PRJ(CommoditySet)GetAggregateBandwidthValue()
							<< "  Maximum flow = "<< mPltIf->RouterMaxBw()
							* mPltIf->FlitWidth() << endl ;
				}

			}
		}

		if (Spec->InV(*U)) {

			if (mI->OutDegree( *U ) > mPltIf->SourceOutDegree() ) {
				cout << "Degree violation at Source "<< *U << endl ;
				cout << "MaxOutDegree = "<< mPltIf->MaxOutDegree()
						<< "  OutDegree = "<< mI->OutDegree( *U ) ;
			}
			if (mI->InDegree( *U ) > mPltIf->DestInDegree() ) {
				cout << "Degree violation at Destination "<< *U << endl ;
				cout << "MaxInDegree = "<< mPltIf->DestInDegree()
						<< "  InDegree = "<< mI->InDegree(*U ) ;
			}

		} else {

			if (mI->InDegree(*U ) > mPltIf->MaxInDegree() ) {
				cout << "Degree violation at router "<< *U << endl ;
				cout << "MaxInDegree = "<< mPltIf->MaxInDegree()
						<< "  InDegree = "<< mI->InDegree(*U ) ;
			}
			if (mI->OutDegree(*U ) > mPltIf->MaxOutDegree() ) {
				cout << "Degree violation at router "<< *U << endl ;
				cout << "MaxOutDegree = "<< mPltIf->MaxOutDegree()
						<< "  OutDegree = "<< mI->OutDegree(*U ) ;
			}
		}
	}

	cout << "Maximum bandwidth constraints are satisfied !"<< endl ;
	cout << "Degree constraints are satisfied ! "<< endl ;

	//Verify that there are true paths between source and destination
	int CurrentV, NextV, OutPort;
	map< pair<int,int> ,int> RoutingTable;
	map< pair<int,int> , int >::iterator Entry;
	//ChannelDependencyGraph CDG("CDG");
	int Nc;
	Nc = 0;

	for (U = Spec->v_begin(); U != Spec->v_end() ; U++) {
		//forall sources
		for (V = Spec->v_begin(); V != Spec->v_end() ; V++) {
			if (Spec->InE(*U , *V ) ) {
				//verify that there is a path to destination
				Nc++;
				Nhops = 0;
				CurrentV = *U ;
				//Get routing table
				while (CurrentV != *V ) {

					//Look for the output port
					if ( !(*mI)[CurrentV].PRJ(TransferTable)HasEntry(Spec->GetName(*U) ,
							Spec->GetName(*V)) ) {
						cout << "Coul not find entry ("<< Spec->GetName(*U)
								<< ","<< Spec->GetName(*V)
								<< ") in routing table of "<< CurrentV << endl ;
					}

					Edge E = (*mI)[CurrentV].PRJ(Ports)BoundTo((*mI)[CurrentV].PRJ(TransferTable)GetPort(Spec->GetName(*U) ,
							Spec->GetName(*V))) ;
					//verify that there is such edge and that
					//it carries this commodity
					int OutV = E.V();
					if ( !mI->InE(E ) ) {
						cout << "There should be an edge ("<< CurrentV << ","
								<< OutV << ") but there isn't"<< endl ;
						return false;
					}

					//Check that all commodities are on this edge

					//					Commodity = (mG->GetCommodities( CurrentV , OutV )).find(pair<int, int>(S,
					//							D)) ;
					//					if (Commodity == (mG->GetCommodities( CurrentV , OutV )).end() ) {
					//						cout << "Edge ("<< CurrentV << ","<< OutV
					//								<< ") should carry commodity ("<< S << ","<< D
					//								<< ") but it does not"<< endl ;
					//						return false;
					//					} else if ((mG->GetCommodities( CurrentV , OutV ))[pair<int,int>(S,D)] != PtP->GetFlow(S, D ) ) {
					//						cout << "Edge ("<< CurrentV << ","<< OutV
					//								<< ") should carry "<< PtP->GetFlow(S, D )
					//								<< " for commodity ("<< S << ","<< D
					//								<< ") but it carried"<< Commodity->second
					//								<< endl ;
					//					}

					cout << "("<< CurrentV << ","<< OutV << ")"<< " ";

					CurrentV = OutV ;
					Nhops++;
				}
				cout << endl ;
				//				if (Nhops > PtP->GetLatency(S, D ) ) {
				//
				//					mViolatedPaths[ pair<int,int>(S,D) ] = pair<int, int>(PtP->GetLatency(S, D ) ,
				//							Nhops ) ;
				//
				//				}
				if (Nhops > mMaxHop ) {
					mMaxHop = Nhops ;
					//mMaxHopPathSource = S ;
					//mMaxHopPathDest = D ;
				}
				if (Nhops < mMinHop )
					mMinHop = Nhops ;
				mHopAvg += Nhops ;
			}

		}
	}
	mHopAvg = mHopAvg / (double)Nc ;

	cout << "There exist a path for each point-to-point constraint !"<< endl ;

	//Check cyclic dependencies
	for (U = mI->v_begin() ; U != mI->v_end() ; U++) {
		//Add a dependencies from inputs to outputs
		for (V = mI->v_begin() ; V != mI->v_end() ; V++) {
			//			if (mI->InE( *U , *V ) ) { //input edge
			//				set< pair<int,int> > InputFlows;
			//				set< pair<int,int> >::iterator Flow;
			//				map< pair<int,int> , int > RoutingTable;
			//				map< pair<int,int> , int >::iterator Entry;
			//				InputFlows = mG->InFlow(Rindex , mG->GetInputPort(Cindex ,
			//						Rindex ) ) ;
			//				RoutingTable = mG->GetRoutingTable(Rindex ) ;
			//				for (Flow = InputFlows.begin() ; Flow != InputFlows.end() ; Flow++) {
			//					//find the output
			//					int Output;
			//					Entry = RoutingTable.find( *Flow ) ;
			//					for (int Out = 0; Out < mG->Order() ; Out++) {
			//						if (mG->IsEdge(Rindex , Out ) && (mG->GetOutputPort(
			//								Rindex , Out ) == Entry->second)) {
			//							CDG.AddDependency(pair<int, int>(Cindex, Rindex) ,
			//									pair<int, int>(Rindex, Out) ) ;
			//							break;
			//						}
			//					}
			//				}
			//
			//			}
		}
	}

	//	if (CDG.Cyclic() ) {
	//		cout
	//				<< "There could be a deadlock (the SystemC simulation might deadlock)"
	//				<< endl ;
	//		return false;
	//	}
	//
	//	cout << "The channel dependency graph is acyclic ! "<< endl ;

	return true;
}

double NetworkCost::GetWireStaticPower() {
	return mWireStaticPower ;
}

double NetworkCost::GetWireDynamicPower() {
	return mWireDynamicPower ;
}

double NetworkCost::GetWireTotalPower() {
	return (mWireStaticPower + mWireDynamicPower );
}

double NetworkCost::GetRouterStaticPower() {
	return mRouterStaticPower ;
}

double NetworkCost::GetRouterDynamicPower() {
	return mRouterDynamicPower ;
}

double NetworkCost::GetRouterTotalPower() {
	return (mRouterStaticPower + mRouterDynamicPower );
}

//double NetworkCost::GetInterfacePower() {
//	return mInterfacePower ;
//}
//
//double NetworkCost::GetInterfaceArea() {
//	return mInterfaceArea ;
//}

double NetworkCost::GetTotalPower() {
	return (GetRouterTotalPower() + GetWireTotalPower() );
}

double NetworkCost::GetWireArea() {
	return (GetWireDeviceArea() + GetWireWiringArea() );
}

double NetworkCost::GetWireDeviceArea() {
	return mWireDeviceArea ;
}

double NetworkCost::GetWireWiringArea() {
	return mWireWiringArea ;
}

double NetworkCost::GetRouterArea() {
	return mRouterArea ;
}

double NetworkCost::GetArea() {
	return (mWireArea + mRouterArea + mInterfaceArea );
}

}
}
}
