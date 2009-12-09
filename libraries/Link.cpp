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
#include "libraries/Link.h"

namespace cosi {

namespace library {

Link::Link() {
	mDirected = true;
	mName = "Unknown";
	mType = "Unknown";
}

Link::~Link() {
}

void Link::SetName(std::string pS) {
	mName = pS ;
}

std::string Link::GetName() {
	return mName ;
}

void Link::SetType(std::string pS) {
	mType = pS ;
}

std::string Link::GetType() {
	return mType ;
}

void Link::SetSource(Name pS) {
	mSource = pS ;
}

Name Link::GetSource() {
	return mSource ;
}

void Link::SetDestination(Name pS) {
	mDest = pS ;

}
Name Link::GetDestination() {
	return mDest ;
}

void Link::SetSourceInterface(PortInterface pS) {
	mSourceInterface = pS ;
}

PortInterface Link::GetSourceInterface() {
	return mSourceInterface ;
}

void Link::SetDestinationInterface(PortInterface pS) {
	mDestInterface = pS ;

}

PortInterface Link::GetDestinationInterface() {
	return mDestInterface ;
}

bool Link::Directed(){
	return mDirected ;
}
void Link::Directed(bool pD){
	mDirected = pD ;
}

}
}
