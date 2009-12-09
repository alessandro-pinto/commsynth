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
#include "IpCore.h"
namespace cosi {

namespace environment {

namespace onchipcommunication {

using namespace std;

IpCore::IpCore() {
	mArea = 0.0;
	mName = "Unnamed";
}

IpCore::IpCore(string Name) {
	mName = Name ;
	mArea = 0.0;
}

IpCore::IpCore(string Name, double Area) {
	mName = Name ;
	mArea = Area ;
}

double IpCore::GetArea() {
	return mArea ;
}

void IpCore::SetArea(double Area) {
	mArea = Area ;
	//assert( mArea >= 0.0 ) ;
}

string IpCore::GetName() {
	return mName ;
}

void IpCore::SetName(string Name) {
	mName = Name ;
}

string IpCore::GetComponentName(){
	return mComponentName ;
}
void IpCore::SetComponentName(string Name){
	mComponentName = Name ;
}

}
}
}
