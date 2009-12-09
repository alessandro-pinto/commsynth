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

#include "BusPlatform.h"

namespace cosi {

namespace platforms {

namespace wiredbas {

BusPlatform::BusPlatform() {
}

BusPlatform::~BusPlatform() {
}

void BusPlatform::AddBusComponent(std::string pType,
		cosi::library::wiredbas::Bus* pBus) {
	mBusComponents[pType] = pBus ;
}
void BusPlatform::AddNodeComponent(std::string pType,
		cosi::library::wiredbas::Node* pNode) {
	mNodeComponents[pType] = pNode ;
}
void BusPlatform::AddWireComponent(std::string pType,
		cosi::library::wiredbas::Link* pWire) {
	mLinkComponents[pType] = pWire ;
}

void BusPlatform::SetWiringRule(cosi::rules::wiredbas::WiringRule* pWiringRule) {
	mWiringRule = pWiringRule ;
}

void BusPlatform::SetNodePositions(
		cosi::rules::wiredbas::NodePosition* pNodePosition) {
	mNodePosition = pNodePosition ;
}

cosi::commstruct::wiredbas::PlatformInstance* BusPlatform::GetPlatformInstance(
		std::string pType, int pId, Position pP) {
	if (mNodeComponents.find(pType) != mNodeComponents.end() )
		return mNodeComponents[pType]->GetPlatformInstance(pId, pP) ;
	else {
		std::cout << "Node of type "<< pType
				<< " does not belong to the platform"<< std::endl ;
	}
	return (new cosi::commstruct::wiredbas::PlatformInstance("Empty"));
}

//Implementation instances
cosi::commstruct::wiredbas::Implementation* BusPlatform::GetImplementationInstance(
		std::string pType, int pId, Position pP) {
	if (mNodeComponents.find(pType) != mNodeComponents.end() )
		return mNodeComponents[pType]->GetImplementationInstance(pId, pP,
				ThreadSet(), TransferTable()) ;
	else {
		std::cout << "Node of type "<< pType
				<< " does not belong to the platform"<< std::endl ;
	}
	return (new cosi::commstruct::wiredbas::Implementation("Empty"));
}
cosi::commstruct::wiredbas::Implementation* BusPlatform::GetImplementationInstance(
		std::string pType, int pId, Position pP, ThreadSet pThreads) {
	if (mNodeComponents.find(pType) != mNodeComponents.end() )
		return mNodeComponents[pType]->GetImplementationInstance(pId, pP,
				pThreads, TransferTable()) ;
	else {
		std::cout << "Node of type "<< pType
				<< " does not belong to the platform"<< std::endl ;
	}
	return (new cosi::commstruct::wiredbas::Implementation("Empty"));
}
cosi::commstruct::wiredbas::Implementation* BusPlatform::GetImplementationInstance(
		std::string pType, int pSourceId, std::string pSourcePort, int pDestId,
		std::string pDestPort, WiringPath pPath) {
	if (mLinkComponents.find(pType) != mLinkComponents.end() ) {
		return mLinkComponents[pType]->GetImplementationInstance(pSourceId,
				pSourcePort, pDestId, pDestPort , pPath) ;
	} else {
		std::cout << "Link of type "<< pType
				<< " does not belong to the platform"<< std::endl ;
	}
	return (new cosi::commstruct::wiredbas::Implementation("Empty"));
}

//Models
double BusPlatform::BusAccessLatencyValue(std::string pType,
		cosi::commstruct::wiredbas::Implementation* pBus) {
	return mBusComponents[pType]->BusAccessLatencyValue(pBus) ;
}

double BusPlatform::BusAccessLatencyValue(std::string pType, int pId,
		cosi::commstruct::wiredbas::Implementation* pBus) {
	return mBusComponents[pType]->BusAccessLatencyValue(pId, pBus) ;
}

WiringPath BusPlatform::WireLayout(Position pStart, Position pEnd) {
	return mWiringRule->GetWiringPath(pStart, pEnd) ;
}

std::list<Position> BusPlatform::GetGatewayPosition() {
	return mNodePosition->GetPositions() ;
}

double BusPlatform::GetBusBandwidthValue(std::string pType,
		cosi::commstruct::wiredbas::Implementation* pBus) {
	return mBusComponents[pType]->GetBusBandwidthValue(pBus) ;
}

//Check rule
bool BusPlatform::Satisfies(std::string pType,
		cosi::commstruct::wiredbas::Implementation* pBus) {
	return mBusComponents[pType]->Satisfies(pBus) ;
}

double BusPlatform::GetNodeCost(std::string pType) {
	if (mNodeComponents.find(pType) != mNodeComponents.end() )
		return mNodeComponents[pType]->GetCost() ;
	else {
		std::cout << "Node of type "<< pType
				<< " does not belong to the platform"<< std::endl ;
		return 0;
	}

}
double BusPlatform::GetLinkCost(std::string pType, WiringPath pPath) {
	if (mLinkComponents.find(pType) != mLinkComponents.end() ) {
		return mLinkComponents[pType]->GetCost(pPath) ;
	} else {
		std::cout << "Link of type "<< pType
				<< " does not belong to the platform"<< std::endl ;
		return 0;
	}

}

double BusPlatform::GetBusCost(std::string pType,
		cosi::commstruct::wiredbas::Implementation* pBus) {
	return mBusComponents[pType]->GetCost(pBus) ;
}

}
}
}
