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
#include "Kmedian.h"

KMedian::KMedian( )
{
    is_initialized = 0 ;
}

KMedian::~KMedian( )
{
    for( int i = 0 ; i < nf ; i++)
        delete x[i] ;
    delete x ;
    delete y ;
}

void KMedian::initialize( Xy_Flow_Graph* G , NocPlatformRefined* Plt , int i_k )
{

    int index , r_index , c_index;
    if ( is_initialized)
    {
        for(  index = 0 ; index < nf ; index++)
        {
            delete x[index] ;
            delete F[index] ;
        }
        for(  index = 0 ; index < nc ; index++)
            delete C[index] ;
        delete x ;
        delete y ;
        delete F ;
        delete C ;
    }
    else
    {
        is_initialized = 1 ;
    }
    mG = G ;
    mPlt = Plt ;

    //Number of Cities
    nc =  G->Ns( ) + G->Nd( )  ;
    C = new double*[nc] ;

    //  ASSIGN CITIES
    for( index = 0 ; index <  nc ; index++ )
    {
        C[index] = new double[2] ;
    }

    for( index = 0 ; index <  mG->Ns( ) ; index++ )
    {
        C[index][0] = mG->GetX( index ) ;
        C[index][1] = mG->GetY( index ) ;
    }

    for( index = mG->Ns( ) ; index <  nc ; index++ )
    {
        C[index][0] =  mG->GetX( index ) ;
        C[index][1] =  mG->GetY( index ) ;
    }

    //  ASSIGN FACILITIES
    //Count the number of allowed points
    //and build a map from the indexes to the id

    nf = 0 ;

    vector<int> Facilities = mPlt->GetCandidateCenterId() ;

    for( r_index = 0 ; r_index < Facilities.size() ; r_index++)
    {
            mIndex2Id[nf] = Facilities[r_index] ;
            nf++ ;
    }

    F = (double**)malloc(nf*sizeof(double*)) ;
	nf = 0 ;
    for( r_index = 0 ; r_index < Facilities.size() ; r_index++)
    {
            F[ nf ] = (double*)malloc(2*sizeof(double));
            F[ nf ][0] = mPlt->GetX( Facilities[r_index])  ;
            F[ nf ][1] =  mPlt->GetY( Facilities[r_index] ) ;
        	nf++ ;
    } ;

    x = new int*[nf];
    for(index=0;index<nf;index++)
    {
        x[index] = new int[nc];
    }
    y = new int[nf] ;
    k = i_k ;

}



int** KMedian::getConnectionMatrix( )
{
    return x ;
}

int* KMedian::getOpenVector( )
{
    return y ;
}

int KMedian::getNofFacilities( )
{
    return n_opened_facilities ;
}

