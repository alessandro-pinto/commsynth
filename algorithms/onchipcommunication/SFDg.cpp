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
#include "SFDg.h"

namespace cosi {

namespace algorithms {

namespace onchipcommunication {

SFDg::SFDg() {
	is_initialized = 0;
	has_run = 0;
	mCheckDegree = true;
	mCheckCapacity = true;
	mCanInstallNodes = true;
	mSourceDestDontTouch = false;
}

SFDg::~SFDg() {
	delete mCDG;
}

void SFDg::initialize(int Src, int Dest, double W,
		map< vector<int> , double > InstalledEdges,
		map< vector< int > , set< pair<int,int> > > EdgeCommodities,
		cosi::commstruct::onchipcommunication::ChannelDependencyGraph* CDG,
		map< int , vector<int> > InstalledNodes,
		cosi::commstruct::onchipcommunication::Specification* PtP,
		cosi::platforms::onchipcommunication::NocPlatformInterface* Plt) {

	mSrc = Src ;
	mDest = Dest ;
	mInstalledEdges = InstalledEdges ;
	mEdgeCommodities = EdgeCommodities ;
	mInstalledNodes = InstalledNodes ;
	mPlt = Plt ;
	mPtP = PtP ;
	mW = W ;
	mCDG = new cosi::commstruct::onchipcommunication::ChannelDependencyGraph( "CDG" );
	mCDG->Union( *CDG ) ;

	//Set size and allocate pi

	if (is_initialized ) {
		mPi.clear() ;
	} else {
		is_initialized = 1;
	};

	mSize = mPlt->Size();

	mPi.resize(mSize , -1);

	if (mSrc >= mSize || mDest >= mSize ) {
		std::cerr << " Requesting to route from "<< mSrc << " to "<< mDest
				<< endl ;
		std::cerr << " But the graph size is "<< mSize << endl ;
		exit( 1) ;
	}
}

void SFDg::CheckDegree(bool B) {
	mCheckDegree = B ;
}

void SFDg::CheckCapacity(bool B) {
	mCheckCapacity = B ;
}

void SFDg::CanInstallNodes(bool B) {
	mCanInstallNodes = B ;
}

void SFDg::DontTouchSourceDest(bool B) {
	mSourceDestDontTouch = B ;
}

void SFDg::InputLocked(set<int> V) {
	mInputLocked = V ;
}

void SFDg::OutputLocked(set<int> V) {
	mOutputLocked = V ;
}

void SFDg::InputConstrained(set<int> V) {
	mInputConstrained = V ;
}

void SFDg::OutputConstrained(set<int> V) {
	mOutputConstrained = V ;
}

vector<int> SFDg::GetPath() {
	return mPi ;
}

bool SFDg::Run() {

	//cout << "Routing form " << mSrc << " to " << mDest << endl ;

	/*Variables*/
	int index;

	/*Variables declaration*/
	Queue Q;
	vector< bool > Found;
	vector< double > Fp;
	Fp.resize(mSize , DBL_MAX) ;
	Found.resize(mSize , false) ;

	/*Auxiliary variables*/
	int current_v;
	double Cost;
	list< int > N;
	list<int>::iterator p_U;

	Q.Insert(mSrc ) ;
	Fp[mSrc] = 0.0;
	Found[mSrc] = true;
	//mDependencySet = mPathDependencies[ pair<int,int>( mSrc, mDest ) ] ;

	while ( !Q.Empty() ) {

		current_v = Q.ExtractMin(Fp ) ;

		//if different from the source then add the dependency
		if (mPlt->AvoidDeadlock() ) {
			if (current_v != mSrc && mPi[current_v] != mSrc ) {
				mCDG->AddDependency(pair<int, int>(mPi[mPi[current_v]],
						mPi[current_v]) , pair<int, int>(mPi[current_v],
						current_v) ) ;
			}
		}

		//cout << "Current vertex " << current_v << endl ;

		if (current_v == mDest ) {
			//cout << "Shortest path found  " << endl ;
			if (Fp[current_v] == DBL_MAX) {
				cout << "Returning an unfeasible path "<< endl ;
			}

			Fp.clear() ;
			Found.clear() ;
			return true;
		}

		/*for all vertexes adjacent to current_v*/
		N.clear() ;

		N = mPlt->Neighbors(current_v , mDest ) ;

		list<int>::iterator It;
		bool Isolated = true;
		for (It = N.begin() ; It != N.end(); It++) {
			vector<int> edge(2);
			edge[0] = current_v ;
			edge[1] = *It;
			if (current_v == mSrc && mSourceDestDontTouch
					&& mInstalledEdges.find(edge) != mInstalledEdges.end()) {
				cout << "Source "<< mSrc << " is not isolated "<< endl ;
			}
			if (*It == mDest && mSourceDestDontTouch
					&& mInstalledEdges.find(edge) != mInstalledEdges.end()) {
				cout << "Destination "<< mDest << " is not isolated "<< endl ;
			}
		}

		for (p_U = N.begin() ; p_U != N.end() ; p_U++) {
			//cout << "Reachable node "<< *p_U << endl ;
			Cost = ComputeEdgeCost(current_v , *p_U ) ;
			//cout << "Cost "<< Cost << endl ;
			//check is the node is new and if new nodes can be installed
			if (mCanInstallNodes ||( ( !mCanInstallNodes )
					&&mInstalledNodes.find( *p_U ) != mInstalledNodes.end() )) {

				if (Fp[*p_U] > Fp[current_v] + Cost ) {

					if ( !Found[*p_U]) {
						Q.Insert( *p_U ) ;
						Found[*p_U] = true;
					}

					//CosiLog << "Relaxing ("<< current_v << ","<< *p_U
						//	<< ") Cost="<< Fp[*p_U];

					Fp[*p_U] = Fp[current_v] + Cost ;
					//CosiLog << " New Cost="<< Fp[*p_U]<< endl ;

					mPi[*p_U] = current_v ;
				}
			}
		}
	}
	//cerr << "Path  " << mSrc << "  To  " << mDest << " could not be routed."  << endl ;
	return false;
}

double SFDg::ComputeEdgeCost(int U, int V) {

	double DeltaSourceDynPower = 0.0;
	double DeltaDestDynPower = 0.0;
	double DeltaEdgeDynPower = 0.0;
	double DeltaSourceLeakPower = 0.0;
	double DeltaDestLeakPower = 0.0;
	double DeltaEdgeLeakPower = 0.0;
	double DeltaSourceArea = 0.0;
	double DeltaDestArea = 0.0;
	double DeltaEdgeArea = 0.0;

	double W;
	double DeltaW;

	{
		vector<int> edge(2);
		edge[0] = U ;
		edge[1] = V ;
		if ( (mSrc == U) && mSourceDestDontTouch && (mInstalledEdges.find(edge)
				== mInstalledEdges.end())) {
			return DBL_MAX;
		}
		if (mDest == V && mSourceDestDontTouch && mInstalledEdges.find(edge)
				== mInstalledEdges.end() ) {
			return DBL_MAX;
		}

	}

	if ( ( !mPlt->AllowPtP() ) && (mSrc == U && mDest == V )) {
		CosiLog << "PtP violation  U="<< U << "  V="<< V << endl ;
		return DBL_MAX;
	}

	if (CyclicDependency(U , V ) ) //avoid deadlock
	{
		CosiLog << "Cyclic dependency constraint violation on edge "<< U << " "
				<< V << endl ;
		return DBL_MAX;
	}

	if (IsInstalled(U , V ) ) {
		//Compute the incremental bandwidth
		//Find the highest commodity that is in exclusion with the current one
		vector<int> edge(2);
		edge[0] = U ;
		edge[1] = V ;
		pair<int,int> ToAdd;
		ToAdd.first = mSrc ;
		ToAdd.second = mDest ;
		DeltaW = ComputeAdditionalBandwidth(mEdgeCommodities[ edge ], ToAdd ) ;
		W = InstalledBandwidth(U , V ) ;
		if (mCheckCapacity && ( (W + DeltaW ) / (double) mPlt->FlitWidth()
				> mPlt->RouterMaxBw() )) // Bandwidth violation
		{
			CosiLog << "Bandwidth violation on edge "<< U << " "<< V << endl ;
			return DBL_MAX;
		}
		//if ( CyclicDependency( U , V ) ) //avoid deadlock
		// {
		//cout << "Cyclic dependency constraint violation on edge " << U << " " << V << endl ;
		//  return DBL_MAX;
		//}

		vector<int> degreeu;
		vector<int> degreev;
		degreeu = mInstalledNodes[U];
		degreev = mInstalledNodes[V];
		DeltaSourceDynPower = mPlt->RouterEnergy(degreeu[0], degreeu[1])
				* DeltaW / (double)mPlt->FlitWidth() ;
		DeltaDestDynPower = mPlt->RouterEnergy(degreev[0], degreev[1]) * DeltaW
				/ (double)mPlt->FlitWidth() ;
		DeltaEdgeDynPower = mPlt->WireEnergy() * DeltaW* mPlt->Distance(U , V );

	} else {

		DeltaEdgeDynPower = mPlt->WireEnergy() * mW * mPlt->Distance(U , V ); //Dynamic Energy of wires
		DeltaEdgeLeakPower = mPlt->WireLeakage()*mPlt->FlitWidth()
				*mPlt->Distance(U , V ) ; //Static Energy
		DeltaEdgeArea = mPlt->FlitWidth()
				* mPlt->WireArea(mPlt->Distance(U , V ) ) ; //Area cost

		//Check if U is installed
		//U has been either installed by previous paths or it
		//was installed by this path, but in this case we paid already for the installation cost
		if (IsInstalled(U ) ) {
			double Rate;
			vector< int > degreeu;
			degreeu = mInstalledNodes[U];
			if ( mPtP->InV(U) ) {
				Rate = OutputRate(U ) ;
			} else
				Rate = InputRate(U ) ;

			//Check outdegree
			if (mCheckDegree ) {

				if (mOutputLocked.find(U ) != mOutputLocked.end() ) {
					return DBL_MAX;
				}

				if ( mPtP->InV(U) && (degreeu[1] + 1
						> mPlt->SourceOutDegree() )) { // too many outputs for the source
					CosiLog << "Source degree constraint violation on edge "
							<< U << " "<< V << endl ;
					return DBL_MAX;
				}

				if ( (degreeu[1] + 1> mPlt->MaxOutDegree() )) {
					CosiLog << "Router degree constraint violation on edge "
							<< U << " "<< V << endl ;
					return DBL_MAX;
				}

			}

			DeltaSourceDynPower
					= (mPlt->RouterEnergy(degreeu[0], degreeu[1]+ 1)*(Rate+mW )
							-mPlt->RouterEnergy(degreeu[0], degreeu[1])*Rate )
							/(double)mPlt->FlitWidth() ;
			DeltaSourceLeakPower = (mPlt->RouterLeakage(degreeu[0], degreeu[1]
					+ 1) - mPlt->RouterLeakage(degreeu[0], degreeu[1]) )
					* mPlt->OperatingFrequency() ;
			DeltaSourceArea = (mPlt->RouterArea(degreeu[0], degreeu[1] +1)
					- mPlt->RouterArea(degreeu[0], degreeu[1]) );

			if (DeltaSourceArea < 0) {
				cout << "Input degree "<< degreeu[0]<< "  Output degree "
						<< degreeu[1]<< endl;
				cout << "Augmented area "<< mPlt->RouterArea(degreeu[0],
						degreeu[1] +1) << endl ;
				cout << "Area "<< mPlt->RouterArea(degreeu[0], degreeu[1])
						<< endl ;
			}

		}
		//Check if V is installed
		if (IsInstalled(V ) ) {
			double Rate;
			vector< int > degreev;
			degreev = mInstalledNodes[V];
			Rate = InputRate(V ) ;

			if (mCheckDegree ) {

				if (mInputLocked.find(V ) != mInputLocked.end() ) {
					return DBL_MAX;
				}

				if ( mPtP->InV(V) && (degreev[0] + 1
						> mPlt->DestInDegree() )) { //too many inputs for the destination

					CosiLog
							<< "Destination degree constraint violation on edge "
							<< U << " "<< V << endl ;
					return DBL_MAX;
				} else if ( (degreev[0] + 1> mPlt->MaxInDegree() )) {
					return DBL_MAX;
				}
			}

			DeltaDestDynPower
					= (mPlt->RouterEnergy(degreev[0] + 1, degreev[1])*(Rate+mW)
							-mPlt->RouterEnergy(degreev[0], degreev[1])*Rate )
							/(double)mPlt->FlitWidth() ;
			DeltaDestLeakPower = (mPlt->RouterLeakage(degreev[0] + 1,
					degreev[1]) - mPlt->RouterLeakage(degreev[0], degreev[1]) )
					* mPlt->OperatingFrequency() ;
			DeltaDestArea = (mPlt->RouterArea(degreev[0] + 1, degreev[1])
					- mPlt->RouterArea(degreev[0], degreev[1]) );

			if (DeltaDestArea < 0) {
				cout << "Input degree "<< degreev[0]<< "  Output degree "
						<< degreev[1]<< endl;
				cout << "Augmented area "<< mPlt->RouterArea(degreev[0] + 1,
						degreev[1]) << endl ;
				cout << "Area "<< mPlt->RouterArea(degreev[0], degreev[1])
						<< endl ;
			}

		} else {
			DeltaDestDynPower = mPlt->RouterEnergy( 1, 1)*mW
					/ (double)mPlt->FlitWidth() ;
			DeltaDestLeakPower = mPlt->RouterLeakage( 1, 1)
					* mPlt->OperatingFrequency() ;
			DeltaDestArea = mPlt->RouterArea( 1, 1) ;

		}
	}

	double RetVal;

	if (DeltaSourceArea < 0)
		cout << "Delta source area "<< DeltaSourceArea << endl ;
	if (DeltaDestArea < 0)
		cout << "Delta dest area "<< DeltaDestArea << endl ;
	if (DeltaEdgeArea < 0)
		cout << "Delta edge area "<< DeltaEdgeArea << endl ;

	if (DeltaSourceDynPower < 0)
		cout << "Delta source dyn power "<< DeltaSourceDynPower << endl ;

	if (DeltaDestDynPower < 0)
		cout << "Delta dest dyn power "<< DeltaDestDynPower << endl ;
	if (DeltaEdgeDynPower < 0)
		cout << "Delta edge dyn power "<< DeltaEdgeDynPower << endl ;

//	cout << "Router area cost "<< DeltaDestArea + DeltaSourceArea << endl ;
//	cout << "Wire area cost "<< DeltaEdgeArea << endl ;
//	cout << "Router power cost "<< DeltaSourceDynPower + DeltaDestDynPower + DeltaSourceLeakPower + DeltaDestLeakPower<< endl ;
//	cout << "Wire power cost "<< DeltaEdgeDynPower +  DeltaEdgeLeakPower << endl ;
//	cout << "Switching fact " << mPlt->SwitchingFactor() <<endl ;
//	cout << "Power/Area " << mPlt->PowerVsArea() <<endl ;


	RetVal = mPlt->PowerVsArea() * ( (mPlt->SwitchingFactor()
			* (DeltaSourceDynPower + DeltaDestDynPower + DeltaEdgeDynPower ))
			+(DeltaSourceLeakPower + DeltaDestLeakPower + DeltaEdgeLeakPower ))
			+(1.0 - mPlt->PowerVsArea() )*(DeltaSourceArea + DeltaDestArea
					+ DeltaEdgeArea );

	return RetVal;

}

bool SFDg::IsInstalled(int U, int V) {

	vector<int> edge(2);
	edge[0] = U ;
	edge[1] = V ;
	return (mInstalledEdges.find(edge ) != mInstalledEdges.end() );

}

bool SFDg::IsInstalled(int U) {

	return (mInstalledNodes.find(U ) != mInstalledNodes.end() );

}

double SFDg::InstalledBandwidth(int U, int V) {

	vector<int> edge(2);
	edge[0] = U ;
	edge[1] = V ;
	if (mInstalledEdges.find(edge ) != mInstalledEdges.end() )
		return mInstalledEdges[edge];
	else
		return 0.0;

}

double SFDg::InputRate(int U) {
	double Rate;
	vector<int> edge;
	map< vector<int> , double >::iterator E;
	Rate = 0;
	for (E = mInstalledEdges.begin() ; E != mInstalledEdges.end() ; E++) {
		edge = E->first;
		if (edge[1] == U ) { // this is an input to U
			Rate = Rate + E->second;
		}
	}

	return Rate ;

}

double SFDg::OutputRate(int U) {
	double Rate;
	vector<int> edge;
	map< vector<int> , double >::iterator E;
	Rate = 0;
	for (E = mInstalledEdges.begin() ; E != mInstalledEdges.end() ; E++) {
		edge = E->first;
		if (edge[0] == U ) { // this is an output
			Rate = Rate + E->second;
		}
	}

	return Rate ;

}

double SFDg::ComputeAdditionalBandwidth(set< pair<int,int> > Commodities,
		pair<int,int> Commodity) {
	if (Commodities.find(Commodity) != Commodities.end() ) {
		return 0;
	}

	return (*mPtP)[cosi::Edge(Commodity.first,Commodity.second)].PRJ(CommoditySet)GetAggregateBandwidthValue() ;

	//	set< pair<int,int> > ExclusionSet;
	//	set< pair<int,int> > Intersection;
	//	set< pair<int,int> >::iterator It;
	//	ExclusionSet = mPtP->GetExclusionSet(Commodity ) ;
	//	set_intersection(ExclusionSet.begin() , ExclusionSet.end() ,
	//			Commodities.begin() , Commodities.end() , inserter(Intersection,
	//					Intersection.begin() ) );
	//	double MaxFlow = 0.0;
	//	for (It = Intersection.begin() ; It !=Intersection.end() ; It++) {
	//		if (mPtP->GetFlow(It->first, It->second) > MaxFlow )
	//			MaxFlow = mPtP->GetFlow(It->first, It->second) ;
	//	}
	//	double DeltaW;
	//	DeltaW = mW - MaxFlow ;
	//	if (DeltaW > 0.0)
	//		return DeltaW ;
	//	else
	//		return 0.0;

}

bool SFDg::CyclicDependency(int U, int V) {
	if (mPlt->AvoidDeadlock() ) {
		return (mCDG->Reachable(pair<int, int>(U , V ) , pair<int, int>(mPi[U],
				U ) ) );
	} else
		return false;
}
}
}
}

