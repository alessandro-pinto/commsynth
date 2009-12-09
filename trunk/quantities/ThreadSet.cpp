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
#include "ThreadSet.h"

namespace cosi {

std::string ThreadSet::sQuantityName="ThreadSet";
std::string ThreadSet::sQuantityType="Set of threads";

ThreadSet::ThreadSet() {
}

ThreadSet::~ThreadSet() {
}

ThreadSet ThreadSet::Get() {
	return (*this);
}

void ThreadSet::Set(ThreadSet pSet) {
	(*this) = pSet ;
	IsBot(false) ;
}

std::set<Thread> ThreadSet::GetValue() {
	return mSet ;
}

void ThreadSet::SetValue(std::set<Thread> pSet) {
	mSet = pSet ;
	IsBot(false) ;
}

void ThreadSet::AddThread(Thread pT) {
	mSet.insert(pT) ;
	IsBot(false) ;
}

int ThreadSet::Size() {
	return mSet.size( ) ;
}

Thread ThreadSet::GetLongestMessage() {

	if ( IsBot() ) {
		Thread T ;
		T.SetMessageLength(0) ;
		return T ;
	}

	iterator It , Longest ;
	int MaxLength = 0 ;
	for( It = ThreadSetBegin() ; It != ThreadSetEnd(); It++) {
		if ( It->GetMessageLength() >= MaxLength ) {
			Longest = It ;
			MaxLength = It->GetMessageLength() ;
		}
	}
	return (*Longest) ;
}

ThreadSet::iterator ThreadSet::ThreadSetBegin()  {
	return mSet.begin() ;
}

ThreadSet::iterator ThreadSet::ThreadSetEnd() {
	return mSet.end() ;
}

bool ThreadSet::operator<(ThreadSet pSet) {
	//check containement
	if ( std::includes(pSet.ThreadSetBegin(),pSet.ThreadSetEnd(),ThreadSetBegin(),ThreadSetEnd()) ) {
		return true ;
	} else {
		return false;
	}
}

bool ThreadSet::operator==(ThreadSet pSet) {
	return (mSet == pSet.GetValue());
}

ThreadSet ThreadSet::operator+(ThreadSet pSet) {
	//take union
	std::set<Thread> RetVal ;
	std::set_union(pSet.ThreadSetBegin(),pSet.ThreadSetEnd(),ThreadSetBegin(),ThreadSetEnd(),std::insert_iterator<std::set<Thread> >(RetVal,RetVal.begin())) ;
	ThreadSet T ;
	T.SetValue(RetVal);
	return T ;
}

std::ostream& operator <<(std::ostream &pOs , const ThreadSet &pT) {
	std::set<Thread>::iterator It ;
	for(It=pT.mSet.begin() ; It!=pT.mSet.end() ; It++) {
		pOs << (*It) ;
	}
	return pOs ;
}

}
