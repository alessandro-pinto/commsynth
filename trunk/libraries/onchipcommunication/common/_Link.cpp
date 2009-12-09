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
#include "_Link.h"

namespace cosi
{

namespace library
{

namespace onchipcommunication
{

_Link::_Link() {
	mDirected = true;
}

_Link::~_Link() {
}

void _Link::SetSource(Name pS) {
	mSource = pS ;
}

Name _Link::GetSource() {
	return mSource ;
}

void _Link::SetDestination(Name pS) {
	mDest = pS ;

}
Name _Link::GetDestination() {
	return mDest ;
}

void _Link::SetSourceInterface(cosi::quantity::onchipcommunication::Interface pS) {
	mSourceInterface = pS ;
}

cosi::quantity::onchipcommunication::Interface _Link::GetSourceInterface() {
	return mSourceInterface ;
}

void _Link::SetDestinationInterface(cosi::quantity::onchipcommunication::Interface pS) {
	mDestInterface = pS ;

}

cosi::quantity::onchipcommunication::Interface _Link::GetDestinationInterface() {
	return mDestInterface ;
}

bool _Link::Directed(){
	return mDirected ;
}
void _Link::Directed(bool pD){
	mDirected = pD ;
}



}

}

}
