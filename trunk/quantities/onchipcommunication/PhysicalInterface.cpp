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
#include "PhysicalInterface.h"

namespace cosi {

namespace quantity {

namespace onchipcommunication {

PhysicalInterface::PhysicalInterface() {
	mWidth = 0;
}

PhysicalInterface::~PhysicalInterface() {
}

int PhysicalInterface::GetWidth() const {
	return mWidth ;
}

void PhysicalInterface::SetWidth(int pWidth) {
	mWidth = pWidth ;
}

double PhysicalInterface::GetSpeed() const {
	return mSpeed ;
}

void PhysicalInterface::SetSpeed(double pSpeed) {
	mSpeed = pSpeed ;
}

int PhysicalInterface::GetNumberOfVirtualChannels()const {
	return mNchannels ;
}
void PhysicalInterface::SetNumberOfVirtualChannels(int pNchannels){
	mNchannels = pNchannels ;
}
int PhysicalInterface::GetBufferLength()const {
	return mLength ;
}

void PhysicalInterface::SetBufferLength(int pLength){
	mLength = pLength ;
}

bool PhysicalInterface::operator==(const PhysicalInterface &pIf) {
	if ( mWidth == pIf.GetWidth() &&
			mSpeed == pIf.GetSpeed() &&
			mNchannels == pIf.GetNumberOfVirtualChannels() &&
			mLength == pIf.GetBufferLength()) {
		return true;
	} else {
		return false;
	}
}

std::ostream& operator<<(std::ostream &pOs, const PhysicalInterface &pP) {
	pOs << "Interface Width " << pP.GetWidth() << std::endl;
	pOs << "Interface Speed " << pP.GetSpeed();
	return pOs;
}

}

}

}
