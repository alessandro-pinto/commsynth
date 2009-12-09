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
#include "RelayStation.h"

namespace cosi {

namespace library {

namespace onchipcommunication {

std::string RelayStation::ComponentName="OCP-Relay-Station";
std::string RelayStation::ComponentType="Repeater";

std::string RelayStation::GetComponentName(){
	return ComponentName ;

}

std::string RelayStation::GetComponentType(){
	return ComponentType ;
}

RelayStation::RelayStation(int Length, int Width, double Fclk,
		cosi::library::onchipcommunication::Technology pTech) {
	mPorts.AddPort("P1", "OCP",
			cosi::quantity::onchipcommunication::Interface::IN) ;
	//configure interface
	mPorts["P1"].SetSpeed(Fclk) ;
	mPorts["P1"].SetWidth(Width) ;
	mPorts["P1"].SetBufferLength(Length) ;
	mPorts["P1"].SetNumberOfVirtualChannels(1) ;
	mPorts.AddPort("P2", "OCP",
			cosi::quantity::onchipcommunication::Interface::OUT) ;
	//configure interface
	mPorts["P2"].SetSpeed(Fclk) ;
	mPorts["P2"].SetWidth(Width) ;
	mPorts["P2"].SetBufferLength(0) ;
	mParam = new cosi::quantity::onchipcommunication::NodeParameter();
	mParam->SetTechnology(pTech) ;

}

RelayStation::~RelayStation() {
}

cosi::commstruct::onchipcommunication::PlatformInstance* RelayStation::GetPlatformInstance(
		int V, Position pP) {
	cosi::commstruct::onchipcommunication::PlatformInstance
			* RetVal = new cosi::commstruct::onchipcommunication::PlatformInstance("");
	RetVal->AddVertex(V) ;
	(*RetVal)[V].PRJ(Name)Set(Name(ComponentName));
	(*RetVal)[V].PRJ(Type)Set(Type(ComponentType));
	(*RetVal)[V].PRJ(Ports)Set(mPorts);
	(*RetVal)[V].PRJ(Position)Set(pP);
	(*RetVal)[V].PRJ(NodeParameter)Set(*mParam);

	double Area = GetArea() ;
	double L = sqrt(Area) ;
	double Xl, Xr, Yt, Yb;
	Xl = pP.GetX() - L/2;
	Xr = pP.GetX() + L/2;
	Yt = pP.GetY() + L/2;
	Yb = pP.GetY() - L/2;

	cosi::quantity::onchipcommunication::IpGeometry G;
	G.SetXl(RealQuantity(Xl));
	G.SetXr(RealQuantity(Xr));
	G.SetYt(RealQuantity(Yt));
	G.SetYb(RealQuantity(Yb));
	G.SetArea(RealQuantity(Area));

	(*RetVal)[V].PRJ(IpGeometry)Set(G);

	return RetVal ;
}

cosi::commstruct::onchipcommunication::Implementation* RelayStation::GetImplementationInstance(
		int V, Position pP, TransferTable pT) {
	cosi::commstruct::onchipcommunication::Implementation
			* RetVal = new cosi::commstruct::onchipcommunication::Implementation("");
	RetVal->AddVertex(V) ;
	(*RetVal)[V].PRJ(Name)Set(Name(ComponentName));
	(*RetVal)[V].PRJ(Type)Set(Type(ComponentType));
	(*RetVal)[V].PRJ(Ports)Set(mPorts);
	(*RetVal)[V].PRJ(Position)Set(pP);
	(*RetVal)[V].PRJ(NodeParameter)Set(*mParam);
	(*RetVal)[V].PRJ(TransferTable)Set(pT);

	double Area = GetArea() ;

	double L = sqrt(Area) ;

	double Xl, Xr, Yt, Yb;
	Xl = pP.GetX() - L/2;
	Xr = pP.GetX() + L/2;
	Yt = pP.GetY() + L/2;
	Yb = pP.GetY() - L/2;

	cosi::quantity::onchipcommunication::IpGeometry G;
	G.SetXl(RealQuantity(Xl));
	G.SetXr(RealQuantity(Xr));
	G.SetYt(RealQuantity(Yt));
	G.SetYb(RealQuantity(Yb));
	G.SetArea(RealQuantity(Area));

	(*RetVal)[V].PRJ(IpGeometry)Set(G);

	return RetVal ;
}

double RelayStation::GetArea() {
	return mAreaModel->GetArea(Name(ComponentName), mPorts, (*mParam)) ;
}

double RelayStation::GetStaticPower() {
	return mPowerModel->GetStaticPower(Name(ComponentName), mPorts, (*mParam)) ;
}

double RelayStation::GetDynamicPower(
		std::map< std::string , CommoditySet > pCommodities) {
	return mPowerModel->GetDynamicPower(Name(ComponentName), mPorts,
			pCommodities, (*mParam));
}

}

}

}
