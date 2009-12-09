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
#include "Link.h"

namespace cosi {

Link::Link() {
}

Link::~Link() {
}

void Link::SetName(string pS) {
	mName = pS ;
}
string Link::GetName() {
	return mName ;
}
void Link::SetType(string pS) {
	mType = pS ;
}
string Link::GetType() {
	return mType ;
}
void Link::SetSource(string pS) {
	mSource = pS ;
}
string Link::GetSource() {
	return mSource ;
}
void Link::SetDestination(string pS) {
	mDest = pS ;
}
string Link::GetDestination() {
	return mDest;
}
void Link::SetSourceInterface(string pS) {
	mSourceInterface = pS ;
}
string Link::GetSourceInterface() {
	return mSourceInterface ;
}
void Link::SetDestinationInterface(string pS) {
	mDestInterface = pS ;
}
string Link::GetDestinationInterface() {
	return mDestInterface ;
}

BiLink::BiLink() {

}

BiLink::~BiLink() {

}

void BiLink::SetName(string pName) {
	mName = pName ;
}
string BiLink::GetName() {
	return mName ;
}

void BiLink::SetType(string pType) {
	mType = pType ;
}
string BiLink::GetType() {
	return mType ;
}

void BiLink::L1(Link pL) {
	this->first = pL ;
}
void BiLink::L2(Link pL) {
	this->second = pL ;
}
Link BiLink::L1() {
	return this->first ;
}
Link BiLink::L2() {
	return this->second ;
}

}
