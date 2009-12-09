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
#include "Commodity.h"

namespace cosi {

Commodity::Commodity() {

}

Commodity::~Commodity() {

}

void Commodity::Set(Commodity C) {
	IsBot(false);
	(*this) = C ;
}
Commodity Commodity::Get() {
	return (*this);
}

void Commodity::SetValue(std::string SourceName, std::string DestName,
		int SourceId, int DestId, Bandwidth AverageBandwidth, RealLatency L) {
	mSourceName = SourceName ;
	mDestName = DestName ;
	mSourceId = SourceId ;
	mDestId = DestId ;
	mAverageBandwidth = AverageBandwidth ;
	mMaximumLatency = L ;
	IsBot(false);
}

void Commodity::SetValue(std::string SourceName, std::string DestName,
		int SourceId, int DestId, double AverageBandwidthValue , double Lvalue ) {
	mSourceName = SourceName ;
	mDestName = DestName ;
	mSourceId = SourceId ;
	mDestId = DestId ;
	mAverageBandwidth.SetValue(AverageBandwidthValue );
	mMaximumLatency.SetValue(Lvalue) ;
	IsBot(false);
}

std::string Commodity::GetSourceName() const{
	return mSourceName ;
}
std::string Commodity::GetDestName() const {
	return mDestName ;
}
int Commodity::GetSourceId() const {
	return mSourceId ;
}
int Commodity::GetDestId() const {
	return mDestId ;
}

Bandwidth Commodity::GetAverageBandwidth() const {
	return mAverageBandwidth ;
}

RealLatency Commodity::GetMaximumLatency() const {
	return mMaximumLatency ;
}


double Commodity::GetAverageBandwidthValue() {
	return mAverageBandwidth.GetValue();
}

double Commodity::GetMaximumLatencyValue() {
	return mMaximumLatency.GetValue() ;
}

bool Commodity::operator<(const Commodity &pC) const {
	if ( mSourceName != pC.GetSourceName() ||
			mDestName != pC.GetDestName() ||
			mSourceId != pC.GetSourceId() ||
			mDestId != pC.GetDestId()) {
		return false;
	}

	return ( (GetAverageBandwidth() < pC.GetAverageBandwidth()) && (GetMaximumLatency() < pC.GetMaximumLatency()));
}

bool Commodity::operator==(const Commodity &pC) const {
	return ((*this)==pC);
}

Commodity Commodity::operator+(Commodity pC) {
	if ( mSourceName != pC.GetSourceName() ||
			mDestName != pC.GetDestName() ||
			mSourceId != pC.GetSourceId() ||
			mDestId != pC.GetDestId()) {
		return Commodity() ;
	}
	Commodity RetVal ;
	Bandwidth B ;
	B = mAverageBandwidth + pC.GetAverageBandwidth() ;
	RealLatency L ;
	L = mMaximumLatency + pC.GetMaximumLatency() ;
	RetVal.SetValue(mSourceName,mDestName,mSourceId,mDestId,B,L) ;
	return RetVal ;
}

std::ostream& operator <<(std::ostream &pOs, const Commodity &pC) {
	pOs << "Source " << pC.GetSourceName() << "  Destination " << pC.GetDestName() << "  Average bandwidth " << pC.GetAverageBandwidth() ;
	return pOs ;
}

std::string sQuantityName="Commodity";
std::string sQuantityType="(int,int,Bandwidth)";

}
