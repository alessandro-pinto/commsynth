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

#ifndef SPECIFICATION_H_
#define SPECIFICATION_H_

#include <string>

#include "quantities/Name.h"
#include "quantities/Position.h"
#include "quantities/onchipcommunication/Ports.h"
#include "quantities/RealLatency.h"
#include "quantities/CommoditySet.h"
#include "quantities/onchipcommunication/IpGeometry.h"
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
typedef Label_5<Name,cosi::quantity::onchipcommunication::Ports,Position,cosi::quantity::onchipcommunication::IpGeometry,CommoditySet> SpecificationLabel;

class Specification : public CommunicationStructure< SpecificationLabel > {
public:
	Specification(std::string pName);
	virtual ~Specification();
	Specification operator+(Specification pC);

};

}

}

}

#endif /*SPECIFICATION_H_*/
