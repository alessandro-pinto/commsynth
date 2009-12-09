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

std::string Ports::sQuantityType = "std::string->std::string";
std::string Ports::sQuantityName = "Ports";
std::string PortRates::sQuantityType = "std::string->Bandwidth";
std::string PortRates::sQuantityName = "PortRates";
std::string PortCapacities::sQuantityType = "std::string->Bandwidth";
std::string PortCapacities::sQuantityName = "PortCapacities";

PortInterface::PortInterface() {

}
PortInterface::~PortInterface() {

}
PortInterface::tDirection PortInterface::GetDirection() const {
	return mDir;
}
void PortInterface::SetDirection(PortInterface::tDirection pDir) {
	mDir = pDir;
}
std::string PortInterface::GetName() const {
	return mName;
}
void PortInterface::SetName(std::string pName) {
	mName = pName;
}

bool PortInterface::operator==(const PortInterface &pP) const {
	return ( mName == pP.GetName() && mDir == pP.GetDirection());
}

bool PortInterface::operator!=(const PortInterface &pP) const {
	return ( !( (*this) == pP ));
}

std::ostream& operator<<(std::ostream &pOs , const PortInterface &pP) {
	pOs << pP.mName << " ";
	if ( pP.mDir == PortInterface::IN ) {
		pOs << "IN";
	} else if (pP.mDir == PortInterface::OUT ) {
		pOs << "OUT";
	} else if (pP.mDir == PortInterface::INOUT ) {
		pOs << "INOUT";
	}
	return pOs;
}

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

std::map<std::string,PortInterface> Ports::GetValue() {
	return mPorts;
}

void Ports::AddPort(std::string pName, std::string pType,PortInterface::tDirection pDir) {
	PortInterface If;
	If.SetDirection(pDir);
	If.SetName(pType);
	mPorts[pName] = If;
	IsBot(false);
}

void Ports::AddPort(std::string pName, PortInterface pIf) {
	mPorts[pName] = pIf;
	IsBot(false);
}

