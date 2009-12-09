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

#include "NetworkPlatform.h"

namespace cosi {

namespace platforms {

namespace wiredbas {

NetworkPlatform::NetworkPlatform() {
}

NetworkPlatform::~NetworkPlatform() {
}



//Add component
void NetworkPlatform::AddNetComponent(std::string pType,
		cosi::library::wiredbas::SubNetwork* pNet){
	mNetComponents[pType] = pNet ;
}

void NetworkPlatform::AddNodeComponent(std::string pType,
		cosi::library::wiredbas::Node* pNode){
	mNodeComponents[pType] = pNode ;
}

void NetworkPlatform::AddWireComponent(std::string pType,
		cosi::library::wiredbas::Link* pWire){
	mLinkComponents[pType] = pWire ;
}

void NetworkPlatform::SetWiringRule(cosi::rules::wiredbas::WiringRule* pWiringRule){
	mWiringRule = pWiringRule ;
}

void NetworkPlatform::SetNodePositions(cosi::rules::wiredbas::NodePosition* pNodePosition){
	mNodePosition = pNodePosition ;
}

//Get component's instance
//Platform instances
cosi::commstruct::wiredbas::PlatformInstance* NetworkPlatform::GetPlatformInstance(
		std::string pType, int pId, Position pP){
	if (mNodeComponents.find(pType) != mNodeComponents.end() )
			return mNodeComponents[pType]->GetPlatformInstance(pId, pP) ;
		else {
			std::cout << "Node of type "<< pType
					<< " does not belong to the platform"<< std::endl ;
		}
		return (new cosi::commstruct::wiredbas::PlatformInstance("Empty"));
}

//Implementation instances
cosi::commstruct::wiredbas::Implementation* NetworkPlatform::GetImplementationInstance(
		std::string pType, int pId, Position pP){
	if (mNodeComponents.find(pType) != mNodeComponents.end() )
			return mNodeComponents[pType]->GetImplementationInstance(pId, pP,
					ThreadSet(), TransferTable()) ;
		else {
			std::cout << "Node of type "<< pType
					<< " does not belong to the platform"<< std::endl ;
		}
		return (new cosi::commstruct::wiredbas::Implementation("Empty"));
}

cosi::commstruct::wiredbas::Implementation* NetworkPlatform::GetImplementationInstance(
		std::string pType, int pId, Position pP, ThreadSet pThreads){
	if (mNodeComponents.find(pType) != mNodeComponents.end() )
			return mNodeComponents[pType]->GetImplementationInstance(pId, pP,
					pThreads, TransferTable()) ;
		else {
			std::cout << "Node of type "<< pType
					<< " does not belong to the platform"<< std::endl ;
		}
		return (new cosi::commstruct::wiredbas::Implementation("Empty"));
}

cosi::commstruct::wiredbas::Implementation* NetworkPlatform::GetImplementationInstance(
		std::string pType, int pSourceId, std::string pSourcePort, int pDestId,
		std::string pDestPort, WiringPath pPath){
	if (mLinkComponents.find(pType) != mLinkComponents.end() ) {
			return mLinkComponents[pType]->GetImplementationInstance(pSourceId,
					pSourcePort, pDestId, pDestPort , pPath) ;
		} else {
			std::cout << "Link of type "<< pType
					<< " does not belong to the platform"<< std::endl ;
		}
		return (new cosi::commstruct::wiredbas::Implementation("Empty"));
}

cosi::commstruct::wiredbas::Implementation* NetworkPlatform::StartNew(string pType){
	return mNetComponents[pType]->StartNew() ;
}

void NetworkPlatform::AddNode(string pNetType,string pCtype, Vertex V, Position pP, ThreadSet pThreads, TransferTable pT){
	mNetComponents[pNetType]->AddNode(pCtype,V,pP,pThreads,pT) ;
}

void NetworkPlatform::AddLink(string pNetType,string pCtype, Vertex U, Vertex V, WiringPath pPath){
	mNetComponents[pNetType]->AddLink(pCtype,U,V,pPath);
}

//Models
double NetworkPlatform::GetLatencyValue(std::string pType,
		cosi::commstruct::wiredbas::Implementation* pNet, Vertex Src,
		std::string Dest, Vertex SrcId, Vertex DestId){
	return mNetComponents[pType]->LatencyValue(pNet,Src,Dest,SrcId,DestId);
}

WiringPath NetworkPlatform::WireLayout(Position pStart, Position pEnd){
	return mWiringRule->GetWiringPath(pStart, pEnd) ;
}

std::list<Position> NetworkPlatform::GetGatewayPosition(){
	return mNodePosition->GetPositions() ;
}

//Check rule
bool NetworkPlatform::Satisfies(std::string pType,
		cosi::commstruct::wiredbas::Implementation* pNet,cosi::commstruct::wiredbas::Specification* pSpec){
	return mNetComponents[pType]->Satisfies(pSpec,pNet) ;
}

double NetworkPlatform::GetNodeCost(std::string pType){
	return mNodeComponents[pType]->GetCost() ;
}

double NetworkPlatform::GetLinkCost(std::string pType, WiringPath pPath){
	return mLinkComponents[pType]->GetCost(pPath) ;
}

double NetworkPlatform::GetNetCost(std::string pType,
		cosi::commstruct::wiredbas::Implementation* pNet){
	return mNetComponents[pType]->GetCost(pNet) ;
}

}

}

}
