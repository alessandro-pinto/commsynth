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
#include "algorithms/graph/Tsp.h"

#ifndef _CHAINGENERATOR_H_
#define _CHAINGENERATOR_H_

namespace cosi {

namespace algorithms {

namespace wiredbas {

class ChainGenerator {
public:

	ChainGenerator();

	void Initialize(cosi::commstruct::wiredbas::Specification* pSpec,
			cosi::platforms::wiredbas::BusPlatform* pPlt);

	void Run();

	std::list< cosi::commstruct::wiredbas::Implementation* > GetChains();
	std::set<int> GetRouters() ;

private:

	cosi::commstruct::wiredbas::Implementation* GenerateCompleteWiring();

	cosi::commstruct::graph::U_RealWeightedGraph* GenerateCompleteDistance(
			cosi::commstruct::wiredbas::Implementation* pW);

	void AddRouter(int Id , Position pP, cosi::commstruct::wiredbas::Implementation* pW,
			cosi::commstruct::graph::U_RealWeightedGraph* pD);

	std::list<int> FindTour(cosi::commstruct::graph::U_RealWeightedGraph* pD);

	void GenerateChains(std::list<int> pTour, int pRouterId,Position P,
			cosi::commstruct::wiredbas::Implementation* pW);
	void RemoveRouter(int pRouterId,
			cosi::commstruct::wiredbas::Implementation* pW);
	void RemoveRouter(int pRouterId,
			cosi::commstruct::graph::U_RealWeightedGraph* pD);

	cosi::commstruct::wiredbas::Implementation* GenerateBusInstance(std::list<int> pSubTour, int RouterId, Position P , cosi::commstruct::wiredbas::Implementation* pW);

	bool CheckLatency(cosi::commstruct::wiredbas::Implementation* pBus );

	cosi::commstruct::wiredbas::Specification* mSpecification;

	cosi::platforms::wiredbas::BusPlatform* mPlt;

	cosi::commstruct::wiredbas::Implementation* mTour;

	std::list< cosi::commstruct::wiredbas::Implementation* > mChains;

	std::set<int> mRouters ;

};

}
}
}
#endif
