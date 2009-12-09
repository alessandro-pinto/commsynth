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

#ifndef SECONDLEVELNETWORK_H_
#define SECONDLEVELNETWORK_H_

#include <iostream>
#include <iterator>
#include <string>
#include <list>
#include <map>
#include <algorithm>
#include "float.h"
#include "networks/wiredbas/WiredBasSpecificaion.h"
#include "networks/wiredbas/WiredBasPlatformInstance.h"
#include "networks/wiredbas/WiredBasImplementation.h"
#include "networks/graph/RealWeightedGraph.h"
#include "platforms/wiredbas/BusPlatform.h"
#include "platforms/wiredbas/NetworkPlatform.h"
#include "io/bas/HmetisInterface.h"
#include "io/bas/PatohInterface.h"
#include "algorithms/graph/Tsp.h"

namespace cosi {

namespace algorithms {

namespace wiredbas {

class SecondLevelNetwork {

	bool mBusBased ;
	cosi::platforms::wiredbas::BusPlatform* mPlt ;
	cosi::platforms::wiredbas::NetworkPlatform* mNetPlt ;
	std::list<cosi::commstruct::wiredbas::Implementation*> mSubNets ;
	cosi::commstruct::wiredbas::Implementation* mI ;


	bool RunBusBased() ;
	bool RunNetBased() ;

public:

	SecondLevelNetwork(
			std::list<cosi::commstruct::wiredbas::Implementation*> SubNets,
			cosi::platforms::wiredbas::BusPlatform* Plt);

	SecondLevelNetwork(
			std::list<cosi::commstruct::wiredbas::Implementation*> SubNets,
			cosi::platforms::wiredbas::NetworkPlatform* Plt);

	bool Run() ;

	virtual ~SecondLevelNetwork();

	cosi::commstruct::wiredbas::Implementation* Get() ;
};

}

}

}

#endif /*SECONDLEVELNETWORK_H_*/
