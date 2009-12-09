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
#ifndef ROUTER_H_
#define ROUTER_H_

#include <string>
#include <sstream>
#include "libraries/onchipcommunication/common/Node.h"
#include "quantities/onchipcommunication/Interface.h"
#include "quantities/onchipcommunication/NodeParameter.h"
#include "networks/onchipcommunication/PlatformInstance.h"
#include "networks/onchipcommunication/Implementation.h"
#include "models/onchipcommunication/common/AreaModel.h"
#include "models/onchipcommunication/common/PowerModel.h"

namespace cosi {

namespace library {

namespace onchipcommunication {

class Router : public cosi::library::onchipcommunication::Node {

	cosi::quantity::onchipcommunication::NodeParameter* mParam;

public:

	Router(int Nin, int Nout, int Vch, int Length, int Width, double Fclk,
			cosi::library::onchipcommunication::Technology pTech);

	virtual ~Router();

	cosi::commstruct::onchipcommunication::PlatformInstance
			* GetPlatformInstance(int V, Position pP);

	cosi::commstruct::onchipcommunication::Implementation
			* GetImplementationInstance(int V, Position pP, TransferTable pT);

	double GetArea();

	double GetStaticPower();

	double GetDynamicPower(std::map< std::string , CommoditySet > pCommodities);

	std::string ComponentName;
	static std::string ComponentType;

	std::string GetComponentName();
	std::string GetComponentType();

};

}

}

}

#endif /*ROUTER_H_*/
