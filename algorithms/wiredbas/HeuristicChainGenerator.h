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
#ifndef HEURISTICCHAINGENERATOR_H_
#define HEURISTICCHAINGENERATOR_H_

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
#include "platforms/wiredbas/BusPlatform.h"

namespace cosi {

namespace algorithms {

namespace wiredbas {

using namespace std;

class Chain {
public:
	Chain(int Id);
	double Cost;
	double Delay;
	double Length;
	double Bandwidth;
	list< int > C;
	void AddLeft(int pV);
	void AddRight(int pV);
	int GetLeft();
	int GetRight();
	bool IsLeft(int pV);
	bool IsRight(int pV);
	bool Contains(int pV);
	int size() const;
	bool operator <(const Chain& C) const;
	set< int > NodeSet();
};

class HeuristicChainGenerator {
public:
	HeuristicChainGenerator(cosi::commstruct::wiredbas::Specification* pSpec,
			cosi::platforms::wiredbas::BusPlatform* pPlt);
	virtual ~HeuristicChainGenerator();
	void Run();
	std::list< cosi::commstruct::wiredbas::Implementation* > GetChains();
	std::set<int> GetRouters( ) ;
private:

	bool IsChainExtensible(Chain C , int RouterId , Position RotuerPosition);
	bool CheckLatency(cosi::commstruct::wiredbas::Implementation* I );
	cosi::commstruct::wiredbas::Implementation* GenerateBusInstance(Chain C , int RouterId , Position RotuerPosition);

	cosi::commstruct::wiredbas::Specification* mSpecification;
	cosi::platforms::wiredbas::BusPlatform* mPlt;


	cosi::commstruct::wiredbas::Implementation
				* mCompleteWiring ;

	cosi::commstruct::wiredbas::Implementation* GenerateCompleteWiring(map<int,Position> pRouters) ;

	set<int> mAssigned;

	Position mGatewayPosition;
	int mGateway;

	int mNsensors;
	int mNactuators;
	map<int,set<Chain> > mChains ;
	std::set<int> mRouters ;
	std::list<cosi::commstruct::wiredbas::Implementation* > mChainImplementation ;
};

}

}

}

#endif /*HEURISTICCHAINGENERATOR_H_*/
