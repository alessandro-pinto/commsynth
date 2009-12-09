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
#include "Position.h"

namespace cosi {

std::string Position::sQuantityName="Position";
std::string Position::sQuantityType="(Real,Real,Real)";

Position::Position() {
	mPosition.resize(3) ;
}

Position::Position(double pX, double pY, double pZ) {
	mPosition.resize(3) ;
	mPosition[0] = pX;
	mPosition[1] = pY;
	mPosition[2] = pZ;
	IsBot(false) ;
}

Position::~Position() {
}

Position Position::Get() {
	return (*this);
}

void Position::Set(Position pP) {
	IsBot(false);
	(*this) = pP ;
}

std::vector<double> Position::GetValue() {
	return mPosition ;
}

double Position::GetX() const {
	return mPosition[0];
}

double Position::GetY() const {
	return mPosition[1];
}

double Position::GetZ() const {
	return mPosition[2];
}

bool Position::operator ==(Position pP) {
	return ( GetValue() == pP.GetValue());
}

bool Position::operator<(const Position &pP) const {
	if( IsBot() & ! pP.IsBot() ) {
		return true ;
	}
	if ( !IsAny() && pP.IsAny()) {
		return true ;
	}
	return false ;
}

Position Position::operator+(Position pP) {
	Position P;
	if ( IsBot() ) {
		return pP;
	}
	if ( pP.IsBot()) {
		return Get();
	}
	if ( ! (Get() == pP.Get()) || IsAny() || pP.IsAny() ) {
		P.IsAny(true);
		return P;
	}
	//they are equal
	return pP;
}

Position Position::operator-(Position pP) {
	if ( IsAny() )
	return pP;
	if ( pP.IsAny())
	return Get();
	if ( Get() == pP.Get() ) {
		return pP;
	}
	return Position();
}

std::ostream& operator <<(std::ostream &pOs, const Position &pP) {
	pOs << "(" << pP.GetX() << "," << pP.GetY() << "," << pP.GetZ() << ")";
	return pOs;
}

double Norm1(Position P1 , Position P2) {
	double RetVal;
	RetVal = 0;
	RetVal+= fabs(P1.GetX() - P2.GetX() );
	RetVal+= fabs(P1.GetY() - P2.GetY() );
	RetVal+= fabs(P1.GetZ() - P2.GetZ() );
	return RetVal;
}

double Norm2(Position P1 , Position P2) {
	double RetVal;
	RetVal = 0;
	RetVal+= (P1.GetX() - P2.GetX())*(P1.GetX() - P2.GetX());
	RetVal+= (P1.GetY() - P2.GetY())*(P1.GetY() - P2.GetY());
	RetVal+= (P1.GetZ() - P2.GetZ())*(P1.GetZ() - P2.GetZ());
	return sqrt(RetVal);
}

}
