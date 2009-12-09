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
#include "../../include/CosiGlobal.h"

#ifndef PLACEMENTPARSER_H_
#define PLACEMENTPARSER_H_

#include <fstream>
#include <iostream>
#include <vector>
#include "float.h"
#include "math.h"
#include "../../environment/onchipcommunication/PlacedIpCore.h"
#include "quantities/onchipcommunication/IpGeometry.h"


namespace cosi {

namespace io {

namespace onchipcommunication {

using namespace std ;
using namespace cosi::environment::onchipcommunication ;
using namespace cosi::quantity::onchipcommunication ;

class PlacementParser
{
 public:
  PlacementParser( string pFname , double pSpareA ) ;
  PlacementParser( ) ;
  void Parse( ) ;

  double GetChipW( ) ;

  double GetChipH( ) ;

  vector< double > GetBottomLeftX( ) ;

  vector< double > GetBottomLeftY( ) ;

  vector< double > GetTopRightX( ) ;

  vector< double > GetTopRightY( ) ;

  vector< string > GetName( ) ;

  PlacedIpCore GetIpCore( string pCoreName ) ;

  vector<IpGeometry> GetIpGeometry() ;
  IpGeometry GetChipGeometry() ;


 private:
  string mFname ;
  vector< PlacedIpCore > mPlacedIp ;
  double mSpareA ;
  double mChipW ;
  double mChipH ;

} ;
}
}
}
#endif /*PLACEMENTPARSER_H_*/
