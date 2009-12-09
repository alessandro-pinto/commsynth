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
#ifndef MONITOR_H_
#define MONITOR_H_

#include "systemc.h"
#include "DestIp.h"
#include <vector>
#include <map>
#include <fstream>

using namespace std ;

template<int FlitWidth>
class Monitor : public sc_module {

 public:

  SC_HAS_PROCESS( Monitor ) ;


  Monitor( sc_module_name n , double Step , vector< DestIp<FlitWidth>* > DestVector ) : sc_module( n ) {

    mStep = Step ;
    mDestVector = DestVector ;

    SC_THREAD( Tick ) ;

  } ;

    void Tick( ) {

      while( true) {

	wait(  mStep , SC_SEC )  ;
	cout << "Simulation time " << sc_simulation_time( )*1e-9 << " sec " << endl ;
	//report destination bandwidth

	vector< map<int,int> >  ReceivedBits ;
        double Average = 0 ;
        double Min = DBL_MAX ;
        double Max = 0 ;
	for( int i = 0 ; i < mDestVector.size( ) ; i++ ) {
	  ReceivedBits.push_back(  ( mDestVector[i] )->GetReceivedBits() ) ;
	  mDestVector[i]->Report( ) ;
          Average += mDestVector[i]->GetAverageLatency( ) ;
          if ( mDestVector[i]->GetMinLatency( ) < Min )
                Min = mDestVector[i]->GetMinLatency( ) ;
          if ( mDestVector[i]->GetMaxLatency( ) > Max )
                Max = mDestVector[i]->GetMaxLatency( ) ;
	}
	mReceivedBits[ sc_simulation_time( )*1e-9 ] = ReceivedBits ;
        cout << "Average lantecy = " << Average / (double) mDestVector.size( ) << endl;
        cout << "Maximum latency = " << Max << "  Minimum latency = " << Min << endl ;
      }

    } ;


    void WriteReport( ) {

      map< double , vector< map< int , int > > >::iterator It ;
      map<int,int>::iterator Sit;
      for( int i = 0 ; i < mDestVector.size( ) ; i++ ) {
	ofstream f;
	string fname ;
	fname = string( mDestVector[i]->name( ) ) + ".trace" ;
	f.open( fname.c_str( ) ) ;
	for( It = mReceivedBits.begin( ) ; It != mReceivedBits.end( ) ; It++ ) {
	  f << It->first << "  " ;
	  for( Sit = (It->second)[i].begin( ) ; Sit != (It->second)[i].end( ) ; Sit++ ) {
	    f << Sit->second << "  "  ;
	  }
	  f << endl ;
	}
	f.close( ) ;
      }
    }

    double mStep ;
    vector< DestIp<FlitWidth>* > mDestVector ;
    map< double , vector< map<int,int> > > mReceivedBits ;

} ;

#endif
