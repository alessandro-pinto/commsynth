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
#include "Router.h"

namespace cosi {

namespace library {

namespace onchipcommunication {

std::string Router::ComponentType="Router";

std::string Router::GetComponentName(){
	return ComponentName ;

}

std::string Router::GetComponentType(){
	return ComponentType ;
}


Router::Router(int Nin, int Nout, int Vch, int Length, int Width, double Fclk,
		cosi::library::onchipcommunication::Technology pTech) {
	//set up the ports
	for (int i = 0; i < Nin ; i++) {
		stringstream s;
		s << i ;
		string Pname = "P" + s.str() ;
		mPorts.AddPort(Pname, "OCP",
				cosi::quantity::onchipcommunication::Interface::IN) ;
		//configure interface
		mPorts[Pname].SetSpeed(Fclk) ;
		mPorts[Pname].SetWidth(Width) ;
		mPorts[Pname].SetNumberOfVirtualChannels(Vch) ;
		mPorts[Pname].SetNumberOfVirtualChannels(Vch) ;
		mPorts[Pname].SetBufferLength(Length) ;
	}
	for (int i = 0; i < Nout ; i++) {
		stringstream s;
		s << Nin + i ;
		string Pname = "P" + s.str() ;
		mPorts.AddPort(Pname, "OCP",
				cosi::quantity::onchipcommunication::Interface::OUT) ;
		//configure interface
		mPorts[Pname].SetWidth(Width) ;

	}


	mParam = new cosi::quantity::onchipcommunication::NodeParameter();
	mParam->SetTechnology(pTech) ;

	//build the component name
	std::stringstream s ;
	s << "OCP-Router_" << Nin << "_" << Nout << "_" << Width << "_" << Vch << "_" << Length << std::endl ;
	ComponentName = s.str();
}

Router::~Router() {
}


cosi::commstruct::onchipcommunication::PlatformInstance* Router::GetPlatformInstance(
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

cosi::commstruct::onchipcommunication::Implementation* Router::GetImplementationInstance(
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

double Router::GetArea() {
	return mAreaModel->GetArea(Name(ComponentName), mPorts, (*mParam)) ;
}

double Router::GetStaticPower() {
	return mPowerModel->GetStaticPower(Name(ComponentName), mPorts, (*mParam)) ;
}

double Router::GetDynamicPower(
		std::map< std::string , CommoditySet > pCommodities) {
	return mPowerModel->GetDynamicPower(Name(ComponentName), mPorts,
			pCommodities, (*mParam));
}

}

}

}
