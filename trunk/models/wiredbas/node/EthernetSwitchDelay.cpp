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
#include "EthernetSwitchDelay.h"

namespace cosi {

namespace models {

namespace wiredbas {

using namespace std;

EthernetSwitchDelay::EthernetSwitchDelay(double Speed) {
	mLinkDelay = new cosi::models::wiredbas::EthernetLinkDelay(Speed);
	mSpeed = Speed;
}

EthernetSwitchDelay::~EthernetSwitchDelay() {
	delete mLinkDelay;
}

RealDelay EthernetSwitchDelay::Get(Vertex NodeId,
		cosi::commstruct::wiredbas::Implementation* pI, Vertex Src,
		std::string Dest) {
	return RealDelay( GetValue(NodeId,pI,Src,Dest) );

}
double EthernetSwitchDelay::GetValue(Vertex NodeId,
		cosi::commstruct::wiredbas::Implementation* pI, Vertex Src,
		std::string DestName) {

	//Name of source and dest
	string SrcName = pI->GetName(Src) ;
	//string DestName = pI->GetName(Dest) ;
	pair<string,string> Commodity(SrcName,DestName) ;

	//Find the output port
	string Out ;
	TransferTable T = (*pI)[NodeId].PRJ(TransferTable)Get() ;
	TransferTable::iterator It;
	for (It = T.TransferTableBegin() ; It != T.TransferTableEnd() ; It++) {
		if (It->first == Commodity) {
			Out = It->second ;
			break ;
		}
	}
	//find the colliding commodities
	//TODO:  the transfer table should have not only names but also ids
	//This should be theoretically right (the names do not change with renaming)
	set< pair< string,string> > Collisions;
	for (It = T.TransferTableBegin() ; It != T.TransferTableEnd() ; It++) {
		if (It->second == Out ) {
			Collisions.insert(It->first) ;
		}
	}
	//Compute the total delay of the colliding commodities
	set< pair< string,string> >::iterator C;
	double RetVal = 0 ;
	for( C = Collisions.begin() ; C != Collisions.end() ; C++) {
		//Get the source id
		int SrcId = pI->GetId(C->first) ;
		int DestId = pI->GetId(C->second) ;
		//get the longest message from the source
		ThreadSet Tset = (*pI)[SrcId].PRJ(ThreadSet)Get() ;
		ThreadSet::iterator Tit ;
		int Length = 0 ;
		for( Tit = Tset.ThreadSetBegin() ; Tit != Tset.ThreadSetEnd() ; Tit++){
			if (Tit->GetDestinationNodeName() == DestName && Tit->GetMessageLength() > Length) {
				Length = Tit->GetMessageLength() ;
			}
		}
		//TODO: we are abstracting the delay of the output wires because we do not know them
		RetVal+= mLinkDelay->GetValue(Length,WiringPath()) ;
	}
	return RetVal ;

}

}

}

}
