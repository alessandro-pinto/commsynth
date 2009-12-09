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
#ifndef ARCNETBUSDELAY_H_
#define ARCNETBUSDELAY_H_

#include "networks/wiredbas/WiredBasImplementation.h"
#include "quantities/RealDelay.h"
#include "models/wiredbas/common/BusDelay.h"
#include "models/wiredbas/common/WireDelayModel.h"
#include "float.h"

namespace cosi {

namespace models {

namespace wiredbas {

class ArcnetBusDelay : public BusDelay {

	cosi::models::wiredbas::WireDelay* mWireDelay;

	int ComputeMessageLength(int pLength);

	double mBusSpeed;

	enum tProtocolPhases {IIT,FBE,ACK,SPAC,LPAC};

	map< int , int > mMsgLength;

public:

	ArcnetBusDelay(double pBusSpeed, cosi::models::wiredbas::WireDelay* pWireDelay);

	~ArcnetBusDelay();

	void SetWireDelayModel(cosi::models::wiredbas::WireDelay* pWireDelay);

	RealDelay Get(cosi::commstruct::wiredbas::Implementation pC);

	RealDelay Get(cosi::commstruct::wiredbas::Implementation* pC);

	double GetValue(cosi::commstruct::wiredbas::Implementation pC);

	double GetValue(cosi::commstruct::wiredbas::Implementation* pC);

};

}

}

}

#endif /*ARCNETBUSDELAY_H_*/
