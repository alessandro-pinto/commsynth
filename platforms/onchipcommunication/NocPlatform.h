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

#ifndef NOCPLATFORM_H_
#define NOCPLATFORM_H_

#include <iostream>
#include <vector>
#include <map>
//Components
#include "libraries/onchipcommunication/common/Node.h"
#include "libraries/onchipcommunication/common/Link.h"
#include "libraries/onchipcommunication/link/PointToPointBus.h"
#include "libraries/onchipcommunication/node/Router.h"
#include "libraries/onchipcommunication/node/RelayStation.h"
#include "libraries/onchipcommunication/node/Ip.h"

//Models
#include "models/onchipcommunication/common/PowerModel.h"
#include "models/onchipcommunication/common/AreaModel.h"
#include "models/onchipcommunication/common/DelayModel.h"
#include "models/onchipcommunication/link/HoAreaModel.h"
#include "models/onchipcommunication/link/HoPowerModel.h"
#include "models/onchipcommunication/link/HoDelayModel.h"
#include "models/onchipcommunication/node/OrionAreaModel.h"
#include "models/onchipcommunication/node/OrionPowerModel.h"

//Rules
#include "rules/onchipcommunication/ComponentPosition.h"
#include "rules/onchipcommunication/InstantiationId.h"

//Abstract platfomr
#include "NocPlatformInterface.h"

namespace cosi {

namespace platforms {

namespace onchipcommunication {

using namespace cosi::rules::onchipcommunication;
using namespace cosi::library::onchipcommunication;
using namespace cosi::commstruct::onchipcommunication;
using namespace cosi::environment::onchipcommunication;
using namespace cosi::models::onchipcommunication;
using namespace cosi::io::onchipcommunication;
using namespace std ;

class NocPlatform
{
	ComponentPosition* mPositionRule;
	InstantiationId* mInstantiation;

	//A vector of nodes
	vector< Node* > mNodeComponent;
	map<string,int> mNodeComponentId;

	//A vector of links
	vector< Link* > mLinkComponent;
	map<string,int> mLinkComponentId;

	//Plt paramters
	//clock frequency: we are assuming synchronous implementation
	//this assumption is not needed, we are using this just to
	//start the implementation of a platform. Other platfomrs
	//can be implemented later.
	double mFclk;
	//Also we fix the flitwidth but again we can remove
	//this assumption
	int mFlitWidth;
public:
	NocPlatform(std::vector< IpGeometry > Ip, IpGeometry ChipArea, double Density, double Fclk, int FlitWidth , Technology T);

	virtual ~NocPlatform();

	//COMPONENT INSTANTIATION

	//NODE
	cosi::commstruct::onchipcommunication::PlatformInstance
			* GetPlatformInstance(Name pName, Position pP);

	cosi::commstruct::onchipcommunication::Implementation
			* GetImplementationInstance(Name pName, Position pP,
					TransferTable pT);

	cosi::commstruct::onchipcommunication::PlatformInstance
			* GetPlatformInstance(Name pName, int Id, Position pP);

	cosi::commstruct::onchipcommunication::Implementation
			* GetImplementationInstance(Name pName, int Id, Position pP,
					TransferTable pT);

	cosi::commstruct::onchipcommunication::PlatformInstance
			* GetPlatformInstance(Type pType, Position pP);

	cosi::commstruct::onchipcommunication::Implementation
			* GetImplementationInstance(Type pType, Position pP,
					TransferTable pT);

	cosi::commstruct::onchipcommunication::PlatformInstance
			* GetPlatformInstance(Type pType, int Id, Position pP);

	cosi::commstruct::onchipcommunication::Implementation
			* GetImplementationInstance(Type pType, int Id, Position pP,
					TransferTable pT);

	//LINKS
	cosi::commstruct::onchipcommunication::PlatformInstance
			* GetPlatformInstance(Name pName, int U, std::string pPortsU,
					int V, std::string pPortV);

	cosi::commstruct::onchipcommunication::Implementation
			* GetImplementationInstance(Name pName, int U, std::string pPortsU,
					int V, std::string pPortV, WiringPath pPath);

	cosi::commstruct::onchipcommunication::Implementation
			* GetImplementationInstance(Name pName, int U, std::string pPortsU,
					int V, std::string pPortV, WiringPath pPath,
					CommoditySet pCommodities);

	cosi::commstruct::onchipcommunication::PlatformInstance
			* GetPlatformInstance(Type pType, int U, std::string pPortsU,
					int V, std::string pPortV);

	cosi::commstruct::onchipcommunication::Implementation
			* GetImplementationInstance(Type pType, int U, std::string pPortsU,
					int V, std::string pPortV, WiringPath pPath);


	cosi::commstruct::onchipcommunication::Implementation
			* GetImplementationInstance(Type pType, int U, std::string pPortsU,
					int V, std::string pPortV, WiringPath pPath,
					CommoditySet pCommodities);

	//Performance and cost metrics

	//NODE
	double GetArea(Name pName);

	double GetStaticPower(Name pName);

	double GetDynamicPower(Name pName,
			std::map< std::string , CommoditySet > pCommodities);

	//LINK
	double GetDeviceArea(Name pName, WiringPath P);

	double GetWiringArea(Name pName, WiringPath P);

	double GetStaticPower(Name pName, WiringPath P);

	double GetDynamicPower(Name pName, WiringPath P, CommoditySet pCommodities);

	double GetDelay(Name pName, WiringPath P) ;

	PlatformInstance Compose(PlatformInstance pOp1, PlatformInstance pOp2);

	Implementation Compose(Implementation pOp1, Implementation pOp2);

	int Id(Name N, Position P);

	//Abstraction
	NocPlatformInterface* Abstract() ;

};

}

}

}

#endif /*NOCPLATFORM_H_*/
