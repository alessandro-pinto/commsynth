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
#ifndef ETHERNETNODE_H_
#define ETHERNETNODE_H_

#include "networks/wiredbas/WiredBasPlatformInstance.h"
#include "networks/wiredbas/WiredBasImplementation.h"
#include "quantities/Position.h"
#include "quantities/Bandwidth.h"
#include "quantities/Ports.h"
#include "quantities/RealDelay.h"
#include "libraries/wiredbas/common/Node.h"
#include "models/wiredbas/node/EthernetNodeCost.h"
#include <string>

namespace cosi {

namespace library {

namespace wiredbas {

class EthernetNode : public cosi::library::wiredbas::Node {
protected:
	cosi::models::wiredbas::EthernetNodeCost* mCostModel;
public:
	EthernetNode();
	virtual ~EthernetNode();
	void SetCostModel(cosi::models::wiredbas::EthernetNodeCost* pCostModel);
	virtual cosi::commstruct::wiredbas::PlatformInstance* GetPlatformInstance(
			int V, Position pP) = 0;
	virtual cosi::commstruct::wiredbas::Implementation
			* GetImplementationInstance(int V, Position pP, ThreadSet pThreads,
					TransferTable pT) = 0;
	virtual RealDelay GetDelay(Vertex NodeId,
			cosi::commstruct::wiredbas::Implementation* pI, Vertex Src,
			std::string Dest) = 0;
	virtual double GetDelayValue(Vertex NodeId,
			cosi::commstruct::wiredbas::Implementation* pI, Vertex Src,
			std::string Dest) = 0;
	virtual double GetCost() =0 ;

};

}

}

}

#endif /*ETHERNETNODE_H_*/
