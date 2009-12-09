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
#ifndef INSTANTIATIONID_H_
#define INSTANTIATIONID_H_

#include <vector>
#include <map>
#include "quantities/Name.h"
#include "quantities/Position.h"
#include "environment/onchipcommunication/InstallationSite.h"
#include "include/CosiGlobal.h"

namespace cosi {

namespace rules {

namespace onchipcommunication {

using namespace cosi::environment::onchipcommunication ;
using namespace std ;

class InstantiationId {

public:

	InstantiationId(int Nip, std::vector< InstallationSite > InstallationSites,
			std::vector<Name> IdType);

	virtual ~InstantiationId();

	int Id(Name pName, Position pP);

	Name GetName( int V ) ;

	Position GetPosition( int V ) ;


private:
	std::vector< InstallationSite > mInstallationSites;
	std::map<Position,int> mId;
	int mNip;
	std::vector<Name> mIdType;
	std::map<Name,int> mTypeId;

};

}

}

}

#endif /*INSTANTIATIONID_H_*/
