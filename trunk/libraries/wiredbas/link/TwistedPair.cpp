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
#include "TwistedPair.h"

namespace cosi {

namespace library {

namespace wiredbas {

std::string TwistedPair::ComponentName = "TwistedPair";
std::string TwistedPair::ComponentType = "Wire";

TwistedPair::TwistedPair(cosi::models::wiredbas::TwistedPairDelay* pDelayModel,
		cosi::models::wiredbas::TwistedPairCost* pCostModel) {
	//Any node
	Name N;
	N.IsBot(false) ;
	N.IsAny(true) ;
	//With EIA-485 interface
	PortInterface P;
	P.SetDirection(PortInterface::INOUT) ;
	P.SetName("EIA-485") ;
	this->SetSource(N) ;
	this->SetSourceInterface(P) ;
	//Define the destination node
	this->SetDestination(N) ;
	this->SetDestinationInterface(P) ;
	this->Directed(false);
	mDelayModel = pDelayModel ;
	mCostModel = pCostModel ;
}

TwistedPair::~TwistedPair() {
}

void TwistedPair::SetDelayModel(cosi::models::wiredbas::TwistedPairDelay* pDelay) {
	mDelayModel = pDelay ;
}

void TwistedPair::SetCostModel(cosi::models::wiredbas::TwistedPairCost* pCost) {
	mCostModel = pCost ;
}

RealDelay TwistedPair::GetDelay(WiringPath pPath) {
	return mDelayModel->Get(pPath);
}

double TwistedPair::GetDelayValue(WiringPath pPath) {
	return mDelayModel->GetValue(pPath);
}

double TwistedPair::GetCost(WiringPath pPath) {
	return mCostModel->GetValue(pPath);
}

cosi::commstruct::wiredbas::PlatformInstance* TwistedPair::GetPlatformInstance(
		int U, std::string pPortU, int V, std::string pPortV) {
	cosi::commstruct::wiredbas::PlatformInstance
			* RetVal = new cosi::commstruct::wiredbas::PlatformInstance("");
	RetVal->AddVertex(U) ;
	(*RetVal)[U].PRJ(Name)IsAny(true);
	(*RetVal)[U].PRJ(Type)IsAny(true);
	(*RetVal)[U].PRJ(Position)IsAny(true);
	Ports PortU;
	PortU.AddPort(pPortU, GetSourceInterface()) ;
	PortU.BoundTo(pPortU, Edge(U, V)) ;
	(*RetVal)[U].PRJ(Ports)Set(PortU);

	RetVal->AddVertex(V) ;
	(*RetVal)[V].PRJ(Name)IsAny(true);
	(*RetVal)[V].PRJ(Type)IsAny(true);
	(*RetVal)[V].PRJ(Position)IsAny(true);
	Ports PortV;
	PortV.AddPort(pPortV, GetDestinationInterface()) ;
	PortU.BoundTo(pPortV, Edge(U, V)) ;
	(*RetVal)[V].PRJ(Ports)Set(PortV);
	RetVal->AddEdge(U, V) ;
	(*RetVal)[Edge(U,V)].PRJ(Name)SetValue(ComponentName);
	(*RetVal)[Edge(U,V)].PRJ(Type)SetValue(ComponentType);
	(*RetVal)[Edge(U,V)].PRJ(WiringPath)IsAny(true);
	return RetVal ;
}

cosi::commstruct::wiredbas::Implementation
			* TwistedPair::GetImplementationInstance(int U, std::string pPortU, int V, std::string pPortV, WiringPath pPath) {
	cosi::commstruct::wiredbas::Implementation
			* RetVal = new cosi::commstruct::wiredbas::Implementation("");
	RetVal->AddVertex(U) ;
	(*RetVal)[U].PRJ(Name)IsAny(true);
	(*RetVal)[U].PRJ(Type)IsAny(true);
	(*RetVal)[U].PRJ(Position)IsAny(true);
	(*RetVal)[U].PRJ(ThreadSet)IsAny(true);
	(*RetVal)[U].PRJ(TransferTable)IsAny(true);
	Ports PortU;
	PortU.AddPort(pPortU, GetSourceInterface()) ;
	PortU.BoundTo(pPortU, Edge(U, V)) ;
	(*RetVal)[U].PRJ(Ports)Set(PortU);

	RetVal->AddVertex(V) ;
	(*RetVal)[V].PRJ(Name)IsAny(true);
	(*RetVal)[V].PRJ(Type)IsAny(true);
	(*RetVal)[V].PRJ(Position)IsAny(true);
	(*RetVal)[U].PRJ(ThreadSet)IsAny(true);
	(*RetVal)[U].PRJ(TransferTable)IsAny(true);
	Ports PortV;
	PortV.AddPort(pPortV, GetDestinationInterface()) ;
	PortV.BoundTo(pPortV, Edge(U, V)) ;
	(*RetVal)[V].PRJ(Ports)Set(PortV);
	RetVal->AddEdge(U, V) ;
	(*RetVal)[Edge(U,V)].PRJ(Name)SetValue(ComponentName);
	(*RetVal)[Edge(U,V)].PRJ(Type)SetValue(ComponentType);
	(*RetVal)[Edge(U,V)].PRJ(WiringPath)Set(pPath);
	return RetVal ;
}

}

}
}
