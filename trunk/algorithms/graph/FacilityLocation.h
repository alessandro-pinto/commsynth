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
#include "stdio.h"
#include "stdlib.h"
#include "float.h"

#ifndef _FACILITY_LOCATION_H_
#define _FACILITY_LOCATION_H_


class FacilityLocation {
public:

	FacilityLocation( ) ;
	~FacilityLocation( ) ;
	void initialize(int i_nf , int i_nc , double i_f , double** i_c) ;
	int** getConnectionMatrix( ) ;
	int* getOpenVector( ) ;
	int getNofFacilities( ) ;
	void run( ) ;
private:
	int minimum( double* T , int* y , int l) ;
	int is_initialized;
	int has_run ;
	int n_opened_facilities ;
	double** c ;
	double f ;
	int nf;
	int nc;
	int** x ;
	int* y ;
};


//int /*Returns the number of opened facilities*/
//facility_location( int nf , /*Total number of facilities*/
//		   int nc , /*Total number of cities*/
//		   double f , /*Cost of opening a facility*/
//		   double** c , /*Connection costs: must be nfxnc*/
//		   int** x , /*Connection matrix: must be allocated and nfxnc*/
//		   int* y) ; /*Opening vector: must be allocated and nfx1*/
//
//
//int /*Returns the position of minimum element in a vector T of length l*/
//minimum( double* T ,
//	 int* y ,
//	 int l ) ;

#endif


