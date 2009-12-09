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
#ifndef SOURCEIP_H_
#define SOURCEIP_H_

#include <vector>
#include "systemc.h"
#include "float.h"
#include "Types.h"
/**This component implements a source that can send packets to many
 * destinations.
 *
 *@author Alessandro Pinto,
 *@author apinto@eecs.berkeley.edu,
 *@author EECS Grad Student,
 *@author 211 Cory Hall #1772,
 *@author Berkeley, CA, 94720-1772
 *@author (510)-6429670

 *@version $Id: SourceIp.h,v 1.3 2008/12/09 23:28:22 apinto Exp $
 */
using namespace std ;

template< int FlitWidth >
class SourceIp : public sc_module
{

public:

  sc_out< sc_bv< FlitWidth > > DataOut ;
  sc_out< bool > ValidOut;
  sc_in< bool > AckIn ;
  sc_in< bool > FullIn ;
  sc_in<bool> Clock ;


  SC_HAS_PROCESS( SourceIp ) ;

  void Fsm( ) ;

 SourceIp( sc_module_name pName , int pId  , vector< Flow > pFlows ) : sc_module( pName )  {

    mId = pId ;

    mFlows = pFlows ;

    mFlitCount.resize( mFlows.size( ) , 0 ) ;

    mSentBits.resize( mFlows.size( ) , 0 ) ;

    mState = IDLE ;

    mWstate = WIDLE ;

    cout << "SourceIp " << name( ) << " initialization done " << endl ;

    SC_THREAD( Fsm ) ;
    sensitive_pos << Clock ;


  };


  enum{IDLE, FLIT } mState;
  enum{ WIDLE , TX } mWstate ;

 private:

  int mId ;

  vector< Flow > mFlows ;

  vector< int > mFlitCount; ;

  vector< int > mSentBits;

  vector< sc_bv< FlitWidth > > mCurrentPacket ;

  int mCurrentDest ;


} ;

template< int FlitWidth >
void SourceIp< FlitWidth >::Fsm( )
{


  int Index ;
  cout << "Source " << name( ) << " running " << endl ;

  while( true ){

    wait( ) ;

    switch( mState ) {

    case IDLE:

      //cout << "Source " << name( ) << "  to  destination "  << mDestId[CurrentDest].to_int() << "  " <<  mFlitSent[Index] << endl ;

      //Pick the flow that is laggin behind
      double MinSlack ;
      MinSlack = DBL_MAX ;
      for( int i = 0 ; i < mFlows.size( ) ; i++ ) {
	if ( ( mSentBits[i] / ( sc_simulation_time( )*1e-9 )) - mFlows[i].Bandwidth < MinSlack ) {
	  MinSlack = ( mSentBits[i] / ( sc_simulation_time( )*1e-9 )) - mFlows[i].Bandwidth ;
	  mCurrentDest = i ;
	}
      }

      //Pick a random destination
      //mCurrentDest = rand( )%(mFlows.size( ) ) ;

      if ( MinSlack < 0 ) {
	//Build up the packet
	mCurrentPacket.clear( ) ;
	mCurrentPacket.resize( mFlows[mCurrentDest].PacketSize , 0 ) ;

	//Header flit
	mCurrentPacket[ mFlows[mCurrentDest].PacketSize - 1 ].range( 7, 0) = mId ;
	mCurrentPacket[ mFlows[mCurrentDest].PacketSize - 1 ].range(15 , 8) = mFlows[mCurrentDest].Destination ;
	mCurrentPacket[ mFlows[mCurrentDest].PacketSize - 1 ].range( 23,16) = mFlows[mCurrentDest].PacketSize ;
	mCurrentPacket[ mFlows[mCurrentDest].PacketSize - 1 ].range(31,24) = mFlows[mCurrentDest].PacketSize ;

	//Random data
	for( int i = mFlows[mCurrentDest].PacketSize - 2 ; i > 0 ; i-- ) {
	  mCurrentPacket[i] = rand( ) ;
	}

	//Tail flit has a time stamp
	mCurrentPacket[0] = (int) sc_simulation_time( ) ;

	mFlitCount[mCurrentDest] = mFlows[mCurrentDest].PacketSize ;
	mState = FLIT ;
      } else
     	mState =IDLE ;

      break;

    case FLIT :

      switch( mWstate ) {

      case WIDLE:
	if ( ! FullIn ) {
	  DataOut = mCurrentPacket[ mFlitCount[mCurrentDest] - 1 ] ;
	  //cout << "Source " << name( ) << " Sending Flit " << mFlitCount[mCurrentDest] << " to " << mFlows[mCurrentDest].Destination << endl ;
	  //cout<< "Worde sent  " << curr_out << endl ;
	  ValidOut = true ;
	  mWstate = TX ;
	}
	break;
      case TX:
	if ( AckIn ) {
	  ValidOut =false ;
	  mSentBits[mCurrentDest] = mSentBits[mCurrentDest] + FlitWidth;
	  mFlitCount[mCurrentDest] -- ;
	  if (  mFlitCount[mCurrentDest] == 0 )
	    mState = IDLE ;
	  else
	    mState = FLIT ;
	  mWstate = WIDLE ;
	}
	break;

      }
    }
  }
}


#endif /*SOURCEIP_H_*/
