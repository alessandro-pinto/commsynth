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
#include "NodeParameter.h"

namespace cosi {

namespace quantity {

namespace onchipcommunication {

NodeParameter::NodeParameter() {
}

NodeParameter::~NodeParameter() {
}

NodeParameter NodeParameter::Get() {
	return (*this);
}

void NodeParameter::Set(NodeParameter pLp) {
	(*this) = pLp ;
}

cosi::library::onchipcommunication::Technology NodeParameter::GetTechnology() const {
	return mTech ;
}

void NodeParameter::SetTechnology(
		cosi::library::onchipcommunication::Technology pTech) {
	mTech = pTech ;
	IsBot(false);
}

bool NodeParameter::operator ==(const NodeParameter &pP) const {
	if ( IsAny() && pP.IsAny() ) {
		return true;
	}
	if ( IsBot() && pP.IsBot() ) {
		return true;
	}
	if ( mTech == pP.GetTechnology()) {
		return true;
	} else {
		return false;
	}
}

bool NodeParameter::operator <(const NodeParameter &pP) const {
	if ( pP.IsAny() && ! IsAny()) {
		return true;
	}
	if ( IsBot() && ! pP.IsBot() ) {
		return true;
	}
	return false;
}

NodeParameter NodeParameter::operator+(NodeParameter pT) {
	if ( IsAny() )
	return pT;
	if ( pT.IsAny())
	return (*this);
	if ( (*this) == pT ) {
		return (*this);
	} else {
		return NodeParameter();
	}
}
NodeParameter NodeParameter::operator-(NodeParameter pT) {
	if ( IsAny() )
	return pT;
	if ( pT.IsAny())
	return (*this);
	if ( (*this) == pT ) {
		return (*this);
	} else {
		return NodeParameter();
	}
}

std::ostream& operator <<(std::ostream &pOs, const NodeParameter &pP) {
	pOs << "Technology " ;
	if ( pP.GetTechnology() == cosi::library::onchipcommunication::T90 )
		pOs << " 90 nm" ;
	else if ( pP.GetTechnology() == cosi::library::onchipcommunication::T65 )
		pOs << " 65 nm" ;
	else if ( pP.GetTechnology() == cosi::library::onchipcommunication::T45 )
			pOs << " 45 nm" ;
	else if ( pP.GetTechnology() == cosi::library::onchipcommunication::T32 )
			pOs << " 32 nm" ;
	return pOs;
}

std::string NodeParameter::sQuantityType="{Technology}";
std::string NodeParameter::sQuantityName="NodeParameters";

}

}

}
