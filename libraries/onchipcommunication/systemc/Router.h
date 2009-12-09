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
#ifndef ROUTER_H_
#define ROUTER_H_

#include "systemc.h"
#include <queue>
#include <map>
#include <set>
#include "Technology.h"

using namespace std ;

template< int Nin , int Nout , int FlitWidth >
  class Router : public sc_module
{

 public:

  sc_in< bool > Clock ;

  //Input ports
  sc_in < sc_bv< FlitWidth > > DataIn[Nin] ;
  sc_in < bool > ValidIn[Nin] ;
  sc_out< bool > AckOut[Nin] ;
  sc_out< bool > FullOut[Nin] ;

  //Output ports
  sc_out< sc_bv< FlitWidth > > DataOut[Nout] ;
  sc_out< bool > ValidOut[Nout] ;
  sc_in< bool > AckIn[Nout] ;
  sc_in< bool > FullIn[Nout] ;

  SC_HAS_PROCESS( Router ) ;

  void Fsm( ) ;
  void InputFsm( ) ;
  void SwitchFsm( ) ;
  void OutputFsm( ) ;
  void PrintState( ) ;

  Router( sc_module_name n ,
	  int T ,
	 int Size ,
	  map< pair<int,int> , int > RoutingTable ,
	  vector< double > ChannelWeights ,
	  TechnologyNode Tech) : sc_module( n ) {

    mTech = Tech ;
    mRoutingTable = RoutingTable ;
    mSize = Size ;
    mT = T ;
    mChannelWeights = ChannelWeights ;

    mLastGranted = 0 ;

    for( int i = 0 ; i < Nin ; i++ ) {
      mRstate[i] = RIDLE ;
      mState[i] = IDLE ;
    }

    for( int i = 0 ; i < Nout ; i++ ) {
      mWstate[i] = WIDLE ;
    }

    mInputGranted.resize( Nin , false ) ;
    mOutput.resize( Nin , -1 ) ;
    mOutputBusy.resize( Nout , false ) ;
    mFlitCount.resize( Nin , 0 ) ;

    mFIFO.resize( Nin ) ;
    mCurrentFlow.resize( Nin ) ;
    mSentFlits.resize( Nin , 0 ) ;
    mTotalFlitSent = 1 ;
    mOutputBuffer.resize( Nout ) ;
    mInput.resize( Nout , -1 ) ;


    SC_THREAD( Fsm ) ;
    sensitive_pos << Clock ;

    mDynamicEnergy = 0 ;

    cout << "Router " << name( ) << " initialized " << endl ;

  }

  double GetDynamicPower( ) {
    return ( mDynamicEnergy/ ( sc_simulation_time( ) * 10e-9) ) ;
  }

  TechnologyNode mTech ;
  map< pair<int,int> , int > mRoutingTable ;
  vector< double > mChannelWeights ;
  int mSize ;
  int mT ;

  vector< queue< sc_bv< FlitWidth > > > mFIFO ;
  //The outbuffer is supposed to be one place only
  vector< queue< sc_bv< FlitWidth > > > mOutputBuffer ;


  //Input states
  enum{ RIDLE , RX } mRstate[Nin] ;
  //Output States
  enum{ WIDLE , TX } mWstate[Nout] ;

  vector< bool > mInputGranted ;

  vector< int > mOutput ;

  vector< int > mInput ;

  vector< bool > mOutputBusy ;

  vector< int > mFlitCount ;

  vector<int> mSentFlits ;

  vector< pair<int,int> > mCurrentFlow ;

  int mTotalFlitSent ;

  //Switch state
  enum{ IDLE , FLIT} mState[Nin] ;

  int mLastGranted ;

  double mDynamicEnergy ;


} ;


