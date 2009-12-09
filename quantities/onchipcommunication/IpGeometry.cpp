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
#include "IpGeometry.h"

namespace cosi {
namespace quantity {
namespace onchipcommunication {

IpGeometry::IpGeometry() {

}

IpGeometry::~IpGeometry() {

}

IpGeometry IpGeometry::Get() {
	return (*this);
}

void IpGeometry::Set(IpGeometry pP) {
	IsBot(false);
	(*this) = pP ;
}

void IpGeometry::SetXl(RealQuantity Xl) {
	mXl = Xl ;
	IsBot(false);
}
void IpGeometry::SetXr(RealQuantity Xr) {
	mXr = Xr ;
	IsBot(false);
}
void IpGeometry::SetYt(RealQuantity Yt) {
	mYt = Yt ;
	IsBot(false);
}
void IpGeometry::SetYb(RealQuantity Yb) {
	mYb = Yb ;
	IsBot(false);
}
void IpGeometry::SetArea(RealQuantity Area) {
	mArea = Area ;
	IsBot(false);
}

RealQuantity IpGeometry::GetXl() const {
	return mXl ;
}
RealQuantity IpGeometry::GetXr() const {
	return mXr ;
}
RealQuantity IpGeometry::GetYt() const {
	return mYt ;
}
RealQuantity IpGeometry::GetYb() const {
	return mYb ;
}
RealQuantity IpGeometry::GetArea() const {
	return mArea ;
}

bool IpGeometry::operator==(const IpGeometry &pP) const {
	if ( (IsBot() && pP.IsBot()) || (IsAny() && pP.IsAny()) )
		return true ;
	return ( GetXl().GetValue() == pP.GetXl().GetValue() && GetXr().GetValue()== pP.GetXr().GetValue() && GetYb().GetValue() == pP.GetYb().GetValue() && GetYt().GetValue() == pP.GetYt().GetValue() );
}

bool IpGeometry::operator<(const IpGeometry &pP) const {
	if (IsBot() && ! pP.IsBot() )
		return true ;
	if (pP.IsAny() && ! IsAny())
		return true ;

	if ( GetXl().GetValue() > pP.GetXl().GetValue() && GetXr().GetValue()< pP.GetXr().GetValue() && GetYb().GetValue() > pP.GetYb().GetValue() && GetYt().GetValue() < pP.GetYt().GetValue() ) {
		return true ;
	}
	return false ;
}

IpGeometry IpGeometry::operator+(IpGeometry pT) {
	if ( (*this) < pT)
		return pT ;
	else
		return (*this) ;
}

IpGeometry IpGeometry::operator-(IpGeometry pT) {
	if ( (*this) < pT)
		return (*this) ;
	else
		return pT ;
}

std::ostream& operator <<(std::ostream &pOs, const IpGeometry &pP) {
	pOs << "Lower-left (" << pP.GetXl() << "," << pP.GetYb() << ")  top-right (" << pP.GetXr() << "," << pP.GetYt() << ")" << std::endl;
	return pOs;
}

std::string IpGeometry::sQuantityType="Rectangle";
std::string IpGeometry::sQuantityName="IpGeometry";

}
}
}
