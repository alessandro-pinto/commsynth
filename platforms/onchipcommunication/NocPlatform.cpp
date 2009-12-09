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

#include "NocPlatform.h"

namespace cosi {

namespace platforms {

namespace onchipcommunication {

using namespace std;

NocPlatform::NocPlatform(std::vector< IpGeometry > IpCores, IpGeometry ChipArea,
		double Density, double Fclk, int FlitWidth, Technology T) {

	mFclk = Fclk ;
	mFlitWidth = FlitWidth ;

	//Set up the position rule
	mPositionRule = new ComponentPosition(IpCores,ChipArea,Density);

	string ComponentName;
	//A vector of nodes
	vector<Name> Components;
	mNodeComponent.resize(3) ;
	mNodeComponent[0] = new Ip(mFlitWidth,mFclk,T);
	ComponentName = (mNodeComponent[0]->GetComponentName() );
	mNodeComponentId[ComponentName] = 0;
	Components.push_back(Name(ComponentName));

	mNodeComponent[1] = new Router(5,5,1,4,mFlitWidth,mFclk,T);
	ComponentName = (mNodeComponent[1]->GetComponentName());
	(mNodeComponent[1])->SetAreaModel(new OrionAreaModel(T));
	(mNodeComponent[1])->SetPowerModel(new OrionPowerModel(T));
	mNodeComponentId[ComponentName] = 1;
	Components.push_back(Name(ComponentName));

	mNodeComponent[2] = new RelayStation(2,mFlitWidth,mFclk,T);
	ComponentName = (mNodeComponent[2]->GetComponentName());
	(mNodeComponent[2])->SetAreaModel(new OrionAreaModel(T));
	(mNodeComponent[2])->SetPowerModel(new OrionPowerModel(T));
	mNodeComponentId[ComponentName] = 2;
	Components.push_back(Name(ComponentName)) ;

	//A vector of links
	mLinkComponent.resize(1);
	mLinkComponent[0] = new PointToPointBus(mFlitWidth,T);
	ComponentName = (mLinkComponent[0]->GetComponentName());
	(mLinkComponent[0])->SetAreaModel(new HoAreaModel(T));
	(mLinkComponent[0])->SetPowerModel(new HoPowerModel(T));
	(mLinkComponent[0])->SetDelayModel(new HoDelayModel(T));
	mLinkComponentId[ComponentName] = 0;

	//Set up the instantiation rule
	mInstantiation = new InstantiationId(IpCores.size(), mPositionRule->GetInstallationSites(),
			Components);

	cerr << "Platform set up done"<< endl ;

}

NocPlatform::~NocPlatform() {
}

//NODE

cosi::commstruct::onchipcommunication::PlatformInstance* NocPlatform::GetPlatformInstance(
		Name pName, int Id, Position pP) {
	return mNodeComponent[mNodeComponentId[pName.GetValue()]]->GetPlatformInstance(Id , pP);
}

cosi::commstruct::onchipcommunication::Implementation* NocPlatform::GetImplementationInstance(
		Name pName, int Id, Position pP, TransferTable pT) {
	return mNodeComponent[mNodeComponentId[pName.GetValue()]]->GetImplementationInstance(Id, pP, pT);
}

cosi::commstruct::onchipcommunication::PlatformInstance* NocPlatform::GetPlatformInstance(
		Name pName, Position pP) {
	int NodeId = mInstantiation->Id(pName, pP) ;
	return GetPlatformInstance(pName , NodeId , pP);
}

cosi::commstruct::onchipcommunication::Implementation* NocPlatform::GetImplementationInstance(
		Name pName, Position pP, TransferTable pT) {
	int NodeId = mInstantiation->Id(pName, pP) ;
	return GetImplementationInstance(pName , NodeId, pP, pT);
}

cosi::commstruct::onchipcommunication::PlatformInstance* NocPlatform::GetPlatformInstance(
		Type pType, int Id, Position pP) {
	for (unsigned int i = 0; i < mNodeComponent.size() ; i++) {
		if (mNodeComponent[i]->GetComponentType() == pType.GetValue()) {
			return mNodeComponent[i]->GetPlatformInstance(Id, pP);
		}
	}
}

cosi::commstruct::onchipcommunication::Implementation* NocPlatform::GetImplementationInstance(
		Type pType, int Id, Position pP, TransferTable pT) {
	for (unsigned int i = 0; i < mNodeComponent.size() ; i++) {
		if (mNodeComponent[i]->GetComponentType() == pType.GetValue()) {
			return mNodeComponent[i]->GetImplementationInstance(Id, pP, pT);
		}
	}
}

cosi::commstruct::onchipcommunication::PlatformInstance* NocPlatform::GetPlatformInstance(
		Type pType, Position pP) {
	//find the component with this type
	for (unsigned int i = 0; i < mNodeComponent.size() ; i++) {
		if (mNodeComponent[i]->GetComponentType() == pType.GetValue()) {
			int NodeId = mInstantiation->Id(
					Name(mNodeComponent[i]->GetComponentName()), pP) ;
			return mNodeComponent[i]->GetPlatformInstance(NodeId, pP);
		}
	}
}

cosi::commstruct::onchipcommunication::Implementation* NocPlatform::GetImplementationInstance(
		Type pType, Position pP, TransferTable pT) {
	//find the component with this type
	for (unsigned int i = 0; i < mNodeComponent.size() ; i++) {
		if (mNodeComponent[i]->GetComponentType() == pType.GetValue()) {
			int NodeId = mInstantiation->Id(
					Name(mNodeComponent[i]->GetComponentName()), pP) ;
			return mNodeComponent[i]->GetImplementationInstance(NodeId, pP, pT);
		}
	}
}

//LINKS
cosi::commstruct::onchipcommunication::PlatformInstance* NocPlatform::GetPlatformInstance(
		Name pName, int U, std::string pPortU, int V, std::string pPortV) {
	return mLinkComponent[mLinkComponentId[pName.GetValue()]]->GetPlatformInstance(U, pPortU, V, pPortV);
}

cosi::commstruct::onchipcommunication::Implementation* NocPlatform::GetImplementationInstance(
		Name pName, int U, std::string pPortU, int V, std::string pPortV,
		WiringPath pPath) {
	return mLinkComponent[mLinkComponentId[pName.GetValue()]]->GetImplementationInstance(U, pPortU, V, pPortV, pPath);

}

cosi::commstruct::onchipcommunication::Implementation* NocPlatform::GetImplementationInstance(
		Name pName, int U, std::string pPortU, int V, std::string pPortV,
		WiringPath pPath, CommoditySet pCommodities) {
	return mLinkComponent[mLinkComponentId[pName.GetValue()]]->GetImplementationInstance(U, pPortU, V, pPortV, pPath,
			pCommodities);
}

cosi::commstruct::onchipcommunication::PlatformInstance* NocPlatform::GetPlatformInstance(
		Type pType, int U, std::string pPortU, int V, std::string pPortV) {
	//find the component with this type
	for (unsigned int i = 0; i < mLinkComponent.size() ; i++) {
		if (mLinkComponent[i]->GetComponentType() == pType.GetValue()) {
			return mLinkComponent[i]->GetPlatformInstance(U, pPortU, V, pPortV);
		}
	}
}

cosi::commstruct::onchipcommunication::Implementation* NocPlatform::GetImplementationInstance(
		Type pType, int U, std::string pPortU, int V, std::string pPortV,
		WiringPath pPath) {
	//find the component with this type
	for (unsigned int i = 0; i < mLinkComponent.size() ; i++) {
		if (mLinkComponent[i]->GetComponentType() == pType.GetValue()) {
			return mLinkComponent[i]->GetImplementationInstance(U, pPortU, V,
					pPortV, pPath);
		}
	}
}

cosi::commstruct::onchipcommunication::Implementation* NocPlatform::GetImplementationInstance(
		Type pType, int U, std::string pPortU, int V, std::string pPortV,
		WiringPath pPath, CommoditySet pCommodities) {
	//find the component with this type
	for (unsigned int i = 0; i < mLinkComponent.size() ; i++) {
		if (mLinkComponent[i]->GetComponentType() == pType.GetValue()) {
			return mLinkComponent[i]->GetImplementationInstance(U, pPortU, V,
					pPortV, pPath, pCommodities);
		}
	}
}

//Performance and cost metrics

//NODE
double NocPlatform::GetArea(Name pName) {
	return mNodeComponent[mNodeComponentId[pName.GetValue()]]->GetArea();
}

double NocPlatform::GetStaticPower(Name pName) {
	return mNodeComponent[mNodeComponentId[pName.GetValue()]]->GetStaticPower();
}

double NocPlatform::GetDynamicPower(Name pName,
		std::map< std::string , CommoditySet > pCommodities) {
	return mNodeComponent[mNodeComponentId[pName.GetValue()]]->GetDynamicPower(pCommodities);
}

//LINK
double NocPlatform::GetDeviceArea(Name pName, WiringPath P) {
	return mLinkComponent[mLinkComponentId[pName.GetValue()]]->GetDeviceArea(P) ;
}

double NocPlatform::GetWiringArea(Name pName, WiringPath P) {
	return mLinkComponent[mLinkComponentId[pName.GetValue()]]->GetWiringArea(P) ;
}

double NocPlatform::GetStaticPower(Name pName, WiringPath P) {
	return mLinkComponent[mLinkComponentId[pName.GetValue()]]->GetStaticPower(P) ;
}

double NocPlatform::GetDelay(Name pName, WiringPath P) {
	return mLinkComponent[mLinkComponentId[pName.GetValue()]]->GetDelay(P) ;
}

double NocPlatform::GetDynamicPower(Name pName, WiringPath P,
		CommoditySet pCommodities) {
	return mLinkComponent[mLinkComponentId[pName.GetValue()]]->GetDynamicPower(P, pCommodities) ;
}

PlatformInstance NocPlatform::Compose(PlatformInstance pOp1, PlatformInstance pOp2) {

}

Implementation NocPlatform::Compose(Implementation pOp1, Implementation pOp2) {

}

int NocPlatform::Id(Name N, Position P) {
	//if
}

NocPlatformInterface* NocPlatform::Abstract() {
	NocPlatformInterface* RetVal = new NocPlatformInterface(mPositionRule);

	//Wire metrics

	//We need to abstract the links in the platform
	//take the average? take the maximum ? Select one particular link?
	//Probably, we should select one particular link and then pass
	//the component name and the link name to the platform so that
	//the algorithms can instantiate the right components

	//Fix a unit length and a bandwidth equal to 1
	//Divide by the flitwidth

	CosiLog << "Platform Abstraction : " << endl ;

	WiringPath P;
	P.AppendPosition(Position(0, 0, 0)) ;
	P.AppendPosition(Position(1, 0, 0)) ;
	Commodity C;
	C.SetValue("", "", 0, 0, 1, 1);
	CommoditySet Cs;
	Cs.Add(C) ;
	RetVal->WireEnergy(GetDynamicPower(Name("OCP-Link"), P, Cs));
	cout << "Wire energy per unit length per bit " << RetVal->WireEnergy() << endl ;

	RetVal->WireLeakage(GetStaticPower(Name("OCP-Link"), P) / (double)mFlitWidth / (double)mFclk );
	cout << "Wire leakage power per unit length per bit " << RetVal->WireLeakage() << endl ;

	RetVal->DeviceAreaPerUnitLengthPerBit(GetDeviceArea(Name("OCP-Link"), P)
			/ (double)mFlitWidth );
	cout << "Wire device area per unit length per bit " << RetVal->WireDeviceArea(1) << endl ;

	RetVal->WiringAreaPerUnitLengthPerBit(GetWiringArea(Name("OCP-Link"), P)
			/ (double)mFlitWidth) ;
	cout << "Wire wiring area per unit length per bit " << RetVal->WireWiringArea(1) << endl ;

	RetVal->WirePitch(GetWiringArea(Name("OCP-Link"), P) / (double)mFlitWidth);
	cout << "Wire pitch " << RetVal->WirePitch() << endl ;

	RetVal->WireLst(1/(mFclk * GetDelay(Name("OCP-Link"), P)));
	cout << "Critical length " << RetVal->WireLst() << endl ;

	RetVal->SetLinkName("OCP-Link") ;


	map< std::string , CommoditySet > Commodities;
	//Note: this is a hack. You should instantiate a
	//router and check the port name.
	//Also, this abstract model should be done in a better way
	//because power consumption might be port-dependent.
	Commodities["P1"] = Cs ;
	double E = GetDynamicPower(Name("OCP-Router"), Commodities);
	double L = GetStaticPower(Name("OCP-Router")) / (double)mFclk;
	double A = GetArea(Name("OCP-Router")) ;
	for (int i = 0; i < 20; i ++) {
		for (int j = 0; j < 20; j ++) {
			if (i <= 5 && j <= 5) {
				RetVal->RouterEnergy(i, j, E);
				RetVal->RouterLeakage(i, j, L);
				RetVal->RouterArea(i, j, A);
			} else {
				RetVal->RouterEnergy(i, j, 10*E);
				RetVal->RouterLeakage(i, j, 10*L);
				RetVal->RouterArea(i, j, 10*A);
			}
		}
	}

	RetVal->SetNodeName("OCP-Router") ;

	//Global network paramters
	RetVal->OperatingFrequency(mFclk);
	RetVal->FlitWidth(mFlitWidth);
	RetVal->SwitchingFactor(0.5);
	RetVal->MaxInDegree(5);
	RetVal->MaxOutDegree(5);
	RetVal->DestInDegree(1);
	RetVal->SourceOutDegree(1);
	return RetVal ;
}

}
}
}
