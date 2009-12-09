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
#include "ArcnetController.h"

namespace cosi
{

namespace library
{

namespace wiredbas
{
std::string ArcnetController::ComponentName = "ArcnetController";
std::string ArcnetController::ComponentType = "Controller";

ArcnetController::ArcnetController() {
	//set up the ports
	mPorts.AddPort("P1", "EIA-485", cosi::PortInterface::INOUT) ;
	mPorts.AddPort("P2", "EIA-485", cosi::PortInterface::INOUT) ;
}

ArcnetController::~ArcnetController() {
}

cosi::commstruct::wiredbas::PlatformInstance* ArcnetController::GetPlatformInstance(
		int V, Position pP) {
	cosi::commstruct::wiredbas::PlatformInstance
			* RetVal = new cosi::commstruct::wiredbas::PlatformInstance("");
	RetVal->AddVertex(V) ;
	(*RetVal)[V].PRJ(Position)Set(pP);
	(*RetVal)[V].PRJ(Name)SetValue(ComponentName) ;
	(*RetVal)[V].PRJ(Type)SetValue(ComponentType);
	(*RetVal)[V].PRJ(Ports)Set(mPorts);
	return RetVal ;
}
cosi::commstruct::wiredbas::Implementation* ArcnetController::GetImplementationInstance(
		int V, Position pP, ThreadSet pThreads, TransferTable pT) {
	cosi::commstruct::wiredbas::Implementation
			* RetVal = new cosi::commstruct::wiredbas::Implementation("");
	RetVal->AddVertex(V) ;
	(*RetVal)[V].PRJ(Position)Set(pP);
	(*RetVal)[V].PRJ(ThreadSet)Set(pThreads);
	(*RetVal)[V].PRJ(TransferTable)Set(pT);
	(*RetVal)[V].PRJ(Name)SetValue(ComponentName) ;
	(*RetVal)[V].PRJ(Type)SetValue(ComponentType);
	(*RetVal)[V].PRJ(Ports)Set(mPorts);
	return RetVal ;
}

double ArcnetController::GetCost() {
	return mCostModel->GetValue(ComponentName,ComponentType,mPorts) ;
}

RealDelay
ArcnetController::GetDelay(Vertex NodeId,
				cosi::commstruct::wiredbas::Implementation* pI, Vertex Src,
				Vertex Dest){
	return RealDelay(0) ;
}
double
ArcnetController::GetDelayValue(Vertex NodeId,
				cosi::commstruct::wiredbas::Implementation* pI, Vertex Src,
				Vertex Dest){
	return 0 ;
}


}

}

}
