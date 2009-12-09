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

#ifndef PLATFORMINSTANCE_H_
#define PLATFORMINSTANCE_H_

#include <string>

#include "quantities/Name.h"
#include "quantities/Type.h"
#include "quantities/Position.h"
#include "quantities/WiringPath.h"
#include "quantities/onchipcommunication/Ports.h"
#include "quantities/onchipcommunication/IpGeometry.h"
#include "quantities/onchipcommunication/NodeParameter.h"
#include "quantities/onchipcommunication/LinkParameter.h"
#include "label/Label.hpp"
#include "core/CommunicationStructure.hpp"

namespace cosi {

namespace commstruct {

namespace onchipcommunication {

/**	Type is the component type and will be used to select a component from the library
 *  Ports is the set of available ports
 * 	Position is the position
 * 	Ports is the port of each node.
 *
 *
 * */
typedef Label_8<Name,Type,cosi::quantity::onchipcommunication::Ports,Position,cosi::quantity::onchipcommunication::NodeParameter,cosi::quantity::onchipcommunication::IpGeometry,WiringPath,cosi::quantity::onchipcommunication::LinkParameter>
		PlatformLabel;

class PlatformInstance : public CommunicationStructure< PlatformLabel >{
public:
	PlatformInstance(std::string pName);
	virtual ~PlatformInstance();
	PlatformInstance operator+(PlatformInstance pC) ;
};

}

}

}

#endif /*PLATFORMINSTANCE_H_*/
