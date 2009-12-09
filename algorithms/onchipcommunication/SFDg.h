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
#include "include/CosiGlobal.h"
#include "stdlib.h"
#include <iostream>
#include "float.h"
#include "math.h"
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <list>
#include "networks/onchipcommunication/ChannelDependencyGraph.h"
#include "platforms/onchipcommunication/NocPlatformInterface.h"
#include "networks/onchipcommunication/Specification.h"
#include "networks/onchipcommunication/Implementation.h"
#include "QueueHandler.h"

#ifndef _SFDG_H_
#define _SFDG_H_

namespace cosi {

namespace algorithms {

namespace onchipcommunication {

class SFDg{

 public:
  SFDg( );
  ~SFDg( );
  void initialize( int Src , int Dest , double W ,
		   map< vector<int> , double > InstalledEdges ,
		   map< vector< int > , set< pair<int,int> > > EdgeCommodities ,
		   cosi::commstruct::onchipcommunication::ChannelDependencyGraph* CDG ,
		   map< int , vector<int> > InstalledNodes ,
		   cosi::commstruct::onchipcommunication::Specification* PtP ,
		   cosi::platforms::onchipcommunication::NocPlatformInterface* Plt);

  void CheckDegree( bool B ) ;

  void CheckCapacity( bool B ) ;

  void CanInstallNodes( bool B ) ;

  void DontTouchSourceDest( bool B) ;

  void InputLocked( set< int > V ) ;

  void OutputLocked( set< int > V ) ;

  void InputConstrained( set<int> V ) ;

  void OutputConstrained( set<int> V ) ;


  bool Run( );
  vector<int> GetPath( );


 private:
  double ComputeAdditionalBandwidth( set< pair<int,int> > Commodities , pair<int,int> Commodity ) ;
  double ComputeEdgeCost( int U , int V ) ;
  bool IsInstalled( int U , int V ) ;
  bool IsInstalled( int U ) ;
  double InstalledBandwidth( int U , int V ) ;
  double InputRate( int U ) ;
  double OutputRate( int U ) ;
  bool CyclicDependency( int U , int V ) ;
  vector< int > mPi ;
  map< vector<int> , double > mInstalledEdges ;
  map< vector< int > , set< pair<int,int> > > mEdgeCommodities ;
  map< int , vector<int> > mInstalledNodes ;
  cosi::commstruct::onchipcommunication::ChannelDependencyGraph* mCDG ;
  set<pair<int,int> > mDependencySet ;
  cosi::platforms::onchipcommunication::NocPlatformInterface* mPlt ;
  cosi::commstruct::onchipcommunication::Specification* mPtP ;
  int mSize ;
  int is_initialized ;
  int has_run;
  int mSrc ;
  int mDest ;
  set<int> mInputLocked ;
  set<int> mInputConstrained ;
  set<int> mOutputLocked ;
  set<int> mOutputConstrained ;
  double mW ;
  bool mCheckDegree ;
  bool mCheckCapacity ;
  bool mCanInstallNodes ;
  bool mSourceDestDontTouch ;

};

}
}
}
#endif
