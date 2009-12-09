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

#include "Node.h"

namespace cosi {

Interface::Interface() {

}

Interface::~Interface() {

}

void Interface::Add(string pName, string pType) {
	(*this)[pName] = pType ;
}
void Interface::Add(string pName, string pType, int pCount) {
	stringstream S;
	for (int i = 0; i<pCount ; i++) {
		//Generate name
		S << pName << "."<< i ;
		(*this)[pName] = pType ;
		S.clear() ;
	}
}

bool Interface::HasPortType(string pType) {
	map<string,string>::iterator It;
	for (It = this->begin() ; It != this->end() ; It++) {
		if (It->second == pType)
			return true;
	}
	return false;
}

bool Interface::HasPortName(string pName) {
	map<string,string>::iterator It;
	for (It = this->begin() ; It != this->end() ; It++) {
		if ( (It->first).find( pName , 0) != string::npos )
			return true ;
	}
	return false;
}

Node::Node() {
	mName = "NA";
	mType = "NA";
}

Node::~Node() {
}

void Node::SetName(string pName) {
	mName = pName;
}

string Node::GetName() {
	return mName;
}

void Node::SetType(string pType) {
	mType = pType;
}

string Node::GetType() {
	return mType;
}

}
