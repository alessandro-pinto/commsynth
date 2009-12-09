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
#ifndef SUBNETWORK_H_
#define SUBNETWORK_H_

#include "libraries/wiredbas/common/InstantiableSubNetwork.h"
#include "networks/wiredbas/WiredBasImplementation.h"
#include "networks/wiredbas/WiredBasSpecificaion.h"


namespace cosi {

namespace library {

namespace wiredbas {

class SubNetwork : public InstantiableSubNetwork {
public:
	SubNetwork(std::map<std::string, cosi::library::wiredbas::InstantiableNode*> N,
			std::map<std::string, cosi::library::wiredbas::InstantiableLink*> L);
	virtual ~SubNetwork() ;
	virtual double LatencyValue(
			cosi::commstruct::wiredbas::Implementation* pNet, Vertex Src, std::string Dest, Vertex SrcId , Vertex DestId) =0;
	virtual bool Satisfies(cosi::commstruct::wiredbas::Specification* pSpec, cosi::commstruct::wiredbas::Implementation* pNet) =0;
	virtual double GetCost(cosi::commstruct::wiredbas::Implementation* pNet) =0;
};

}

}

}

#endif /*SUBNETWORK_H_*/
