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
#include "OrionPowerModel.h"

namespace cosi {

namespace models {

namespace onchipcommunication {

using namespace cosi::library::onchipcommunication;

OrionPowerModel::OrionPowerModel(Technology pTech) {

	cosi::io::onchipcommunication::HoOrionModelParser* P = 0x0;
	if (pTech == T90 ) {
		P = new cosi::io::onchipcommunication::HoOrionModelParser(CosiRoot + "/models/onchipcommunication/node/ho-orion-90nm.xml");
	} else if (pTech == T65 ) {
		P = new cosi::io::onchipcommunication::HoOrionModelParser(CosiRoot + "/models/onchipcommunication/node/ho-orion-65nm.xml");
	} else if (pTech == T45) {
		P = new cosi::io::onchipcommunication::HoOrionModelParser(CosiRoot + "/models/onchipcommunication/node/ho-orion-45nm.xml");
	}

	P->Parse() ;

	mMaxIn = P->GetMaxIn() ;
	mMaxOut = P->GetMaxOut() ;

	mErouter1ch32 = P->GetDynamicEnergy1ch32() ;
	mErouter4ch32 = P->GetDynamicEnergy4ch32() ;
	mErouter1ch128 = P->GetDynamicEnergy1ch128() ;
	mErouter4ch128 = P->GetDynamicEnergy4ch128() ;
	mErouterLeak1ch32 = P->GetStaticEnergy1ch32() ;
	mErouterLeak4ch32 = P->GetStaticEnergy4ch32() ;
	mErouterLeak1ch128 = P->GetStaticEnergy1ch128() ;
	mErouterLeak4ch128 = P->GetStaticEnergy4ch128() ;

	delete P;
}

OrionPowerModel::~OrionPowerModel() {
}

double OrionPowerModel::GetDynamicPower(Name pName,
		cosi::quantity::onchipcommunication::Ports pPorts,
		std::map< std::string , CommoditySet > pCommodities,
		cosi::quantity::onchipcommunication::NodeParameter pParam) {
	//get the largest port in terms of width and approximate to 32 or 128
	cosi::quantity::onchipcommunication::Ports::iterator It;
	int MaxWidth = 0;
	int MaxVch = 0;
	int Nin = 0;
	int Nout = 0;
	double B = 0 ;
	for (It = pPorts.Begin() ; It != pPorts.End() ; It++) {
		if (pPorts.If(It).GetWidth() > MaxWidth ) {
			MaxWidth = pPorts.If(It).GetWidth() ;
		}
		if (pPorts.If(It).GetNumberOfVirtualChannels() > MaxVch ) {
			MaxVch = pPorts.If(It).GetNumberOfVirtualChannels() ;
		}
		if ((It->second).GetDirection() == cosi::quantity::onchipcommunication::Interface::IN ) {
			Nin++;
			B += pCommodities[It->first].GetAggregateBandwidthValue() ;

		}
		if (pPorts.If(It).GetDirection()
				== cosi::quantity::onchipcommunication::Interface::OUT ) {
			Nout++;
		}

	}


	if( Nin > mMaxIn )
		Nin = mMaxIn ;
	if( Nout > mMaxOut)
		Nout = mMaxOut ;

	if (MaxWidth <= 32) {
		if (MaxVch == 1) {
			return mErouter1ch32[Nin][Nout] * B / (double)32;
		} else {
			return mErouter4ch32[Nin][Nout] * B / (double)32;
		}
	} else {
		if (MaxVch == 1) {
			return mErouter1ch128[Nin][Nout] * B /(double) 128 ;
		} else {
			return mErouter4ch128[Nin][Nout] * B / (double) 128;
		}
	}
}
double OrionPowerModel::GetStaticPower(Name pName,
		cosi::quantity::onchipcommunication::Ports pPorts,
		cosi::quantity::onchipcommunication::NodeParameter pParam) {
	//get the largest port in terms of width and approximate to 32 or 128
	cosi::quantity::onchipcommunication::Ports::iterator It;
	int MaxWidth = 0;
	int MaxVch = 0;
	int Nin = 0;
	int Nout = 0;
	double MaxF = 0 ;
	for (It = pPorts.Begin() ; It != pPorts.End() ; It++) {
		if (pPorts.If(It).GetWidth() > MaxWidth ) {
			MaxWidth = pPorts.If(It).GetWidth() ;
		}
		if (pPorts.If(It).GetNumberOfVirtualChannels() > MaxVch ) {
			MaxVch = pPorts.If(It).GetNumberOfVirtualChannels() ;
		}
		if ( pPorts.If(It).GetSpeed() > MaxF ) {
			MaxF = pPorts.If(It).GetSpeed() ;
		}
		if (pPorts.If(It).GetDirection()
				== cosi::quantity::onchipcommunication::Interface::IN ) {
			Nin++;
		}
		if (pPorts.If(It).GetDirection()
				== cosi::quantity::onchipcommunication::Interface::OUT ) {
			Nout++;
		}

	}

	if( Nin > mMaxIn )
		Nin = mMaxIn ;
	if( Nout > mMaxOut)
		Nout = mMaxOut ;

	if (MaxWidth <= 32) {
		if (MaxVch == 1) {
			return mErouterLeak1ch32[Nin][Nout] * (double) MaxF;
		} else {
			return mErouterLeak4ch32[Nin][Nout] * (double) MaxF;
		}
	} else {
		if (MaxVch == 1) {
			return mErouterLeak1ch128[Nin][Nout] * (double) MaxF;
		} else {
			return mErouterLeak4ch128[Nin][Nout] * (double) MaxF;
		}
	}
}

}

}

}
