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
#include "MFDg.h"

namespace cosi {

namespace algorithms {

namespace onchipcommunication {

MFDg::MFDg() {
	is_initialized = 0;
	mDontTouch = false;
	mSourceDestDontTouch = false;
}

MFDg::~MFDg() {

	mPi.clear() ;

	mSrc.clear() ;

	mDest.clear() ;

	mW.clear() ;

}

void MFDg::Initialize(
		cosi::commstruct::onchipcommunication::Specification* PtP,
		cosi::platforms::onchipcommunication::NocPlatformInterface* Plt) {
	if (is_initialized ) {
		mPi.clear() ;
		mSrc.clear() ;
		mDest.clear() ;
		mW.clear() ;
		delete mCDG;
	} else {
		is_initialized = 1;
	}

	mPtP = PtP ;
	mPlt = Plt ;

	mCDG = new cosi::commstruct::onchipcommunication::ChannelDependencyGraph( "NoName");

	//    EXTRACT POINT-TO-POINT CONSTRAINTS
	// Count the number of constraints
	mNc = 0;

	IdGraph::v_iterator U, V;
	for (U = mPtP->v_begin() ; U != mPtP->v_end() ; U++) {
		for (V = mPtP->v_begin() ; V != mPtP->v_end() ; V++) {
			if (mPtP->InE(*U, *V)) {
				mNc++;
				mSrc.push_back(*U) ;
				mDest.push_back(*V) ;
				mW.push_back((*mPtP)[Edge(*U,*V)].PRJ(CommoditySet)GetAggregateBandwidthValue() ) ;
			}
		}
	}

}

void MFDg::Initialize(
		cosi::commstruct::onchipcommunication::Specification* PtP,
		cosi::platforms::onchipcommunication::NocPlatformInterface* Plt,
		map< int , vector< int > > pNodes, map< vector<int > , double > pEdges) {
	mDontTouch = true;
	mDontTouchNodes = pNodes ;
	mDontTouchEdges = pEdges ;
	mInstalledNodes = pNodes ;
	mInstalledEdges = pEdges ;
	Initialize(PtP, Plt) ;
	//set dont touch for sources and destinations
	mSourceDestDontTouch = true;
}

void MFDg::DontTouchSourceDest(bool B) {
	mSourceDestDontTouch = B ;
}

map< pair<int,int> , map<int,int> > MFDg::GetPaths() {
	return mPi ;
}

map<int,int> MFDg::GetPath(int i) {
	return mPi[pair<int,int>( mSrc[i] , mDest[i] ) ];
}

map<int,int> MFDg::GetPath(int Src, int Dest) {
	return mPi[pair<int,int>( Src , Dest ) ];
}

bool MFDg::Run() {

	cout << endl
			<< "MFDg : Running optimization without degree constraints ...  "
			<< endl ;

	if ( !RunFeasibility() )
		return false;

	if (!RunNoDegree() )
		return false;

	cout << "Running the degree reduce algorithm ...  "<< endl ;

	while ( !RunDegreeReduce() ) {

		if ( !RunAddRouter() ) {
			return false;
		}

	}
	cout << "Done "<< endl ;
	return true;
}

bool MFDg::Run_2() {

	cout << endl
			<< "MFDg_2 : Running optimization without degree constraints ...  "
			<< endl ;

	RunFeasibility() ;

	vector<int> mark;
	vector<int> ordering;
	double max_found;
	int max_index;

	int index, r_index, c_index;
	int commodity;

	int current_node;
	int prev_node;

	mark.resize(mNc , 0) ;
	ordering.resize(mNc ) ;

	/*Sort all commodities*/

	for (r_index = 0; r_index < mNc ; r_index++) {
		max_found = 0;
		for (c_index = 0; c_index < mNc ; c_index++) {
			if (mW[c_index] >= max_found && !mark[c_index]) {
				max_found = mW[c_index];
				max_index = c_index ;
			}
		}
		ordering[r_index] = max_index ;
		mark[max_index] = 1;
	}

	vector<int> edge( 2);
	SFDg* sp;
	vector< int > mCurrentPath;
	for (index = 0; index < mNc ; index++) {

		commodity = ordering[ index ];
		mOrder[pair<int,int>( mSrc[commodity] , mDest[commodity])] = index ;
		cout << " Routing constraint from "<< mSrc[commodity]<< " to "
				<< mDest[commodity]<< "   Bw = "<< mW[commodity]<< endl ;
		sp = new SFDg;
		sp->initialize(mSrc[commodity], mDest[commodity], mW[commodity],
				mInstalledEdges , mEdgeCommodities , mCDG , mInstalledNodes ,
				mPtP , mPlt );

		sp->CheckDegree( false) ;
		sp->CheckCapacity( true) ;
		sp->CanInstallNodes( true) ;
		sp->Run();

		mCurrentPath = sp->GetPath() ;

		delete sp;

		AddReversePath(mSrc[ commodity ], mDest[ commodity ], mCurrentPath ) ;
		while ( !RunDegreeReduce() ) {
			RunAddRouter() ;
			//mU.clear( ) ;
			//mV.clear( ) ;
			//mE.clear( ) ;
		}
		mU.clear() ;
		mV.clear() ;

	}
	//cout << "Done "<< endl ;
	//cout << "Running the degree reduce algorithm ...  "<< endl ;
	cout << "Done "<< endl ;
	return true;
}

bool MFDg::RunFeasibility() {

	IdGraph::v_iterator U, V;
	for (U = mPtP->v_begin() ; U != mPtP->v_end() ; U++) {
		double Rate = 0;
		for (V = mPtP->v_begin() ; V != mPtP->v_end() ; V++) {
			if (mPtP->InE( *U, *V ) ) {
				Rate += (*mPtP)[Edge(*U,*V)].PRJ(CommoditySet)GetAggregateBandwidthValue() ;
			}
		}
		if (mPlt->RouterMaxBw() * mPlt->SourceOutDegree() * mPlt->FlitWidth()
				< Rate ) {
			cout
					<< "The problem is not feasible, please increase the number of output ports of a source"
					<< endl;
			cout << "Source "<< mPtP->GetName( *U) << " output bandwidth "
					<< Rate << endl ;
			cout << "Maximum channel frequency "<< mPlt->RouterMaxBw() << endl ;
			cout << "Maximum source output degree "<< mPlt->SourceOutDegree()
					<<endl ;
			cout << "Flit-width "<< mPlt->FlitWidth() << endl ;
			return false;
		}
	}
	for (U = mPtP->v_begin() ; U != mPtP->v_end() ; U++) {
		double Rate = 0;
		for (V = mPtP->v_begin() ; V != mPtP->v_end() ; V++) {
			if (mPtP->InE( *V, *U ) ) {
				Rate += (*mPtP)[Edge(*V,*U)].PRJ(CommoditySet)GetAggregateBandwidthValue() ;
			}
		}
		if (mPlt->RouterMaxBw() * mPlt->DestInDegree() * mPlt->FlitWidth()
				< Rate ) {
			cout
					<< "The problem is not feasible, please increase the number of input ports of a destination"
					<< endl;
			cout << "Destination "<< mPtP->GetName(*U) << " input bandwidth "
					<< Rate<< " maximum input bandwidth "<< mPlt->RouterMaxBw()
					* mPlt->DestInDegree()* mPlt->FlitWidth() << endl ;
			return false;
		}
	}
	return true;
}

bool MFDg::RunNoDegree() {

	vector<int> mark;
	vector<int> ordering;
	double max_found;
	int max_index;

	int index, r_index, c_index;
	int commodity;

	int current_node;
	int prev_node;

	mark.resize(mNc , 0);
	ordering.resize(mNc );

	/*Sort all commodities*/

	for (r_index = 0; r_index < mNc; r_index++) {
		max_found = 0;
		for (c_index = 0; c_index < mNc; c_index++) {
			if (mW[c_index] >= max_found && !mark[c_index]) {
				max_found = mW[c_index];
				max_index = c_index;
			}
		}
		ordering[r_index] = max_index;
		mark[max_index] = 1;
	}

	vector<int> edge( 2);
	SFDg* sp;
	vector< int > mCurrentPath;
	for (index = 0; index < mNc; index++) {

		commodity = ordering[ index ];
		mOrder[pair<int,int>( mSrc[commodity] , mDest[commodity])] = index;
		cout << " Routing constraint from "<< mSrc[commodity]<< " to "
				<< mDest[commodity]<< "   Bw = "<< mW[commodity]<< endl;
		sp = new SFDg;
		sp->initialize(mSrc[commodity], mDest[commodity], mW[commodity],
				mInstalledEdges , mEdgeCommodities , mCDG , mInstalledNodes ,
				mPtP , mPlt );

		sp->CheckDegree( false);
		sp->CheckCapacity( true);
		sp->CanInstallNodes( true);
		sp->DontTouchSourceDest(mSourceDestDontTouch);
		if (sp->Run() ) {
			mCurrentPath = sp->GetPath();
			delete sp;
			AddReversePath(mSrc[ commodity ], mDest[ commodity ], mCurrentPath );
		} else {
			delete sp;
			return false;
		}
	}

	/*Variables deallocation*/
	mark.clear();
	ordering.clear();

	cout << "MFD : Done "<< endl << endl;
	return true;
}

bool MFDg::RunDegreeReduce() {

	bool EdgeRemoved = true;
	bool DegreeViolation = true;

	while (EdgeRemoved ) {
		EdgeRemoved = false;

		///////////////////////////////////////////////////////////////////////////
		//
		//           UPDATE LINK METRICS AND NODE VIOLATIONS
		//           U : NODES WITH OUTPUT DEGREE VIOLATIONS
		//           V : NODES WITH INPUT DEGREE VIOLATIONS
		//
		///////////////////////////////////////////////////////////////////////////

		mU.clear();
		mV.clear();
		map<vector<int> , double >::iterator EdgeIt;
		map< pair<int,int> , double > EdgeMetric;
		map< pair<int,int> , double > NotRemoved;
		for (EdgeIt = mInstalledEdges.begin(); EdgeIt != mInstalledEdges.end(); EdgeIt++) {
			int Origin = (EdgeIt->first)[0];
			int Target = (EdgeIt->first)[1];
			if ( mPtP->InV(Origin) && mInstalledNodes[Origin][1]
					> mPlt->SourceOutDegree() ) {
				//Compute the edge metric
				double Metric = (mPlt->RouterMaxBw()* (double)mPlt->FlitWidth()
						- EdgeIt->second)* (mPlt->Distance(Origin , Target ) );
				//				if (Metric == 0) {
				//					cout << "Origin "<< Origin << " Target "<< Target<< "  Bw "
				//							<< EdgeIt->second<< " Capacity  "
				//							<< mPlt->RouterMaxBw() * (double)mPlt->FlitWidth()
				//							<< "  distance "<< mPlt->Distance(Origin , Target )
				//							<< endl ;
				//				}
				//save the edge
				EdgeMetric[ pair<int,int>(Origin,Target) ] = Metric;
				if (mInstalledNodes[Origin][1] > mPlt->SourceOutDegree() ) {
					DegreeViolation = true;
				}
				mU.insert(Origin );

			}

			if (  mPtP->InV(Target) && mInstalledNodes[Target][0]
					> mPlt->DestInDegree() ) {
				//Compute the edge metric
				double Metric = (mPlt->RouterMaxBw()* (double)mPlt->FlitWidth()
						- EdgeIt->second)* (mPlt->Distance(Origin , Target ) );
				//				if (Metric == 0) {
				//					cout << "Origin "<< Origin << " Target "<< Target<< "  Bw "
				//							<< EdgeIt->second<< " Capacity  "
				//							<< mPlt->RouterMaxBw() * (double)mPlt->FlitWidth()
				//							<< "  distance "<< mPlt->Distance(Origin , Target )
				//							<< endl ;
				//				}
				//save the edge
				EdgeMetric[ pair<int,int>(Origin,Target) ] = Metric;
				if (mInstalledNodes[Target][0] > mPlt->DestInDegree() )
					DegreeViolation = true;

				mV.insert(Target );
			}
			if (!mPtP->InV(Origin) && mInstalledNodes[Origin][1]
					> mPlt->MaxOutDegree() ) {
				//Compute the edge metric
				double Metric = (mPlt->RouterMaxBw()* (double)mPlt->FlitWidth()
						- EdgeIt->second)* (mPlt->Distance(Origin , Target ) );
				//save the edge
				//				if (Metric == 0) {
				//					cout << "Origin "<< Origin << " Target "<< Target<< "  Bw "
				//							<< EdgeIt->second<< " Capacity  "
				//							<< mPlt->RouterMaxBw() * (double)mPlt->FlitWidth()
				//							<< "  distance "<< mPlt->Distance(Origin , Target )
				//							<< endl ;
				//				}
				EdgeMetric[ pair<int,int>(Origin,Target) ] = Metric;
				if (mInstalledNodes[Origin][1] > mPlt->MaxOutDegree() )
					DegreeViolation = true;
				mU.insert(Origin );

			}
			if ( !mPtP->InV(Target) && mInstalledNodes[Target][0]
					> mPlt->MaxInDegree() ) {
				double Metric = (mPlt->RouterMaxBw()* (double)mPlt->FlitWidth()
						- EdgeIt->second)* (mPlt->Distance(Origin , Target ) );
				//				if (Metric == 0) {
				//					cout << "Origin "<< Origin << " Target "<< Target<< "  Bw "
				//							<< EdgeIt->second<< " Capacity  "
				//							<< mPlt->RouterMaxBw() * (double)mPlt->FlitWidth()
				//							<< "  distance "<< mPlt->Distance(Origin , Target )
				//							<< endl ;
				//				}
				//save the edge
				EdgeMetric[ pair<int,int>(Origin,Target) ] = Metric;
				if (mInstalledNodes[Target][0] > mPlt->MaxInDegree() )
					DegreeViolation = true;
				mV.insert(Target );
			}
		}

		int NedgeRemoved = 0;

		NotRemoved = EdgeMetric;

		map< pair<int,int> , double >::iterator EdgeMetricIt;

		cout << mV.size() << " input violations    and   "<< mU.size()
				<< " output violations "<< endl;

		///////////////////////////////////////////////////////////////////////
		//
		//       WHILE NO EDGES HAVE BEED REMOVED
		//          TRY TO REMOVE THE WORST EDGE
		//          IF IT CANNOT BE REMOVED, THEN BACKTRACK
		//
		///////////////////////////////////////////////////////////////////////

		while ( (NedgeRemoved == 0) && (EdgeMetric.size() > 0)) {

			//          FIND THE EDGE WITH THE MAXIMUM INSTALLATION COST WITH RESPECT TO THE BANDWIDTH THAT IT CARRIES

			double MaxMetric= DBL_MIN;
			int Origin, Target;
			for (EdgeMetricIt = EdgeMetric.begin(); EdgeMetricIt
					!= EdgeMetric.end(); EdgeMetricIt++) {
				if (EdgeMetricIt->second < 0) {
					cout << "Negative edge metric "<< EdgeMetricIt->second
							<< " on edge ("<< ( EdgeMetricIt->first).first<< ","<< ( EdgeMetricIt->first).second<< ")"<< endl;
					return false;
				}
				if (EdgeMetricIt->second >= MaxMetric ) {
					MaxMetric = EdgeMetricIt->second;
					Origin = ( EdgeMetricIt->first).first;
					Target = ( EdgeMetricIt->first).second;
				}
			}

			//cout << "Trying to remove edge (" << Origin << "," << Target << ") with metric " << EdgeMetric[pair<int,int>(Origin,Target)]<<  endl ;

			EdgeMetric.erase(pair<int, int>(Origin , Target ) );

			//          REROUTE ALL THE PATHS ON THIS EDGE
			//            REMOVE ALL PATHS RELATIVE TO THE COMMODITIES ON THI EDGE


			vector<int> Edge(2);
			Edge[0] = Origin;
			Edge[1] = Target;
			set<pair<int,int> > Commodities = mEdgeCommodities[ Edge ];
			//cout << "Paths to be rerouted " << Commodities.size( ) << endl ;
			set< pair<int,int> >::iterator Cit;
			map< pair<int,int> , map<int,int> > RemovedPaths;
			map< pair<int,int> , double > RemovedArea;
			for (Cit = Commodities.begin(); Cit != Commodities.end(); Cit++) {
				RemovedPaths[ pair<int,int>(Cit->first,Cit->second) ] = GetPath(Cit->first,
						Cit->second);
				RemovePath(Cit->first, Cit->second);
			}

			//          REROUTE ALL COMMODITIES
			//             IF SOME OF SOME OF THEM CANNOT BE REROUTED, REMOVE THE ADDED PATHS AND ADD BACK THE ORIGINA PATHS

			set< pair<int,int> > AddedPath;
			SFDg* sp;
			vector<int> mCurrentPath;
			bool CannotBeRemoved = false;
			for (Cit = Commodities.begin(); Cit != Commodities.end(); Cit++) {

				//find the new shortest paths
				//cout << " Re-Routing constraint from " << Cit->first << " to " << Cit->second << "   Bw = " << mPtP->GetFlow( Cit->first , Cit->second) <<  endl ;
				sp = new SFDg;

				//Run shortest path without forbidden nodes....
				double W = (*mPtP)[cosi::Edge(Cit->first,Cit->second)].PRJ(CommoditySet)GetAggregateBandwidthValue() ;
				sp->initialize(Cit->first, Cit->second, W , mInstalledEdges ,
						mEdgeCommodities , mCDG , mInstalledNodes , mPtP , mPlt );

				sp->CheckDegree( true);
				sp->CheckCapacity( true);
				sp->CanInstallNodes( false);
				sp->DontTouchSourceDest(mSourceDestDontTouch);
				sp->InputLocked(mV );
				sp->OutputLocked(mU );

				if ( !sp->Run() ) {

					//if a constraint cannot be re-routed
					set< pair< int , int > >::iterator ToRemove;
					for (ToRemove = AddedPath.begin(); ToRemove
							!= AddedPath.end(); ToRemove++) {
						//cout << "Removing added path (" << ToRemove->first << "," << ToRemove->second << ")" << endl ;
						RemovePath(ToRemove->first, ToRemove->second);

					}
					//add the removed paths back
					map< pair<int,int> , map<int,int> >::iterator Pit;
					for (Pit = RemovedPaths.begin(); Pit != RemovedPaths.end(); Pit++) {
						//cout << "Adding back path (" << (Pit->first).first  << "," << (Pit->first).second << ")" << endl ;
						AddPath((Pit->first).first, (Pit->first).second, Pit->second);
					}
					CannotBeRemoved = true;
					break;
				} else {
					mCurrentPath = sp->GetPath();
					delete sp;
					AddReversePath(Cit->first, Cit->second, mCurrentPath );
					AddedPath.insert(pair<int, int>(Cit->first, Cit->second) );
				}
			}

			if ( !CannotBeRemoved ) {
				NedgeRemoved++;
				EdgeRemoved = true;
				NotRemoved.erase(pair<int, int>(Origin, Target) );
			}

		}

		if (NedgeRemoved == 0&& NotRemoved.size() > 0) {
			return false;
		}
	}

	return true;
}

bool MFDg::RunAddRouter() {

	//build a list of nodes with violations and of edges that
	//should be removed

	map<vector<int> , double >::iterator EdgeIt;
	//edges that can potentially be removed
	map< pair<int,int> , double > EdgeMetric;
	//nodes with input violations
	map< int , int > InViolation;
	//nodes with output violations
	map< int , int > OutViolation;
	for (EdgeIt = mInstalledEdges.begin(); EdgeIt != mInstalledEdges.end(); EdgeIt++) {
		int Origin = (EdgeIt->first)[0];
		int Target = (EdgeIt->first)[1];
		if ( mPtP->InV(Origin) && mInstalledNodes[Origin][1]
				> mPlt->SourceOutDegree() ) {
			//Compute the edge metric
			double Metric = (mPlt->RouterMaxBw() * (double)mPlt->FlitWidth()
					- EdgeIt->second) * (mPlt->Distance(Origin , Target ) );
			//save the edge
			EdgeMetric[ pair<int,int>(Origin,Target) ] = Metric;
			OutViolation[Origin] = mInstalledNodes[Origin][1]
					- mPlt->SourceOutDegree();
		}

		if ( mPtP->InV(Target) && mInstalledNodes[Target][0]
				> mPlt->DestInDegree() ) {
			//Compute the edge metric
			double Metric = (mPlt->RouterMaxBw() * (double)mPlt->FlitWidth()
					- EdgeIt->second) * (mPlt->Distance(Origin , Target ) );
			//save the edge
			EdgeMetric[ pair<int,int>(Origin,Target) ] = Metric;
			InViolation[Target] = mInstalledNodes[Target][0]
					- mPlt->DestInDegree();
		}
		if ( !mPtP->InV(Origin) && mInstalledNodes[Origin][1]
				> mPlt->MaxOutDegree() ) {
			//Compute the edge metric
			double Metric = (mPlt->RouterMaxBw() * (double)mPlt->FlitWidth()
					- EdgeIt->second) * (mPlt->Distance(Origin , Target ) );
			//save the edge
			EdgeMetric[ pair<int,int>(Origin,Target) ] = Metric;
			OutViolation[Origin] = mInstalledNodes[Origin][1]
					- mPlt->MaxOutDegree();
		}
		if ( !mPtP->InV(Target) && mInstalledNodes[Target][0]
				> mPlt->MaxInDegree() ) {
			double Metric = (mPlt->RouterMaxBw() * (double)mPlt->FlitWidth()
					- EdgeIt->second) * (mPlt->Distance(Origin , Target ) );
			//save the edge
			EdgeMetric[ pair<int,int>(Origin,Target) ] = Metric;
			InViolation[Target]=mInstalledNodes[Target][0]- mPlt->MaxInDegree();
		}

	}

	//look for a node with violation such that a router can be added
	bool RouterAdded = false;
	set< pair<int,int> > EdgesToMerge;
	map<int,int>::iterator NodeIt;
	bool InPicked;

	while ( !RouterAdded && (InViolation.size() > 0|| OutViolation.size() > 0)) {
		//pick the router with highest violation
		int In = MaxInViolation(InViolation);
		int Out = MaxOutViolation(OutViolation);
		if (InViolation.size() > 0&& InViolation[ In ] > OutViolation[Out]) {
			//Get the edges that could be removed from the input of In
			//The set of edges that can be removed had to satisfy the following constraints
			//-number of edges less than or equal to the maximum output degree of a router
			//-total bandwidth less than or equal to edge capacity
			//-Hop constraint for each flow on edges cannot be violated
			map< pair<int,int> , double > InputEdges = GetInputEdges(In ,
					EdgeMetric );
			double TotalBandwidth = 0;
			int Nedges = 0;
			//map< pair<int,int> , double >::iterator Eit ;
			while (InputEdges.size() > 0) {
				pair<int,int> Ep = MaxMetric(InputEdges );
				InputEdges.erase(Ep );
				vector<int> E(2);
				E[ 0 ] = Ep.first;
				E[ 1 ] = Ep.second;
				if (Nedges + 1<= mPlt->MaxInDegree() ) {
					TotalBandwidth += mInstalledEdges[ E ];
					if (TotalBandwidth <= mPlt->RouterMaxBw()*mPlt->FlitWidth() ) {

						EdgesToMerge.insert(Ep );
						Nedges++;

					}
				}
			}
			InViolation.erase(In );
			InPicked = true;
		} else {
			//Get the edges that could be removed from the Output of Out
			map< pair<int,int> , double > OutputEdges = GetOutputEdges(Out ,
					EdgeMetric );
			double TotalBandwidth = 0;
			int Nedges = 0;
			//map< pair<int,int> , double >::iterator Eit ;
			while (OutputEdges.size() > 0) {
				pair<int,int> Ep = MaxMetric(OutputEdges );
				OutputEdges.erase(Ep );
				vector<int> E(2);
				E[ 0 ] = Ep.first;
				E[ 1 ] = Ep.second;
				if (Nedges + 1<= mPlt->MaxOutDegree() ) {
					TotalBandwidth += mInstalledEdges[ E ];
					if (TotalBandwidth <= mPlt->RouterMaxBw()*mPlt->FlitWidth() ) {

						EdgesToMerge.insert(Ep );
						Nedges++;

					}
				}
			}
			OutViolation.erase(Out );
			InPicked = false;
		}

		//If edges can be removed than do that and set RouterAdded to true
		if (EdgesToMerge.size() > 1) {
			//Find the new router to add
			cout << "Number of edges to merge "<< EdgesToMerge.size() << endl;
			int MinId;
			if (InPicked ) {
				MinId = FindRouter(In , EdgesToMerge );
				cout << "------------------------------------------ "<< endl;
				cout << "Adding new router "<< MinId << "at the input of "<< In
						<< endl;
				cout << "Input degree before addition is "
						<< mInstalledNodes[In][0]<< endl;
				cout << "------------------------------------------ "<< endl;
			} else {
				MinId = FindRouter(Out , EdgesToMerge );
				cout << "------------------------------------------ "<< endl;
				cout << "Adding new router "<< MinId << " at the output of "
						<< Out << endl;
				cout << "Output degree before addition is "
						<< mInstalledNodes[Out][1]<< endl;
				cout << "------------------------------------------ "<< endl;
			}

			mInstalledNodes[MinId] = vector<int>(2);
			mInstalledNodes[MinId][0] = 0;
			mInstalledNodes[MinId][1] = 0;
			//Connect all edges
			set< pair<int,int> >::iterator Eit;
			for (Eit = EdgesToMerge.begin(); Eit != EdgesToMerge.end(); Eit++) {
				//connect source of the edge to router
				//connect destination router to destination of the edge
				int Origin = Eit->first;
				int Target = Eit->second;
				vector<int> edge(2);
				vector<int> O2R(2);
				vector<int> R2T(2);
				edge[0] = Origin;
				edge[1] = Target;
				O2R[0] = Origin;
				O2R[1] = MinId;
				R2T[0] = MinId;
				R2T[1] = Target;

				if (mInstalledEdges.find(O2R ) == mInstalledEdges.end() ) {
					mInstalledEdges[ O2R ] = mInstalledEdges[edge];
					mInstalledNodes[ MinId ][0] = mInstalledNodes[ MinId ][0]
							+ 1;
					mInstalledNodes[ Origin ][1] = mInstalledNodes[ Origin ][1]
							+ 1;
				} else {
					mInstalledEdges[ O2R ] += mInstalledEdges[edge];
				}
				if (mInstalledEdges.find(R2T ) == mInstalledEdges.end() ) {
					mInstalledEdges[ R2T ] = mInstalledEdges[edge];
					mInstalledNodes[ MinId ][1] = mInstalledNodes[ MinId ][1]
							+ 1;
					mInstalledNodes[ Target ][0] = mInstalledNodes[ Target ][0]
							+ 1;
				} else {
					mInstalledEdges[ R2T ] += mInstalledEdges[edge];
				}

				set<pair<int,int> > Commodities = mEdgeCommodities[ edge ];
				set< pair<int,int> >::iterator Cit;
				for (Cit = Commodities.begin(); Cit != Commodities.end(); Cit++) {
					mEdgeCommodities[ O2R ].insert( *Cit );
					mEdgeCommodities[ R2T ].insert( *Cit );
					mPi[ *Cit ][Origin] = MinId;
					mPi[ *Cit ][ MinId ] = Target;
				}

				//remove the broken edge
				mInstalledEdges.erase(edge );
				mEdgeCommodities.erase(edge);
				mInstalledNodes[ Origin ][1] = mInstalledNodes[ Origin ][1] - 1;
				mInstalledNodes[ Target ][0] = mInstalledNodes[ Target ][0] - 1;
			}
			RouterAdded = true;
		} else {
			EdgesToMerge.clear();
		}
		//otherwise try another node
	}
	if ( !RouterAdded ) {
		cout << "CosiNoc could not find a solution"<< endl;
		return false;
	}
	return true;
}

cosi::commstruct::onchipcommunication::Implementation* MFDg::GetImplementation(
		cosi::platforms::onchipcommunication::Platform* Plt) {

	cosi::commstruct::onchipcommunication::Implementation
			* RetVal = new cosi::commstruct::onchipcommunication::Implementation("");

	//Add all the IPs
	IdGraph::v_iterator U, V;
	for (U = mPtP->v_begin() ; U != mPtP->v_end() ; U++) {
		//Set the component name. The component name should be
		//availalbe in the specificaiton for each IP core
		Name ComponentName = (*mPtP)[*U].PRJ(Name)Get();
		//Ask the platform to provide an instance of the core with the same
		//id of the specification.
		cosi::commstruct::onchipcommunication::Implementation
				* N = Plt->GetImplementationInstance(ComponentName, *U, (*mPtP)[*U].PRJ(Position)Get(), TransferTable());
		N->SetName(*U, mPtP->GetName(*U)) ;
		(*N)[*U].PRJ(IpGeometry)Set((*mPtP)[*U].PRJ(IpGeometry)Get()) ;
		//Add the core to the implementation
		(*RetVal) = (*RetVal) + (*N);
		delete N;
	}

	//Add all the other nodes
	map< int , vector< int > >::iterator Uit;
	for (Uit = mInstalledNodes.begin() ; Uit != mInstalledNodes.end() ; Uit++) {
		//cout << "Node " << Uit->first << " in degree " << (Uit->second)[0] << " out degree " << (Uit->second)[1] << endl ;

		//if the node is not one of the cores
		if ( !mPtP->InV(Uit->first)) {
//			if ((Uit->second)[0] > mPlt->MaxInDegree() ) {
//				cout << "Input degree violations at node "<< Uit->first<< endl ;
//			}
//			if ((Uit->second)[1] > mPlt->MaxOutDegree() ) {
//				cout << "Output degree violations at node "<< Uit->first<< endl ;
//			}
			//Add the router to the implementation.
			int Nin = (Uit->second)[0] ;
			int Nout = (Uit->second)[1] ;
			cosi::commstruct::onchipcommunication::Implementation
					* N = Plt->GetImplementationInstance(
							Nin,Nout, Uit->first, Position(
									mPlt->GetX(Uit->first),
									mPlt->GetY(Uit->first), 0), TransferTable()) ;
			(*RetVal) = (*RetVal) + (*N);
			delete N;
		} else {
//			if ((Uit->second)[0] > mPlt->DestInDegree() ) {
//				cout << "Input degree violations at node "<< Uit->first<< endl ;
//				cout << "Input degree " << (Uit->second)[0] << endl ;
//			}
//			if ((Uit->second)[1] > mPlt->SourceOutDegree() ) {
//				cout << "Output degree violations at node "<< Uit->first<< endl ;
//				cout << "Output degree " << (Uit->second)[1] << endl ;
//			}
		}
	}

	//Add all the links
	map< vector< int > , double >::iterator Eit;
	vector<int> edge;
	double De;

	for (Eit = mInstalledEdges.begin() ; Eit != mInstalledEdges.end() ; Eit++) {

		edge = Eit->first;

		string PortU, PortV;
		//Get the output port of the IP core that is not bound
		//to any link
		vector<string> OutU = (*RetVal)[edge[0]].PRJ(Ports)GetNotBoundOut();
		//IF there are not free ports then return
		if (OutU.size() == 0) {
			CosiLog << "Router "<< edge[0]
					<< " does not have an available output port "<< endl ;
			return RetVal;
		} else {
			PortU = OutU[0];
		}
		vector<string> InV = (*RetVal)[edge[1]].PRJ(Ports)GetNotBoundIn();
		//If there are no available ports then return
		if (InV.size() == 0) {
			CosiLog << "Router "<< edge[1]
					<< " does not have an available input port "<< endl ;
			return RetVal;
		} else {
			PortV = InV[0];
		}
		//Setup the wiring path
		WiringPath P;
		P.AppendPosition(Position(mPlt->GetX(edge[0]), mPlt->GetY(edge[0]), 0)) ;
		P.AppendPosition(Position(mPlt->GetX(edge[1]), mPlt->GetY(edge[1]), 0)) ;
		//Setup commodities
		CommoditySet Cs;
		Commodity C;
		set<pair<int,int> > Commodities = mEdgeCommodities[ edge ];
		set<pair<int,int> >::iterator It;
		for (It = Commodities.begin() ; It != Commodities.end() ; It++) {
			C.SetValue(mPtP->GetName(It->first), mPtP->GetName(It->second),
					It->first, It->second, (*mPtP)[Edge(It->first,It->second)].PRJ(CommoditySet)GetAggregateBandwidth() ,
					RealLatency());
			Cs.Add(C) ;
		}
		cosi::commstruct::onchipcommunication::Implementation
				* L = Plt->GetImplementationInstance(Name(mPlt->GetLinkName()),
						edge[0], PortU, edge[1], PortV, P, Cs);
		//Add the link to the implementation
		(*RetVal) = (*RetVal) + (*L);
		delete L;
	}

	//Routing tables

	int CurrentNode, NextNode, Index;
	for (Index = 0; Index < mNc ; Index++) {
		pair<int,int> Commodity;
		Commodity.first = mSrc[Index];
		Commodity.second = mDest[Index];
		CurrentNode = mSrc[Index];
		while (CurrentNode != mDest[Index]) {
			NextNode = ( mPi[Commodity] )[CurrentNode];
			string P = (*RetVal)[CurrentNode].PRJ(Ports)IsBoundTo(Edge(CurrentNode, NextNode)) ;
			(*RetVal)[CurrentNode].PRJ(TransferTable)AddEntry(mPtP->GetName(mSrc[Index]),
					mPtP->GetName(mDest[Index]) , P);
			CurrentNode = NextNode ;
		};
	}

	return RetVal ;

}

map< pair<int,int> , int> MFDg::GetOrder() {
	return mOrder;
}

int MFDg::GetSource(int C) {
	return mSrc[C];
}

int MFDg::GetDest(int C) {
	return mDest[C];
}

double MFDg::GetW(int C) {
	return mW[C];
}

void MFDg::PrintMap(map< int , int > M) {
	map<int ,int >::iterator It;
	for (It= M.begin(); It!=M.end(); It++) {
		cout << "First : "<< It->first<< "  Second : "<< It->second<< endl;
	}

}

double MFDg::ComputeAdditionalBandwidth(set< pair<int,int> > Commodities,
		pair<int,int> Commodity, double W) {

	if (Commodities.find(Commodity) == Commodities.end() ) {
		return W ;
	}
	return 0;

	//
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
	//	DeltaW = W - MaxFlow ;
	//	if (DeltaW > 0.0)
	//		return DeltaW ;
	//	else
	//		return 0.0;

}

void MFDg::AddReversePath(int Src, int Dest, vector< int > Path) {
	//use the other add path
	map< int , int > ToAdd;
	int current_node = Dest;
	int prev_node;
	while (current_node != Src ) {
		prev_node = Path[ current_node ];
		cout << "("<< current_node << ","<< prev_node << ")";
		ToAdd[ prev_node ] = current_node;
		current_node = prev_node;
	}
	cout << endl;
	AddPath(Src , Dest , ToAdd );
}

void MFDg::AddPath(int Src, int Dest, map< int , int > Path) {

	vector< int > edge(2);
	int current_node = Src;
	int next_node;
	//cout << "Analysis of path " << endl ;

	//Dependency set is empty
	pair<int,int> PrevChannel(-1, -1);
	pair<int,int> CurrentChannel(-1, -1);
	while (current_node != Dest ) {

		next_node = Path[current_node];

		if (PrevChannel.first == -1) { //First iteration
			PrevChannel.first = current_node;
			PrevChannel.second = next_node;
		} else { //Add a dependency
			CurrentChannel.first = current_node;
			CurrentChannel.second = next_node;
			if ( !mCDG->InD(PrevChannel , CurrentChannel ))
				mCDG->AddDependency(PrevChannel , CurrentChannel );
			PrevChannel = CurrentChannel;
		}

		//Update Installed Edges and nodes
		edge[0] = current_node;
		edge[1] = next_node;
		if (mInstalledEdges.find(edge ) == mInstalledEdges.end() ) {
			mInstalledEdges[ edge ] = (*mPtP)[Edge(Src , Dest )].GetAggregateBandwidthValue();

			//Update commodity
			mEdgeCommodities[ edge ] = set<pair<int,int> >();
			pair<int,int> ToAdd;
			ToAdd.first = Src;
			ToAdd.second = Dest;
			mEdgeCommodities[ edge ].insert(ToAdd);

			if (mInstalledNodes.find(current_node ) == mInstalledNodes.end() ) {
				vector<int> degree( 2);
				degree[0] = 0;
				degree[1] = 1;
				mInstalledNodes[ current_node ] = degree;
			} else {
				vector<int> degree;
				degree = mInstalledNodes[ current_node ];
				degree[1] = degree[1] + 1;
				mInstalledNodes[ current_node ] = degree;
			}

			if (mInstalledNodes.find(next_node ) == mInstalledNodes.end() ) {
				vector<int> degree( 2);
				degree[0] = 1;
				degree[1] = 0;
				mInstalledNodes[ next_node ] = degree;
			} else {
				vector<int> degree;
				degree = mInstalledNodes[ next_node ];
				degree[0] = degree[0] + 1;
				mInstalledNodes[ next_node ] = degree;
			}

		} else {

			//Update flows and commodities
			//Add the commodity
			pair<int,int> ToAdd;
			ToAdd.first = Src;
			ToAdd.second = Dest;
			double DeltaW;
			DeltaW = ComputeAdditionalBandwidth(mEdgeCommodities[ edge ],
					ToAdd , (*mPtP)[Edge(Src, Dest)].PRJ(CommoditySet)GetAggregateBandwidthValue());
			mEdgeCommodities[ edge ].insert(ToAdd);
			//Compute the delta bandwidth
			//We need to find the aggregate bandwidth

			double W;
			W = mInstalledEdges[ edge ];
			if (W < 0)
				cout << "A flow cannot be negative  "<< endl;
			mInstalledEdges[ edge ] = W + DeltaW;

		}
		current_node = next_node;
	}
	mPi[pair<int,int>(Src,Dest)] = Path;
}

void MFDg::RemovePath(int Src, int Dest) {

	//remove also the path from the set of paths

	//update the channel dependency graph

	//Remember that we store path forward and not backwards
	map< int , int > PathToRemove = mPi[ pair<int,int>(Src,Dest) ];
	vector< int > edge(2);
	int current_node = Src;
	int next_node;
	//cout << "Analysis of path " << endl ;

	//Dependency set is empty
	pair<int,int> PrevChannel(-1, -1);
	pair<int,int> CurrentChannel(-1, -1);
	while (current_node != Dest ) {

		next_node = PathToRemove[current_node];

		if (PrevChannel.first == -1) { //First iteration
			PrevChannel.first = current_node;
			PrevChannel.second = next_node;
		} else { //Add a dependency
			CurrentChannel.first = current_node;
			CurrentChannel.second = next_node;
			//mCDG->RemoveDependency( PrevChannel , CurrentChannel);
			PrevChannel = CurrentChannel;
		}

		//Update Installed Edges and nodes
		edge[0] = current_node;
		edge[1] = next_node;
		//Remove the commodity
		set< pair<int,int> >::iterator
				CommodityToErase = mEdgeCommodities[ edge ].find(pair<int, int>(
						Src, Dest));
		mEdgeCommodities[edge].erase(CommodityToErase );
		pair<int,int> ToAdd;
		ToAdd.first = Src;
		ToAdd.second = Dest;
		double DeltaW;
		DeltaW = ComputeAdditionalBandwidth(mEdgeCommodities[ edge ], ToAdd , (*mPtP)[Edge(Src, Dest)].PRJ(CommoditySet)GetAggregateBandwidthValue());
		mInstalledEdges[ edge ] = mInstalledEdges[ edge ] - DeltaW;
		if (mEdgeCommodities[ edge ].size() == 0) {
			//erase the edge
			if ( ! (mDontTouch && (mDontTouchEdges.find(edge)
					!= mDontTouchEdges.end() ))) {
				mInstalledEdges.erase(edge );
				mInstalledNodes[ current_node ][1]
						= mInstalledNodes[ current_node ][1] - 1;
				mInstalledNodes[ next_node ][0]
						= mInstalledNodes[ next_node ][0]- 1;
			}
			mEdgeCommodities.erase(edge );

			//decrease the degree of orign vertex
			//decrease the degree of the target vertex

			//remove empty nodes
			if (current_node != Src && current_node != Dest ) {
				if (!(mDontTouch && (mDontTouchNodes.find(current_node)
						!= mDontTouchNodes.end()))) {
					if (mInstalledNodes[ current_node ][1] == 0
							&& mInstalledNodes[ current_node ][0] == 0) {
						mInstalledNodes.erase(current_node );
					}
				}
			}
			if (next_node != Src && next_node != Dest ) {
				if (!(mDontTouch && (mDontTouchNodes.find(current_node)
						!= mDontTouchNodes.end()))) {

					if (mInstalledNodes[ next_node ][1] == 0
							&& mInstalledNodes[ next_node ][0] == 0) {
						mInstalledNodes.erase(next_node );
					}
				}
			}
		}

		current_node = next_node;
	}
	mPi.erase(pair<int, int>(Src, Dest) );
}

int MFDg::MaxInViolation(map<int,int> pInViolations) {
	map< int,int >::iterator InIt;
	int MaxIn;
	int MaxViolation = 0;
	for (InIt = pInViolations.begin(); InIt != pInViolations.end(); InIt++) {
		if (InIt->second > MaxViolation ) {
			MaxViolation = InIt->second;
			MaxIn = InIt->first;
		}
	}
	return MaxIn;
}

int MFDg::MaxOutViolation(map<int,int> pOutViolations) {
	map< int,int >::iterator OutIt;
	int MaxOut;
	int MaxViolation = 0;
	for (OutIt = pOutViolations.begin(); OutIt != pOutViolations.end(); OutIt++) {
		if (OutIt->second > MaxViolation ) {
			MaxViolation = OutIt->second;
			MaxOut = OutIt->first;
		}
	}
	return MaxOut;
}

pair<int,int> MFDg::MaxMetric(map< pair<int,int> , double > pEdgeMetric) {
	map< pair<int,int> , double >::iterator EdgeIt;
	pair<int,int> MaxEdge;
	double MaxMetric = 0;
	for (EdgeIt = pEdgeMetric.begin(); EdgeIt != pEdgeMetric.end(); EdgeIt++) {
		if (EdgeIt->second > MaxMetric ) {
			MaxMetric = EdgeIt->second;
			MaxEdge = EdgeIt->first;
		}
	}
	return MaxEdge;
}

map< pair<int,int> , double > MFDg::GetInputEdges(int pId,
		map< pair<int,int> , double > pEdgeMetric) {
	map< pair<int,int> , double > RetVal;
	map< pair<int,int> , double >::iterator It;
	for (It = pEdgeMetric.begin(); It != pEdgeMetric.end(); It++) {
		if ((It->first).second == pId ) {
			RetVal[ It->first ] = It->second;
		}
	}
	return RetVal;
}

map< pair<int,int> , double > MFDg::GetOutputEdges(int pId,
		map< pair<int,int> , double > pEdgeMetric) {
	map< pair<int,int> , double > RetVal;
	map< pair<int,int> , double >::iterator It;
	for (It = pEdgeMetric.begin(); It != pEdgeMetric.end(); It++) {
		if ((It->first).first == pId ) {
			RetVal[ It->first ] = It->second;
		}
	}
	return RetVal;
}

int MFDg::FindRouter(int Id, set< pair<int,int> > pEdges) {
	double X = 0;
	double Y = 0;
	double TotalBandwidth = 0;
	set< pair<int,int> >::iterator Eit;
	for (Eit = pEdges.begin(); Eit != pEdges.end(); Eit++) {
		int Origin = Eit->first;
		int Target = Eit->second;
		vector<int> Edge(2);
		Edge[ 0 ] = Origin;
		Edge[ 1 ] = Target;
		X += mInstalledEdges[ Edge ] * mPlt->GetX(Eit->first);
		Y += mInstalledEdges[ Edge ] * mPlt->GetY(Eit->second);
		TotalBandwidth += mInstalledEdges[ Edge ];
	}
	X = X / TotalBandwidth;
	Y = Y / TotalBandwidth;

	list<int> N = mPlt->Neighbors(Id , mDest[0]);
	double MinDistance= DBL_MAX;
	int MinId;
	list< int >::iterator It;
	for (It = N.begin(); It != N.end(); It++) {
		if ( (fabs(mPlt->GetX( *It ) - X ) + fabs(mPlt->GetY(*It) - Y )
				< MinDistance) && (mInstalledNodes.find( *It )
				== mInstalledNodes.end() )&& (*It != mDest[0])) {
			MinDistance = fabs(mPlt->GetX( *It ) - X ) + fabs(mPlt->GetY(*It)
					- Y );
			MinId = *It;
		}
	}
	return MinId;
}

}
}
}
