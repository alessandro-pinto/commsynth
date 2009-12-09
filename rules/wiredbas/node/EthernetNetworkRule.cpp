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
#include "EthernetNetworkRule.h"

namespace cosi {

namespace rules {

namespace wiredbas {

EthernetNetworkRule::EthernetNetworkRule(double Speed) {
	mSpeed = Speed;
	mLinkDelay = new cosi::models::wiredbas::EthernetLinkDelay(Speed);
}

EthernetNetworkRule::~EthernetNetworkRule() {
}

bool EthernetNetworkRule::Satisfies(
		cosi::commstruct::wiredbas::Specification* pSpec,
		cosi::commstruct::wiredbas::Implementation pC) {

}

bool EthernetNetworkRule::Satisfies(
		cosi::commstruct::wiredbas::Specification* pSpec,
		cosi::commstruct::wiredbas::Implementation* pC) {
	//check cable length
	IdGraph::v_iterator U, V;
	for (U = pC->v_begin() ; U != pC->v_end() ; U++) {
		for (V = pC->v_begin() ; V != pC->v_end() ; V++) {
			if (pC->InE(*U, *V)) {
				if ((*pC)[Edge(*U,*V)].PRJ(WiringPath)Length() > 120) {
					cout << "Length violation: max "<< 120<< " actual "<< (*pC)[Edge(*U,*V)].PRJ(WiringPath)Length() << endl ;
					return false;
				}
			}
		}
	}

	//check number of devices attached to each switch
	for (U = pC->v_begin() ; U != pC->v_end() ; U++) {
		Ports P = (*pC)[*U].PRJ(Ports)Get() ;
		//cout << (*pC)[*U].PRJ(Name)GetValue() << " " << (*pC)[*U].PRJ(Type)GetValue()  << "  " << endl << P <<endl ;
		int Nin = 0;
		int Nout = 0;
		int Ninout = 0;
		Ports::iterator Pit;
		for (Pit = P.Begin() ; Pit != P.End() ; Pit++) {
			if (P.If(Pit).GetDirection() == PortInterface::IN) {
				Nin++;
			} else if (P.If(Pit).GetDirection() == PortInterface::OUT) {
				Nout++;
			} else if (P.If(Pit).GetDirection() == PortInterface::INOUT) {
				Ninout++;
			}
		}

		if ((*pC)[*U].PRJ(Type)GetValue() == "Router") {

			//			if (pC->InDegree(*U) > Nin) {
			//				cout << "Input degree violation: max " << Nin << " actual " << pC->InDegree(*U) << endl ;
			//				return false;
			//			}
			//			if (pC->OutDegree(*U) > Nout){
			//				cout << "Output degree violation: max " << Nout << " actual " << pC->OutDegree(*U) << endl ;
			//				return false;
			//			}

			if (pC->InDegree(*U) + pC->OutDegree(*U) > Ninout) {
				cout << "Degree violation: max "<< Ninout << " actual "
						<< pC->InDegree(*U) + pC->OutDegree(*U) << endl ;
				return false;
			}

			//TODO: add the check for sensors and controllers as well

			//check bandwidth at the input of each router

			//get the routing table
			TransferTable T = (*pC)[*U].PRJ(TransferTable)Get() ;
			//compute the bandwidth of each flow passing by this router
			TransferTable::iterator It;
			double B = 0;
			for (It = T.TransferTableBegin() ; It != T.TransferTableEnd() ; It++) {
				string SrcName = (It->first).first;
				string DestName = (It->first).second;
				int SrcId = pSpec->GetId(SrcName) ;
				ThreadSet Tset = (*pSpec)[SrcId].PRJ(ThreadSet)Get() ;
				ThreadSet::iterator Tit;
				for (Tit = Tset.ThreadSetBegin() ; Tit != Tset.ThreadSetEnd() ; Tit++) {
					if (Tit->GetDestinationNodeName() == DestName) {
						B += mLinkDelay->GetValue(Tit->GetMessageLength(),
								WiringPath()) * mSpeed/ (Tit->GetPeriod());
					}
				}
			}
			if (B > mSpeed) {
				cout << "Bandwidth violation: max "<< mSpeed << " actual "<< B
						<< endl ;
				return false;
			}
		}
	}
	return true;

}

}

}

}
