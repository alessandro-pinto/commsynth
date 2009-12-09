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
#include "ArcnetBusRule.h"

namespace cosi {

namespace rules {

namespace wiredbas {

ArcnetBusRule::ArcnetBusRule(double pBusSpeed) {
	mBusSpeed = pBusSpeed ;
	mMsgLength[IIT] = 39;
	mMsgLength[FBE ] = 39;
	mMsgLength[ACK ] = 17;
	mMsgLength[SPAC] = 94;
	mMsgLength[LPAC] = 105;
}

ArcnetBusRule::~ArcnetBusRule() {
}

bool ArcnetBusRule::Satisfies(cosi::commstruct::wiredbas::Implementation* pC) {
	//check the degree, the total length and the bandwidth

	//Degree
	if (mBusSpeed == 2.5e6&& pC->Order() > 8) {
		std::cout<< "Degree rule violation : Maximum degree 8 , actual degree "
				<< pC->Order() << std::endl ;
		return false;
	} else if (mBusSpeed == 78e3&& pC->Order() > 32) {
		std::cout
				<< "Degree rule violation : Maximum degree 32 , actual degree "
				<< pC->Order() << std::endl ;
		return false;
	}

	//Compute total length
	double Length = 0;
	IdGraph::v_iterator U, V;
	for (U = pC->v_begin() ; U != pC->v_end() ; U++) {
		for (V = pC->v_begin() ; V != pC->v_end() ; V++) {
			if (pC->InE(*U, *V)) {
				if ((*pC)[Edge(*U,*V)].PRJ(WiringPath)IsAny() || (*pC)[Edge(*U,*V)].PRJ(WiringPath)IsBot()) {
					return false;
				}
				Length+= (*pC)[Edge(*U,*V)].PRJ(WiringPath)Length();
			}
		}
	}

	//Length
	if (mBusSpeed == 2.5e6&& Length > 120) {
		std::cout
				<< "Length rule violation : Maximum length 120 , actual length "
				<< Length << std::endl ;
		return false;
	} else if (mBusSpeed == 78e3&& Length > 500) {
		std::cout
				<< "Length rule violation : Maximum length 500 , actual length "
				<< Length << std::endl ;
		return false;
	}

	double B = GetBandwidthValue(pC) ;
	if (mBusSpeed < B ) {
		std::cout<< "Bandwdith Violation : Maximum "<< mBusSpeed << " actual "
				<< B << endl ;
		return false;
	}

	return true;

}

bool ArcnetBusRule::Satisfies(cosi::commstruct::wiredbas::Implementation pC) {
	return Satisfies(&pC) ;
}

double ArcnetBusRule::GetBandwidthValue(
		cosi::commstruct::wiredbas::Implementation* pC) {
	//Compute total bandwdith
	double Bandwidth = 0;
	int IitBits = 0;
	int Nbits = 0;
	IdGraph::v_iterator U ;
	for (U = pC->v_begin() ; U != pC->v_end() ; U++) {
		//If it is a destination only add the IIT
		if ( ((*pC)[*U].PRJ(ThreadSet)Size() == 0)) {
			Nbits += mMsgLength[IIT];
			IitBits = mMsgLength[IIT];
		} else if (((*pC)[*U].PRJ(ThreadSet)GetLongestMessage()).GetMessageLength() == 0) {
			Nbits += mMsgLength[IIT];
			IitBits = mMsgLength[IIT];
		} else {
			int Tmp = 0;
			Tmp+= mMsgLength[IIT] + mMsgLength[FBE]+ 2*mMsgLength[ACK];
			int Bits = ((*pC)[*U].PRJ(ThreadSet)GetLongestMessage()).GetMessageLength() ;
			int Bytes;
			if (Bits%8== 0)
				Bytes = Bits/8;
			else
				Bytes = Bits/8+ 1;
			Tmp += 11*Bytes ;
			//check if it is a short packet or a long packet
			if (Bytes <= 256) {
				Tmp+= mMsgLength[ SPAC ];
			} else {
				Tmp += mMsgLength[ LPAC ];
			}
			Nbits += Tmp ;
			//Sum up the bandwidths from all threads
			ThreadSet::iterator Tit;
			for (Tit = (*pC)[*U].PRJ(ThreadSet)ThreadSetBegin() ; Tit != (*pC)[*U].PRJ(ThreadSet)ThreadSetEnd() ; Tit++) {
				if (Tit->GetMessageLength() != 0) {
					Tmp = 0;
					Tmp+= mMsgLength[IIT] + mMsgLength[FBE]+ 2*mMsgLength[ACK];
					int Bits = Tit->GetMessageLength() ;
					int Bytes;
					if (Bits%8== 0)
						Bytes = Bits/8;
					else
						Bytes = Bits/8+ 1;
					Tmp += 11*Bytes ;
					//check if it is a short packet or a long packet
					if (Bytes <= 256) {
						Tmp+= mMsgLength[ SPAC ];
					} else {
						Tmp += mMsgLength[ LPAC ];
					}

					Bandwidth += ((double) Tmp )/ (double) Tit->GetPeriod() ;
				}
			}
		}

	}
	//add the IIT overhead
	Bandwidth += mBusSpeed * ((double) IitBits / (double) Nbits);
	return Bandwidth ;
}

}

}

}
