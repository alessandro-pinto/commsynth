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
#include "systemc.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Technology.h"

#ifndef _PTPCONNECTION_H_
#define _PTPCONNECTION_H_

//////////////////////////////////////////////////////////////////////////
//// ptpconnection


/** This class implement a point-to-point connection. The model is word
    accurate where the word width is determined by a definition
    stored in the common.h file.

 *@author Alessandro Pinto,
 *@author apinto@eecs.berkeley.edu,
 *@author EECS Grad Student,
 *@author 211 Cory Hall #1772,
 *@author Berkeley, CA, 94720-1772
 *@author (510)-6429670

 *@version $Id: PtP.h,v 1.3 2008/12/09 23:28:22 apinto Exp $
*/

template<int FlitWidth>
class PtP : public sc_module
{
 public:
  //////////////////////////////////////////////////////////////////////////
  ///////                 Ports                        /////////////////////

  /**Input ports */
  sc_in< sc_bv< FlitWidth > > DataIn;
  sc_in< bool > ValidIn ;
  sc_out< bool > AckOut ;
  sc_out< bool > FullOut ;


  /**Output ports */
  sc_out< sc_bv< FlitWidth > > DataOut;
  sc_out< bool > ValidOut ;
  sc_in< bool > AckIn ;
  sc_in< bool > FullIn ;



  SC_HAS_PROCESS( PtP );

    //////////////////////////////////////////////////////////////////////////
    ///////                 Constructor                  /////////////////////

    /**There is only one method that accounts for delay and energy consumption.*/

 PtP( sc_module_name n,
      TechnologyNode tech,
      int layer,
      double length ) : sc_module( n ),
    _tech( tech ),
    _layer( layer ),
    _length( length )    {


      SC_THREAD( Transfer ) ;
      sensitive << DataIn << ValidIn << AckIn << FullIn ;

    }

  double GetDynamicPower( ) {
    return ( total_sw_energy / ( sc_simulation_time( ) * 10e-9) ) ;
  }

  double GetLeakagePower( ) {
    return ( total_leak_energy / ( sc_simulation_time( ) * 10e-9) ) ;
  }

  void Transfer( ) ;

    TechnologyNode _tech ;
    int _layer ;
    double _length ;

    double total_sw_energy;
    double total_sc_energy;
    double total_leak_energy;
    double total_energy;

private:
    sc_bv< FlitWidth > _prev_word ;
     bool  _prev_valid ;
    bool  _prev_ack ;
     bool  _prev_full ;
    double _prev_time ;

};

template< int FlitWidth >
void PtP<FlitWidth>::Transfer( )
{
  double _delay, _tau;
  double _e_switching, _e_leakage, _e_sc ;

  sc_bv< FlitWidth > _curr_word ;

  while (true)
    {

      wait( ) ;

      if ( _prev_valid != ValidIn ) {
	total_sw_energy = total_sw_energy + _tech.Edyn*_length ;
	total_energy = total_sw_energy ;
	_prev_valid = ValidIn ;
	ValidOut = ValidIn ;
      }

      if ( _prev_ack != AckIn ) {
	total_sw_energy = total_sw_energy + _tech.Edyn*_length ;
	total_energy = total_sw_energy ;
	_prev_ack = AckIn ;
	AckOut = AckIn ;
      }

      if ( _prev_full != FullIn ) {
	total_sw_energy = total_sw_energy + _tech.Edyn*_length ;
	total_energy = total_sw_energy ;
	_prev_full = FullIn ;
	FullOut = FullIn ;
      }

      _curr_word = DataIn ;
      if ( _prev_word != _curr_word )  {



	//charge delay
	//wait( _delay , SC_SEC );

	//compute energy


	for( int i = 0 ; i < FlitWidth ; i ++ )
	  {
	    if ( _curr_word[i] != _prev_word[i] )
	      {
		total_sw_energy = total_sw_energy + _tech.Edyn*_length ;
		total_energy = total_sw_energy ;
	      }
	  }

	_prev_word = _curr_word ;

	_prev_time = (double)sc_simulation_time( );

	DataOut = _curr_word;
      }
    }
};


#endif
