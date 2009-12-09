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
#include "Technology.h"
#include <fstream>
#include <iostream>
#include "math.h"

using namespace std ;

TechnologyNode GetTechParameters(  string fname )
{

    TechnologyNode TechParam ;
    string token ;
    ifstream f ;
    int layer ;
    int nlayer ;
    double c ;
    double r ;


    f.open( fname.c_str() ) ;

    cout << "Parsing technology data from " << fname << endl ;

    f >> TechParam.name ;

    //Parse the technology file.
    f >> token ;
    while ( token != "router" )
    {

        if ( token == "fclk" )
        {
            f >> TechParam.fclk ;
            cout << "Clock frequency " << TechParam.fclk << endl ;
        }
        else if ( token == "V_DD")
        {
            f >> TechParam.V_DD ;
            cout << "VDD " << TechParam.V_DD << endl ;
        }
        else if ( token == "I_off" )
        {
            f >> TechParam.I_off ;
            cout << "I_off " << TechParam.I_off << endl ;
        }
        else if ( token == "W_min" )
        {
            f >> TechParam.W_min ;
            cout << "W_min " << TechParam.W_min << endl ;
        }
        else if ( token == "I_sc" )
        {
            f >> TechParam.I_sc ;
           cout  << "I_sc " << TechParam.I_sc << endl ;
        }
        else if ( token == "r_0" )
        {
            f >> TechParam.r_0 ;
            cout << "r_0 " << TechParam.r_0 << endl ;
        }
        else if ( token == "c_p" )
        {
            f >> TechParam.c_p ;
            cout << "c_p " << TechParam.c_p << endl ;
        }
        else if (token == "c_0" )
        {
            f >> TechParam.c_0 ;
            cout << "c_0 " << TechParam.c_0 << endl ;
        }
        else if ( token == "nlayers")
        {
            f >>  nlayer ;
            TechParam.nlayer = nlayer  ;
            TechParam.c = new double[TechParam.nlayer];
            TechParam.r = new double[TechParam.nlayer];
        }
        else if ( token == "layer")
        {
            f >> layer ;
        }
        else if ( token == "c")
        {
            if ( layer < TechParam.nlayer )
            {
                f >> c ;
                TechParam.c[layer] = c  ;
            }
        }
        else if ( token == "r" )
        {
            if ( layer < TechParam.nlayer )
            {
                f >> r ;
                TechParam.r[layer] = r ;
            }
        }
        f >> token ;
    }


    //READ THE ROUTER ENERGY MODEL

    int nin , nout ;
    f >> nin ;
    f >> nout ;

    TechParam.Erouter = new double*[nin] ;
    TechParam.Arouter = new double*[nin] ;
    TechParam.ErouterLeak = new double*[nin] ;


    for ( int i = 0 ; i < nin ; i++ )
    {
        TechParam.Erouter[i] = new double[nout] ;
        TechParam.ErouterLeak[i] = new double[nout] ;
        for ( int j = 0 ; j < nout ; j++ )
        {
            f >> TechParam.Erouter[i][j] >> TechParam.ErouterLeak[i][j] ;
        }
    }

    for ( int i = 0 ; i < nin ; i++ )
    {
        TechParam.Arouter[i] = new double[nout] ;
        for ( int j = 0 ; j < nout ; j++ )
        {
            f>> TechParam.Arouter[i][j];
            TechParam.Arouter[i][j] = TechParam.Arouter[i][j]*1e-12 ;
            if ( TechParam.name == "70nm")
            {
                TechParam.Arouter[i][j] = TechParam.Arouter[i][j]*0.5 ;
            }
            else if ( TechParam.name == "50nm" )
            {
                TechParam.Arouter[i][j] = TechParam.Arouter[i][j]*0.25 ;
            }
        }
    }


    // COMPUTE OTHER PARAMETERS

    double lst ; //Critical sequential length


    //Compute optimal repeater size
    TechParam.kopt = ( int ) sqrt( TechParam.r_0 * ( TechParam.c[6] ) / (  TechParam.r[6] *TechParam.c_0)) ;
    //Optimal sequential length
    TechParam.hopt = sqrt( 2*TechParam.r_0*( TechParam.c_0 + TechParam.c_p)/( TechParam.r[6] * ( TechParam.c[6] ) )) ;
    //Delay per unit length
    TechParam.d = 2*sqrt( TechParam.r_0*TechParam.c_0*(TechParam.r[6])*(TechParam.c[6]) )*( 1+ sqrt( 0.5 * (1 + TechParam.c_p/TechParam.c_0) ) );


    //Compute Energy and Power
    double k1, k2 , k3 ;

    //Dynamic power multiplier
    k1 = 0.25*TechParam.V_DD*TechParam.V_DD*TechParam.fclk;
    //Static power multiplier
    k2 = 0.5*TechParam.V_DD*3*TechParam.I_off*TechParam.W_min ;
    //Short circuit power multiplier
    k3 = 0.25*TechParam.V_DD*TechParam.W_min*TechParam.I_sc*TechParam.fclk*1.1;

    double P , Pdyn , Psc ;
    double Edyn , Esc ;
    //Dynamic power
    Pdyn = k1 * ( TechParam.kopt*(TechParam.c_0+TechParam.c_p)/TechParam.hopt+(TechParam.c[6])) ;
    TechParam.Edyn = Pdyn / TechParam.fclk ;
    //Leackage power
    TechParam.Pl = k2*TechParam.kopt/TechParam.hopt ;
    //Short circuit power
    Psc = k3*TechParam.kopt*TechParam.d ;
    TechParam.Esc = Psc/TechParam.fclk ;
    //Critical sequential length
    TechParam.lst = 1/(TechParam.fclk*TechParam.d) ;

    //Area: by now it is constant
    TechParam.Winst = 32*100e-9 ;
    TechParam.Rinst = 0.1e-6 ;

    return TechParam ;

}

void PrintTechnologyNode( TechnologyNode N )
{

    cout << "Technology " << N.name << endl ;
    cout << "Target clock " << N.fclk*1e-9 << " GHz" << endl ;
    cout << "VDD " << N.V_DD << " V" << endl ;
    cout << "Off current " << N.I_off*1e6 << " uA" << endl ;
    cout << "Minimum sized repeater's transistor size " << N.W_min*1e9 << " nm"<< endl ;
    cout << "Short cirtuit current " << N.I_sc*1e6 << " uA" <<  endl ;
    cout << "Transistor output resistance " << N.r_0*1e-3 << " KOhm" <<  endl ;
    cout << "Transitor output capacitance " << N.c_p*1e15<<" fF" << endl ;
    cout << "Tranisotr input capacitance " << N.c_0 << " fF" << endl << endl  ;
    cout << "Wire Model " << endl ;
    cout << "Critical Sequential Length " << N.lst*1e3 << " mm" << endl ;
    cout << "Energy per unit length " << N.Edyn*1e9 << " pJ/mm" << endl ;
}
