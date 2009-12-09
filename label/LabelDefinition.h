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


#ifndef LABELDEFINITION_H_
#define LABELDEFINITION_H_

#include <string>
#include "LabelMap.hpp"
#include "LabelSet.hpp"
#include "../position/Xyz.h"
#include "../position/RoutingTable.h"
#include "../core/IdGraph.h"
#include "../core/Node.h"
#include "../core/Link.h"

using namespace std ;

namespace cosi {

/** This heder file contains the definition of label map and label set types.
 *
 *
 *
 *@author Alessandro Pinto,
 *@author The Donald O. Pederson Center for Electronic System Design
 *@author Electrical Engineering Department
 *@author University of California, Berkeley
 *@author 545P Cory Hall, Berkeley, CA 94720
 *@author +1 (510) 642 9670
 *@author http://www.eecs.berkeley.edu/~apinto
 *
 *@version $Id: LabelDefinition.h,v 1.6 2008/12/09 23:28:22 apinto Exp $

 */


/**\brief Assigns a position to each vertex.
 *
 * A vertex is associated to a position in the Euclidean space.
 *
 * @see Xyz .*/
typedef LabelMap< Xyz > MapXyz;

/**\brief Assigns a routing table to each vertex.
 *
 * A vertex is associated to a routing table.
 *
 * @see RoutingTable .*/
typedef LabelMap< RoutingTable > MapRoutingTable;

typedef LabelMap<  long int> MapBandwidth ;

typedef LabelMap<  double > MapArea;

typedef LabelMap<  string > MapInterfaceType;

typedef LabelMap<  int > MapHopConstraint;

typedef LabelMap< Node > NodeMap ;

typedef LabelMap<  Link > LinkMap ;


}

#endif /*LABELDEFINITION_H_*/
