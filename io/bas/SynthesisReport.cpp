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
#include "SynthesisReport.h"

namespace cosi {

namespace io {

namespace bas {

using namespace std;
//BUS-BUS
SynthesisReport::SynthesisReport(
		cosi::commstruct::wiredbas::Specification* pSpec,
		std::list<cosi::commstruct::wiredbas::Implementation*> Buses,
		cosi::platforms::wiredbas::BusPlatform* pPlt,
		cosi::commstruct::wiredbas::Implementation* SecondLevel,
		std::string pFileName) {
	mSpec = pSpec ;
	mBuses = Buses ;
	mSecondLevel = SecondLevel ;
	mPlt = pPlt ;
	mFileName = pFileName ;
	mAppend = false;
	mBusBased = true;
	mBusStar = false;
}

//BUS-STAR
SynthesisReport::SynthesisReport(
		cosi::commstruct::wiredbas::Specification* pSpec,
		std::list<cosi::commstruct::wiredbas::Implementation*> Buses,
		cosi::platforms::wiredbas::BusPlatform* pPlt,
		cosi::commstruct::wiredbas::Implementation* SecondLevel,
		cosi::platforms::wiredbas::NetworkPlatform* pNetPlt,
		std::string pFileName) {
	mSpec = pSpec ;
	mBuses = Buses ;
	mSecondLevel = SecondLevel ;
	mPlt = pPlt ;
	mNetPlt = pNetPlt;
	mFileName = pFileName ;
	mAppend = false;
	mBusBased = true;
	mBusStar = true;
}

SynthesisReport::SynthesisReport(
		cosi::commstruct::wiredbas::Specification* pSpec,
		std::list<cosi::commstruct::wiredbas::Implementation*> Buses,
		cosi::platforms::wiredbas::NetworkPlatform* pPlt,
		cosi::commstruct::wiredbas::Implementation* SecondLevel,
		std::string pFileName) {
	mSpec = pSpec ;
	mBuses = Buses ;
	mSecondLevel = SecondLevel ;
	mNetPlt = pPlt ;
	mFileName = pFileName ;
	mAppend = false;
	mBusBased = false;
	mBusStar = false;
}

void SynthesisReport::Append(std::string pFileName) {
	mAppend = true;
	mAppendFileName = pFileName ;
}

void SynthesisReport::Run() {
	if (mBusBased )
		RunBusBased() ;
	else
		RunNetBased() ;
}

void SynthesisReport::RunBusBased() {

	ofstream F;
	F.open(mFileName.c_str()) ;

	mSecondLevelWiring = 0;

	IdGraph::v_iterator U, V;
	list<cosi::commstruct::wiredbas::Implementation*>::iterator Bus;
	//check that every sensor is covered
	std::set<int> Uncovered;
	for (U =mSpec->v_begin() ; U != mSpec->v_end() ; U++) {
		bool Covered = false;
		for (Bus = mBuses.begin() ; Bus != mBuses.end() ; Bus++) {
			if ((*Bus)->InV(*U)) {
				Covered = true;
			}
		}
		if ( !Covered ) {
			Uncovered.insert(*U) ;
		}
	}

	if (Uncovered.size() != 0) {
		F << "Automatically generated by COSI-NES"<< endl << endl ;
		F << "This solution is only a partial cover, "<< endl ;
		F << "The following nodes are uncovered "<< endl ;
		std::set<int>::iterator It;
		for (It = Uncovered.begin() ; It != Uncovered.end() ; It++) {
			F << mSpec->GetName(*It) << endl ;
		}
		F << endl ;
		F.close();
		return;
	}

	//get component list and cost
	for (Bus = mBuses.begin() ; Bus != mBuses.end() ; Bus++) {
		cosi::commstruct::wiredbas::Implementation* I = *Bus ;
		for (U = I->v_begin() ; U != I->v_end() ; U++) {
			string ComponentName = (*I)[*U].PRJ(Name)GetValue() ;
			string ComponentType = (*I)[*U].PRJ(Type)GetValue() ;
			//cout << "Type " << ComponentType << endl ;
			if (mNcomponents.find(ComponentName ) != mNcomponents.end()) {
				mNcomponents[ComponentName] ++;
				mCosts[ComponentName] += mPlt->GetNodeCost(ComponentType) ;
			} else {
				mNcomponents[ComponentName] = 1;
				mCosts[ComponentName] = mPlt->GetNodeCost(ComponentType) ;
			}
			for (V = I->v_begin() ; V != I->v_end() ; V++) {
				if (I->InE(*U, *V)) {
					string ComponentName = (*I)[Edge(*U,*V)].PRJ(Name)GetValue() ;
					string ComponentType = (*I)[Edge(*U,*V)].PRJ(Type)GetValue() ;
					if (mNcomponents.find(ComponentName ) != mNcomponents.end()) {
						mNcomponents[ComponentName] ++;
						mCosts[ComponentName]+= mPlt->GetLinkCost(
								ComponentType, (*I)[Edge(*U,*V)].PRJ(WiringPath)Get()) ;
					} else {
						mNcomponents[ComponentName] = 1;
						cout << "Component type "<< ComponentType << endl ;
						mCosts[ComponentName]= mPlt->GetLinkCost(ComponentType,
								(*I)[Edge(*U,*V)].PRJ(WiringPath)Get()) ;
					}
				}
			}
		}
	}

	//Get the second level wiring cost
	for (U = mSecondLevel->v_begin() ; U != mSecondLevel->v_end() ; U++) {
		for (V = mSecondLevel->v_begin() ; V != mSecondLevel->v_end() ; V++) {
			if (mSecondLevel->InE(*U, *V)) {
				string ComponentType = (*mSecondLevel)[Edge(*U,*V)].PRJ(Type)GetValue() ;
				if (mBusStar)
					mSecondLevelWiring += mNetPlt->GetLinkCost(ComponentType, (*mSecondLevel)[Edge(*U,*V)].PRJ(WiringPath)Get()) ;
				else
					mSecondLevelWiring += mPlt->GetLinkCost(ComponentType, (*mSecondLevel)[Edge(*U,*V)].PRJ(WiringPath)Get()) ;

			}
		}
	}

	F << "Automatically generated by COSI-NES"<< endl << endl ;
	F << "Number of buses "<< mBuses.size() << endl << endl ;
	F << "Component list : "<< endl ;
	map<string,int>::iterator It;
	for (It = mNcomponents.begin() ; It != mNcomponents.end(); It++) {
		F << It->second<< " "<< It->first<< "  cost = "<< mCosts[It->first]
				<< endl ;
	}
	F << "Second level network wiring cost "<< mSecondLevelWiring << endl ;
	F << endl << endl ;

	vector<double> Slacks;
	vector<double> Hslacks;
	vector< double > B;
	vector< double > L;
	double Hbandwidth = 0;

	for (Bus = mBuses.begin() ; Bus != mBuses.end() ; Bus++) {
		//compute bandwidth
		double Bw = mPlt->GetBusBandwidthValue("Bus", *Bus) ;
		B.push_back(Bw ) ;
	}

	for (U = mSpec->v_begin() ; U != mSpec->v_end() ; U++) {
		for (V = mSpec->v_begin() ; V != mSpec->v_end() ; V++) {
			if (mSpec->InE(*U, *V)) {
				double Latency = (*mSpec)[Edge(*U,*V)].PRJ(RealLatency)GetValue() ;
				cosi::commstruct::wiredbas::Implementation* Iu;
				cosi::commstruct::wiredbas::Implementation* Iv;
				for (Bus = mBuses.begin() ; Bus != mBuses.end() ; Bus++) {
					if ((*Bus)->InV(*U)) {
						Iu = *Bus ;
					}
					if ((*Bus)->InV(*V) ) {
						Iv = *Bus ;
					}
				}
				if (Iu == Iv ) {
					double Delay = mPlt->BusAccessLatencyValue("Bus", Iu) ;
					L.push_back(Delay) ;
					Slacks.push_back(Latency-Delay) ;
				} else {
					double Delay = mPlt->BusAccessLatencyValue("Bus", Iu)
							+ mPlt->BusAccessLatencyValue("Bus", Iv);
					L.push_back(Delay) ;
					Slacks.push_back(Latency-Delay) ;
					Hslacks.push_back(Latency-Delay) ;
					ThreadSet::iterator Tit;
					for (Tit = (*mSpec)[*U].PRJ(ThreadSet)ThreadSetBegin() ; Tit != (*mSpec)[*U].PRJ(ThreadSet)ThreadSetEnd() ; Tit++) {
						if (Tit->GetDestinationNodeId() == *V ) {
							Hbandwidth += ((double) Tit->GetMessageLength())
									/ Tit->GetPeriod() ;
						}
					}
				}
			}
		}
	}

	double MinLatency= DBL_MAX;
	double MaxLatency = 0;
	double AvgLatency = 0;
	double StdDevLatency = 0;
	double MinSlack= DBL_MAX;
	double MaxSlack = 0;
	double AvgSlack = 0;
	double StdDevSlack = 0;
	double MinHslack= DBL_MAX;
	double MaxHslack = 0;
	double AvgHslack = 0;
	double StdDevHslack = 0;
	double MinBandwidth= DBL_MAX;
	double MaxBandwidth = 0;
	double AvgBandwidth = 0;
	double StdDevBandwidth = 0;
	//min max and total
	for (unsigned int i = 0; i < B.size() ; i++) {
		if (B[i] < MinBandwidth ) {
			MinBandwidth = B[i];
		}
		if (B[i] > MaxBandwidth ) {
			MaxBandwidth = B[i];
		}
		AvgBandwidth += B[i];
	}
	AvgBandwidth = AvgBandwidth / (double) B.size() ;
	//stddev
	for (unsigned int i = 0; i < B.size() ; i++) {
		StdDevBandwidth += (B[i] - AvgBandwidth)*(B[i] - AvgBandwidth);
	}
	StdDevBandwidth = sqrt(StdDevBandwidth) ;

	//min max and total
	for (unsigned int i = 0; i < L.size() ; i++) {
		if (L[i] < MinLatency ) {
			MinLatency = L[i];
		}
		if (L[i] > MaxLatency ) {
			MaxLatency = L[i];
		}
		AvgLatency += L[i];
	}
	AvgLatency = AvgLatency / (double) L.size() ;
	//stddev
	for (unsigned int i = 0; i < L.size() ; i++) {
		StdDevLatency += (L[i] - AvgLatency)*(L[i] - AvgLatency);
	}
	StdDevLatency = sqrt(StdDevLatency) ;

	//min max and total
	for (unsigned int i = 0; i < Slacks.size() ; i++) {
		if (Slacks[i] < MinSlack ) {
			MinSlack = Slacks[i];
		}
		if (Slacks[i] > MaxSlack ) {
			MaxSlack = Slacks[i];
		}
		AvgSlack += Slacks[i];
	}
	AvgSlack = AvgSlack / (double) Slacks.size() ;
	//stddev
	for (unsigned int i = 0; i < Slacks.size() ; i++) {
		StdDevSlack += (Slacks[i] - AvgSlack)*(Slacks[i] - AvgSlack);
	}
	StdDevSlack = sqrt(StdDevSlack) ;

	//min max and total
	for (unsigned int i = 0; i < Hslacks.size() ; i++) {
		if (Hslacks[i] < MinHslack ) {
			MinHslack = Hslacks[i];
		}
		if (Hslacks[i] > MaxHslack ) {
			MaxHslack = Hslacks[i];
		}
		AvgHslack += Hslacks[i];
	}
	AvgHslack = AvgHslack / (double) Hslacks.size() ;
	//stddev
	for (unsigned int i = 0; i < Hslacks.size() ; i++) {
		StdDevHslack += (Hslacks[i] - AvgHslack)*(Hslacks[i] - AvgHslack);
	}
	StdDevHslack = sqrt(StdDevHslack) ;

	F << "Latency and bandwdith analysis : "<< endl ;
	F << "Bus bandwidth :  min = "<< MinBandwidth << "  max = "<< MaxBandwidth
			<< "  avg = "<< AvgBandwidth << " sigma = "<< StdDevBandwidth
			<< endl ;
	F << "End-to-end latency :  min = "<< MinLatency << "  max = "<< MaxLatency
			<< "  avg = "<< AvgLatency<< " sigma = "<< StdDevLatency << endl ;
	F << "End-to-end slacks :  min = "<< MinSlack << "  max = "<< MaxSlack
			<< "  avg = "<< AvgSlack<< " sigma = "<< StdDevSlack << endl ;

	F << endl << endl ;

	F << "Analysis of the higher level network (not implemented yet) "<< endl ;
	F << "Total bandwidth "<< Hbandwidth << endl ;
	F << "End-to-end slacks :  min = "<< MinHslack << "  max = "<< MaxHslack
			<< "  avg = "<< AvgHslack<< " sigma = "<< StdDevHslack << endl ;
	F << endl ;
	F.close();

}

void SynthesisReport::RunNetBased() {
	ofstream F;
	F.open(mFileName.c_str()) ;
	mSecondLevelWiring = 0;
	IdGraph::v_iterator U, V;
	list<cosi::commstruct::wiredbas::Implementation*>::iterator Bus;
	//check that every sensor is covered
	std::set<int> Uncovered;
	for (U =mSpec->v_begin() ; U != mSpec->v_end() ; U++) {
		bool Covered = false;
		for (Bus = mBuses.begin() ; Bus != mBuses.end() ; Bus++) {
			if ((*Bus)->InV(*U)) {
				Covered = true;
			}
		}
		if ( !Covered ) {
			Uncovered.insert(*U) ;
		}
	}

	if (Uncovered.size() != 0) {
		F << "Automatically generated by COSI-NES"<< endl << endl ;
		F << "This solution is only a partial cover, "<< endl ;
		F << "The following nodes are uncovered "<< endl ;
		std::set<int>::iterator It;
		for (It = Uncovered.begin() ; It != Uncovered.end() ; It++) {
			F << mSpec->GetName(*It) << endl ;
		}
		F << endl ;
		F.close();
		return;
	}

	//get component list and cost
	for (Bus = mBuses.begin() ; Bus != mBuses.end() ; Bus++) {
		cosi::commstruct::wiredbas::Implementation* I = *Bus ;
		for (U = I->v_begin() ; U != I->v_end() ; U++) {
			string ComponentName = (*I)[*U].PRJ(Name)GetValue() ;
			string ComponentType = (*I)[*U].PRJ(Type)GetValue() ;
			if (mNcomponents.find(ComponentName ) != mNcomponents.end()) {
				mNcomponents[ComponentName] ++;
				mCosts[ComponentName] += mNetPlt->GetNodeCost(ComponentType) ;
			} else {
				mNcomponents[ComponentName] = 1;
				mCosts[ComponentName] = mNetPlt->GetNodeCost(ComponentType) ;
			}
			for (V = I->v_begin() ; V != I->v_end() ; V++) {
				if (I->InE(*U, *V)) {
					string ComponentName = (*I)[Edge(*U,*V)].PRJ(Name)GetValue() ;
					string ComponentType = (*I)[Edge(*U,*V)].PRJ(Type)GetValue() ;
					if (mNcomponents.find(ComponentName ) != mNcomponents.end()) {
						mNcomponents[ComponentName] ++;
						mCosts[ComponentName]+= mNetPlt->GetLinkCost(
								ComponentType, (*I)[Edge(*U,*V)].PRJ(WiringPath)Get()) ;
					} else {
						mNcomponents[ComponentName] = 1;
						cout << "Component type "<< ComponentType << endl ;
						mCosts[ComponentName]= mNetPlt->GetLinkCost(
								ComponentType, (*I)[Edge(*U,*V)].PRJ(WiringPath)Get()) ;
					}
					cout << (*I)[Edge(*U,*V)].PRJ(WiringPath)Get() << endl ;
					cout << " Length "<< (*I)[Edge(*U,*V)].PRJ(WiringPath)Length() << endl ;
					cout << " Cost "<< mNetPlt->GetLinkCost(ComponentType, (*I)[Edge(*U,*V)].PRJ(WiringPath)Get()) << endl ;
				}
			}
		}
	}

	//Get the second level wiring cost
	for (U = mSecondLevel->v_begin() ; U != mSecondLevel->v_end() ; U++) {
		for (V = mSecondLevel->v_begin() ; V != mSecondLevel->v_end() ; V++) {
			if (mSecondLevel->InE(*U, *V)) {
				string ComponentType = (*mSecondLevel)[Edge(*U,*V)].PRJ(Type)GetValue() ;
				mSecondLevelWiring += mNetPlt->GetLinkCost(ComponentType, (*mSecondLevel)[Edge(*U,*V)].PRJ(WiringPath)Get()) ;
			}
		}
	}

	F << "Automatically generated by COSI-NES"<< endl << endl ;
	F << "Number of sub-networks "<< mBuses.size() << endl << endl ;
	F << "Component list : "<< endl ;
	map<string,int>::iterator It;
	for (It = mNcomponents.begin() ; It != mNcomponents.end(); It++) {
		F << It->second<< " "<< It->first<< "  cost = "<< mCosts[It->first]
				<< endl ;
	}
	F << "Second level network wiring cost "<< mSecondLevelWiring << endl ;
	F << endl << endl ;

	vector<double> Slacks;
	vector<double> Hslacks;
	vector< double > B;
	vector< double > L;
	double Hbandwidth = 0;
	cosi::models::wiredbas::EthernetLinkDelay
			*LinkDelay = new cosi::models::wiredbas::EthernetLinkDelay(100e6);
	for (Bus = mBuses.begin() ; Bus != mBuses.end() ; Bus++) {

		//find the router
		int RouterId = 0;
		for (U = (*Bus)->v_begin() ; U != (*Bus)->v_end() ; U++) {
			if ((*(*Bus))[*U].PRJ(Type)GetValue() == "Router") {
				RouterId = *U ;
				break;
			}
		}

		//Sum up the bandwidth al all nodes entering the router
		//get the routing table

		TransferTable T = (*(*Bus))[*U].PRJ(TransferTable)Get() ;
		//compute the bandwidth of each flow passing by this router
		TransferTable::iterator It;
		double Bw = 0;
		for (It = T.TransferTableBegin() ; It != T.TransferTableEnd() ; It++) {
			string SrcName = (It->first).first;
			string DestName = (It->first).second;
			int SrcId = mSpec->GetId(SrcName) ;
			ThreadSet Tset = (*mSpec)[SrcId].PRJ(ThreadSet)Get() ;
			ThreadSet::iterator Tit;
			for (Tit = Tset.ThreadSetBegin() ; Tit != Tset.ThreadSetEnd() ; Tit++) {
				if (Tit->GetDestinationNodeName() == DestName) {
					Bw += LinkDelay->GetValue(Tit->GetMessageLength(),
							WiringPath()) * 100e6/ (Tit->GetPeriod());
				}
			}
		}
		B.push_back(Bw ) ;

	}

	for (U = mSpec->v_begin() ; U != mSpec->v_end() ; U++) {
		for (V = mSpec->v_begin() ; V != mSpec->v_end() ; V++) {
			if (mSpec->InE(*U, *V)) {
				double Latency = (*mSpec)[Edge(*U,*V)].PRJ(RealLatency)GetValue() ;
				cosi::commstruct::wiredbas::Implementation* Iu;
				cosi::commstruct::wiredbas::Implementation* Iv;
				for (Bus = mBuses.begin() ; Bus != mBuses.end() ; Bus++) {
					if ((*Bus)->InV(*U)) {
						Iu = *Bus ;
					}
					if ((*Bus)->InV(*V) ) {
						Iv = *Bus ;
					}
				}
				if (Iu == Iv ) {
					//latency of two links and the switch
					double Delay = mNetPlt->GetLatencyValue("Net", Iu, *U,
							mSpec->GetName(*V), *U, *V) ;
					L.push_back(Delay) ;
					Slacks.push_back(Latency-Delay) ;
				} else {
					//latency of two links and two switches
					//need to find the two routers
					int RouterU = 0;
					int RouterV = 0;
					IdGraph::v_iterator R;
					for (R = Iu->v_begin() ; R != Iu->v_end() ; R++) {
						if ((*Iu)[*R].PRJ(Type)GetValue() == "Router") {
							RouterU = *R ;
							break;
						}
					}
					for (R = Iv->v_begin() ; R != Iv->v_end() ; R++) {
						if ((*Iv)[*R].PRJ(Type)GetValue() == "Router") {
							RouterV = *R ;
							break;
						}
					}

					double Delay =mNetPlt->GetLatencyValue("Net", Iu, *U,
							mSpec->GetName(*V), *U, RouterU)
							+ mNetPlt->GetLatencyValue("Net", Iv, *U,
									mSpec->GetName(*V), RouterV, *V) ;
					L.push_back(Delay) ;
					Slacks.push_back(Latency-Delay) ;
					Hslacks.push_back(Latency-Delay) ;
				}
			}
		}
	}

	double MinLatency= DBL_MAX;
	double MaxLatency = 0;
	double AvgLatency = 0;
	double StdDevLatency = 0;
	double MinSlack= DBL_MAX;
	double MaxSlack = 0;
	double AvgSlack = 0;
	double StdDevSlack = 0;
	double MinHslack= DBL_MAX;
	double MaxHslack = 0;
	double AvgHslack = 0;
	double StdDevHslack = 0;
	double MinBandwidth= DBL_MAX;
	double MaxBandwidth = 0;
	double AvgBandwidth = 0;
	double StdDevBandwidth = 0;
	//min max and total
	for (unsigned int i = 0; i < B.size() ; i++) {
		if (B[i] < MinBandwidth ) {
			MinBandwidth = B[i];
		}
		if (B[i] > MaxBandwidth ) {
			MaxBandwidth = B[i];
		}
		AvgBandwidth += B[i];
	}
	AvgBandwidth = AvgBandwidth / (double) B.size() ;
	//stddev
	for (unsigned int i = 0; i < B.size() ; i++) {
		StdDevBandwidth += (B[i] - AvgBandwidth)*(B[i] - AvgBandwidth);
	}
	StdDevBandwidth = sqrt(StdDevBandwidth) ;

	//min max and total
	for (unsigned int i = 0; i < L.size() ; i++) {
		if (L[i] < MinLatency ) {
			MinLatency = L[i];
		}
		if (L[i] > MaxLatency ) {
			MaxLatency = L[i];
		}
		AvgLatency += L[i];
	}
	AvgLatency = AvgLatency / (double) L.size() ;
	//stddev
	for (unsigned int i = 0; i < L.size() ; i++) {
		StdDevLatency += (L[i] - AvgLatency)*(L[i] - AvgLatency);
	}
	StdDevLatency = sqrt(StdDevLatency) ;

	//min max and total
	for (unsigned int i = 0; i < Slacks.size() ; i++) {
		if (Slacks[i] < MinSlack ) {
			MinSlack = Slacks[i];
		}
		if (Slacks[i] > MaxSlack ) {
			MaxSlack = Slacks[i];
		}
		AvgSlack += Slacks[i];
	}
	AvgSlack = AvgSlack / (double) Slacks.size() ;
	//stddev
	for (unsigned int i = 0; i < Slacks.size() ; i++) {
		StdDevSlack += (Slacks[i] - AvgSlack)*(Slacks[i] - AvgSlack);
	}
	StdDevSlack = sqrt(StdDevSlack) ;

	//min max and total
	for (unsigned int i = 0; i < Hslacks.size() ; i++) {
		if (Hslacks[i] < MinHslack ) {
			MinHslack = Hslacks[i];
		}
		if (Hslacks[i] > MaxHslack ) {
			MaxHslack = Hslacks[i];
		}
		AvgHslack += Hslacks[i];
	}
	AvgHslack = AvgHslack / (double) Hslacks.size() ;
	//stddev
	for (unsigned int i = 0; i < Hslacks.size() ; i++) {
		StdDevHslack += (Hslacks[i] - AvgHslack)*(Hslacks[i] - AvgHslack);
	}
	StdDevHslack = sqrt(StdDevHslack) ;

	F << "Latency and bandwdith analysis : "<< endl ;
	F << "Bus bandwidth :  min = "<< MinBandwidth << "  max = "<< MaxBandwidth
			<< "  avg = "<< AvgBandwidth << " sigma = "<< StdDevBandwidth
			<< endl ;
	//	F << "End-to-end latency :  min = "<< MinLatency << "  max = "<< MaxLatency
	//			<< "  avg = "<< AvgLatency<< " sigma = "<< StdDevLatency << endl ;
	//	F << "End-to-end slacks :  min = "<< MinSlack << "  max = "<< MaxSlack
	//			<< "  avg = "<< AvgSlack<< " sigma = "<< StdDevSlack << endl ;
	//
	//	F << endl << endl ;
	//
	//	F << "Analysis of the higher level network (not implemented yet) "<< endl ;
	//	F << "Total bandwidth "<< Hbandwidth << endl ;
	//	F << "End-to-end slacks :  min = "<< MinHslack << "  max = "<< MaxHslack
	//			<< "  avg = "<< AvgHslack<< " sigma = "<< StdDevHslack << endl ;
	F << endl ;
	F.close();
}

SynthesisReport::~SynthesisReport() {

}

}

}

}