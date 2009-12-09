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
#include <map>
#include <vector>
#include "stdio.h"
#include "stdlib.h"
#include "float.h"
#include "limits.h"
#include "math.h"
#include "FacilityLocation.h"

#ifndef _K_MEDIAN_H_
#define _K_MEDIAN_H_


class KMedian
{
public:


    //    CONSTRUCTORS


    /** \brief k-median algorithm constructor
     *
     *  The constructor only sets the initialization
     *  flag to false indicating that the algorithm
     *  class has never been initialized.
     */
    KMedian( ) ;


    //    DESTRUCTOR


    /** \brief k-median destructor
     *
     * The destructor releases the memory allocated to
     * store the opened facilities and the allocations
     * of cities to facilities.
     */
    ~KMedian( ) ;


    //    PUBLIC METHODS AND MEMBERS


    /** \brief Initializes the algorithm.
     *
     *The initialize method first cheks if this is the first
     *initialization. If it is not, it releases the memory
     *that was allocated in the previous execution of the
     *algorithm. Then, it allocates enough memory for storing
     *the result, the position of cities and facilities.
     *It also assignes the parameters to internal
     *variables.
     */

    void initialize( Xy_Flow_Graph* PtP , NocPlatformRefined* Plt , int i_k) ;

    //void initialize( Graph* g ,
    //	   		   Graph* gmax ,
    //           Parameters* p ,
    //         int i_k) ;


    /** \brief Runs the k-median algorithm.
     *
     *The k-median algorithm computes one vector and one
     *matrix with the following meanings:
     *    - vector \f$ y \f$ is the vector representing the
     *      facilities that have been opened: \f$ y[i] = 1 \iff \f$
     *      facility \f$ i \f$ has been opened.
     *    - Matrix \f$ x \f$ is the matrix representing the allocation
     *      of cities to facilities: \f$ x[i][j] = 1 \iff \f$ city \f$ j\f$
     *      is connected to facility \f$ i \f$.
     */
    int run( );


    int** getConnectionMatrix( ) ;

    int* getOpenVector( );

    int getNofFacilities( ) ;


    /** \brief Returns a graph as result of k-median.
     *
     *The graph is built in the following way:
     *    - the number of vertices is equal to the number of
     *      facilities plus the number of cities. The number
     *      of sources is given as a parameter, the rest of the
     *      cities are taken as destinations.
     *    - The edges are directly taken from the matrix \f$ x \f$.
     *    - The position of sources, destinations and other node
     *      are available as input to the k-median problem
     *    - indegree, outdegree are also set.
     *    - no commodities, flow, input ports, output ports are assigned.
     */

     /**\brief Given a source or a destination return the network interface it is connected to.*/
     int ConnectedToFacility( int V ) ;

     /**\brief returns a point to point graph among network interfaces.*/

     map< int , int > GetCityToFacility( ) ;

private:


    //    PRIVATE METHODS AND MEMBERS



    double dist( double** F , int f1, int f2) ;
    int nf ;
    int nc ;
    int k;
    double** F;
    double** C;
    int** x ;
    int* y;
    int is_initialized ;
    int n_opened_facilities ;
    Xy_Flow_Graph* mG ;
    NocPlatformRefined* mPlt ;
  	map< int , int > mCityToFacility ;
    map<int,int> mIndex2Id ;


};


//int
//k_median( int nf, /*Number of available facilities*/
//	  int nc, /*Number of cities*/
//	  int k, /*Maximum number of facilities to be opened*/
//	  double** F, /*Facility location*/
//	  double** C, /*City location*/
//	  int** x , /*Connection matrix: must be allocated and nfxnc*/
//	  int* y ) ; /*Opening vector: must be allocated and nfx1*/



#endif
