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
#ifndef POINTTOPOINTBUS_H_
#define POINTTOPOINTBUS_H_

#include "networks/onchipcommunication/PlatformInstance.h"
#include "networks/onchipcommunication/Implementation.h"
#include "libraries/onchipcommunication/common/Link.h"
#include "quantities/onchipcommunication/Interface.h"
#include "models/onchipcommunication/common/AreaModel.h"
#include "models/onchipcommunication/common/PowerModel.h"
#include "include/onchipcommunication/typedefs.h"
#include "quantities/onchipcommunication/LinkParameter.h"
#include "quantities/onchipcommunication/NodeParameter.h"


namespace cosi {

namespace library {

namespace onchipcommunication {

class PointToPointBus : public Link {

	cosi::quantity::onchipcommunication::LinkParameter* mParam;
public:
	//technology and paramters
	PointToPointBus(int Width , Technology pTech);

	virtual ~PointToPointBus();


	double GetDeviceArea(WiringPath P);

	double GetWiringArea(WiringPath P);

	double GetStaticPower(WiringPath P);

	double GetDynamicPower(WiringPath P, CommoditySet pCommodities);

	double GetDelay(WiringPath P) ;

	cosi::commstruct::onchipcommunication::PlatformInstance
			* GetPlatformInstance(int U, std::string pPortsU, int V,
					std::string pPortV) ;

	cosi::commstruct::onchipcommunication::Implementation
			* GetImplementationInstance(int U, std::string pPortsU, int V,
					std::string pPortV, WiringPath pPath) ;

	cosi::commstruct::onchipcommunication::Implementation
			* GetImplementationInstance(int U, std::string pPortsU, int V,
					std::string pPortV, WiringPath pPath,
					CommoditySet pCommodities);

	static std::string ComponentName;
	static std::string ComponentType;

	 std::string GetComponentName()  ;
	 std::string GetComponentType()  ;

};

}

}

}

#endif /*POINTTOPOINTBUS_H_*/
