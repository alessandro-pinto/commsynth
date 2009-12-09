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
#include "ArcnetBusDelay.h"

namespace cosi {

namespace models {

namespace wiredbas {

ArcnetBusDelay::ArcnetBusDelay(double pBusSpeed,
		cosi::models::wiredbas::WireDelay* pWireDelay) {
	mBusSpeed = pBusSpeed ;
	mMsgLength[IIT] = 39;
	mMsgLength[FBE ] = 39;
	mMsgLength[ACK ] = 17;
	mMsgLength[SPAC] = 94;
	mMsgLength[LPAC] = 105;
	//Sum n*11 for a message of length n bytes.
	mWireDelay = pWireDelay ;
}

ArcnetBusDelay::~ArcnetBusDelay() {
}

void ArcnetBusDelay::SetWireDelayModel(
		cosi::models::wiredbas::WireDelay* pWireDelay) {
	mWireDelay = pWireDelay ;
}

RealDelay ArcnetBusDelay::Get(cosi::commstruct::wiredbas::Implementation pC) {
	return RealDelay(GetValue(pC)) ;
}

RealDelay ArcnetBusDelay::Get(cosi::commstruct::wiredbas::Implementation* pC) {
	return RealDelay(GetValue(pC)) ;
}

double ArcnetBusDelay::GetValue(cosi::commstruct::wiredbas::Implementation pC) {
	return GetValue(&pC) ;
}

double ArcnetBusDelay::GetValue(cosi::commstruct::wiredbas::Implementation* pC) {

	//do not consider wiring delay that are indeed negligible.
	IdGraph::v_iterator U;
	int Nbits = 0;
	for (U = pC->v_begin() ; U != pC->v_end() ; U++) {
		//If it is a destination only add the IIT
		if ( ( (*pC)[*U].PRJ(ThreadSet)Size() == 0) ) {
			Nbits += mMsgLength[IIT];
		}
		else if ( ((*pC)[*U].PRJ(ThreadSet)GetLongestMessage()).GetMessageLength() == 0 ) {
			Nbits += mMsgLength[IIT];
		}
		else {
			Nbits += ComputeMessageLength( ((*pC)[*U].PRJ(ThreadSet)GetLongestMessage()).GetMessageLength());
		}
	}
	return (Nbits/mBusSpeed);
}

int ArcnetBusDelay::ComputeMessageLength(int pBits) {
	int RetVal = 0;
	RetVal+= mMsgLength[IIT] + mMsgLength[FBE]+ 2*mMsgLength[ACK];

	int Bytes;
	if (pBits%8== 0)
		Bytes = pBits/8;
	else
		Bytes = pBits/8+ 1;
	RetVal += 11*Bytes ;
	//check if it is a short packet or a long packet
	if (Bytes <= 256) {
		RetVal+= mMsgLength[ SPAC ];
	} else {
		RetVal += mMsgLength[ LPAC ];
	}

	return RetVal ;
}

}

}

}
