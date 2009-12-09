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
#ifndef SYNTHESISREPORT_H_
#define SYNTHESISREPORT_H_

#include <string>
#include <fstream>
#include "networks/wiredbas/WiredBasSpecificaion.h"
#include "networks/wiredbas/WiredBasImplementation.h"
#include "platforms/wiredbas/BusPlatform.h"
#include "platforms/wiredbas/NetworkPlatform.h"
#include "algorithms/graph/ConnectedComponents.h"
#include "models/wiredbas/link/EthernetLinkDelay.h"

namespace cosi {

namespace io {

namespace bas {

class SynthesisReport {
public:

	//BUS-BUS
	SynthesisReport(cosi::commstruct::wiredbas::Specification* pSpec,
			std::list<cosi::commstruct::wiredbas::Implementation*> Buses,
			cosi::platforms::wiredbas::BusPlatform* pPlt,
			cosi::commstruct::wiredbas::Implementation* pSecondLevel,
			std::string pFileName);
	//BUS-STAR
	SynthesisReport(cosi::commstruct::wiredbas::Specification* pSpec,
			std::list<cosi::commstruct::wiredbas::Implementation*> Buses,
			cosi::platforms::wiredbas::BusPlatform* pPlt,
			cosi::commstruct::wiredbas::Implementation* pSecondLevel,
			cosi::platforms::wiredbas::NetworkPlatform* pNetPlt,
			std::string pFileName);
	//STAR-STAR
	SynthesisReport(cosi::commstruct::wiredbas::Specification* pSpec,
			std::list<cosi::commstruct::wiredbas::Implementation*> Buses,
			cosi::platforms::wiredbas::NetworkPlatform* pPlt,
			cosi::commstruct::wiredbas::Implementation* pSecondLevel,
			std::string pFileName);

	void Append(std::string pFileName);
	void Run();
	virtual ~SynthesisReport();
private:

	void RunBusBased();
	void RunNetBased();

	cosi::commstruct::wiredbas::Specification* mSpec;
	std::list<cosi::commstruct::wiredbas::Implementation*> mBuses;
	cosi::commstruct::wiredbas::Implementation* mSecondLevel;
	cosi::platforms::wiredbas::BusPlatform* mPlt;
	cosi::platforms::wiredbas::NetworkPlatform* mNetPlt;
	std::string mFileName;
	bool mAppend;
	std::string mAppendFileName;
	std::map<std::string,int> mNcomponents;
	std::map<std::string,double> mCosts;
	double mSecondLevelWiring;

	bool mBusBased;
	bool mBusStar ;

};

}

}

}

#endif /*SYNTHESISREPORT_H_*/
