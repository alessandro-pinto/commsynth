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
#include "HoAreaModel.h"

namespace cosi {

namespace models {

namespace onchipcommunication {

using namespace cosi::library::onchipcommunication ;

HoAreaModel::HoAreaModel(Technology pTech) {


	cosi::io::onchipcommunication::HoOrionModelParser* P = 0x0 ;

	if ( pTech == T90 ) {
		P = new cosi::io::onchipcommunication::HoOrionModelParser(CosiRoot + "/models/onchipcommunication/link/ho-orion-90nm.xml") ;
	} else if ( pTech == T65 ) {
		P = new cosi::io::onchipcommunication::HoOrionModelParser(CosiRoot + "/models/onchipcommunication/link/ho-orion-65nm.xml") ;
	} else if (pTech == T45) {
		P = new cosi::io::onchipcommunication::HoOrionModelParser(CosiRoot + "/models/onchipcommunication/link/ho-orion-45nm.xml") ;
	}

	P->Parse() ;

	mWmin = P->GetWmin() ;
	mC0 = P->GetC0() ;
	mCp = P->GetCp() ;
	mR0 = P->GetR0() ;
	mR = P->GetR();
	mC = P->GetC() ;
	mPitch = P->GetPitch() ;


	delete P ;

	//Compute optimal repeater size
	mKopt = ( int ) sqrt(mR0 * (mC[6] )/ (mR[6] *mC0)) ;
	//Optimal sequential length
	mHopt = sqrt( 2*mR0*(mC0 + mCp)/(mR[6] * (mC[6] ))) ;
}

HoAreaModel::~HoAreaModel() {
}

double HoAreaModel::GetDeviceArea(Name pName,
		cosi::quantity::onchipcommunication::Interface pSource,
		cosi::quantity::onchipcommunication::Interface pDest, WiringPath pPath,
		cosi::quantity::onchipcommunication::LinkParameter pParam) {

	//Compute the length
	double Length = pPath.Length() ;
	//Compute the flit width
	int Width = pSource.GetWidth() ;

	return (Length/mHopt*mWmin*mWmin*mKopt*Width/2.0);

}

double HoAreaModel::GetWiringArea(Name pName,
		cosi::quantity::onchipcommunication::Interface pSource,
		cosi::quantity::onchipcommunication::Interface pDest, WiringPath pPath,
		cosi::quantity::onchipcommunication::LinkParameter pParam) {

	//Compute the length
	double Length = pPath.Length() ;
	//Compute the flit width
	int Width = pSource.GetWidth() ;
	return (Length * mPitch[6] * Width );

}



}

}

}
