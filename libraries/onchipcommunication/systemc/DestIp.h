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
#ifndef DESTIP_H_
#define DESTIP_H_

#include "systemc.h"
#include <map>
#include <sstream>

template< int FlitWidth >
class DestIp : public sc_module
{
 public:

  sc_in< sc_bv< FlitWidth > > DataIn ;
  sc_in< bool > ValidIn ;
  sc_out< bool > AckOut ;
  sc_out< bool > FullOut ;
  sc_in< bool > Clock ;

  SC_HAS_PROCESS( DestIp ) ;

 DestIp( sc_module_name n , int pId ) : sc_module( n ) {

    mState = IDLE ;
    mRstate = VALID ;
    mId = pId ;

    SC_THREAD( Fsm ) ;
    sensitive_pos << Clock ;

    cout << "Destination " << name() <<  "  initialized" << endl ;

  }

  void Fsm( ) ;

  void Report( ) ;

  string GetLatencies( ) ;
  string GetBandwidths( ) ;

  map< int , int > GetReceivedBits( ) ;

  enum{ IDLE , FLIT } mState ;
  enum{ VALID , RX } mRstate ;

  int mId ;
  //For each source, stores the number of received packets.
  map< int , int > mReceivedBits ;
  //Sum up the latencies to compute the average at the end
  map< int , double > mLatencies ;

  double GetAverageLatency( ) ;
  double GetMinLatency( ) ;
  double GetMaxLatency( ) ;
  int mPacketSize ;
  int mFlitCount ;
  int mCurrentSource ;
  int mCurrentDest ;

  sc_bv< FlitWidth > mCurrentIn ;

} ;

template< int FlitWidth >
void DestIp< FlitWidth >::Fsm( )
{

  cout << "Destination " << name( ) << " running " << endl ;

  FullOut = false ;

  AckOut = false ;

  while( true ) {
    wait( ) ;

    switch( mState ) {
    case IDLE :

      switch( mRstate ) {
      case VALID :
	if ( ValidIn ) { //Get the header
	  mCurrentIn = DataIn ;
	  mCurrentSource = ( mCurrentIn.range( 7 , 0 ) ).to_int( ) ;
	  mCurrentDest = ( mCurrentIn.range( 15 , 8 ) ).to_int( ) ;
	  mPacketSize =  ( mCurrentIn.range( 23 , 16 ) ).to_int( ) ;
	  mFlitCount = mPacketSize - 1 ;
	  //cout << "Destination " << mId << " receives header from " << mCurrentSource << endl ;
	  if ( mReceivedBits.find( mCurrentSource ) == mReceivedBits.end( ) ) {
	    mReceivedBits[ mCurrentSource ] = FlitWidth ;
	  } else {
	    mReceivedBits[ mCurrentSource ] = mReceivedBits[ mCurrentSource ] + FlitWidth ;
	  }
	  mRstate = RX ;
	  AckOut = true ;
	}
	break ;
      case RX :
	AckOut = false ;
	mRstate = VALID ;
	mState = FLIT ;
	break ;
      }
      break;
    case FLIT :
      switch( mRstate ) {
      case VALID :
	if ( ValidIn ) { //Get the Flit
	  mFlitCount -- ;
	  //cout << "Destination " << mId << " receiving flit " << mFlitCount << endl ;
	  mReceivedBits[ mCurrentSource ] = mReceivedBits[ mCurrentSource ] + FlitWidth ;
	  //If it is the tail flit, compute latency
	  if( mFlitCount == 0 ) {
	    if ( mLatencies.find( mCurrentSource ) == mLatencies.end( ) ){
	      mCurrentIn = DataIn ;
	      mLatencies[mCurrentSource] = sc_simulation_time( ) - mCurrentIn.to_int( )  ;
	    } else {
	      mCurrentIn = DataIn ;
	      mLatencies[mCurrentSource] += sc_simulation_time( ) - mCurrentIn.to_int( ) ;
	    }
	  }
	  mRstate = RX ;
	  AckOut = true ;
	}
	break ;
      case RX :
	AckOut = false ;
	mRstate = VALID ;
	if( mFlitCount == 0 ) {
	  mState = IDLE ;
	}else{
	  mState = FLIT ;
	}
	break ;
      }
      break;
    }
  }
}


template< int FlitWidth >
map< int , int > DestIp< FlitWidth >::GetReceivedBits( ) {
  return mReceivedBits;
}


template< int FlitWidth >
void DestIp< FlitWidth >::Report( ) {

  //Report bandwidth
  cout << "Bandwidth report for " << name( ) << endl ;
  map<int,int>::iterator It ;
  for( It = mReceivedBits.begin( ) ; It != mReceivedBits.end( ) ; It++ ) {
    cout << "\t Average bandwidth from source " << It->first << " = " << It->second /( sc_simulation_time( ) *1e-9 ) << endl ;
  }

  cout << "Delay report for " << name( ) << endl ;
  map<int,double>::iterator Lit ;
  for( Lit = mLatencies.begin( ) ; Lit != mLatencies.end( ) ; Lit++ ) {
    cout << "\t Average latency from source " << Lit->first << " = " << FlitWidth * Lit->second / ( double )mReceivedBits[Lit->first] << endl ;
  }

}

template< int FlitWidth >
string DestIp< FlitWidth >::GetLatencies( ) {
  stringstream s ;
  map<int,double>::iterator Lit ;
  for( Lit = mLatencies.begin( ) ; Lit != mLatencies.end( ) ; Lit++ ) {
    s << FlitWidth * Lit->second / ( double )mReceivedBits[Lit->first] << endl ;
  }
  return s.str() ;

}

template< int FlitWidth >
string DestIp< FlitWidth >::GetBandwidths( ) {
  stringstream s ;
  //Report bandwidth
  map<int,int>::iterator It ;
  for( It = mReceivedBits.begin( ) ; It != mReceivedBits.end( ) ; It++ ) {
    s  << It->second /( sc_simulation_time( ) *1e-9 ) << endl ;
  }

  return s ;
}

template< int FlitWidth >
double  DestIp< FlitWidth >::GetAverageLatency( ) {

  map<int,double>::iterator Lit ;
  double Average = 0 ;
  for( Lit = mLatencies.begin( ) ; Lit != mLatencies.end( ) ; Lit++ ) {
    Average+= FlitWidth * Lit->second / ( double ) mReceivedBits[Lit->first]  ;
  }
  return Average/(double)( mLatencies.size( ) ) ;

}

template< int FlitWidth >
double  DestIp< FlitWidth >::GetMinLatency( ) {

  map<int,double>::iterator Lit ;
  double Min = DBL_MAX ;
  for( Lit = mLatencies.begin( ) ; Lit != mLatencies.end( ) ; Lit++ ) {
    if ( FlitWidth * Lit->second / ( double ) mReceivedBits[Lit->first] < Min ) {
      Min = FlitWidth * Lit->second / ( double ) mReceivedBits[Lit->first]  ;
    }
  }
  return Min ;

}

template< int FlitWidth >
double  DestIp< FlitWidth >::GetMaxLatency( ) {

  map<int,double>::iterator Lit ;
  double Max = 0 ;
  for( Lit = mLatencies.begin( ) ; Lit != mLatencies.end( ) ; Lit++ ) {
    if ( FlitWidth * Lit->second / ( double ) mReceivedBits[Lit->first] > Max ) {
      Max = FlitWidth * Lit->second / ( double ) mReceivedBits[Lit->first]  ;
    }
  }
  return Max ;

}



#endif /*DESTIP_H_*/
