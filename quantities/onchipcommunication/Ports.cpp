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
#include "Ports.h"

namespace cosi {

namespace quantity {

namespace onchipcommunication {

std::string Ports::sQuantityType = "std::string->std::string";
std::string Ports::sQuantityName = "Ports";

Ports::Ports() {
}

Ports::~Ports() {
}

Ports Ports::Get() {
	return (*this);
}

void Ports::Set(Ports pP) {
	(*this) = pP;
	IsBot(false);
}

std::map<std::string,Interface> Ports::GetValue() {
	return mPorts;
}

void Ports::AddPort(std::string pName, std::string pType,
		Interface::tDirection pDir) {
	Interface If;
	If.SetDirection(pDir);
	If.SetName(pType);
	mPorts[pName] = If;
	IsBot(false);
}

void Ports::AddPort(std::string pName, Interface pIf) {
	mPorts[pName] = pIf;
	IsBot(false);
}

bool Ports::HasPortType(std::string pType) {
	std::map< std::string , Interface >::iterator It;
	for (It=mPorts.begin(); It!=mPorts.end(); It++) {
		if ((It->second).GetName() == pType) {
			return true;
		}
	}
	return false;
}

bool Ports::HasPort(std::string pName) {
	return (mPorts.find(pName) != mPorts.end());
}

Ports::iterator Ports::Begin() {
	return mPorts.begin() ;
}
Ports::iterator Ports::End() {
	return mPorts.end();
}
std::string Ports::Name(iterator It) {
	return It->first;
}

Interface Ports::If(iterator It) {
	return It->second;
}

bool Ports::IsBound(std::string pName) {
	return (mBound.find(pName)!=mBound.end());
}

void Ports::UnBound( string pName) {
	mBound.erase(pName) ;
}

Edge Ports::BoundTo(std::string pName) {
	return mBound[pName];
}

string Ports::IsBoundTo(Edge pE, Interface::tDirection pDir) {
	iterator It;
	for (It = Begin() ; It!= End() ; It++) {
		if ( If(It).GetDirection() == pDir && BoundTo(Name(It)) == pE ) {
			return Name(It) ;
		}
	}
	return string("") ;
}

void Ports::BoundTo(std::string pName, Edge pE) {
	if (IsBound(pName) ) {
		if ( !(BoundTo(pName) == pE))
			CosiLog << "WARNING : port "<< pName << " already bound"<< endl;
	}
	mBound[pName] = pE;
}

std::vector<std::string> Ports::GetNotBoundIn() {
	iterator It;
	std::vector<std::string> RetVal;
	for (It = Begin() ; It != End() ; It++) {
		if ( (!IsBound(Name(It)) ) && (If(It).GetDirection() == Interface::IN )) {
			RetVal.push_back(Name(It)) ;
		}
	}
	return RetVal ;
}

std::vector<std::string> Ports::GetNotBoundOut() {
	iterator It;
	std::vector<std::string> RetVal;
	for (It = Begin() ; It != End() ; It++) {
		if ((!IsBound(Name(It))) && (If(It).GetDirection() == Interface::OUT)) {
			RetVal.push_back(Name(It)) ;
		}
	}
	return RetVal ;
}

std::vector<std::string> Ports::GetNotBoundInOut() {
	iterator It;
	std::vector<std::string> RetVal;
	for (It = Begin() ; It != End() ; It++) {
		if ( (!IsBound(Name(It))) && (If(It).GetDirection() == Interface::INOUT)) {
			RetVal.push_back(Name(It)) ;
		}
	}
	return RetVal ;
}

std::string Ports::GetInputPortNotBound() {
	std::vector<std::string> In = GetNotBoundIn();
	if (In.size() == 0) {
		return "";
	} else {
		return In[0];
	}
}
std::string Ports::GetOutputPortNotBound() {
	std::vector<std::string> Out = GetNotBoundOut();
	if (Out.size() == 0) {
		return "";
	} else {
		return Out[0];
	}
}

Ports Ports::operator+( Ports pP) {
	//take union of ports
	//conditions: ports with the same name must have the same interface
	//a port can be bound to one edge only.
	Ports RetVal;

	//Get the set of ports of the right hand side
	std::map<std::string,Interface> P = GetValue();
	std::map<std::string,Interface>::iterator It;
	for(It = P.begin(); It != P.end();It++) {
		//If the right hand side also has the same port width the same interface
		if ( pP.HasPort(It->first) && It->second == pP[It->first]) {
			//add the interface to the set of ports to return
			RetVal.AddPort(It->first,It->second);
			//Bound the port to the edge (notice that if the two ports
			//are bound to two different edges, the user is notified but the
			//port will be bounded to the edge of the right hand side
			if ( this->IsBound(It->first)) {
				RetVal.BoundTo(It->first,BoundTo(It->first));
			}
			if ( pP.IsBound(It->first)) {
				RetVal.BoundTo(It->first,pP.BoundTo(It->first));
			}
		}
		//if the interafaces are different then return the empty set of ports
		//this should be fixed by returning the empty interface instead
		else if ( pP.HasPort(It->first) && It->second != pP[It->first] ) {
			return Ports();
		}
		//if the right hand side does not have this port
		//add the port to the return value directly
		else if ( ! pP.HasPort(It->first) ) {
			RetVal.AddPort(It->first,It->second);
			if ( this->IsBound(It->first)) {
				RetVal.BoundTo(It->first,BoundTo(It->first));
			}
		}
	}

	//add all the ports in the right hand side that are not
	//in the left hand side
	P = pP.GetValue();
	for(It = P.begin(); It != P.end();It++) {
		if ( ! HasPort(It->first) ) {
			RetVal.AddPort(It->first,It->second);
			if ( pP.IsBound(It->first)) {
				RetVal.BoundTo(It->first,pP.BoundTo(It->first));
			}
		}
	}
	return RetVal;
}

Ports Ports::operator-( Ports pP) {
	//Return the intersection
	Ports RetVal;
	//Get the set of ports of the left hand side
	std::map<std::string,Interface> P = GetValue();
	std::map<std::string,Interface>::iterator It;
	for(It = P.begin(); It != P.end();It++) {
		//if the port is in also in the right hand side and
		//if the interface is the same
		//add the port to the return value
		if ( pP.HasPort(It->first) && It->second == pP[It->first]) {
			RetVal.AddPort(It->first,It->second);
			//if are both bound to the same edge then bound it
			//otherwise don't
			if ( IsBound(It->first) && pP.IsBound(It->first) && (BoundTo(It->first) == pP.BoundTo(It->first))) {
				RetVal.BoundTo(It->first,BoundTo(It->first) );
			}
		}
	}
	return RetVal;
}

Interface& Ports::operator[](std::string pName) {
	if (HasPort(pName)) {
		return mPorts[pName];
	}
	Interface If;
	return If;
}

bool Ports::operator==(Ports pP) {
	return (GetValue() == pP.GetValue());
}

std::ostream& operator<<(std::ostream &pOs , const Ports &pP) {
	std::map<std::string,Interface> P;
	std::map<std::string,Edge> B;
	P = pP.mPorts;
	B = pP.mBound;
	std::map<std::string,Interface>::iterator It;
	for(It = P.begin(); It != P.end(); It++) {
		pOs << "Port " << It->first << " with interface " << It->second;
		if ( B.find(It->first) == B.end()) {
			pOs << " not bound " << std::endl;
		} else {
			pOs << " bound to " << B[It->first] << std::endl;
		}
	}
	return pOs;
}

}
}
}
