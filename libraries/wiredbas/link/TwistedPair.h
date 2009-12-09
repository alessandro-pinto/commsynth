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
#ifndef TWISTED_PAIR_H_
#define TWISTED_PAIR_H_

#include <string>
#include "networks/wiredbas/WiredBasPlatformInstance.h"
#include "networks/wiredbas/WiredBasImplementation.h"
#include "quantities/WiringPath.h"
#include "quantities/RealLatency.h"
#include "quantities/Name.h"
#include "quantities/Ports.h"
#include "libraries/wiredbas/common/Link.h"
#include "models/wiredbas/link/TwistedPairDelay.h"
#include "models/wiredbas/link/TwistedPairCost.h"

namespace cosi {

namespace library {

namespace wiredbas {

class TwistedPair : public cosi::library::wiredbas::Link {

	cosi::models::wiredbas::TwistedPairDelay* mDelayModel;

	cosi::models::wiredbas::TwistedPairCost* mCostModel;

public:

	TwistedPair(cosi::models::wiredbas::TwistedPairDelay* pDelayModel,cosi::models::wiredbas::TwistedPairCost* pCostModel);

	virtual ~TwistedPair();

	void SetDelayModel(cosi::models::wiredbas::TwistedPairDelay* pDelay);

	void SetCostModel(cosi::models::wiredbas::TwistedPairCost* pCost);

	RealDelay GetDelay(WiringPath pPath);

	double GetDelayValue(WiringPath pPath);

	double GetCost(WiringPath pPath);

	cosi::commstruct::wiredbas::PlatformInstance* GetPlatformInstance(
			int U, std::string pPortU , int V , std::string pPortV);

	cosi::commstruct::wiredbas::Implementation
			* GetImplementationInstance(int U, std::string pPortU, int V, std::string pPortV, WiringPath pPath);

	static std::string ComponentName;
	static std::string ComponentType;

};

}
}
}

#endif /*TWISTEDPAIR_H_*/
