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
#ifndef ETHERNETNETWORKRULE_H_
#define ETHERNETNETWORKRULE_H_

#include "rules/wiredbas/common/NetworkRule.h"
#include "networks/wiredbas/WiredBasImplementation.h"
#include "networks/wiredbas/WiredBasSpecificaion.h"
#include "models/wiredbas/link/EthernetLinkDelay.h"

namespace cosi {

namespace rules {

namespace wiredbas {

class EthernetNetworkRule : public NetworkRule {
	double mSpeed;
	cosi::models::wiredbas::EthernetLinkDelay* mLinkDelay ;
public:
	EthernetNetworkRule(double pSpeed);
	virtual ~EthernetNetworkRule();
	bool Satisfies(cosi::commstruct::wiredbas::Specification* pSpec , cosi::commstruct::wiredbas::Implementation pC);
	bool Satisfies(cosi::commstruct::wiredbas::Specification* pSpec , cosi::commstruct::wiredbas::Implementation* pC);
};

}

}

}

#endif /*ETHERNETNETWORKRULE_H_*/
