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
#ifndef _NETWORK_COST_H_
#define _NETWORK_COST_H_

#include <map>
#include "float.h"
#include "networks/onchipcommunication/Specification.h"
#include "networks/onchipcommunication/ChannelDependencyGraph.h"
#include "networks/onchipcommunication/Implementation.h"
#include "platforms/onchipcommunication/NocPlatformInterface.h"
#include "platforms/onchipcommunication/Platform.h"

namespace cosi {

namespace algorithms {

namespace onchipcommunication {

class NetworkCost {
 public:
  NetworkCost( ) ;
  ~NetworkCost( ) ;

  bool Verify( cosi::commstruct::onchipcommunication::Specification* Spec ) ;
  void Initialize(cosi::commstruct::onchipcommunication::Implementation* I ,
		  cosi::platforms::onchipcommunication::NocPlatformInterface* PltIf ,
		  cosi::platforms::onchipcommunication::Platform* Plt) ;

  void RunApprox( ) ;
  void Run();


  double GetWireStaticPower( ) ;
  double GetWireDynamicPower( ) ;
  double GetWireTotalPower( ) ;
  double GetRouterStaticPower( ) ;
  double GetRouterDynamicPower( ) ;
  double GetRouterTotalPower( ) ;
  double GetTotalPower( ) ;
  double GetWireArea( ) ;
  double GetWireDeviceArea( ) ;
  double GetWireWiringArea( ) ;
  double GetRouterArea( ) ;
  double GetArea( ) ;

  double GetMaxDist( ) { return mMaxDist ; };
  double GetMinDist( ) { return mMinDist; };
  double GetTotalDist( ) { return mTotalDist; };
  double GetAverageDist( ) { return mAverageDist; };

  int GetMinInDegree( ) { return mMinInDegree; };
  int GetMaxInDegree( ) { return mMaxInDegree; };

  int GetMinOutDegree( ) { return mMinOutDegree; };
  int GetMaxOutDegree( ) { return mMaxOutDegree; };



  double GetAverageInDegree( ) { return mAverageInDegree; };
  double GetAverageOutDegree( ) { return mAverageOutDegree; };


  int GetNnodes( ) { return mNnodes; };
  int GetNlinks( ) { return mNlinks ;};

  int GetMaxHop( ) { return mMaxHop ;} ;
  int GetMinHop( ) { return mMinHop ;}  ;

  //map< pair<int,int> , pair<int,int> > GetViolatedPaths( ) { return mViolatedPaths ; } ;

  double GetAverageHop( ) { return mHopAvg ;};

 private:

  cosi::commstruct::onchipcommunication::Implementation* mI ;
  cosi::platforms::onchipcommunication::NocPlatformInterface* mPltIf ;
  cosi::platforms::onchipcommunication::Platform* mPlt ;
  double mWireStaticPower ;
  double mWireDynamicPower ;
  double mInterfacePower ;
  double mInterfaceArea ;
  double mRouterDynamicPower ;
  double mRouterStaticPower ;
  double mWireArea ;
  double mWireDeviceArea ;
  double mWireWiringArea ;
  double mRouterArea ;

  double mMaxDist ;
  double mMinDist ;
  double mTotalDist ;
  double mAverageDist ;

  int mMinInDegree ;
  int mMaxInDegree ;

  int mMinOutDegree ;
  int mMaxOutDegree ;



  double mAverageInDegree ;
  double mAverageOutDegree ;

  int mNnodes ;
  int mNlinks ;

  int mMaxHop ;
  int mMaxHopPathSource ;
  int mMaxHopPathDest ;
  int mMinHop ;
  double mHopAvg ;

} ;

}
}
}

#endif
