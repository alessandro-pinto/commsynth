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
#ifndef _TECHNOLOGY_H_
#define _TECHNOLOGY_H_

#include <string>
using namespace std ;

/** Definition of the technology parameters data structure.
 *
 *@author Alessandro Pinto,
 *@author apinto@eecs.berkeley.edu,
 *@author EECS Grad Student,
 *@author 211 Cory Hall #1772,
 *@author Berkeley, CA, 94720-1772
 *@author (510)-6429670

 *@version $Id: Technology.h,v 1.3 2008/12/09 23:28:22 apinto Exp $
 */


typedef struct
{
	/**\brief Technology name.*/
    string name ;
    /**\brief Target clock.*/
    double fclk ;
    /**\brief Number of metal layers*/
    int nlayer;
    /**\brief Output resistance of a minimum sized transitor.*/
    double r_0;
    /**\brief Output capacitance of the minimum sized transitor.*/
    double c_p;
    /**\brief Input capacitance of the minimum sized transitor.*/
    double c_0;
    /**\brief Capacitance per unit lenght. One value for each metal layer.*/
    double* c;
    /**\brief Resistance per unit lenght. One value for each metal layer.*/
    double* r;
    /**\brief Supply voltage.*/
    double V_DD;
    /**\brief Transistor's off current.*/
    double I_off;
    /**\brief Minimum transistor width.*/
    double W_min;
 	/**\brief Transistor's short-circuit current.*/
    double I_sc;
    /**\brief Router's energy consumption
     * Erouter[i][j] is the energy consumption
     * of a router with i+1 input ports and j+1 output ports.
     * This values are taken from Orion.
     * */
    double** Erouter ;
    /**\brief Optimal repeater size. Computed from previous values.*/
    double** ErouterLeak ;
    int kopt ;
    /**\brief Optimal inter-repeater distance. Computed from previous values.*/
    double hopt ;
    /**\brief Delay per unit length. Computed from previous values.*/
    double d ;
    /**\brief Dynamic energy consumption. Computed from previous values.*/
    double Edyn ;
    /**\brief Leakage power. Computed from previous values.*/
    double Pl ;
    /**\brief Short circuit energy. Computed from previous values.*/
    double Esc ;
    /**\brief Critical sequential length. Computed from previous values.*/
    double lst ;
	/**\brief Wire area (we consider 32 bit).*/
    double Winst ;
    /**\brief Router area (we consider 0.1mm^2*/
    double Rinst ;

    double** Arouter ;

} TechnologyNode ;

/**\brief Reads the tehcnology specification from a file.*/
TechnologyNode GetTechParameters( std::string fname ) ;

void PrintTechnologyNode( TechnologyNode N ) ;

#endif
