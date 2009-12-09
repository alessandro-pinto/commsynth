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
#include "ArcnetBusCost.h"

namespace cosi {

namespace models {

namespace wiredbas {

ArcnetBusCost::ArcnetBusCost(cosi::models::wiredbas::WireCost* pWireCost) {
	mNodeCost = new cosi::models::wiredbas::ArcnetNodeCost;
	mWireCost = pWireCost ;
}

ArcnetBusCost::~ArcnetBusCost() {
	delete mNodeCost;
}

void ArcnetBusCost::SetWireCost(cosi::models::wiredbas::WireCost* pWireCost) {
	mWireCost = pWireCost ;
}

double ArcnetBusCost::GetValue(cosi::commstruct::wiredbas::PlatformInstance pC) {
	return GetValue(&pC) ;
}

double ArcnetBusCost::GetValue(cosi::commstruct::wiredbas::PlatformInstance* pC) {
	//Assume that this is a bus
	double RetVal = 0;
	IdGraph::v_iterator U, V;
	//Sum all node cost
	for (U = pC->v_begin() ; U != pC->v_end() ; U++) {
		//compute the cost of a node
		RetVal += mNodeCost->GetValue(pC->GetName(*U), (*pC)[*U].PRJ(Type)GetValue(),(*pC)[*U].PRJ(Ports)Get());
	}
	//Sum all wire cost
	for (U = pC->v_begin() ; U != pC->v_end() ; U++) {
		for (V = pC->v_begin() ; V != pC->v_end() ; V++) {
			if (pC->InE(*U, *V)) {
				RetVal += mWireCost->GetValue((*pC)[Edge(*U,*V)].PRJ(WiringPath)Get());
			}
		}
	}
	return RetVal ;
}

double ArcnetBusCost::GetValue(cosi::commstruct::wiredbas::Implementation pC) {
	return GetValue(&pC) ;
}

double ArcnetBusCost::GetValue(cosi::commstruct::wiredbas::Implementation* pC) {
	//Assume that this is a bus
	double RetVal = 0;
	IdGraph::v_iterator U, V;
	//Sum all node cost
	for (U = pC->v_begin() ; U != pC->v_end() ; U++) {
		//compute the cost of a node
		RetVal += mNodeCost->GetValue(pC->GetName(*U), (*pC)[*U].PRJ(Type)GetValue(),(*pC)[*U].PRJ(Ports)Get());
	}

	//cout << "Node Cost " << RetVal << endl  ;

	//Sum all wire cost
	for (U = pC->v_begin() ; U != pC->v_end() ; U++) {
		for (V = pC->v_begin() ; V != pC->v_end() ; V++) {
			if (pC->InE(*U, *V)) {
				RetVal += mWireCost->GetValue((*pC)[Edge(*U,*V)].PRJ(WiringPath)Get());
			}
		}
	}

	//cout << "Total Cost " << RetVal << endl ;

	return RetVal ;
}


}

}

}
