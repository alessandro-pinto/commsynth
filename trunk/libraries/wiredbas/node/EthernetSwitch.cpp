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
#include "EthernetSwitch.h"

namespace cosi {

namespace library {

namespace wiredbas {

using namespace std ;

EthernetSwitch::EthernetSwitch(int Nin, int Nout, double Speed) {
	ComponentType = "Router";
	for (int i = 0; i < Nin; i++) {
		stringstream s;
		s << i;
		string Pname = "P" + s.str() ;
		mPorts.AddPort(Pname, "Eth", cosi::PortInterface::INOUT) ;
	}

	for (int i = 0; i < Nout; i++) {
		stringstream s;
		s << Nin + i;
		string Pname = "P" + s.str() ;
		mPorts.AddPort(Pname, "Eth", cosi::PortInterface::INOUT) ;
	}

	stringstream s;
	s << "Ethernet-Switch_" << Nin + Nout << std::endl;
	ComponentName = s.str();
	mDelayModel = new cosi::models::wiredbas::EthernetSwitchDelay(Speed) ;
}

EthernetSwitch::~EthernetSwitch() {
}

cosi::commstruct::wiredbas::PlatformInstance* EthernetSwitch::GetPlatformInstance(
		int V, Position pP) {
	cosi::commstruct::wiredbas::PlatformInstance * RetVal =
			new cosi::commstruct::wiredbas::PlatformInstance("");
	RetVal->AddVertex(V) ;
	(*RetVal)[V].PRJ(Name)SetValue(ComponentName);
	(*RetVal)[V].PRJ(Type)SetValue(ComponentType);
	(*RetVal)[V].PRJ(Position)Set(pP);
	(*RetVal)[V].PRJ(Ports)Set(mPorts);
	return RetVal;
}

cosi::commstruct::wiredbas::Implementation * EthernetSwitch::GetImplementationInstance(
		int V, Position pP, ThreadSet pThreads, TransferTable pT) {
	cosi::commstruct::wiredbas::Implementation
					* RetVal = new cosi::commstruct::wiredbas::Implementation("");
	RetVal->AddVertex(V) ;
	(*RetVal)[V].PRJ(Name)SetValue(ComponentName);
	(*RetVal)[V].PRJ(Type)SetValue(ComponentType);
	(*RetVal)[V].PRJ(Position)Set(pP);
	(*RetVal)[V].PRJ(Ports)Set(mPorts);
	(*RetVal)[V].PRJ(ThreadSet)Set(pThreads);
	(*RetVal)[V].PRJ(TransferTable)Set(pT);
	return RetVal;
}

double EthernetSwitch::GetCost() {
	return mCostModel->GetValue(ComponentName, ComponentType, mPorts) ;
}

RealDelay EthernetSwitch::GetDelay(Vertex NodeId,
		cosi::commstruct::wiredbas::Implementation* pI,Vertex Src, std::string Dest){
	return mDelayModel->Get(NodeId,pI,Src,Dest) ;
}
double EthernetSwitch::GetDelayValue(Vertex NodeId,
		cosi::commstruct::wiredbas::Implementation* pI,Vertex Src, std::string Dest){
	return mDelayModel->GetValue(NodeId,pI,Src,Dest) ;
}

}

}

}
