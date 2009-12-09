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
#include "EthernetLinkDelay.h"

namespace cosi {

namespace models {

namespace wiredbas {

EthernetLinkDelay::EthernetLinkDelay(double pSpeed) {
	mSpeed = pSpeed;
	enum Field {PREAMBLE,SOF,DEST,SRC,TYPE,PAYLOAD,CRC,IFD};
	mFieldLength[PREAMBLE] = 56;
	mFieldLength[SOF] = 8;
	mFieldLength[DEST] = 48;
	mFieldLength[SRC] = 48;
	mFieldLength[TYPE] = 16;
	mFieldLength[CRC] = 32;
	mFieldLength[IFD] = 96;
	mOverhead = mFieldLength[PREAMBLE] + mFieldLength[SOF] + mFieldLength[DEST]
			+ mFieldLength[SRC] +mFieldLength[TYPE]+mFieldLength[CRC]
			+ mFieldLength[IFD] ;
}

EthernetLinkDelay::~EthernetLinkDelay() {
}

RealDelay EthernetLinkDelay::Get(int MsgLength, WiringPath pPath) {

}

double EthernetLinkDelay::GetValue(int MsgLength, WiringPath pPath) {
	int Nframe = (int)ceil((double)MsgLength/ 12000.0) ;
	int LastFrame = Nframe%12000;
	if (LastFrame < 368)
		LastFrame = 368;
	int Nbits = (Nframe-1)*(mOverhead + 12000)  + mOverhead + (int) ceil((double)LastFrame/8.0);
	return ( (double)Nbits / mSpeed ) ;
}

}

}

}
