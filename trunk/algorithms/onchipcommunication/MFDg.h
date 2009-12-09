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
#ifndef _MFDG_H_
#define _MFDG_H_

#include "include/CosiGlobal.h"
#include "float.h"
#include "stdlib.h"
#include "stdio.h"
#include "limits.h"
#include <map>
#include <vector>
#include <set>
#include <algorithm>
#include "networks/onchipcommunication/ChannelDependencyGraph.h"
#include "platforms/onchipcommunication/NocPlatformInterface.h"
#include "platforms/onchipcommunication/Platform.h"
#include "networks/onchipcommunication/Specification.h"
#include "networks/onchipcommunication/Implementation.h"
#include "SFDg.h"

//    OPTIMAL ROUTING

/** Optimal routing with fixed cost and distance
 *  constraints.
 *
 *@author Alessandro Pinto,
 *@author apinto@eecs.berkeley.edu,
 *@author EECS Grad Student,
 *@author 211 Cory Hall #1772,
 *@author Berkeley, CA, 94720-1772
 *@author (510)-6429670

 *@version $Id: MFDg.h,v 1.4 2008/12/07 23:18:29 apinto Exp $
 */

namespace cosi {

namespace algorithms {

namespace onchipcommunication {


class MFDg {
public:

	//    CONSTRUCTORS


	/** \brief The constructor only sets the initialization flag.
	 */
	MFDg();

	//    Destructors


	/** \brief The destructor releases the momory allocated for the
	 *         paths. */
	~MFDg();

	//    PUBLIC MEMBERS AND METHODS

	void Initialize(cosi::commstruct::onchipcommunication::Specification* PtP,
			cosi::platforms::onchipcommunication::NocPlatformInterface* Plt);
	void Initialize(cosi::commstruct::onchipcommunication::Specification* PtP,
			cosi::platforms::onchipcommunication::NocPlatformInterface* Plt,
			map<int,vector<int> > pNodes, map<vector<int>,double > pEdges);

	/** \brief
	 *
	 */
	bool Run();
	bool Run_2();

	bool RunNoDegree();

	bool RunDegreeReduce();

	bool RunAddRouter();

	bool RunFeasibility();

	/** \brief
	 *
	 */
	map< pair<int,int> , map<int,int> > GetPaths();

	/** \brief
	 *
	 */
	map<int,int> GetPath(int i);

	map<int,int> GetPath(int Src, int Dest);

	/** \brief Returns a graph.
	 */
	cosi::commstruct::onchipcommunication::Implementation* GetImplementation(cosi::platforms::onchipcommunication::Platform* Plt);

	void PrintMap(map<int,int> M);

	map< pair<int,int> , int> GetOrder();

	int GetSource(int C);

	int GetDest(int C);

	double GetW(int C);

	void DontTouchSourceDest(bool B);

private:

	//    PRIVATE MEMBERS AND METHODS
	double ComputeAdditionalBandwidth(set< pair<int,int> > Commodities,
			pair<int,int> Commodity, double W);

	void RemovePath(int Src, int Dest);

	void AddReversePath(int Src, int Dest, vector<int> Path);

	void AddPath(int Src, int Dest, map< int , int > Path);

	int MaxInViolation(map<int,int> pInViolations);
	int MaxOutViolation(map<int,int> pOutViolations);
	pair<int,int> MaxMetric(map< pair<int,int> , double> pEdgeMetric);
	map< pair<int,int> , double > GetInputEdges(int pId,
			map< pair<int,int> , double > pEdgeMetric);
	map< pair<int,int> , double > GetOutputEdges(int pId,
			map< pair<int,int> , double > pEdgeMetric);
	int FindRouter(int Id, set< pair<int,int> > pEdges);

	map< vector< int > , double > mInstalledEdges;
	map< vector< int > , set< pair<int,int> > > mEdgeCommodities;
	map< int , vector< int > > mInstalledNodes;

	bool mDontTouch;
	map< vector< int > , double > mDontTouchEdges;
	map< vector< int > , set< pair<int,int> > > mDontTouchEdgeCommodities;
	map< int , vector< int > > mDontTouchNodes;

	cosi::commstruct::onchipcommunication::ChannelDependencyGraph* mCDG;

	/** \brief
	 *
	 */
	int is_initialized;

	/** \brief
	 *
	 */
	int has_run;

	/** \brief
	 *
	 */
	map< pair<int,int> , map<int,int> > mPi;

	cosi::commstruct::onchipcommunication::Specification* mPtP;

	/** \brief
	 *
	 */

	cosi::platforms::onchipcommunication::NocPlatformInterface* mPlt;

	/** \brief
	 *
	 */
	vector<int> mSrc;

	/** \brief
	 *
	 */
	vector<int> mDest;

	set<int> mU;
	set<int> mV;

	set< pair<int,int> > mE;

	/** \brief
	 *
	 */
	vector<double> mW;

	map< pair<int,int> , int > mOrder;

	/** \brief
	 *
	 */
	int mNc;

	bool mSourceDestDontTouch;

};

}
}
}

#endif
