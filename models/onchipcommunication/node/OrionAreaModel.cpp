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
#include "OrionAreaModel.h"

namespace cosi {

namespace models {

namespace onchipcommunication {

using namespace cosi::library::onchipcommunication ;

OrionAreaModel::OrionAreaModel(cosi::library::onchipcommunication::Technology pTech) {
	cosi::io::onchipcommunication::HoOrionModelParser* P = 0x0;
	if (pTech == T90 ) {
		P = new cosi::io::onchipcommunication::HoOrionModelParser(CosiRoot + "/models/onchipcommunication/node/ho-orion-90nm.xml");
	} else if (pTech == T65 ) {
		P = new cosi::io::onchipcommunication::HoOrionModelParser(CosiRoot + "/models/onchipcommunication/node/ho-orion-65nm.xml");
	} else if (pTech == T45) {
		P = new cosi::io::onchipcommunication::HoOrionModelParser(CosiRoot + "/models/onchipcommunication/node/ho-orion-45nm.xml");
	}

	P->Parse() ;

	mArouter1ch32 = P->GetArea1ch32() ;
	mArouter4ch32 = P->GetArea4ch32() ;
	mArouter1ch128 = P->GetArea1ch128() ;
	mArouter4ch128 = P->GetArea4ch128() ;

	mMaxIn = P->GetMaxIn() ;
	mMaxOut = P->GetMaxOut() ;

	delete P ;

}

OrionAreaModel::~OrionAreaModel() {
}

double OrionAreaModel::GetArea(Name pName,
		cosi::quantity::onchipcommunication::Ports pPorts,
		cosi::quantity::onchipcommunication::NodeParameter pParam) {

	//get the largest port in terms of width and approximate to 32 or 128
	cosi::quantity::onchipcommunication::Ports::iterator It;
	int MaxWidth = 0;
	int MaxVch = 0;
	int Nin = 0;
	int Nout = 0;
	for (It = pPorts.Begin() ; It != pPorts.End() ; It++) {
		if (pPorts.If(It).GetWidth() > MaxWidth ) {
			MaxWidth = pPorts.If(It).GetWidth() ;
		}
		if (pPorts.If(It).GetNumberOfVirtualChannels() > MaxVch ) {
			MaxVch = pPorts.If(It).GetNumberOfVirtualChannels() ;
		}
		if (pPorts.If(It).GetDirection()
				== cosi::quantity::onchipcommunication::Interface::IN ) {
			Nin++;
		}
		if (pPorts.If(It).GetDirection()
				== cosi::quantity::onchipcommunication::Interface::OUT ) {
			Nout++;
		}
	}

	if( Nin > mMaxIn )
		Nin = mMaxIn ;
	if( Nout > mMaxOut)
		Nout = mMaxOut ;

	if (MaxWidth <= 32) {
		if (MaxVch == 1) {
			return mArouter1ch32[Nin][Nout] ;
		} else {
			return mArouter4ch32[Nin][Nout] ;
		}
	} else {
		if (MaxVch == 1) {
			return mArouter1ch128[Nin][Nout] ;
		} else {
			return mArouter4ch128[Nin][Nout] ;
		}
	}

}

}

}

}
