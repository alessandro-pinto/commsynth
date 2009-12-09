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
#ifndef ARCNETBUS_H_
#define ARCNETBUS_H_

#include <string>
#include "libraries/wiredbas/common/Bus.h"
#include "rules/wiredbas/link/WiringRule.h"
#include "rules/wiredbas/node/ArcnetBusRule.h"
#include "models/wiredbas/common/WireCost.h"
#include "models/wiredbas/common/WireDelayModel.h"
#include "models/wiredbas/node/ArcnetBusCost.h"
#include "models/wiredbas/node/ArcnetBusDelay.h"

namespace cosi {

namespace library {

namespace wiredbas {

class ArcnetBus : public Bus {
	//Wiring model
	cosi::rules::wiredbas::WiringRule* mWiringRule;
	//Bus rules
	cosi::rules::wiredbas::ArcnetBusRule* mArcnetBusRule;
	//Cost model
	cosi::models::wiredbas::WireCost* mWireCost;
	cosi::models::wiredbas::ArcnetBusCost* mBusCost;
	//Delay model
	cosi::models::wiredbas::WireDelay* mWireDelay;
	cosi::models::wiredbas::ArcnetBusDelay* mBusDelay;

	double mBusSpeed;
public:

	ArcnetBus(double pBusSpeed, cosi::rules::wiredbas::WiringRule* pWiringRule,
			cosi::models::wiredbas::WireCost* pWireCost,
			cosi::models::wiredbas::WireDelay* pWireDelay);

	virtual ~ArcnetBus();

	void SetWiringRule(cosi::rules::wiredbas::WiringRule* pWiringRule);
	void SetWireCostModel(cosi::models::wiredbas::WireCost* pWireCost);
	void SetWireDelayModel(cosi::models::wiredbas::WireDelay* pWireDealy);

	double BusAccessLatencyValue(
			cosi::commstruct::wiredbas::Implementation* pBus);
	double BusAccessLatencyValue(int pNodeId,
			cosi::commstruct::wiredbas::Implementation* pBus);
	bool Satisfies(cosi::commstruct::wiredbas::Implementation* pBus);

	double
			GetBusBandwidthValue(
					cosi::commstruct::wiredbas::Implementation* pBus);

	double GetCost(cosi::commstruct::wiredbas::Implementation* pBus);

	static std::string ComponentName;
	static std::string ComponentType;
};

}
}
}
#endif /*ARCNET_H_*/
