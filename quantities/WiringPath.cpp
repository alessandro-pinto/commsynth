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
#include "WiringPath.h"

namespace cosi {

std::string WiringPath::sQuantityName = "WiringPath";
std::string WiringPath::sQuantityType = "List of positions";

WiringPath::WiringPath() {
}

WiringPath::~WiringPath() {
}

void WiringPath::AppendPosition(Position pP) {
	mPath.push_back(pP) ;
	IsBot(false) ;
}
void WiringPath::PrependPosition(Position pP) {
	mPath.push_front(pP) ;
	IsBot(false) ;
}

std::list<Position> WiringPath::GetValue() {
	return mPath ;
}

void WiringPath::SetValue(std::list<Position> pPath) {
	mPath = pPath ;
	IsBot(false) ;
}

WiringPath WiringPath::Get() {
	return (*this);
}

void WiringPath::Set(WiringPath pPath) {
	(*this) = pPath ;
	IsBot(false) ;
}

WiringPath::iterator WiringPath::PathBegin() {
	return mPath.begin();
}

WiringPath::iterator WiringPath::PathEnd() {
	return mPath.end();
}

Position WiringPath::First() {
	return mPath.front();
}
Position WiringPath::Last() {
	return mPath.back();
}

double WiringPath::Length() {
	iterator Current, Next;
	Current = PathBegin() ;
	Next = PathBegin() ;
	Next++;
	double RetVal = 0;
	for (; Next != PathEnd() ; Next++) {
		RetVal+= Norm1(*Current, *Next) ;
		Current++;
	}
	return RetVal ;
}

bool WiringPath::operator<(WiringPath pPath) {
	if ( ! (First() == pPath.First()) || !(Last() == pPath.Last()) ) {
		return false;
	} else {
		return (Length() > pPath.Length());
	}
}

bool WiringPath::operator>(WiringPath pPath) {
	if ( ! (First() == pPath.First()) || !(Last() == pPath.Last()) ) {
		return false;
	} else {
		return (Length() < pPath.Length());
	}
}

WiringPath WiringPath::operator+(WiringPath pPath) {
	if (IsAny()) {
		return Get();
	}
	if ( pPath.IsAny() ) {
		return pPath;
	}
	if ( IsBot() ) {
		return pPath;
	}
	if ( pPath.IsBot() ) {
		return Get();
	}
	if ( ! (First() == pPath.First()) || !(Last() == pPath.Last()) ) {
		WiringPath P;
		P.IsAny(true);
		return P;
	}
	if ( pPath > (*this)) {
		return pPath;
	} else {
		return (*this);
	}
}

WiringPath WiringPath::operator-(WiringPath pPath) {
	if (IsAny()) {
		return pPath;
	}
	if ( pPath.IsAny() ) {
		return Get();
	}
	if ( IsBot() ) {
		return Get();
	}
	if ( pPath.IsBot() ) {
		return pPath;
	}
	if ( ! (First() == pPath.First()) || !(Last() == pPath.Last()) ) {
		WiringPath P;
		return P;
	}
	if ( pPath < (*this)) {
		return pPath;
	} else {
		return (*this);
	}
}

std::ostream& operator <<(std::ostream &pOs , const WiringPath &pP) {
	std::list<Position> P;
	std::list<Position>::iterator It;
	P = pP.mPath;
	for( It = P.begin(); It != P.end(); It++ ) {
		pOs << (*It) << "->";
	}
	return pOs;
}

}
