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
#ifndef ETHERNETSUBNETWORK_H_
#define ETHERNETSUBNETWORK_H_

#include <string>
#include "libraries/wiredbas/common/SubNetwork.h"
#include "rules/wiredbas/link/WiringRule.h"
#include "rules/wiredbas/node/EthernetNetworkRule.h"
#include "models/wiredbas/common/WireCost.h"
#include "models/wiredbas/common/LinkDelayModel.h"
#include "models/wiredbas/node/EthernetNetworkCost.h"
#include "models/wiredbas/node/EthernetNetworkDelay.h"
#include "libraries/wiredbas/common/InstantiableLink.h"
#include "libraries/wiredbas/common/InstantiableNode.h"

namespace cosi {

namespace library {

namespace wiredbas {

class EthernetSubNetwork : public SubNetwork {
	//Wiring model
	cosi::rules::wiredbas::WiringRule* mWiringRule;
	cosi::rules::wiredbas::EthernetNetworkRule* mEthernetRule;
	cosi::models::wiredbas::WireCost* mWireCost;
	cosi::models::wiredbas::EthernetNetworkCost* mNetCost;
	cosi::models::wiredbas::LinkDelayModel* mLinkDelay;
	cosi::models::wiredbas::EthernetNetworkDelay* mNetDelay;

public:
	EthernetSubNetwork(cosi::rules::wiredbas::WiringRule* Wr,
	cosi::rules::wiredbas::EthernetNetworkRule* Er,
	cosi::models::wiredbas::WireCost* Wc,
	cosi::models::wiredbas::EthernetNetworkCost* Nc,
	cosi::models::wiredbas::LinkDelayModel* Ld,
	cosi::models::wiredbas::EthernetNetworkDelay* Nd,
	std::map<std::string, cosi::library::wiredbas::InstantiableNode*> N,
	std::map<std::string, cosi::library::wiredbas::InstantiableLink*> L);
	virtual ~EthernetSubNetwork();
    void AddNode(string Name, Vertex V, Position pP, ThreadSet pThreads,
			TransferTable pT) ;
	void AddLink(string Name, Vertex U, Vertex V, WiringPath pPath) ;
	double LatencyValue(cosi::commstruct::wiredbas::Implementation* pNet,
			Vertex Src, std::string Dest, Vertex SrcId, Vertex DestId);
	bool Satisfies(cosi::commstruct::wiredbas::Specification* pSpec ,cosi::commstruct::wiredbas::Implementation* pNet);
	double GetCost(cosi::commstruct::wiredbas::Implementation* pNet);
};

}

}

}

#endif /*ETHERNETSUBNETWORK_H_*/
