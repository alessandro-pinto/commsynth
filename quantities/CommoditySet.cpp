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
#include "CommoditySet.h"

namespace cosi {

CommoditySet::CommoditySet() {

}

CommoditySet::~CommoditySet() {

}

CommoditySet::iterator CommoditySet::Begin() const {
	return mCommoditySet.begin();
}

CommoditySet::iterator CommoditySet::End() const {
	return mCommoditySet.end() ;
}

int CommoditySet::Size() {
	return mCommoditySet.size();
}

CommoditySet CommoditySet::Get() const {
	return (*this);
}

void CommoditySet::Set(CommoditySet pSet) {
	IsBot(false);
	if (IsAny() )
		IsAny(false) ;
	(*this) = pSet ;
}

std::set<Commodity> CommoditySet::GetValue() const {
	return mCommoditySet ;
}

void CommoditySet::SetValue(std::set<Commodity> pSet) {
	IsBot(false);
	if (IsAny() )
		IsAny(false) ;
	mCommoditySet = pSet ;
}

double CommoditySet::GetAggregateBandwidthValue() {
	return (GetAggregateBandwidth().GetValue());
}

Bandwidth CommoditySet::GetAggregateBandwidth() {
	iterator It;
	Bandwidth B(0);
	for (It = Begin() ; It != End() ; It++) {
		B = B + It->GetAverageBandwidth();
	}
	return B ;

}

void CommoditySet::Add(Commodity pC) {
	IsBot(false) ;
	if (IsAny() )
		IsAny(false) ;
	mCommoditySet.insert(pC) ;
}

bool CommoditySet::operator<(const CommoditySet &pSet) const {
	//containement
	return (std::includes(pSet.Begin() , pSet.End(),Begin(),End()));
}

bool CommoditySet::operator==(const CommoditySet &pSet) const {
	return ( pSet.GetValue() == GetValue() );
}

CommoditySet CommoditySet::operator+(CommoditySet pSet) {
	//take union
	std::set<Commodity> S;
	S.insert(mCommoditySet.begin(),mCommoditySet.end());
	S.insert(pSet.Begin(),pSet.End());
	CommoditySet RetVal;
	RetVal.SetValue(S);
	return RetVal;
}

std::ostream& operator <<(std::ostream &pOs, const CommoditySet &pC) {
	std::set<Commodity> S = pC.GetValue();
	std::set<Commodity>::iterator It;
	for(It = S.begin(); It != S.end(); It++ ) {
		pOs << *It << std::endl;
	}
	return pOs;
}

std::string CommoditySet::sQuantityName="CommoditySet";
std::string CommoditySet::sQuantityType="Set of Commodity";

}
