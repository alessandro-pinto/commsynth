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
#include "NodeLinkLibrary.h"

namespace cosi {

NodeLinkLibrary::NodeLinkLibrary() {
}

NodeLinkLibrary::NodeLinkLibrary(string pName) {
	mName = pName ;
}

NodeLinkLibrary::~NodeLinkLibrary() {
}

string NodeLinkLibrary::GetName() {
	return mName ;
}

void NodeLinkLibrary::SetName(string pName) {
	mName = pName ;
}

void NodeLinkLibrary::AddNode(Node pN) {
	mNodes[ pN.GetName()] = pN ;
}

void NodeLinkLibrary::AddLink(Link pL) {
	mLinks[pL.GetName()] = pL ;
}

void NodeLinkLibrary::AddBiLink(string pName, Link pL1, Link pL2) {
	BiLink L;
	L.L1(pL1 ) ;
	L.L2(pL2 ) ;
	L.SetName(pName ) ;
	mBiLinks[pName] = L ;
}

void NodeLinkLibrary::AddBiLink(BiLink pL ) {
	mBiLinks[pL.GetName()] = pL ;
}



}
