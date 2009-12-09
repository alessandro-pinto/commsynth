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

#include "Xyz.h"

namespace cosi
{

Xyz::Xyz()
{
	mX = mY = mZ = 0;
}

Xyz::Xyz(double pX)
{
	mX = pX ;
	mY = mZ = 0 ;
}

Xyz::Xyz(double pX , double pY)
{
	mX = pX ;
	mY = pY ;
	mZ = 0;
}

Xyz::Xyz(double pX , double pY,double pZ)
{
	mX = pX ;
	mY = pY ;
	mZ = pZ ;
}

Xyz::~Xyz()
{
}

void Xyz::SetX(double pX) {
	mX = pX ;
}

void Xyz::SetY(double pY) {
	mY = pY ;
}

void Xyz::SetZ(double pZ) {
	mZ = pZ ;
}

double Xyz::GetX() {
	return mX ;
}

double Xyz::GetY() {
	return mY ;
}

double Xyz::GetZ() {
	return mZ ;
}

string Xyz::Print( ) {
	stringstream s ;
	s << "X = " << mX << ", Y = " << mY << ", Z = " << mZ << endl ;
	return s.str( ) ;
}

}
