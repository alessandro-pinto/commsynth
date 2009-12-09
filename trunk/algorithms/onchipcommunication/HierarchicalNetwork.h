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
#ifndef HIERARCHICALNETWORK_H_
#define HIERARCHICALNETWORK_H_

#include <sstream>
#include <vector>
#include <set>
#include "include/CosiGlobal.h"
#include "include/onchipcommunication/CosiGlobal.h"
#include "networks/onchipcommunication/Specification.h"
#include "networks/onchipcommunication/PlatformInstance.h"
#include "networks/onchipcommunication/Implementation.h"
#include "networks/graph/RealWeightedGraph.h"
#include "platforms/onchipcommunication/Platform.h"
#include "io/onchipcommunication/ParquetCodeGen.h"
#include "io/onchipcommunication/HmetisInterface.h"
#include "io/onchipcommunication/PlacementParser.h"
#include "algorithms/graph/ShortestPath.h"
#include "algorithms/onchipcommunication/NetworkCost.h"


namespace cosi {

namespace algorithms {

namespace onchipcommunication {

class HierarchicalNetwork {
public:
	HierarchicalNetwork(
			cosi::commstruct::onchipcommunication::Specification* Spec,
			cosi::platforms::onchipcommunication::Platform* Plt);
	virtual ~HierarchicalNetwork();
	bool Run();
	cosi::io::onchipcommunication::PlacementParser* GetPlacement();
	cosi::commstruct::onchipcommunication::Implementation* GetImplementation();
private:
	std::vector<std::set<int> > GenerateHyperedges(cosi::commstruct::onchipcommunication::Specification* Spec) ;
	cosi::commstruct::onchipcommunication::Implementation* StarInstance(std::set<int> P, int CenterId,int Nports) ;
	cosi::commstruct::onchipcommunication::Implementation* CompleteGraph(vector<cosi::commstruct::onchipcommunication::Implementation* > I, vector<int> C);
	cosi::commstruct::onchipcommunication::Implementation* Ring(vector<cosi::commstruct::onchipcommunication::Implementation* > I, vector<int> C);
	void FindRoutes(cosi::commstruct::onchipcommunication::Implementation* I, cosi::commstruct::onchipcommunication::Specification* Spec);
	cosi::commstruct::onchipcommunication::Implementation* CleanUp(cosi::commstruct::onchipcommunication::Implementation* Net) ;
	void Floorplan(cosi::commstruct::onchipcommunication::Implementation* I) ;
	cosi::commstruct::onchipcommunication::Specification* mSpec;
	cosi::platforms::onchipcommunication::Platform* mPlt;
	cosi::commstruct::onchipcommunication::Implementation* mI;
	cosi::io::onchipcommunication::PlacementParser* mPlacement;

};

}

}

}

#endif /*HIERARCHICALNETWORK_H_*/
