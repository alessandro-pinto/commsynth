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
#include "EthernetNetworkDelay.h"

namespace cosi {

namespace models {

namespace wiredbas {

EthernetNetworkDelay::EthernetNetworkDelay(double pSpeed) {
	mSpeed = pSpeed;
	mLinkDelay = new cosi::models::wiredbas::EthernetLinkDelay(pSpeed);
	mNodeDelay = new cosi::models::wiredbas::EthernetSwitchDelay(pSpeed);
}

EthernetNetworkDelay::~EthernetNetworkDelay() {
}

RealDelay EthernetNetworkDelay::Get(
		cosi::commstruct::wiredbas::Implementation pC, Vertex Src, std::string  Dest,
		Vertex SrcId, Vertex DestId) {
	return RealDelay(GetValue(pC, Src, Dest, SrcId, DestId));
}

RealDelay EthernetNetworkDelay::Get(
		cosi::commstruct::wiredbas::Implementation* pC, Vertex Src,
		std::string  Dest, Vertex SrcId, Vertex DestId) {
	return RealDelay(GetValue(pC, Src, Dest, SrcId, DestId));
}

double EthernetNetworkDelay::GetValue(
		cosi::commstruct::wiredbas::Implementation pC, Vertex Src, std::string  Dest,
		Vertex SrcId, Vertex DestId) {
	return GetValue(&pC, Src, Dest, SrcId, DestId);
}

double EthernetNetworkDelay::GetValue(
		cosi::commstruct::wiredbas::Implementation* pC, Vertex Src,
		string Dest, Vertex SrcId, Vertex DestId) {


	//Get the flow infrormation
	ThreadSet Tset = (*pC)[Src].PRJ(ThreadSet)Get() ;
	ThreadSet::iterator Tit;
	int Length = 0;
	for (Tit = Tset.ThreadSetBegin() ; Tit != Tset.ThreadSetEnd() ; Tit++) {
		if (Tit->GetDestinationNodeName() == Dest && Tit->GetMessageLength()
				> Length) {
			Length = Tit->GetMessageLength() ;
		}
	}
	//find the path from SrcId to DestId

	//for each link sum up the link and the destination node delay

	//break and return infinity if a path does not exist



	std::string SrcName = pC->GetName(Src) ;
	//std::string DestName = pC->GetName(Dest) ;
	std::string DestName = Dest ;

	//cout << "Generating delay info for commodity " << SrcName << " " << Src <<" " << DestName << " " << Dest << " from " << SrcId << " to " << DestId << endl ;

	Vertex CurrentNode = SrcId;
	Vertex NextNode = SrcId;
	double RetVal = 0;
	while (NextNode != DestId) {
		//Find the destination
		//Look for the output port
		//cout << "Node " << CurrentNode << endl;
		//cout << (*pC)[CurrentNode].PRJ(TransferTable)Get() << endl ;
		std::string Out = (*pC)[CurrentNode].PRJ(TransferTable)GetPort(SrcName, DestName) ;
		//Get the edge
		//cout << " output port " << Out << endl ;
		if (Out.size() == 0) {
			return DBL_MAX;
		} else {
			Edge E = (*pC)[CurrentNode].PRJ(Ports)BoundTo(Out) ;
			if ( E.V() == CurrentNode )
				NextNode = E.U() ;
			else
				NextNode = E.V() ;
			RetVal += mLinkDelay->GetValue(Length, (*pC)[E].PRJ(WiringPath)Get()) ;
			RetVal += mNodeDelay->GetValue(NextNode, pC, Src, Dest) ;
		}
		CurrentNode = NextNode;
	}
	return RetVal ;

}

}

}

}
