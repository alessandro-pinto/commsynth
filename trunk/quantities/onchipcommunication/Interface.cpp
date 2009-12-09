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
#include "Interface.h"

namespace cosi {

namespace quantity {

namespace onchipcommunication {

Interface::Interface() {
}

Interface::~Interface() {
}

Interface::tDirection Interface::GetDirection() const {
	return mDir ;
}
void Interface::SetDirection(Interface::tDirection pDir) {
	mDir = pDir ;
}
std::string Interface::GetName() const {
	return mName ;
}
void Interface::SetName(std::string pName) {
	mName = pName ;
}

bool Interface::operator==(const Interface &pP) const {
	//this is the right thing to do
	//however, each field in the physical interface
	//should be a quantity so that the bot and
	//top values can be used
//	if ( mName == pP.GetName() && mDir == pP.GetDirection() &&
//			(PhysicalInterface)(*this) == (PhysicalInterface)(pP)) {
//		return true;
//	} else {
//		return false;
//	}

	//By now simply check name and direction
	if ( mName == pP.GetName() && mDir == pP.GetDirection()) {
		return true;
	} else {
		return false;
	}
}

bool Interface::operator!=(const Interface &pP) const {
	return ( !( (*this) == pP ));
}

std::ostream& operator<<(std::ostream &pOs , const Interface &pP) {
	pOs << pP.mName << " ";
	if ( pP.mDir == Interface::IN ) {
		pOs << "IN";
	} else if (pP.mDir == Interface::OUT ) {
		pOs << "OUT";
	} else if (pP.mDir == Interface::INOUT ) {
		pOs << "INOUT";
	}
	pOs << std::endl;
	pOs << (PhysicalInterface)pP;
	return pOs;
}

}

}

}
