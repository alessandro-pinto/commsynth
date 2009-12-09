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
/**This class generates networks on demand.
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
 *@version $Id: Factory.h,v 1.4 2008/12/07 23:20:20 apinto Exp $
 */

#ifndef FACTORY_H_
#define FACTORY_H_

#include <string>
#include <vector>
#include <list>
#include "IdGraph.h"
#include "Network.hpp"

using namespace std ;

namespace cosi {

class Factory {
public:
	Factory();
	virtual ~Factory();

	//Define the object to generate
	void AllocateNetwork( string pType );
	void SetNetworkName(string pName);
	//Fill in the graph
	void AddVertex(Vertex pV);
	void AddVertex(Vertex pV,string pName);
	void AddEdge( Edge pE ) ;
	void AddEdge( Edge pE , string pName) ;
	//Fill in the labels
	void SetLabelMapEntry(string pMapName , Vertex pV , list<string> pString , list<int> pInt , list<double> pDouble );
	void SetLabelMapEntry(string pMapName , Edge pE , list<string> pString , list<int> pInt , list<double> pDouble );


	//Return the object
	IdGraph* GetNetwork( );

private:

	IdGraph* mNetwork;

};

}

#endif /*FACTORY_H_*/
