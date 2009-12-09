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
#include "Type.h"

namespace cosi {

std::string Type::sQuantityName = "Type";

Type::Type() {

}

Type::Type(std::string pType) :
	StringQuantity(pType) {

}

Type Type::Get() const {
	return (*this);
}

void Type::Set(Type pType) {
	(*this) = pType ;
}

bool Type::operator ==(Type pType) {
	return ( this->GetValue() == pType.GetValue());
}

void Type::operator << ( Type pValue ) {
	(*this) = pValue;
}

const Type Type::operator+(const Type &pT) const{
	Type T ;
	if ( IsBot() ) {
		return pT ;
	}
	if ( pT.IsBot()) {
		return Get() ;
	}
	if ( ! (Get() == pT.Get()) || IsAny() || pT.IsAny() ) {
		T.IsAny(true) ;
		return T ;
	}
	//they are equal
	return pT ;
}

const Type Type::operator-(const Type &pT) const{
	if ( IsAny() )
		return pT ;
	if ( pT.IsAny())
		return Get();
	if ( Get() == pT.Get() ) {
		return pT ;
	}
	return Type() ;
}

std::ostream& operator <<(std::ostream &pOs, const Type &pT) {
	pOs << pT.mValue;
	return pOs;
}

}
