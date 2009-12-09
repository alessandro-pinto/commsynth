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
#ifndef LIBRARIES_WIREDBAS_COMMON_NODE_H_
#define LIBRARIES_WIREDBAS_COMMON_NODE_H_

#include "InstantiableNode.h"
#include "quantities/CommoditySet.h"
#include "models/onchipcommunication/common/AreaModel.h"
#include "models/onchipcommunication/common/PowerModel.h"


namespace cosi {

namespace library {

namespace onchipcommunication {

class Node : public cosi::library::onchipcommunication::InstantiableNode {
protected:
	cosi::models::onchipcommunication::NodeAreaModel* mAreaModel;

	cosi::models::onchipcommunication::NodePowerModel* mPowerModel;

public:
	Node();
	virtual ~Node();

	void SetAreaModel(cosi::models::onchipcommunication::NodeAreaModel* pModel);

	void
			SetPowerModel(
					cosi::models::onchipcommunication::NodePowerModel* pModel);

	//COST METRICS
	virtual double GetArea() = 0 ;
	virtual double GetStaticPower() = 0  ;
	virtual double GetDynamicPower(
			std::map< std::string , CommoditySet > pCommodities) = 0 ;

	virtual std::string GetComponentName() = 0 ;
	virtual std::string GetComponentType() = 0 ;


};

}

}

}

#endif /*NODE_H_*/
