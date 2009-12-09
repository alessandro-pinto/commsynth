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
#include "platforms/wiredbas/NetworkPlatform.h"
#include "io/bas/HmetisInterface.h"
#include "io/bas/PatohInterface.h"
#include "algorithms/graph/Tsp.h"

#ifndef PARTITIONGENERATOR_H_
#define PARTITIONGENERATOR_H_

namespace cosi {

namespace algorithms {

namespace wiredbas {

class PartitionGenerator {
public:
	PartitionGenerator(cosi::commstruct::wiredbas::Specification* pSpec,
			cosi::platforms::wiredbas::BusPlatform* pPlt);
	PartitionGenerator(cosi::commstruct::wiredbas::Specification* pSpec,
				cosi::platforms::wiredbas::NetworkPlatform* pPlt);
	virtual ~PartitionGenerator();

	void Run();

	std::list< cosi::commstruct::wiredbas::Implementation* > GetChains();
	std::set<int> GetRouters();

	void SetBandwidthWeight(double W);
	void SetLatencyWeight(double W);
	void SetDistanceWeight(double W);

private:
	//INPUTS
	cosi::commstruct::wiredbas::Specification* mSpecification;
	cosi::platforms::wiredbas::BusPlatform* mPlt;
	cosi::platforms::wiredbas::NetworkPlatform* mNetPlt;
	//OUTPUTS
	std::list< cosi::commstruct::wiredbas::Implementation* > mChains;
	std::map<int,Position> mRouters;
	//PARAMTERS
	double mBandwidthWeight;
	double mLatencyWeight;
	double mDistanceWeight;

	//PRIVATE METHODS
	cosi::commstruct::graph::RealWeightedGraph* GenerateWeightedGraph();

	std::list<cosi::commstruct::wiredbas::Implementation* > GenerateChains(
			std::vector<std::set<int> > Partitions);

	std::list<cosi::commstruct::wiredbas::Implementation* > GenerateNets(
				std::vector<std::set<int> > Partitions);

	std::list<cosi::commstruct::wiredbas::Implementation* > GenerateTrees(
			std::vector<std::set<int> > Partitions);

	double ComputeAverageBandwidth(int U, int V);

	std::list<int> GenerateChain(std::set<int> Nodes, int RouterId);


	cosi::commstruct::graph::U_RealWeightedGraph* GenerateTree(std::set<int> Nodes, int RouterId);

	std::list<int> GenerateShortestChain(std::set<int> Nodes, int RouterId);

	cosi::commstruct::wiredbas::Implementation* GenerateBusInstance(
			std::list<int> Chain, int RouterId);

	cosi::commstruct::wiredbas::Implementation* GenerateNetInstance(
			std::set<int> Nodes, int RouterId);

	cosi::commstruct::wiredbas::Implementation* GenerateBusInstance(
			cosi::commstruct::graph::U_RealWeightedGraph* G, int RouterId);

	bool CheckLatency(cosi::commstruct::wiredbas::Implementation* pBus);

	bool CheckNetLatency(cosi::commstruct::wiredbas::Implementation* pNet);

	bool mBusBased ;

};

}

}

}

#endif /*PARTITIONGENERATOR_H_*/
