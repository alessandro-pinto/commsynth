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
#ifndef PBCOVERGENERATION_H_
#define PBCOVERGENERATION_H_

#include <list>
#include <map>
#include <vector>
#include <string>
#include "networks/wiredbas/WiredBasSpecificaion.h"
#include "networks/wiredbas/WiredBasImplementation.h"
#include "platforms/wiredbas/BusPlatform.h"
#include "platforms/wiredbas/NetworkPlatform.h"
#include "io/bas/PbCodeGen.h"
#include "include/bas/CosiGlobal.h"

namespace cosi {

namespace algorithms {

namespace wiredbas {

class PbCoverGeneration {
public:
	PbCoverGeneration(cosi::commstruct::wiredbas::Specification* pSpec ,
			std::list<cosi::commstruct::wiredbas::Implementation*> pChains, std::set<int> pRotuers,
			cosi::platforms::wiredbas::BusPlatform* pPlt);
	PbCoverGeneration(cosi::commstruct::wiredbas::Specification* pSpec ,
				std::list<cosi::commstruct::wiredbas::Implementation*> pChains, std::set<int> pRotuers,
				cosi::platforms::wiredbas::NetworkPlatform* pPlt);
	virtual ~PbCoverGeneration();
	bool Run();
	std::list<cosi::commstruct::wiredbas::Implementation*> GetCover();
private:

	void SetUpVariables(cosi::io::bas::PbCodeGen* pSolver) ;
	int GetChainIndex(cosi::commstruct::wiredbas::Implementation* pI) ;
	int GetVertexToChainIndex(int pV , cosi::commstruct::wiredbas::Implementation* pI ) ;
	int GetRouterToChainIndex(int pV , cosi::commstruct::wiredbas::Implementation* pI ) ;

	cosi::commstruct::wiredbas::Specification* mSpec ;
	std::list<cosi::commstruct::wiredbas::Implementation*> mChains;
	std::list<cosi::commstruct::wiredbas::Implementation*> mCover;
	cosi::platforms::wiredbas::BusPlatform* mPlt;
	cosi::platforms::wiredbas::NetworkPlatform* mNetPlt;
	std::set<int> mRouters;

	bool mBusBased ;

	std::map<cosi::commstruct::wiredbas::Implementation*,int> mI2Index;
	std::map<int,cosi::commstruct::wiredbas::Implementation*> mIndex2I;
	std::map<pair<int,int>,int> mVertexChain2Index;
	std::map<int,pair<int,int> > mIndex2VertexChain;
	std::map<pair<int,int>,int> mRouterChain2Index;
	std::map<int,pair<int,int> > mIndex2RouterChain;
};

}

}

}

#endif /*PBCOVERGENERATION_H_*/
