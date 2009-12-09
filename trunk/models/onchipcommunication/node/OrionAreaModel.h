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
#ifndef ORIONAREAMODEL_H_
#define ORIONAREAMODEL_H_

#include <vector>
#include "models/onchipcommunication/common/AreaModel.h"
#include "include/onchipcommunication/typedefs.h"
#include "quantities/onchipcommunication/Ports.h"
#include "quantities/onchipcommunication/NodeParameter.h"
#include "io/onchipcommunication/HoOrionModelParser.h"

namespace cosi {

namespace models {

namespace onchipcommunication {

class OrionAreaModel : public NodeAreaModel {

public:

	OrionAreaModel(cosi::library::onchipcommunication::Technology pTech);

	virtual ~OrionAreaModel();

	double GetArea(Name pName,
			cosi::quantity::onchipcommunication::Ports pPorts,
			cosi::quantity::onchipcommunication::NodeParameter pParam);

private:
	std::vector<std::vector<double> > mArouter1ch32;
	std::vector<std::vector<double> > mArouter4ch32;
	std::vector<std::vector<double> > mArouter1ch128;
	std::vector<std::vector<double> > mArouter4ch128;
	int mMaxIn ;
	int mMaxOut ;

};

}

}

}

#endif /*NODEAREAMODEL_H_*/
