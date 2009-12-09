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
#include "InstantiationId.h"

namespace cosi {

namespace rules {

namespace onchipcommunication {

InstantiationId::InstantiationId(int Nip,
		std::vector< InstallationSite > InstallationSites,
		std::vector<Name> IdType) {
	mInstallationSites = InstallationSites ;
	mNip = Nip ;
	mIdType = IdType ;
	//build the inverse map
	for (unsigned int i = 0; i< mInstallationSites.size() ; i++) {
		mId[mInstallationSites[i].GetPosition()] = i ;
	}
	for( unsigned int i = 0  ; i < IdType.size() ; i++ ){
		mTypeId[IdType[i]] = i ;
	}

}

InstantiationId::~InstantiationId() {
}

int InstantiationId::Id(Name pName, Position pP) {
	//get the index of the component
	if (mTypeId.find(pName) == mTypeId.end() ) {
		CosiLog << " Component "<< pName << " not present in component table "
				<< std::endl ;
		return -1;
	}
	int C = mTypeId[pName];
	if (mId.find(pP) == mId.end() ) {
		CosiLog << " Position "<< pP << " is not an installation site, return -1"
				<< std::endl ;
		return -1 ;
	} else {
		int P = mId[pP];
		return ( C*mId.size() + P ) ;
	}
}

Name InstantiationId::GetName(int V){
	int C = V/mId.size() ;
	return mIdType[C] ;
}

Position InstantiationId::GetPosition(int V){
	if ( V == -1 ) {
		return Position() ;
	}
	int P = V%mId.size() ;
	return mInstallationSites[P].GetPosition() ;
}

}

}

}
