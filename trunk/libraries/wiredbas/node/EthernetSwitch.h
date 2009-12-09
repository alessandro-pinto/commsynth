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
#ifndef ETHERNETSWITCH_H_
#define ETHERNETSWITCH_H_

#include <sstream>
#include "EthernetNode.h"
#include "models/wiredbas/node/EthernetSwitchDelay.h"

namespace cosi {

namespace library {

namespace wiredbas {

class EthernetSwitch : public EthernetNode {
	cosi::models::wiredbas::EthernetSwitchDelay* mDelayModel;
public:
	EthernetSwitch(int Nin, int Nout, double Speed);
	virtual ~EthernetSwitch();
	cosi::commstruct::wiredbas::PlatformInstance* GetPlatformInstance(int V,
			Position pP);

	cosi::commstruct::wiredbas::Implementation * GetImplementationInstance(
			int V, Position pP, ThreadSet pThreads, TransferTable pT);

	RealDelay GetDelay(Vertex NodeId,
			cosi::commstruct::wiredbas::Implementation* pI, Vertex Src,
			std::string Dest);
	double GetDelayValue(Vertex NodeId,
			cosi::commstruct::wiredbas::Implementation* pI, Vertex Src,
			std::string Dest);
	double GetCost();

	 RealDelay GetDelay(Vertex NodeId,
			cosi::commstruct::wiredbas::Implementation* pI, Vertex Src,
			Vertex Dest) {return RealDelay(0);};
	 double GetDelayValue(Vertex NodeId,
			cosi::commstruct::wiredbas::Implementation* pI, Vertex Src,
			Vertex Dest) {return 0;};

	std::string ComponentName;
	std::string ComponentType;
};

}

}

}

#endif /*ETHERNETSWITCH_H_*/
