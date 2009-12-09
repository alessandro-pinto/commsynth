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
#ifndef PARQUETCODEGEN_H_
#define PARQUETCODEGEN_H_

#include <vector>
#include <string>
#include <fstream>
#include "float.h"
#include "environment/onchipcommunication/IpCore.h"
#include "environment/onchipcommunication/PlacedIpCore.h"
#include "Constraint.h"
#include "networks/onchipcommunication/Specification.h"
#include "networks/onchipcommunication/PlatformInstance.h"
#include "networks/onchipcommunication/Implementation.h"

namespace cosi {

namespace io {

namespace onchipcommunication {

using namespace std ;
using namespace cosi::environment::onchipcommunication ;
using namespace cosi::commstruct::onchipcommunication ;

void ParquetBlocksFile( string pFname , vector< IpCore > pIp , vector< PlacedIpCore> pPlacedIp ) ;
void ParquetNetsFile( string pFname , vector< Constraint > pConstraints , vector< PlacedIpCore> pPlacedIp) ;
void ParquetWightsFile( string pFname , vector< Constraint > pConstraints , vector< PlacedIpCore> pPlacedIp ) ;
void ParquetPlFile( string pFname , vector< IpCore > pIp , vector< PlacedIpCore> pPlacedIp ) ;

void ParquetInputGen( string pBlockFname , string pNetFname , string pWeightsFname, string pPlFname,
		Specification* pSpec ) ;

void ParquetInputGen( string pBlockFname , string pNetFname , string pWeightsFname, string pPlFname,
		PlatformInstance* pP ) ;

void ParquetInputGen( string pBlockFname , string pNetFname , string pWeightsFname, string pPlFname,
		Implementation* pI ) ;


}
}
}
#endif /*PARQUETCODEGEN_H_*/
