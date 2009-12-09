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
#include "PointToPointBus.h"

namespace cosi {

namespace library {

namespace onchipcommunication {

std::string PointToPointBus::ComponentName="OCP-Link";
std::string PointToPointBus::ComponentType="Link";

std::string PointToPointBus::GetComponentName(){
	return ComponentName ;

}

std::string PointToPointBus::GetComponentType(){
	return ComponentType ;
}

PointToPointBus::PointToPointBus(int Width ,Technology pTech) {
	//Any node
	Name N;
	N.IsBot(false) ;
	N.IsAny(true) ;
	cosi::quantity::onchipcommunication::Interface S, D;
	S.SetDirection(cosi::quantity::onchipcommunication::Interface::OUT) ;
	S.SetName("OCP") ;
	S.SetWidth(Width) ;
	this->SetSource(N) ;
	this->SetSourceInterface(S) ;
	//Define the destination node
	this->SetDestination(N) ;
	D.SetDirection(cosi::quantity::onchipcommunication::Interface::IN) ;
	D.SetName("OCP") ;
	D.SetWidth(Width) ;
	this->SetDestinationInterface(D) ;
	this->Directed(true);
	mParam = new cosi::quantity::onchipcommunication::LinkParameter(pTech);

}

PointToPointBus::~PointToPointBus() {
}



double PointToPointBus::GetDeviceArea(WiringPath P) {
	return mAreaModel->GetDeviceArea(Name(ComponentName) ,
			GetSourceInterface() , GetDestinationInterface() , P , (*mParam));

}

double PointToPointBus::GetWiringArea(WiringPath P) {
	return mAreaModel->GetWiringArea(Name(ComponentName),
			GetSourceInterface() , GetDestinationInterface() , P , (*mParam));
}

double PointToPointBus::GetStaticPower(WiringPath P) {
	return mPowerModel->GetStaticPower(Name(ComponentName),
			GetSourceInterface() , GetDestinationInterface() , P , (*mParam)) ;
}

double PointToPointBus::GetDynamicPower(WiringPath P, CommoditySet pCommodities) {
	return mPowerModel->GetDynamicPower(Name(ComponentName),
			GetSourceInterface() , GetDestinationInterface() , P ,
			pCommodities , (*mParam));
}

double PointToPointBus::GetDelay(WiringPath P) {
	return mDelayModel->GetDelay(Name(ComponentName),
			GetSourceInterface() , GetDestinationInterface() , P , (*mParam)) ;
}

cosi::commstruct::onchipcommunication::PlatformInstance* PointToPointBus::GetPlatformInstance(
		int U, std::string pPortU, int V, std::string pPortV) {
	cosi::commstruct::onchipcommunication::PlatformInstance
			* RetVal = new cosi::commstruct::onchipcommunication::PlatformInstance("");
	RetVal->AddVertex(U) ;
	(*RetVal)[U].PRJ(Name)IsAny(true);
	(*RetVal)[U].PRJ(Type)IsAny(true);
	(*RetVal)[U].PRJ(Position)IsAny(true);
	(*RetVal)[U].PRJ(NodeParameter)IsAny(true);
	(*RetVal)[U].PRJ(IpGeometry)IsAny(true);
	cosi::quantity::onchipcommunication::Ports PortU;
	PortU.AddPort(pPortU, GetSourceInterface());
	PortU.BoundTo(pPortU, Edge(U, V)) ;
	(*RetVal)[U].PRJ(Ports)Set(PortU);

	RetVal->AddVertex(V) ;
	(*RetVal)[V].PRJ(Name)IsAny(true);
	(*RetVal)[V].PRJ(Type)IsAny(true);
	(*RetVal)[V].PRJ(Position)IsAny(true);
	(*RetVal)[V].PRJ(NodeParameter)IsAny(true);
	(*RetVal)[V].PRJ(IpGeometry)IsAny(true);
	cosi::quantity::onchipcommunication::Ports PortV;
	PortV.AddPort(pPortV, GetDestinationInterface()) ;
	PortV.BoundTo(pPortV, Edge(U, V)) ;
	(*RetVal)[V].PRJ(Ports)Set(PortV);
	RetVal->AddEdge(U, V) ;
	(*RetVal)[Edge(U,V)].PRJ(Name)SetValue(ComponentName);
	(*RetVal)[Edge(U,V)].PRJ(Type)SetValue(ComponentType);
	(*RetVal)[Edge(U,V)].PRJ(WiringPath)IsAny(true);
	(*RetVal)[Edge(U,V)].PRJ(LinkParameter)Set(*mParam);
	return RetVal ;

}

cosi::commstruct::onchipcommunication::Implementation* PointToPointBus::GetImplementationInstance(
		int U, std::string pPortU, int V, std::string pPortV, WiringPath pPath) {
	cosi::commstruct::onchipcommunication::Implementation
			* RetVal = new cosi::commstruct::onchipcommunication::Implementation("");
	RetVal->AddVertex(U) ;
	(*RetVal)[U].PRJ(Name)IsAny(true);
	(*RetVal)[U].PRJ(Type)IsAny(true);
	(*RetVal)[U].PRJ(Position)IsAny(true);
	(*RetVal)[U].PRJ(NodeParameter)IsAny(true);
	(*RetVal)[U].PRJ(IpGeometry)IsAny(true);
	(*RetVal)[U].PRJ(TransferTable)IsAny(true);
	cosi::quantity::onchipcommunication::Ports PortU;
	PortU.AddPort(pPortU, GetSourceInterface()) ;
	PortU.BoundTo(pPortU, Edge(U, V)) ;
	(*RetVal)[U].PRJ(Ports)Set(PortU);

	RetVal->AddVertex(V) ;
	(*RetVal)[V].PRJ(Name)IsAny(true);
	(*RetVal)[V].PRJ(Type)IsAny(true);
	(*RetVal)[V].PRJ(Position)IsAny(true);
	(*RetVal)[V].PRJ(NodeParameter)IsAny(true);
	(*RetVal)[V].PRJ(IpGeometry)IsAny(true);
	(*RetVal)[V].PRJ(TransferTable)IsAny(true);
	cosi::quantity::onchipcommunication::Ports PortV;
	PortV.AddPort(pPortV, GetDestinationInterface()) ;
	PortV.BoundTo(pPortV, Edge(U, V)) ;
	(*RetVal)[V].PRJ(Ports)Set(PortV);
	RetVal->AddEdge(U, V) ;
	(*RetVal)[Edge(U,V)].PRJ(Name)SetValue(ComponentName);
	(*RetVal)[Edge(U,V)].PRJ(Type)SetValue(ComponentType);
	(*RetVal)[Edge(U,V)].PRJ(WiringPath)Set(pPath);
	(*RetVal)[Edge(U,V)].PRJ(CommoditySet)IsAny(true);
	(*RetVal)[Edge(U,V)].PRJ(LinkParameter)Set(*mParam);
	return RetVal ;

}

cosi::commstruct::onchipcommunication::Implementation* PointToPointBus::GetImplementationInstance(
		int U, std::string pPortU, int V, std::string pPortV, WiringPath pPath,
		CommoditySet pCommodities) {
	cosi::commstruct::onchipcommunication::Implementation
			* RetVal = new cosi::commstruct::onchipcommunication::Implementation("");
	RetVal->AddVertex(U) ;
	(*RetVal)[U].PRJ(Name)IsAny(true);
	(*RetVal)[U].PRJ(Type)IsAny(true);
	(*RetVal)[U].PRJ(Position)IsAny(true);
	(*RetVal)[U].PRJ(NodeParameter)IsAny(true);
	(*RetVal)[U].PRJ(IpGeometry)IsAny(true);
	(*RetVal)[U].PRJ(TransferTable)IsAny(true);
	cosi::quantity::onchipcommunication::Ports PortU;
	PortU.AddPort(pPortU, GetSourceInterface()) ;
	PortU.BoundTo(pPortU, Edge(U, V)) ;
	(*RetVal)[U].PRJ(Ports)Set(PortU);

	RetVal->AddVertex(V) ;
	(*RetVal)[V].PRJ(Name)IsAny(true);
	(*RetVal)[V].PRJ(Type)IsAny(true);
	(*RetVal)[V].PRJ(Position)IsAny(true);
	(*RetVal)[V].PRJ(NodeParameter)IsAny(true);
	(*RetVal)[V].PRJ(IpGeometry)IsAny(true);
	(*RetVal)[V].PRJ(TransferTable)IsAny(true);
	cosi::quantity::onchipcommunication::Ports PortV;
	PortV.AddPort(pPortV, GetDestinationInterface()) ;
	PortV.BoundTo(pPortV, Edge(U, V)) ;
	(*RetVal)[V].PRJ(Ports)Set(PortV);
	RetVal->AddEdge(U, V) ;
	(*RetVal)[Edge(U,V)].PRJ(Name)SetValue(ComponentName);
	(*RetVal)[Edge(U,V)].PRJ(Type)SetValue(ComponentType);
	(*RetVal)[Edge(U,V)].PRJ(WiringPath)Set(pPath);
	(*RetVal)[Edge(U,V)].PRJ(CommoditySet)Set(pCommodities);
	(*RetVal)[Edge(U,V)].PRJ(LinkParameter)Set(*mParam);
	return RetVal ;

}

}

}

}