template< int Nin , int Nout , int FlitWidth >
  void Router< Nin , Nout , FlitWidth >::PrintState( ) {

  cout << name( ) << endl ;
  cout << "Input queues " << endl ;
  for( int Input = 0 ; Input < Nin ; Input++ ) {
    cout << "Input " << Input << " size = " << mFIFO[Input].size( ) ;
    if ( !mState[Input] == FLIT ) {
      cout << " Header "  ;
      sc_bv< FlitWidth > Header ;
      Header = mFIFO[Input].front( ) ;
      pair<int,int> Sd ;
      Sd.first = ( Header.range( 7 , 0 )).to_int( )  ;
      Sd.second = (Header.range( 15 , 8 )).to_int( )  ;
      //cout << name( ) <<  " Input " << i << " competing for output " << mRoutingTable[Sd] << endl ;
      cout << "Flow (" << Sd.first << "," << Sd.second << ") to output " << mRoutingTable[Sd] ;
    }
    else {
      cout << " Flit" << " Flow (" << mCurrentFlow[Input].first << "," << mCurrentFlow[Input].second  ;
    }
    if ( mInputGranted[Input] ) {
      cout << " Assigned to " << mOutput[Input] ;
      cout << " need to send " << mFlitCount[Input] << endl ;
    } else {
      cout << " Not assigned " <<  endl ;
    }
  }

  cout << "Output queues " << endl ;
  for( int Output = 0 ; Output < Nout ; Output++ ) {
    cout << "Output " << Output << " size = " << mOutputBuffer[Output].size( )  << "  " ;
    if( mOutputBusy[Output] )
      cout << "Busy " << endl ;
    else
      cout << "Available" << endl ;
  }

}



template< int Nin , int Nout , int FlitWidth >
  void Router< Nin , Nout , FlitWidth >::Fsm( ) {

  cout << "Router " << name( )  << " running " << endl ;

  while( true ) {

    //Singals from the FIFOs
    for( int i = 0 ; i < Nin ; i++ ) {
      if ( mFIFO[i].size( ) == mSize ) {
	FullOut[i] = true ;
      } else {
	FullOut[i] = false ;
      }
    }

    wait( ) ;
    //PrintState( ) ;
    InputFsm( ) ;
    SwitchFsm( ) ;
    OutputFsm( ) ;
  }

}

template< int Nin , int Nout , int FlitWidth >
  void Router< Nin , Nout , FlitWidth >::InputFsm( ) {
  for( int Input = 0 ; Input < Nin ; Input++ ) {
    //receive from all inputs
    switch( mRstate[Input] ) {
    case RIDLE :
      if ( ValidIn[Input] ) { //Get the Flit
	mFIFO[Input].push( DataIn[Input] ) ;
	mRstate[Input] = RX ;
	AckOut[Input] = true ;
      }
      break ;
    case RX :
      AckOut[Input] = false ;
      mRstate[Input] = RIDLE ;
      break ;
    }
  }
}


template< int Nin , int Nout , int FlitWidth >
  void Router< Nin , Nout , FlitWidth >::OutputFsm( ) {

  for( int Output = 0 ; Output < Nout ; Output++ ) {
    if ( ! mOutputBuffer[Output].empty( ) ) {
      switch( mWstate[Output] ) {
      case WIDLE:
	if ( ! FullIn[Output] ) {
	  DataOut[Output] = mOutputBuffer[Output].front( ) ;
	  ValidOut[Output] = true ;
	  mWstate[Output] = TX ;
	}
	break;
      case TX:
	if ( AckIn[Output] ) {
	  ValidOut[Output] =false ;
	  mOutputBuffer[Output].pop( ) ;
	  mFlitCount[mInput[Output]]-- ;
	  mSentFlits[mInput[Output]]++ ;
	  mTotalFlitSent++ ;

	  //Annotate dynamic energy
	  mDynamicEnergy += mTech.Erouter[Nin][Nout] ;

	  if ( mFlitCount[mInput[Output]] == 0 ) {
	    mInputGranted[ mInput[Output] ] = false ;
	    mOutputBusy[ Output ] = false ;
	    mState[mInput[Output]] = IDLE ;
	  }
	  mWstate[Output] = WIDLE ;
	}
	break;
      }
    }
  }
}

