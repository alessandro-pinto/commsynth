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
#include "FacilityLocation.h"

FacilityLocation::FacilityLocation( ) {
  is_initialized = 0 ;
  has_run = 0 ;
}

FacilityLocation::~FacilityLocation( ) {
  for( int i = 0 ; i < nf ; i++)
    delete x[i] ;
  delete x ;
  delete y ;
}

void FacilityLocation::initialize(int i_nf , int i_nc , double i_f , double** i_c) {
  if ( is_initialized) {
    for( int i = 0 ; i < nf ; i++)
      delete x[i] ;
    delete x ;
    delete y ;
  } else {
    is_initialized = 1 ;
  }
  c = i_c ;
  f = i_f ;
  nc = i_nc ;
  nf = i_nf ;
  x = new int*[nf];
  for(int i=0;i<nf;i++){
    x[i] = new int[nc];
  }
  y = new int[nf] ;
}

int** FacilityLocation::getConnectionMatrix( ) {
	return x ;
}

int* FacilityLocation::getOpenVector( ) {
	return y ;
}

int FacilityLocation::getNofFacilities( ){
  return n_opened_facilities ;
}

int FacilityLocation::minimum( double* T , int* y , int l) {
  double min_found ;
  int index ;
  int min_index ;
  int first ;
  first = 0 ;
  while( y[first] && (first < l ) )
    first++ ;
  if( first == l) {
    printf( "All facilities are opened \n") ;
    exit(1) ;
  }
  min_found = T[first] ;
  min_index = first;

  for( index = first ; index < l ; index++ ) {
    if ( !y[index] && T[index] < min_found ) {
      min_found = T[index] ;
      min_index = index ;
    }
  } ;

  return min_index ;
}