int KMedian::run ( )
{
	cout << "Running K-median : Number of cities " <<  nc << "  Number of facilities " << nf << "   Number of medians  " << k << endl ;
    double** cost ; /*Computed connection cost*/
    int index, r_index, c_index;
    double maxcost;
    double mincost;
    double z1,z2;
    int n_opened_facilities1;
    int n_opened_facilities2;
    int n_opened_facilities;
    double current_cost;
    int closest;
    int card_B_p;
    int card_D ;
    int** x1;
    int** x2;
    int* y1;
    int *y2;
    int *B_p;
    int *D ;
    int *B_minus_B_p ;
    int *closest_facility;
    int* paired;
    int n_additional_facilities ;
    double f_random ;
    int i_random ;
    double a_prob , b_prob;
    double E_A ;
    double E_B ;
    double E_D ;
    double min_E ;
    int min_E_index;
    int facility_found ;
    int i1 , i2 ;

    x1 = new int*[nf] ;
    x2 = new int*[nf] ;
    y1 = new int[nf] ;
    y2 = new int[nf] ;

    for( index = 0 ; index < nf ; index++ )
    {
        x1[index] = new int[nc];
        x2[index] = new int[nc];
    } ;

    cost = new double*[nf];

    for( index = 0 ; index < nf ; index++ )
    {
        cost[index] = new double[nc];
    } ;

    /*Compute distance*/
    maxcost = 0.0 ;
    mincost = DBL_MAX;
    for( r_index = 0 ; r_index < nf ; r_index++ )
    {
        for( c_index = 0 ; c_index < nc ; c_index++ )
        {
            cost[r_index][c_index] = fabs( F[r_index][0] - C[c_index][0] ) + fabs( F[r_index][1] - C[c_index][1] ) ;
            if ( cost[r_index][c_index] > maxcost )
                maxcost = cost[r_index][c_index];
            if ( cost[r_index][c_index] < mincost && cost[r_index][c_index] > 0.0 )
                mincost = cost[r_index][c_index];
        }
    }


    /*binary search between (nc+nf)*maxcost and zero*/
    /*stop when z1-z2 < mincost/(12*nc^2)*/
    current_cost = (nc+nf)*maxcost;
    z1 = maxcost ;
    z2 = 0.0 ;
    FacilityLocation* fl = new FacilityLocation;
    //n_opened_facilities = 0 ;

    while( z1 - z2 > mincost/(12*nc*nc ) )
    {

        //while( n_opened_facilities < k ) {

        fl->initialize( nf , nc , current_cost , cost );
        fl->run( );
        x = fl->getConnectionMatrix();
        y= fl->getOpenVector();
        n_opened_facilities = fl->getNofFacilities();

        if ( n_opened_facilities < k )
        {
            current_cost = current_cost - current_cost/2;
            z1 = current_cost ;
            n_opened_facilities1 = n_opened_facilities ;
            for( r_index = 0 ; r_index < nf ; r_index++ )
            {
                y1[r_index] = y[r_index];
                for( c_index = 0 ; c_index < nc ; c_index++ )
                {
                    x1[r_index][c_index] = x[r_index][c_index] ;
                }
            }
        }
        else
        {
            current_cost = current_cost + current_cost/2;
            z2 = current_cost;
            n_opened_facilities2 = n_opened_facilities ;
            for( r_index = 0 ; r_index < nf ; r_index++ )
            {
                y2[r_index] = y[r_index];
                for( c_index = 0 ; c_index < nc ; c_index++ )
                {
                    x2[r_index][c_index] = x[r_index][c_index] ;
                }
            }
        }
        printf( "Current cost: %f \t Number of facilities: %d\n" , current_cost, n_opened_facilities );
    }

    if ( n_opened_facilities2 == k )
    {

        for( r_index = 0 ; r_index < nf ; r_index++ )
        {

            for( c_index = 0 ; c_index < nc ; c_index++ )
            {
                x[r_index][c_index] = x2[r_index][c_index] ;

            }

        }



        for( index = 0 ; index < nf ; index++ )
        {
            //delete cost[index];
            delete x1[index];
            delete x2[index];
        }

        //delete cost;
        delete x1;
        delete x2;
        delete y1;
        delete y2;

        n_opened_facilities = n_opened_facilities2 ;

    }
    else
    {


        /*Reset x, y */
        for( r_index = 0 ; r_index < nf ; r_index++ )
        {
            y[r_index] = 0;
            for( c_index = 0 ; c_index < nc ; c_index++ )
            {
                x[r_index][c_index] = 0 ;
            }
        }

        /*Compute B'*/
        B_p = new int[nf] ;
        B_minus_B_p = new int[nf];
        D = new int[nf];
        closest_facility = new int[nf];
        paired = new int[nf];
        for( index = 0 ; index < nf ; index++)
        {
            B_p[index] = 0 ;
            D[index] = 0 ;
            B_minus_B_p[index] = y2[index] ;
            paired[index] = 0 ;
        }
        card_B_p = 0 ;

        /*Pair up facilities*/
        for( r_index = 0 ; r_index < nf ; r_index++)
        { /*For each facility in y1*/
            if( y1[r_index] )
            {
                mincost = DBL_MAX ;
                for( c_index = 0 ; c_index < nf ; c_index++)
                {
                    if ( y2[c_index] && !paired[c_index] && (dist( F , r_index , c_index ) < mincost) )
                    {
                        mincost = dist( F , r_index , c_index ) ;
                        closest = c_index ;
                    }
                }
                //printf( "Facility %d closest to %d \n", closest , r_index) ;
                B_p[closest] = 1;
                card_B_p++ ;
                B_minus_B_p[closest] = 0;
                closest_facility[r_index] = closest ;
                paired[closest] = 1 ;
            }
        }

        //printf( "Cardinality of B_p %d , should be %d \n", card_B_p , n_opened_facilities1);

        a_prob = (double)(n_opened_facilities2 - k)/(double)(n_opened_facilities2 - n_opened_facilities1) ;
        b_prob = 1 - a_prob ;

        /*Compute expected value in the case of A and B_p*/
        E_A = 0.0 ;
        E_B = 0.0 ;
        for( c_index = 0 ; c_index < nc ; c_index++)
        { /*For all cities*/
            for( r_index = 0 ; r_index < nf ; r_index++)
            {
                if ( x2[r_index][c_index] )
                {
                    i2 = r_index;
                }
                if ( x1[r_index][c_index] )
                {
                    i1 = r_index;
                }
            }
            if ( B_minus_B_p[i2])
            {
                E_A = E_A + b_prob*cost[i2][c_index] + a_prob*cost[i1][c_index] ;
                E_B = E_B + cost[i2][c_index] + 2*a_prob*cost[i1][c_index] ;
            }
            else
            {
                E_A = E_A + cost[i1][c_index] ;
                E_B = E_B + cost[i2][c_index] ;
            }
        }

        /*Pick the set A or the set B*/
        if ( E_A > E_B)
        {
            printf( "Set B opened ... \n" ) ;
            for ( index = 0 ; index < nf ; index++)
            {
                y[index] = B_p[index];
            }
        }
        else
        {
            printf( "Set A opened ... \n" ) ;
            for ( index = 0 ; index < nf ; index++)
            {
                y[index] = y1[index];
            }
        }

        /*Pick the set D properly*/
        if ( E_A > E_B )
        {
            card_D = 0 ;
            /*until cardinality of D < k - k1 */
            while ( card_D < k - n_opened_facilities1 )
            {
                /*pick an element in B_minus_B_p that minimizies E_D*/
                min_E = DBL_MAX ;
                for ( index = 0 ; index < nf ; index++)
                {
                    if ( B_minus_B_p[index] )
                    {
                        /*try with this element and see if E is minimum*/
                        E_D = 0.0 ;
                        for( c_index = 0 ; c_index < nc ; c_index++)
                        { /*For all cities*/

                            for( r_index = 0 ; r_index < nf ; r_index++)
                            {
                                if ( x2[r_index][c_index] )
                                {
                                    i2 = r_index;
                                }
                                if ( x1[r_index][c_index] )
                                {
                                    i1 = r_index;
                                }
                            }

                            if ( B_p[i2] || D[i2] || i2 == index )
                                E_D = E_D + cost[i2][c_index] ;
                            else
                                E_D = E_D + cost[i2][c_index] + 2*a_prob*cost[i1][c_index] ;
                        }
                        if ( E_D < min_E)
                        {
                            min_E = E_D ;
                            min_E_index = index ;
                        }
                    }
                }
                //printf ( "Facility %d added in D \n",min_E_index) ;
                D[min_E_index] = 1 ;
                card_D++;
                y[min_E_index] = 1 ;
                B_minus_B_p[min_E_index] = 0 ;
                //printf("Number of facilities opened = %d \n",card_D + card_B_p);
            }

        }
        else
        {

            card_D = 0 ;
            /*until cardinality of D < k - k1 */
            while ( card_D < k - n_opened_facilities1 )
            {
                /*pick an element in B_minus_B_p that minimizies E_D*/
                min_E = DBL_MAX ;
                for ( index = 0 ; index < nf ; index++)
                {
                    if ( B_minus_B_p[index] )
                    {
                        /*try with this element and see if E is minimum*/
                        E_D = 0.0 ;
                        for( c_index = 0 ; c_index < nc ; c_index++)
                        { /*For all cities*/

                            for( r_index = 0 ; r_index < nf ; r_index++)
                            {
                                if ( x2[r_index][c_index] )
                                {
                                    i2 = r_index;
                                }
                                if ( x1[r_index][c_index] )
                                {
                                    i1 = r_index;
                                }
                            }

                            if ( B_p[i2] || D[i2] || i2 == index )
                                E_D = E_D + cost[i1][c_index] ;
                            else
                                E_D = E_D + b_prob*cost[i2][c_index] + a_prob*cost[i1][c_index] ;

                        }
                        if ( E_D < min_E)
                        {
                            min_E = E_D ;
                            min_E_index = index ;
                        }
                    }
                }
                printf ( "Facility %d added in D \n",min_E_index) ;
                D[min_E_index] = 1 ;
                y[min_E_index] = 1 ;
                card_D++;
                B_minus_B_p[min_E_index] = 0 ;

            }
            //printf("Number of facilities opened = %d \n",card_D + n_opened_facilities1);
        }

        /*Connect cities*/
        for( c_index = 0 ; c_index < nc ; c_index++)
        { /*For all cities*/

            for( r_index = 0 ; r_index < nf ; r_index++)
            {
                if ( x2[r_index][c_index] )
                {
                    i2 = r_index;
                }
                if ( x1[r_index][c_index] )
                {
                    i1 = r_index;
                }
            }

            if ( y[i2] )
            {
                x[i2][c_index] = 1 ;
                //printf("City %d connected to facility %d \n",i2,c_index) ;
            }
            else
            {
                if ( y[i1] )
                {
                    x[i1][c_index] = 1 ;
                    //printf("City %d connected to facility %d \n",i1,c_index) ;
                }
                else
                {
                    x[closest_facility[i1]][c_index] = 1 ;
                    //printf("City %d connected to facility %d \n",closest_facility[i1],c_index) ;
                }
            }
        }


        n_opened_facilities = 0 ;
        for( index = 0 ; index < nf ; index++)
        {
            if ( y[index])
            {
                n_opened_facilities++;
            }
        }

        printf( "I've opened %d facilities \n",n_opened_facilities) ;

        delete B_p ;
        delete B_minus_B_p ;
        delete D ;
        delete closest_facility ;
        delete paired ;

        for( index = 0 ; index < nf ; index++ )
        {
            //delete cost[index];
            delete x1[index];
            delete x2[index];
        }

        //delete cost;
        delete x1;
        delete x2;
        delete y1;
        delete y2;

    }

    for( index = 0 ; index < nf ;index++ )
    {
        for( c_index = 0 ; c_index < nc ; c_index++ )
        {
            if ( x[index][c_index] && ( cost[index][c_index] > 1.1*mPlt->WireLst() ))
            {
                for( index = 0 ; index < nf ; index++ )
                {
                    delete cost[index];
                }
                delete cost;
                return 0 ;
            }
        }
    }

    for( index = 0 ; index < nf ; index++ )
    {
        delete cost[index];
    }
    delete cost;



    for( int i = 0 ; i < nf ; i++ )
    {
        for ( int j = 0 ; j < nc ;j++ )
        {

            if ( x[i][j] )
            {
                mCityToFacility[j] = i ;

            }
        }
    }


    return 1 ;

}