template< int Nin , int Nout , int FlitWidth >
  void Router< Nin , Nout , FlitWidth >::SwitchFsm( ) {

  /////////////////////////////////////////////////////////////////////////////
  //                 Channel assignment
  /////////////////////////////////////////////////////////////////////////////

  //Compute the slacks
  vector< double > Slack( Nin , 0 ) ;
  for( int i = 0 ; i < Nin ; i++ ) {
    Slack[i] = ( (double)mSentFlits[i])/((double)mTotalFlitSent)  - mChannelWeights[i] ;
    //cout << name( ) << " Slack for input " << i << " = " << Slack[i] << endl ;
  }

  //look for competing inputs
  vector< set< int > > CompetingInputs( Nout ) ;
  sc_bv< FlitWidth > Header ;
  pair<int,int> Sd ;
  for( int i = 0 ; i < Nin ; i++ ) {

    //If this input is not in any transfer and has some flits waiting
    //(then in must be the header flit)

    if ( ( ! mInputGranted[i] ) && ( ! mFIFO[i].empty( ) ) ) {
      Header = mFIFO[i].front( ) ;
      Sd.first = ( Header.range( 7 , 0 )).to_int( )  ;
      Sd.second = (Header.range( 15 , 8 )).to_int( )  ;
      mCurrentFlow[i] = Sd ;
      //cout << name( ) <<  " Input " << i << " competing for output " << mRoutingTable[Sd] << endl ;
      CompetingInputs[mRoutingTable[Sd]].insert( i ) ;
    }

  }

  //Assign each avalable output to inputs
  //in decreasing order of slacks
  for( int i = 0 ; i < Nout ; i++ ) {

    if ( ! mOutputBusy[i] && ! CompetingInputs[i].empty( ) ) {
      //Scan the competing inputs for
      //minimum slacks
      set<int>::iterator It ;
      double MinSlack = DBL_MAX ; int MinIn = 0 ;
      for( It = CompetingInputs[i].begin( ) ; It != CompetingInputs[i].end( ) ; It++ ) {
	if ( Slack[*It] < MinSlack ) {
	  MinSlack = Slack[*It] ;
	  MinIn = *It ;
	}
      }
      //Assign Output
      //cout << name( ) << " Output " << i << " assigned to input " << MinIn << endl ;
      mInputGranted[ MinIn ] = true ;
      mOutput[ MinIn ] = i ;
      mInput[ i ] = MinIn ;
      mOutputBusy[ i ] = true ;
    }
  }

  /////////////////////////////////////////////////////////////////////////////
  //                 Flit input to output transfer
  /////////////////////////////////////////////////////////////////////////////
  int Input ;
  for( Input = 0 ; Input < Nin ; Input++ ) {
    //If the input has been granted and the queue is not empty
    if ( mInputGranted[Input]  ) {

      switch( mState[Input] ) {
      case IDLE :
	if ( mOutputBuffer[ mOutput[ Input ] ].size( ) == 0 && ! mFIFO[Input].empty( ) ) {
	  //this is the header flit
	  Header = mFIFO[Input].front( ) ;
	  mFlitCount[Input] =  ( Header.range( 23 , 16 )).to_int( )  ;
	  //cout << " Need to send " << mFlitCount[Input] << " flits " << endl ;
	  //put the flit in the output buffer and wait for the transfer to finish
	  mOutputBuffer[ mOutput[ Input ] ].push( Header ) ;
	  mFIFO[Input].pop( ) ;
	  mState[Input] = FLIT ;
	}
	break ;
      case FLIT :
	if ( mOutputBuffer[ mOutput[ Input ] ].size( ) == 0 && ! mFIFO[Input].empty( ) ) {
	  //tranfer is done

	    mOutputBuffer[ mOutput[ Input ] ].push( mFIFO[Input].front( ) ) ;
	    //cout << " Sending flit " <<  mFlitCount[Input] << endl ;
	    mFIFO[Input].pop( ) ;

	}
	break ;
      }
    }
  }
}

#endif /*ROUTER_H_*/
