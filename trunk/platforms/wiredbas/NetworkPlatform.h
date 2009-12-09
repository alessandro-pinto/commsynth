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

#ifndef NETWORKPLATFORM_H_
#define NETWORKPLATFORM_H_

#include <string>
#include <map>
#include "libraries/wiredbas/common/Bus.h"
#include "libraries/wiredbas/common/SubNetwork.h"
#include "libraries/wiredbas/common/Link.h"
#include "libraries/wiredbas/common/Node.h"
#include "networks/wiredbas/WiredBasPlatformInstance.h"
#include "networks/wiredbas/WiredBasImplementation.h"
#include "rules/wiredbas/link/WiringRule.h"
#include "rules/wiredbas/node/NodePosition.h"
#include "quantities/Position.h"
#include "quantities/ThreadSet.h"
#include "quantities/WiringPath.h"

namespace cosi {

namespace platforms {

namespace wiredbas {

class NetworkPlatform {
	//Networks
	std::map< std::string , cosi::library::wiredbas::SubNetwork* >
			mNetComponents;
	//Nodes
	std::map<std::string , cosi::library::wiredbas::Node* > mNodeComponents;
	//Links
	std::map<std::string , cosi::library::wiredbas::Link* > mLinkComponents;
	//Rules
	cosi::rules::wiredbas::WiringRule* mWiringRule;
	cosi::rules::wiredbas::NodePosition* mNodePosition;
public:
	NetworkPlatform();
	virtual ~NetworkPlatform();

	//Add component
	virtual void AddNetComponent(std::string pType,
			cosi::library::wiredbas::SubNetwork* pNet);
	virtual void AddNodeComponent(std::string pType,
			cosi::library::wiredbas::Node* pNode);
	virtual void AddWireComponent(std::string pType,
			cosi::library::wiredbas::Link* pWire);

	virtual void SetWiringRule(cosi::rules::wiredbas::WiringRule* pWiringRule);
	virtual void SetNodePositions(
			cosi::rules::wiredbas::NodePosition* pNodePosition);
	//Get component's instance
	//Platform instances
	virtual cosi::commstruct::wiredbas::PlatformInstance* GetPlatformInstance(
			std::string pType, int pId, Position pP);

	//Implementation instances
	virtual cosi::commstruct::wiredbas::Implementation
			* GetImplementationInstance(std::string pType, int pId, Position pP);
	virtual cosi::commstruct::wiredbas::Implementation
			* GetImplementationInstance(std::string pType, int pId,
					Position pP, ThreadSet pThreads);
	virtual cosi::commstruct::wiredbas::Implementation
			* GetImplementationInstance(std::string pType, int pSourceId,
					std::string pSourcePort, int pDestId,
					std::string pDestPort, WiringPath pPath);

	virtual cosi::commstruct::wiredbas::Implementation* StartNew(string pType);
	virtual void AddNode(string pNetType, string pCtype, Vertex V, Position pP, ThreadSet pThreads,
			TransferTable pT) ;
	virtual void AddLink(string pNetType, string pCtype,Vertex U, Vertex V, WiringPath pPath) ;

	//Models
	virtual double GetLatencyValue(std::string pType,
			cosi::commstruct::wiredbas::Implementation* pNet,Vertex Src,std::string Dest,Vertex SrcId,Vertex DestId);

	virtual WiringPath WireLayout(Position pStart, Position pEnd);

	virtual std::list<Position> GetGatewayPosition();


	//Check rule
	virtual bool Satisfies(std::string pType,
			cosi::commstruct::wiredbas::Implementation* pNet,cosi::commstruct::wiredbas::Specification* pSpec);

	virtual double GetNodeCost(std::string pType);
	virtual double GetLinkCost(std::string pType, WiringPath pPath);
	virtual double GetNetCost(std::string pType,
			cosi::commstruct::wiredbas::Implementation* pNet);

};

}

}

}

#endif /*NETWORKPLATFORM_H_*/