double KMedian::dist( double** F , int f1, int f2)
{

    return (  fabs(F[f1][0] - F[f2][0]) + fabs(F[f1][1] - F[f2][1]) )  ;

}



//Xy_Flow_Graph* KMedian::toGraph( )
//{

//
//    Graph* g ;
//    int index , r_index , c_index ;
//
//    g = new Graph( ns , nc - ns , nf) ;
//
//    //  ASSIGN POSITION OF SOURCES
//    for( index = 0 ; index < ns ; index++ )
//    {
//        ( g->S[index] ).x = C[index][0] ;
//        ( g->S[index] ).y = C[index][1] ;
//    }
//    //  ASSIGN POSITION OF DESTINATIONS
//    for( index = 0 ; index < nc - ns ; index++ )
//    {
//        ( g->D[index] ).x = C[ns + index][0] ;
//        ( g->D[index] ).y = C[ns + index][1] ;
//    }
//    //  ASSIGN POSITION OF ROUTERS
//    for( index = 0 ; index < nf ; index++ )
//    {
//        ( g->R[index] ).x = F[index][0] ;
//        ( g->R[index] ).y = F[index][1] ;
//    }
//
//    //  GRAPH INITIALIZATION
//    for( r_index = 0 ; r_index < nc + nf ; r_index++ )
//    {
//        // Indegree
//        g->indegree[r_index] = 0 ;
//        g->outdegree[r_index] = 0 ;
//        for( c_index = 0 ; c_index < nc + nf ; c_index++ )
//        {
//            g->Adj[r_index][c_index] = 0 ;
//        }
//    }
//
//    //  ADJ MATRIX: SOURCES TO ROUTERS
//    for( r_index = 0 ; r_index < ns ; r_index++ )
//    {
//        for( c_index = nc ; c_index < nc + nf ; c_index++ )
//        {
//            g->indegree[c_index] ++ ;
//            g->outdegree[r_index] ++ ;
//            g->Adj[r_index][c_index] = x[c_index - nc][r_index];
//
//        }
//    }
//
//    //  ADJ MATRIX: DESTINATIONS TO ROUTERS
//    for( r_index = nc ; r_index < nc + nf ; r_index++ )
//    {
//        for( c_index = ns ; c_index < nc ; c_index++ )
//        {
//            g->indegree[c_index] ++ ;
//            g->outdegree[r_index] ++ ;
//            g->Adj[r_index][c_index] = x[r_index - nc][c_index];
//        }
//    }
//
//    return g ;

//}


int KMedian::ConnectedToFacility( int V )
{
    return mIndex2Id[ mCityToFacility[V] ] ;
}

map< int , int > KMedian::GetCityToFacility( )
{
	map<int,int> RetVal;
	map<int,int>::iterator It ;
	for ( It =mCityToFacility.begin() ; It != mCityToFacility.end( ) ; It++  ) {
		RetVal[ It->first ] = mIndex2Id[ mCityToFacility[It->first] ] ;
	}
    return RetVal;

}


