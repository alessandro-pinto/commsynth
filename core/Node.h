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

#ifndef NODE_H_
#define NODE_H_

#include <string>
#include <sstream>
#include <map>

using namespace std;

namespace cosi
{

/** The interface of a node is a multiset of availalbe port interfaces.
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
 *@version $Id: Node.h,v 1.3 2008/12/07 23:20:21 apinto Exp $
 */

class Interface : public map<string,string> {
public:
	Interface( ) ;
	~Interface( ) ;
	/**\brief Adds a single port interface.*/
	void Add(string pName,string pType) ;
	/**\brief Adds a number of port interface.*/
	void Add(string pName , string pType , int pCount ) ;
	/**\brief Returns trie if the port interface is contained in the interface of the node.*/
	bool HasPortType( string pType ) ;
	bool HasPortName( string pName ) ;
private:
};


/** A node has an associated name, type and a set of interfaces.
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
 *@version $Id: Node.h,v 1.3 2008/12/07 23:20:21 apinto Exp $
 */


class Node : public Interface
{
public:
	Node();
	virtual ~Node();
	/**\brief Sets the node name.*/
	void SetName( string pName ) ;
	/**\brief Returns the node name.*/
	string GetName( ) ;
	/**\brief Sets the node type.*/
	void SetType( string Type ) ;
	/**\brief Returns the node type.*/
	string GetType( ) ;

private:
	string mName ;
	string mType ;
};

}

#endif /*NODE_H_*/
