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
#include "libraries/wiredbas/node/ArcnetBus.h"

namespace cosi {

namespace library {

namespace wiredbas {

std::string ArcnetBus::ComponentName = "ArcnetBus";
std::string ArcnetBus::ComponentType = "Bus";

ArcnetBus::ArcnetBus(double pBusSpeed,cosi::rules::wiredbas::WiringRule* pWiringRule,cosi::models::wiredbas::WireCost* pWireCost,cosi::models::wiredbas::WireDelay* pWireDelay) {
	mBusSpeed = pBusSpeed ;
	//Instantiation of arcnet models
	mBusCost = new cosi::models::wiredbas::ArcnetBusCost(pWireCost);
	mBusDelay = new cosi::models::wiredbas::ArcnetBusDelay(pBusSpeed,pWireDelay);
	mWiringRule = pWiringRule ;
	mWireCost = pWireCost ;
	mWireDelay = pWireDelay ;
	mArcnetBusRule = new cosi::rules::wiredbas::ArcnetBusRule(pBusSpeed) ;
}

ArcnetBus::~ArcnetBus() {
	delete mBusCost;
	delete mBusDelay;
}

void ArcnetBus::SetWiringRule(cosi::rules::wiredbas::WiringRule* pWiringRule) {
	mWiringRule = pWiringRule ;
}
void ArcnetBus::SetWireCostModel(cosi::models::wiredbas::WireCost* pWireCost) {
	mWireCost = pWireCost ;
	mBusCost->SetWireCost(pWireCost) ;
}
void ArcnetBus::SetWireDelayModel(cosi::models::wiredbas::WireDelay* pWireDelay) {
	mWireDelay = pWireDelay ;
	mBusDelay->SetWireDelayModel(pWireDelay) ;
}

double ArcnetBus::BusAccessLatencyValue(cosi::commstruct::wiredbas::Implementation* pBus){
	return mBusDelay->GetValue(pBus) ;
}
double ArcnetBus::BusAccessLatencyValue(int pNodeId,
		cosi::commstruct::wiredbas::Implementation* pBus){
	return mBusDelay->GetValue(pBus) ;
}
bool ArcnetBus::Satisfies(cosi::commstruct::wiredbas::Implementation* pBus){
	return mArcnetBusRule->Satisfies(pBus) ;
}

double
			ArcnetBus::GetBusBandwidthValue(
					cosi::commstruct::wiredbas::Implementation* pBus){
	return mArcnetBusRule->GetBandwidthValue(pBus) ;
}


double ArcnetBus::GetCost(cosi::commstruct::wiredbas::Implementation* pBus) {
	return mBusCost->GetValue(pBus) ;
}

}
}
}
