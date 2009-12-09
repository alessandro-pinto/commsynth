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
#include "EthernetSubNetwork.h"

namespace cosi {

namespace library {

namespace wiredbas {

EthernetSubNetwork::EthernetSubNetwork(cosi::rules::wiredbas::WiringRule* Wr,
		cosi::rules::wiredbas::EthernetNetworkRule* Er,
		cosi::models::wiredbas::WireCost* Wc,
		cosi::models::wiredbas::EthernetNetworkCost* Nc,
		cosi::models::wiredbas::LinkDelayModel* Ld,
		cosi::models::wiredbas::EthernetNetworkDelay* Nd,
		std::map<std::string, cosi::library::wiredbas::InstantiableNode*> N,
		std::map<std::string, cosi::library::wiredbas::InstantiableLink*> L) :
	SubNetwork(N, L) {
	mWiringRule = Wr;
	mEthernetRule = Er;
	mWireCost = Wc;
	mNetCost = Nc;
	mLinkDelay = Ld;
	mNetDelay = Nd;
}

EthernetSubNetwork::~EthernetSubNetwork() {

}

double EthernetSubNetwork::LatencyValue(
		cosi::commstruct::wiredbas::Implementation* pNet, Vertex Src,
		std::string Dest, Vertex SrcId, Vertex DestId) {
	return mNetDelay->GetValue(pNet, Src, Dest, SrcId, DestId);
}

bool EthernetSubNetwork::Satisfies(cosi::commstruct::wiredbas::Specification* pSpec,
		cosi::commstruct::wiredbas::Implementation* pNet) {
	return mEthernetRule->Satisfies(pSpec,pNet);
}

double EthernetSubNetwork::GetCost(
		cosi::commstruct::wiredbas::Implementation* pNet) {
	return mNetCost->GetValue(pNet) ;
}

void EthernetSubNetwork::AddNode(string Name, Vertex V, Position pP,
		ThreadSet pThreads, TransferTable pT) {
	cosi::commstruct::wiredbas::Implementation
			* N = mNodes[Name]->GetImplementationInstance(V, pP, pThreads, pT) ;
	(*mI) = (*mI) + (*N);
	delete N;
}

void EthernetSubNetwork::AddLink(string Name, Vertex U, Vertex V,
		WiringPath pPath) {
	//Find availalbe ports on U and V
	string Out = (*mI)[U].PRJ(Ports)GetInputOutputPortNotBound();
	string In = (*mI)[V].PRJ(Ports)GetInputOutputPortNotBound();
	cosi::commstruct::wiredbas::Implementation
			* L =mLinks[Name]->GetImplementationInstance(U, Out, V, In, pPath) ;
	(*mI) = (*mI) + (*L);
	delete L;
}

}

}

}
