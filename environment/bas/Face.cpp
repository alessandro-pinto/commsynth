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
#include "Face.h"

namespace cosi {

namespace environment {

namespace bas {

Face::Face( ) {
  Name = "Unknonw" ;
  mV.resize( 4 ) ;
}


void Face::SetPoints( Position pV1 , Position pV2 , Position pV3 , Position pV4 ) {

  if ( pV1.IsAny() || pV2.IsAny() || pV3.IsAny() || pV4.IsAny()) {
    CosiLog << "In order to set the points of a surface," << std::endl ;
    CosiLog << "all of them must have a value." << std::endl ;
    return  ;
  }

  mV[0] = pV1 ;
  mV[1] = pV2 ;
  mV[2] = pV3 ;
  mV[3] = pV4 ;

  //Find the closest point to the origin
  int ClosestIndex = 0 ;
  Position Origin( 0 , 0 , 0 ) ;
  if ( Norm2( mV[1] , Origin ) < Norm2( mV[ClosestIndex] , Origin ) ) {
    ClosestIndex = 1 ;
  }
  if ( Norm2( mV[2] , Origin ) < Norm2( mV[ClosestIndex] , Origin ) ) {
    ClosestIndex = 2 ;
  }
  if ( Norm2( mV[3] , Origin ) < Norm2( mV[ClosestIndex] , Origin ) ) {
    ClosestIndex = 3 ;
  }

  //Swap closest
  if ( ClosestIndex != 0 ) {
    Position Tmp ;
    Tmp = mV[0] ;
    mV[0] = mV[ClosestIndex] ;
    mV[ClosestIndex] = Tmp ;
  }

  //Find the farthest to V1
  int FarIndex = 1 ;
  if ( Norm2( mV[2] , mV[0] ) > Norm2( mV[FarIndex] , mV[0] ) ) {
    FarIndex = 2 ;
  }
  if ( Norm2( mV[3] , mV[0] ) > Norm2( mV[FarIndex] , mV[0] ) ) {
    FarIndex = 3 ;
  }

  //Swap farthest
  if ( FarIndex != 2 ) {
    Position Tmp ;
    Tmp = mV[2] ;
    mV[2] = mV[FarIndex] ;
    mV[FarIndex] = Tmp ;
  }

  //Order the other two vertexes
  if ( mV[1].GetX( ) < mV[3].GetX() ) {
    Position Tmp ;
    Tmp = mV[3] ;
    mV[3] = mV[1] ;
    mV[1] = Tmp ;
  }

  if ( ! CheckFace( ) )
    CosiLog << "The four points do not lie on the same plane." << std::endl ;
}


bool Face::CheckFace( ) {

  //Check if the vector v1->v3 can be obtained as
  //linear combination of v1->v2 and v1->v4

  //Poject everything on the x,y plane
  // and compute the linear combination
  double Dx21 , Dx31 , Dy21 , Dy31 , Dx41 , Dy41 , Det ;
  Dx21 = mV[1].GetX( ) - mV[0].GetX( ) ;
  Dx31 = mV[2].GetX( ) - mV[0].GetX( ) ;
  Dy21 = mV[1].GetY( ) - mV[0].GetY( ) ;
  Dy31 = mV[2].GetY( ) - mV[0].GetY( ) ;
  Dx41 = mV[3].GetX( ) - mV[0].GetX( ) ;
  Dy41 = mV[3].GetY( ) - mV[0].GetY( ) ;



  Det= ( Dx21*Dy31 - Dx31*Dy21) ;

  double L1 , L2 ;

  L1 = ( Dx41*Dy31 - Dy41*Dx31)/Det ;
  L2 = ( -Dx41*Dy21 + Dy41*Dx21)/Det ;

  double Dz21 , Dz31 , Dz41 ;
  Dz21 = mV[1].GetZ( ) - mV[0].GetZ( ) ;
  Dz31 = mV[2].GetZ( ) - mV[0].GetZ( ) ;
  Dz41 = mV[3].GetZ( ) - mV[0].GetZ( ) ;

  if ( Dz41 == Dz21*L1 + Dz31*L2 )
    return true ;
  else
    return false ;

}

void Face::SetV1( Position pV1 ) {
  mV[0] = pV1 ;
}

void Face::SetV2( Position pV2 ) {
  mV[1] = pV2 ;
}

void Face::SetV3( Position pV3 ) {
  mV[2] = pV3 ;
}

void Face::SetV4( Position pV4 ) {
  mV[3] = pV4 ;
}


Position Face::GetV1( ) {

  return mV[0] ;
}


Position Face::GetV2( ) {
  return mV[1] ;
}


Position Face::GetV3( ) {
  return mV[2] ;
}


Position Face::GetV4( ) {
  return mV[3] ;
}

}
}
}
