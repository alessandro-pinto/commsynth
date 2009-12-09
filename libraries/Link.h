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
#ifndef LIBRARIES_LINK_H_
#define LIBRARIES_LINK_H_

#include <string>
#include "quantities/Ports.h"
#include "quantities/Name.h"
#include "quantities/Type.h"


namespace cosi {

namespace library {

/** This base class defines a link. A link is a connection between two
 * ports of two nodes. A link can be directed or indirected.
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
 *@version $Id: Link.h,v 1.9 2008/12/09 23:28:22 apinto Exp $
 */

class Link {

public:

	/**\brief  Default constructor.
	 *
	 * Sets all fields to Unknown.*/
	Link();

	/**\brief Virtual default destructor.*/
	virtual ~Link();

	/**\brief Sets the name of this link.
	 *
	 * This is a name associated with the instance of the link as opposed to
	 * the name associated to a library element. The name of a library element
	 * should be a static field defined when this class is derived.*/
	void SetName(std::string pS);

	/**\brief Returns the name of the instance of a link.*/
	std::string GetName();

	/**\brief Sets the type of the link.
	 *
	 * This function is obsolete. The type should be instance dependent but should
	 * be associated to the library element and defined as a static field of the
	 * library element.*/
	void SetType(std::string pS);

	/**\brief Returns the type of the link.*/
	std::string GetType();

	/**\brief Sets the name of the source node.
	 *
	 * This is the name of the library element that can be a source for
	 * this link.*/
	void SetSource(Name pS);

	/**\brief Returns the name of the library element that can be the source of this link.*/
	Name GetSource();

	/**\brief Sets the name of the destination node.
	 *
	 * This is the name of the library element that can be a destination for
	 * this link.*/
	void SetDestination(Name pS);

	/**\brief Returns the name of the library elements that can be the destination of this link*/
	Name GetDestination();

	/**\brief Sets the type of the interface that can be the source interface of this link.
	 *
	 * The source node can have many interfaces. A link connectes one source interface to one
	 * destination interface.*/
	void SetSourceInterface(PortInterface pIf);

	/**\brief Returns that source interface of this link*/
	PortInterface GetSourceInterface();

	/**\brief Sets the type of the interface that can be the destination interface of this link.
	 *
	 * The destination node can have many interfaces. A link connectes one source interface to one
	 * destination interface.*/
	void SetDestinationInterface(PortInterface pIf);

	/**\brief Returns the destination interface of this node.*/
	PortInterface GetDestinationInterface();

	bool Directed() ;
	void Directed(bool pD) ;

private:

	/**\brief This field is true if the link is directed.*/
	bool mDirected;

	/**\brief  The name of this instance.*/
	std::string mName;

	/**\brief The type of this link (obsolete).*/
	std::string mType;

	/**\brief The name of the source node of this link.*/
	Name mSource;

	/**\brief The name of the destination node of this link.*/
	Name mDest;

	/**\brief The type of the source interface of this link*/
	PortInterface mSourceInterface;

	/**\brief The type of the destination interface of this link.*/
	PortInterface mDestInterface;

};

}
}

#endif /*LINK_H_*/