void FacilityLocation::run( ) {
  /*
    Data structures:
    - alpha[j] : total price paid by city j
    - beta[i][j] : connection cost to facility i paid by city j
    - T[i] : estimated time to pay off facility i
    - c_ordered[k][2] : connection costs in increasing order
    -- ( c_odered[k][0] , c_ordered[k][1] ) is the k-th cost
    -
  */

  /*Variables declaration*/
  double* alpha ;
  int* connected ;
  double** beta ;
  int** x_temp ;
  int** tight ;
  double* T ;
  int** T_graph ;
  int** T2_graph ;
  int** H_graph ;
  int** c_ordered ;
  int** c_marked ;

  /*Support variables*/
  int r_index , c_index , index , facility_index , city_index , node_index ;
  double index_th , min_found , max_found ;
  int unconnected_cities ;
  double next_time_stamp ;
  double next_tight_time ;
  double current_time ;
  double already_paid ;
  int current_edge ;
  int facility , city ;
  int facility_to_be_opened ;
  int* node_mark;
  int* independent_set;
  int* ncontribcities;
  int facility_found ;
  int tight_found ;


  /*Variables allocation and initialization*/
  alpha = new double[nc] ;
  connected = new int[nc] ;
  ncontribcities = new int[nf] ;
  for( city_index = 0 ; city_index < nc ; city_index++ ) {
    alpha[city_index] = 0.0 ;
    connected[city_index] = 0 ;
  } ;
  T = new double[nf] ;
  beta = new double*[nf] ;
  tight = new int*[nf];
  x_temp = new int*[nf] ;
  c_marked = new int*[nf];
  for( facility_index = 0 ;  facility_index < nf ;  facility_index++ ) {
    T[facility_index] = DBL_MAX ; /*Initially all times are infinite becasue no cities contribute to any facility.*/
    y[facility_index] = 0 ;
    ncontribcities[facility_index] = 0 ;
    beta[ facility_index] = new double[nc] ;
    tight[ facility_index] = new int[nc];
    c_marked[ facility_index] = new int[nc];
    x_temp[ facility_index] = new int[nc];
    for( city_index = 0 ; city_index < nc ; city_index++ ) {
      beta[facility_index][city_index] = 0.0 ;
      tight[facility_index][city_index] = 0 ;
      c_marked[facility_index][city_index] = 0 ;
      x_temp[facility_index][city_index] = 0 ;
      x[facility_index][city_index] = 0 ;
    } ;
  } ;

  c_ordered = new int*[nf*nc];
  for( index = 0 ; index < nf*nc ; index++ ) {
    c_ordered[index] = new int[2];
    c_ordered[index][0] = 0 ;
    c_ordered[index][1] = 0 ;
  } ;


  /*Reorder costs*/

  for( index = nf*nc-1 ; index >=0 ; index-- ){

    max_found = 0.0;

    for( r_index = 0 ; r_index < nf ; r_index++ ) {
      for( c_index = 0 ; c_index < nc ; c_index++ ) {
	if ( !c_marked[r_index][c_index] ) {
	  if ( c[r_index][c_index] >= max_found ){
	    max_found = c[r_index][c_index] ;
	    c_ordered[index][0] = r_index ;
	    c_ordered[index][1] = c_index ;
	  }
	}
      } ;
    } ;


    if ( (c_ordered[index][0] > nf -1) || (c_ordered[index][1] > nc - 1)) {
      printf( "Out of bounds") ;
      exit(1);
    } ;

    c_marked[c_ordered[index][0]][c_ordered[index][1]] = 1 ;

  } ;

  /*While there are unconnected cities*/
  unconnected_cities = nc ;
  current_edge = 0 ;
  current_time = 0 ;

  while( unconnected_cities > 0 ) {

    //printf( "Unconnected cities : %d \n " , unconnected_cities ) ;

    /*Compute next minimum time (here assume that only one edge goes tight at the time*/
    if ( current_edge < nf*nc ) {
      city = c_ordered[current_edge][1] ;
      while ( connected[city]) {
	current_edge++;
	city = c_ordered[current_edge][1] ;
      } ;
      facility = c_ordered[current_edge][0] ;
      next_tight_time = current_time + ( c[ facility ][ city ]  - alpha[city] ) ;
    } else {
      next_tight_time = DBL_MAX ;
    }


    facility_to_be_opened = minimum( T , y , nf ) ;


    next_time_stamp = T[ facility_to_be_opened ] ;


    //printf("Facility to be opened %d  Next Time Stamp: %f \n",facility_to_be_opened, next_time_stamp) ;

    if ( next_tight_time <= next_time_stamp ){


      /*----------------------------  An edge goes tight ---------------------------*/

      current_time = next_tight_time ;

      //printf( "Current time: %f \n",current_time) ;
      //printf( "Edge (%d,%d) tight \n " , facility , city ) ;

      /*Update all tight edges*/
      tight[facility][city] = 1 ;

      /*Update alpha: for all cities that are not connected increment alpha*/
      // printf( "\n\n-------------------- Alpha  -------------------------- \n \n") ;

      for( city_index = 0 ; city_index < nc ; city_index++ ) {
	if ( ! connected[ city_index ] ) {
	  alpha[ city_index ] = current_time ;
	} ;
	//printf( "Alpha[%d] = %f \n",city_index,alpha[city_index]) ;
      } ; /*End update alpha*/

      /*Update beta: update beta of all tight edges for which the cities are unconnected*/
      // printf( "\n\n-------------------- Beta -------------------------- \n \n") ;
      for( facility_index = 0 ; facility_index < nf ; facility_index++ ) {
	for( city_index = 0 ; city_index < nc ; city_index++ ) {
	  if ( tight[ facility_index ][ city_index ] && !connected[ city_index ] ) {
	    beta[ facility_index ][ city_index ] = alpha[ city_index ] - c[ facility_index ][ city_index ] ;
	  } ;
	  //  printf( "Beta[%d][%d] = %f \n",facility_index,city_index,beta[facility_index][city_index]) ;
	} ;
      } ; /*End update beta*/


      if ( ! y[facility] ) { /*Facility is not temporarily opened*/

	ncontribcities[facility]++;
	/*Update estimated time for each facility*/
	for( facility_index = 0 ; facility_index < nf ; facility_index++ ) {
	  already_paid = 0.0 ;
	  tight_found = 0 ;
	  for( city_index = 0 ; city_index < nc ; city_index++ ) {
	    if ( tight[facility_index][city_index] && !connected[ city_index ]  ) { /*If city j is contributing to facility i*/
	      already_paid = already_paid + beta[facility_index][city_index] ;
	      tight_found = 1;
	    } ;
	  } ;
	  if ( tight_found ){
	    if ( ncontribcities[facility_index] == 0 ){
	      //printf( "Number of contrib facilitites equal 0 \n") ;
	      //exit( 1) ;
	      T[facility_index] = DBL_MAX ;
	    } else
	      T[facility_index] = current_time + (f - already_paid) / (double)ncontribcities[facility_index] ;
	  }
	} ; /*End time update*/

      } else { /*Facility is already temporarily opened*/

	/*Connecte the city to the facility*/
	x_temp[facility][city] = 1 ;
	/*Mark the city connected*/
	connected[city] = 1 ;
	unconnected_cities-- ;
	/*Recompute time for all facilities that city was contributing towards*/
	for( facility_index = 0 ; facility_index < nf ; facility_index++ ) {
	  if ( tight[facility_index][city] && (facility_index != facility)) { /*If facility was counting city as contributor*/
	    ncontribcities[facility_index]--;
	  }
	} ;
	for( facility_index = 0 ; facility_index < nf ; facility_index++ ) {
	  already_paid = 0.0 ;
	  tight_found = 0 ;
	  for( city_index = 0 ; city_index < nc ; city_index++ ) {
	    if ( tight[facility_index][city_index] && !connected[ city_index ]  ) { /*If city j is contributing to facility i*/
	      already_paid = already_paid + beta[facility_index][city_index] ;
	      tight_found = 1;
	    } ;
	  } ;
	  if ( tight_found ){
	    if ( ncontribcities[facility_index] == 0 ){
	      //printf( "Number of contrib facilitites equal 0 \n") ;
	      //exit( 1) ;
	      T[facility_index] = DBL_MAX ;
	    } else
	      T[facility_index] = current_time + (f - already_paid) / (double)ncontribcities[facility_index] ;
	  }
	} ; /*End time update*/

      } ;
      current_edge++;
    } ; /*End tight edge event*/


    if ( next_tight_time >= next_time_stamp ) {

      /*----------------------------  A facility is totally paid ---------------------------*/


      current_time = next_time_stamp ;
      //printf( "Current time: %f \n",current_time) ;
      //printf( "Facility to be opened : %d \n " , facility_to_be_opened ) ;

      //printf( "\n\n-------------------- Alpha of unconnected cities -------------------------- \n \n") ;
      for( city_index = 0 ; city_index < nc ; city_index++ ) {
	if ( ! connected[ city_index ] ) {
	  alpha[ city_index ] = current_time ;
	  //printf( "Alpha[%d] = %f \n",city_index,alpha[city_index]) ;
	} ;
      } ; /*End update alpha*/

      /*Update beta: update beta of all tight edges for which the cities are unconnected*/
      //printf( "\n\n-------------------- Beta -------------------------- \n \n") ;
      for( facility_index = 0 ; facility_index < nf ; facility_index++ ) {
	for( city_index = 0 ; city_index < nc ; city_index++ ) {
	  if ( tight[ facility_index ][ city_index ] && ( !connected[ city_index ] ) ) {
	    beta[ facility_index ][ city_index ] = alpha[ city_index ] - c[ facility_index ][ city_index ] ;
	  } ;
	  // printf( "Beta[%d][%d] = %f \n",facility_index,city_index,beta[facility_index][city_index]) ;
	} ;
      } ; /*End update beta*/



      /*Temporarily open the facility*/
      y[facility_to_be_opened] = 1 ;
      T[facility_to_be_opened] = DBL_MAX ; /*Removes this from the time wheel*/
      /*Connect cities to it and update facility times*/
      for( city_index = 0 ; city_index < nc ; city_index++ ) {
	if ( tight[facility_to_be_opened][city_index] && !connected[city_index]) {
 	  connected[city_index] = 1 ;
	  unconnected_cities-- ;
	  x_temp[facility_to_be_opened][city_index] = 1 ;
	  for( facility_index = 0 ; facility_index < nf ; facility_index++ ) {
	    if ( tight[facility_index][city_index] && facility_index != facility_to_be_opened) { /*If facility was counting city as contributor*/
	      ncontribcities[facility_index]--;
	    } ;
	  } ;
	} ;
      } ;

      for( facility_index = 0 ; facility_index < nf ; facility_index++ ) {
	already_paid = 0.0 ;
	tight_found = 0 ;
	for( city_index = 0 ; city_index < nc ; city_index++ ) {
	  if ( tight[facility_index][city_index]  && !connected[city_index] ) { /*If city j is contributing to facility i*/
	    already_paid = already_paid + beta[facility_index][city_index] ;
	    tight_found = 1;
	  } ;
	} ;
	if ( tight_found )
	  T[facility_index] = current_time + (f - already_paid) / (double)ncontribcities[facility_index] ;
      } ; /*End time update*/

    } ;

  } ;

  /*Initialize graphs*/
  node_mark = new int[nf+nc];
  independent_set = new int[nf+nc];
  //opened_facilities = ( int* )malloc( nf*sizeof( int ) ) ;
  T_graph = new int*[nf+nc];
  T2_graph = new int*[nf+nc];
  H_graph = new int*[nf+nc];

  for( index = 0 ; index < nf+nc ; index++ ) {
    node_mark[index] = 0 ;
    independent_set[index] = 0 ;
    T_graph[index] = new int[nf+nc];
    T2_graph[index] = new int[nf+nc];
    H_graph[index] = new int[nf+nc];
    for( node_index = 0 ; node_index < nf+nc ; node_index++ ) {
      T_graph[index][node_index] = 0 ;
      T2_graph[index][node_index] = 0 ;
      H_graph[index][node_index] = 0 ;
    }
  }

  /* Build T: order of nodes <F,C>*/
  for( facility_index = 0 ; facility_index < nf  ; facility_index++ ){
    for( city_index = 0 ; city_index < nc ; city_index++ ){
      if ( beta[facility_index][city_index] > 0.0 ) {
	T_graph[facility_index][nf+city_index] = 1 ;
	T_graph[nf+city_index][facility_index] = 1 ;
      }
      T2_graph[facility_index][nf+city_index] = T_graph[facility_index][nf+city_index]  ;
      T2_graph[nf+city_index][facility_index] = T_graph[nf+city_index][facility_index] ;

    }
  }

  /*Build T^2*/
  for( r_index = 0 ; r_index < nf + nc ; r_index++ ){
    for( c_index = 0 ; c_index < nf+nc ; c_index++ ){
      if ( T_graph[r_index][c_index] ) {
	for ( index = 0 ; index < nf + nc ; index++) {
	  if ( T_graph[c_index][index] && r_index != index) {
	    T2_graph[r_index][index] = 1 ;
	    T2_graph[index][r_index] = 1 ;
	  }
	}

      }
    }
  }

  /*Build H*/
  for( facility_index = 0 ; facility_index < nf ; facility_index++ ){
    if ( y[facility_index] ) {
      for( c_index = 0 ; c_index < nf+nc ; c_index++ ){
	H_graph[facility_index][c_index] = T2_graph[facility_index][c_index]  ;
	H_graph[c_index][facility_index] = T2_graph[c_index][facility_index]  ;
      }
    }
  }

  /*Maximal Independent set on H*/
  for( r_index = 0 ; r_index < nf+nc ; r_index++) {
    if ( ! node_mark[r_index] &&
	 (  r_index >= nf || (  r_index < nf && y[ r_index ] ) ) )  {
      independent_set[r_index] = 1 ;
      node_mark[r_index] = 1 ;
      //printf( "Including node %d in the independent set\n" , r_index);
      /*mark all nodes adjacient to this*/
      for( c_index = 0 ; c_index < nf+nc ; c_index++){
	if( H_graph[r_index][c_index] ) {
	  //printf( "Node %d is removed \n" , c_index);
	  node_mark[c_index] = 1 ;
	}
      }
    }
  }


  /*Finalize x*/

  for( city_index = 0 ; city_index < nc ; city_index++ ) {
    /*Find the facility this city is connected to*/
    facility_found = 0;
    for( facility_index = 0 ; facility_index < nf ; facility_index++ ) {
      if ( !facility_found &&
	   beta[facility_index][city_index] > 0 &&  /*look among special edges*/
	   independent_set[facility_index] ) { /*Directly connected*/
	x[facility_index][city_index] = 1 ;
	//printf( "City %d connected to facility %d \n", city_index , facility_index) ;
	facility_found = 1;
      }
    }
    if (!facility_found){
      for( facility_index = 0 ; facility_index < nf ; facility_index++ ) {
	if ( !facility_found && tight[facility_index][city_index] &&
	     x_temp[facility_index][city_index] ){ /*look for tight edge*/
	  if ( independent_set[facility_index] ) { /*Directly connected*/
	    x[facility_index][city_index] = 1 ;
	    //printf( "City %d connected to facility %d \n", city_index , facility_index) ;
	    facility_found = 1;
	  } else { /*Indirectly connected*/
	    for( index = 0 ; index < nf ; index++ ){
	      if ( !facility_found && independent_set[index] && H_graph[index][facility_index] ) {
		x[index][city_index] = 1 ;
		//printf( "City %d connected to facility %d indirectly\n", city_index , index) ;
		facility_found = 1;
	      }
	    }
	  }
	}
      }
    }

  }
  /*Finalize y*/
  n_opened_facilities = 0 ;
  for( index = 0 ; index < nf ; index++){
    y[index] = independent_set[index];
    if (independent_set[index]){
      //printf( "Facility %d opened \n" , index);
      n_opened_facilities++;
    }
  };


  /*Memory deallocation*/
  delete alpha ;
  delete T ;
  delete connected;
  delete node_mark;
  delete independent_set;
  delete ncontribcities;

  for( index = 0 ; index < nf ; index++ ) {
    delete beta[index] ;
    delete tight[index];
    delete x_temp[index];
    delete c_marked[index];
  } ;
  delete beta ;
  delete tight;
  delete x_temp;
  delete c_marked;

  for( index = 0 ; index < nf+nc ; index++ ) {
    delete T_graph[index];
    delete T2_graph[index];
    delete H_graph[index];

  } ;

  delete T_graph;
  delete T2_graph;
  delete H_graph;

  for( index = 0 ; index < nf*nc ; index++ ) {
    delete c_ordered[index];

  } ;
  delete c_ordered ;

  /*Return number of facilities opened*/

} ;

