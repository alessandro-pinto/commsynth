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
#include "Thread.h"

namespace cosi {

Thread::Thread() {
}

Thread::~Thread() {
}

Thread Thread::Get() {
	return (*this);
}

void Thread::Set(Thread pT) {
	(*this) = pT ;
	IsBot(false) ;
}

std::string Thread::GetThreadName() const {
	return mThreadName ;
}

void Thread::SetThreadName(std::string pName) {
	mThreadName = pName ;
	IsBot(false) ;
}

std::string Thread::GetDestinationThreadName() const {
	return mDestinationThreadName ;
}

void Thread::SetDestinationThreadName(std::string pName) {
	mDestinationThreadName = pName ;
	IsBot(false) ;
}

std::string Thread::GetDestinationNodeName()  const {
	return mDestinationNodeName ;
}

void Thread::SetDestinationNodeName(std::string pName) {
	mDestinationNodeName = pName ;
	IsBot(false) ;
}

int Thread::GetDestinationNodeId() const {
	return mDestId ;
}

void Thread::SetDestinationNodeId(int pId) {
	mDestId = pId ;
	IsBot(false) ;
}

double Thread::GetPeriod() const {
	return mPeriod ;
}

void Thread::SetPeriod(double pPeriod) {
	mPeriod = pPeriod ;
	IsBot(false) ;
}

int Thread::GetMessageLength() const {
	return mMsgLength ;
}

void Thread::SetMessageLength(int pMessageLength) {
	mMsgLength = pMessageLength ;
	IsBot(false) ;
}

Thread& Thread::operator=(const Thread& pT) {
	mThreadName = pT.GetThreadName();
	mDestinationNodeName = pT.GetDestinationNodeName();
	mDestId = pT.GetDestinationNodeId();
	mDestinationThreadName = pT.GetDestinationThreadName();
	mPeriod = pT.GetPeriod();
	mMsgLength = pT.GetMessageLength();
	IsBot(false) ;
	return *this ;
}

bool Thread::operator<(const Thread &pT) const {
	return (mThreadName < pT.GetThreadName());
}

bool Thread::operator==(const Thread &pT) const {
	return (mThreadName == pT.GetThreadName());
}

std::ostream& operator<<(std::ostream &pOs , const Thread &pT) {
	pOs << "Thread " << pT.mThreadName << std::endl ;
	pOs << "Destination Thread " << pT.mDestinationThreadName << " on node " << pT.mDestinationNodeName << " with id " << pT.mDestId << std::endl ;
 	pOs << "Period = " << pT.mPeriod << " s   Message = " << pT.mMsgLength << " bits" << std::endl ;
	return pOs ;
}

}
