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
#ifndef _LINK_H_
#define _LINK_H_

#include <string>
#include "quantities/onchipcommunication/Interface.h"
#include "quantities/Name.h"

namespace cosi {

namespace library {

namespace onchipcommunication {

class _Link {
public:
	_Link();
	virtual ~_Link();

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
	void SetSourceInterface(cosi::quantity::onchipcommunication::Interface pIf);

	/**\brief Returns that source interface of this link*/
	cosi::quantity::onchipcommunication::Interface GetSourceInterface();

	/**\brief Sets the type of the interface that can be the destination interface of this link.
	 *
	 * The destination node can have many interfaces. A link connectes one source interface to one
	 * destination interface.*/
	void SetDestinationInterface(
			cosi::quantity::onchipcommunication::Interface pIf);

	/**\brief Returns the destination interface of this node.*/
	cosi::quantity::onchipcommunication::Interface GetDestinationInterface();

	bool Directed();
	void Directed(bool pD);
private:
	/**\brief This field is true if the link is directed.*/
	bool mDirected;


	/**\brief The name of the source node of this link.*/
	Name mSource;

	/**\brief The name of the destination node of this link.*/
	Name mDest;

	/**\brief The type of the source interface of this link*/
	cosi::quantity::onchipcommunication::Interface mSourceInterface;

	/**\brief The type of the destination interface of this link.*/
	cosi::quantity::onchipcommunication::Interface mDestInterface;

};

}

}

}

#endif /*_LINK_H_*/
