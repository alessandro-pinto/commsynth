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

#ifndef LIBRARIES_NODE_H_
#define LIBRARIES_NODE_H_

#include <string>
#include <set>
#include "quantities/Ports.h"


namespace cosi
{

namespace library {

/** A node has ports associated with it.
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
 *@version $Id: Node.h,v 1.9 2008/12/09 23:28:22 apinto Exp $
 */


class Node
{
public:

	/**\brief Default constructor.*/
	Node();
	/**\brief Default destructur.*/
	virtual ~Node();
	/**\brief Sets the name of this instance.*/
	void SetName( std::string pName ) ;
	/**\brief Returns the name of this instance.*/
	std::string GetName( ) ;
	/**\brief Sets the node type (obsolete).*/
	void SetType( std::string Type ) ;
	/**\brief Returns the node type (obsolete).*/
	std::string GetType( ) ;

	Ports GetPorts( ) ;

private:
	std::string mName ;
	std::string mType ;
protected:
	Ports mPorts ;
};

}
}

#endif /*NODE_H_*/
