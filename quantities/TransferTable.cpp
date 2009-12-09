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
#include "TransferTable.h"

namespace cosi {

std::string TransferTable::sQuantityName="TransferTable";
std::string TransferTable::sQuantityType="(string,string)->string";

TransferTable::TransferTable() {
}

TransferTable::~TransferTable() {
}

TransferTable TransferTable::Get() {
	return (*this);
}

void TransferTable::Set(TransferTable pT) {
	(*this) = pT ;
	IsBot(false) ;
}

std::map< std::pair<std::string,std::string> , std::string > TransferTable::GetValue() {
	return mTable ;
}

void TransferTable::SetValue(
		std::map< std::pair<std::string,std::string> , std::string > pTable) {
	mTable = pTable ;
	IsBot(false) ;
}

void TransferTable::AddEntry(std::string pSourceName, std::string pDestName,
		std::string pPortName) {
	std::pair<std::string,std::string> Key(pSourceName, pDestName);
	mTable[Key] = pPortName ;
	IsBot(false) ;
}

bool TransferTable::HasEntry(std::string pSourceName, std::string pDestName,
		std::string pPortName) {
	std::pair<std::string,std::string> Key(pSourceName, pDestName);
	return (mTable.find(Key) != mTable.end());
}

bool TransferTable::HasEntry(std::string pSourceName, std::string pDestName){
	std::pair<std::string,std::string> Key(pSourceName, pDestName);
	return ( mTable.find(Key) != mTable.end() ) ;
}

std::string TransferTable::GetPort(std::string pSourceName, std::string pDestName){
	if ( HasEntry(pSourceName , pDestName) ) {
		std::pair<std::string,std::string> Key(pSourceName, pDestName);
		return mTable[Key] ;
	}
	return "" ;
}

TransferTable::iterator TransferTable::TransferTableBegin() {
	return mTable.begin();
}

TransferTable::iterator TransferTable::TransferTableEnd() {
	return mTable.end();
}

bool TransferTable::operator<(TransferTable pT) {
	return ( std::includes( TransferTableBegin(),TransferTableEnd(),pT.TransferTableBegin(),pT.TransferTableEnd()));
}

TransferTable TransferTable::operator+(TransferTable pT) {
	std::map< std::pair<std::string,std::string> , std::string > Table;
	std::set_union(TransferTableBegin(),TransferTableEnd(),pT.TransferTableBegin(),pT.TransferTableEnd(),std::insert_iterator<std::map< std::pair<std::string,std::string> , std::string> >(Table,Table.begin()));
	TransferTable RetVal;
	RetVal.SetValue(Table);
	return RetVal;
}

std::ostream& operator<<(std::ostream &pOs, const TransferTable &pT) {
	std::map< std::pair<std::string,std::string> , std::string > T;
	std::map< std::pair<std::string,std::string> , std::string >::iterator It;
	T = pT.mTable;
	for(It = T.begin(); It != T.end(); It++ ) {
		pOs << "(" << (It->first).first << "," <<(It->first).second << "," << It->second << ")" << std::endl;
	}
	return pOs;
}

}
