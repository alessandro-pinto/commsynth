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
#include "Quantity.h"

namespace cosi {

std::string RealQuantity::sQuantityType = "Real";
std::string RealQuantity::sQuantityName = "Real";
std::string IntQuantity::sQuantityType = "Int";
std::string StringQuantity::sQuantityType = "String";

Quantity::Quantity() {
	mAny = false;
	mBot = true;
	mOrdered = false;
}

Quantity::~Quantity() {
}

bool Quantity::IsAny() const {
	return mAny;
}
void Quantity::IsAny(bool pAny) {
	if (pAny == true) {
		IsBot(false) ;
	}
	mAny = pAny;
}
bool Quantity::IsBot() const {
	return mBot;
}
void Quantity::IsBot(bool pBot) {
	if (pBot == true) {
		IsAny(false) ;
	}
	mBot = pBot;
}
bool Quantity::IsOrdered() const {
	return mOrdered;
}
void Quantity::IsOrdered(bool pOrdered) {
	mOrdered = pOrdered;
}

RealQuantity::RealQuantity() {

}
RealQuantity::RealQuantity(double pValue) {
	mValue = pValue ;
	IsBot(false);
}
std::string RealQuantity::QuantityType() {
	return sQuantityType ;
}

RealQuantity RealQuantity::Get() {
	return (*this);
}

void RealQuantity::Set(RealQuantity pValue) {
	*this = pValue ;
}

double RealQuantity::GetValue() const {
	return mValue ;
}
void RealQuantity::SetValue(double pValue) {
	mValue = pValue ;
	IsBot(false);
}

std::ostream& operator<<(std::ostream &pOs, const RealQuantity &pL) {
	double Val;
	Val = pL.mValue;
	pOs << Val;
	return pOs;
}

IntQuantity::IntQuantity() {

}
IntQuantity::IntQuantity(int pValue) {
	mValue = pValue ;
	IsBot(false);
}
std::string IntQuantity::QuantityType() {
	return sQuantityType ;
}
int IntQuantity::GetValue() const {
	return mValue ;
}
void IntQuantity::SetValue(int pValue) {
	mValue = pValue ;
	IsBot(false);
}

StringQuantity::StringQuantity() {

}
StringQuantity::StringQuantity(std::string pValue) {
	mValue = pValue ;
	IsBot(false);
}
std::string StringQuantity::QuantityType() {
	return sQuantityType ;
}
std::string StringQuantity::GetValue() const {
	return mValue ;
}
void StringQuantity::SetValue(std::string pValue) {
	mValue = pValue ;
	IsBot(false);
}

}