bool Ports::HasPortType(std::string pType) {
	std::map< std::string , PortInterface >::iterator It;
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

bool Ports::IsBound(std::string pName) {
	return (mBound.find(pName)!=mBound.end());
}
Edge Ports::BoundTo(std::string pName) {
	return mBound[pName];
}

void Ports::BoundTo(std::string pName, Edge pE) {
	if ( IsBound(pName) ) {
		if ( !(BoundTo(pName) == pE))
		std::cout << "WARNING : port " << pName << " already bound" << endl;
	}
	mBound[pName] = pE;
}

string Ports::IsBoundTo(Edge pE, PortInterface::tDirection pDir) {
	iterator It;
	for (It = Begin() ; It!= End() ; It++) {
		if ( If(It).GetDirection() == pDir && BoundTo(Name(It)) == pE ) {
			return Name(It) ;
		}
	}
	return string("") ;
}

std::vector<std::string> Ports::GetNotBoundIn() {
	iterator It;
	std::vector<std::string> RetVal;
	for (It = Begin(); It != End(); It++) {
		if ( (!IsBound(Name(It)) ) && (If(It).GetDirection() == PortInterface::IN )) {
			RetVal.push_back(Name(It));
		}
	}
	return RetVal;
}

std::vector<std::string> Ports::GetNotBoundOut() {
	iterator It;
	std::vector<std::string> RetVal;
	for (It = Begin(); It != End(); It++) {
		if ((!IsBound(Name(It))) && (If(It).GetDirection() == PortInterface::OUT)) {
			RetVal.push_back(Name(It));
		}
	}
	return RetVal;
}

std::vector<std::string> Ports::GetNotBoundInOut() {
	iterator It;
	std::vector<std::string> RetVal;
	for (It = Begin(); It != End(); It++) {
		if ( (!IsBound(Name(It))) && (If(It).GetDirection() == PortInterface::INOUT)) {
			RetVal.push_back(Name(It));
		}
	}
	return RetVal;
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

std::string Ports::GetInputOutputPortNotBound() {
	std::vector<std::string> InOut = GetNotBoundInOut();
	if (InOut.size() == 0) {
		return "";
	} else {
		return InOut[0];
	}
}


Ports::iterator Ports::Begin() {
	return mPorts.begin();
}
Ports::iterator Ports::End() {
	return mPorts.end();
}
std::string Ports::Name(iterator It) {
	return It->first;
}

PortInterface Ports::If(iterator It) {
	return It->second;
}

Ports Ports::operator+( Ports pP) {
	//take union of ports
	//conditions: ports with the same name must have the same interface
	//a port can be bound to one edge only.
	Ports RetVal;
	std::map<std::string,PortInterface> P = GetValue();
	std::map<std::string,PortInterface>::iterator It;
	for(It = P.begin(); It != P.end();It++) {
		if ( pP.HasPort(It->first) && It->second == pP[It->first]) {
			RetVal.AddPort(It->first,It->second);
			if ( this->IsBound(It->first)) {
				RetVal.BoundTo(It->first,BoundTo(It->first));
			}
			if ( pP.IsBound(It->first)) {
				RetVal.BoundTo(It->first,pP.BoundTo(It->first));
			}
		}
		else if ( pP.HasPort(It->first) && It->second != pP[It->first] ) {
			return Ports();
		} else if ( ! pP.HasPort(It->first) ) {
			RetVal.AddPort(It->first,It->second);
			if ( this->IsBound(It->first)) {
				RetVal.BoundTo(It->first,BoundTo(It->first));
			}
		}
	}

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
	std::map<std::string,PortInterface> P = GetValue();
	std::map<std::string,PortInterface>::iterator It;
	for(It = P.begin(); It != P.end();It++) {
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

PortInterface Ports::operator[](std::string pName) {
	if (HasPort(pName)) {
		return mPorts[pName];
	}
	return PortInterface();
}

bool Ports::operator==(Ports pP) {
	return (GetValue() == pP.GetValue());
}

PortRates::PortRates() {
}

PortRates::~PortRates() {
}

PortRates PortRates::Get() {
	return (*this);
}

void PortRates::Set(PortRates pP) {
	(*this) = pP;
}

std::map<std::string,Bandwidth> PortRates::GetValue() {
	return mPortRates;
}

void PortRates::AddRate(std::string pName, Bandwidth pB) {
	mPortRates[pName] = pB;
}
void PortRates::AddRate(std::string pName, double pValue) {
	mPortRates[pName] = Bandwidth(pValue);
}

Bandwidth PortRates::GetRate(std::string pName) {
	return mPortRates[pName];
}

bool PortRates::HasPort(std::string pName) {
	return (mPortRates.find(pName) != mPortRates.end());
}

bool PortRates::operator==(PortRates pP) {
	return (GetValue() == pP.GetValue());
}

PortRates PortRates::operator+(PortRates pP) {

	PortRates RetVal;

	std::map<std::string,Bandwidth> P = GetValue();
	std::map<std::string,Bandwidth>::iterator It;
	for(It = P.begin(); It != P.end();It++) {
		if ( pP.HasPort(It->first) ) {
			RetVal.AddRate(It->first,It->second + pP.GetRate(It->first));
		}
		else {
			RetVal.AddRate(It->first,It->second);
		}
	}

	P = pP.GetValue();
	for(It = P.begin(); It != P.end();It++) {
		if ( ! HasPort(It->first) ) {
			RetVal.AddRate(It->first,It->second);
		}
	}
	return RetVal;

}

void PortRates::operator <<(PortRates pP) {
	(*this) = pP;
}

PortCapacities::PortCapacities() {

}

PortCapacities::~PortCapacities() {

}

PortCapacities PortCapacities::Get() {
	return (*this);
}

PortRates PortCapacities::operator+(PortRates pP) {

	PortRates RetVal;
	std::map<std::string,Bandwidth> P = GetValue();
	std::map<std::string,Bandwidth>::iterator It;
	for(It = P.begin(); It != P.end();It++) {
		if ( pP.HasPort(It->first) ) {
			RetVal.AddRate(It->first,It->second + pP.GetRate(It->first));
		}
		else {
			RetVal.AddRate(It->first,It->second);
		}
	}

	P = pP.GetValue();
	for(It = P.begin(); It != P.end();It++) {
		if ( ! HasPort(It->first) ) {
			RetVal.AddRate(It->first,It->second);
		}
	}
	return RetVal;
}

std::ostream& operator<<(std::ostream &pOs , const Ports &pP) {
	std::map<std::string,PortInterface> P;
	std::map<std::string,Edge> B;
	P = pP.mPorts;
	B = pP.mBound;
	std::map<std::string,PortInterface>::iterator It;
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
