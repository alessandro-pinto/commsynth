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
#include "HoOrionModelParser.h"

namespace cosi {

namespace io {

namespace onchipcommunication {

using namespace std;

HoOrionModelParser::HoOrionModelParser(string pFname) {
	mFname = pFname ;
}

void HoOrionModelParser::Parse() {

	TiXmlDocument doc(mFname.c_str() );
	bool loadSuccess = doc.LoadFile() ;
	if ( !loadSuccess ) {
		cout << "The Xml file "<< mFname << "  could not be loaded correctly "
				<< endl ;
		exit( 1) ;
		return;
	} else {
		ParseNode( &doc ) ;
	}
}

void HoOrionModelParser::ParseNode(TiXmlNode* pParent) {
	if ( !pParent )
		return;

	int Type = pParent->Type() ;
	string Elem;
	switch (Type ) {

	case TiXmlNode::DOCUMENT :
		CosiLog << "Parsing the library from file  "<< mFname << endl ;
		break;
	case TiXmlNode::ELEMENT :
		Elem = pParent->ValueStr() ;
		if (Elem == "Model") {
			ParseModel(pParent ) ;
		} else if (Elem == "Technology") {
			ParseTechnology(pParent ) ;
		} else if (Elem == "Wire") {
			ParseWire(pParent );
		} else if (Elem == "Router") {
			ParseRouter(pParent );
		} else {
			cout << "Unknown XML element "<< Elem << " in file "<< mFname
					<< endl ;
		}
		break;

	case TiXmlNode::COMMENT :
		cout << pParent->Value() << endl ;
		break;

	case TiXmlNode::TEXT :
		break;

	case TiXmlNode::DECLARATION :
		break;

	default:
		break;

	}

	TiXmlNode* Child;
	for (Child = pParent->FirstChild() ; Child != 0; Child
			= Child->NextSibling() ) {
		ParseNode(Child ) ;
	}
}

void HoOrionModelParser::ParseModel(TiXmlNode* pParent) {
	// Get the library parameter that is actually the name
	string LibraryName;
	TiXmlAttribute* Attribute = ( pParent->ToElement())->FirstAttribute() ;
	while (Attribute ) {
		if (string(Attribute->Name() ) == "name") {
			mModelName = string(Attribute->Value() ) ;
		} else {
			cout << "Unknown attribute "<< Attribute->Name() << " for model"
					<< endl ;
		}
		Attribute = Attribute->Next() ;
	}
	CosiLog << "Model "<< mModelName << endl ;
}

void HoOrionModelParser::ParseTechnology(TiXmlNode* pParent) {
	double Tmp;
	int TmpInt;
	TiXmlAttribute* Attribute = ( pParent->ToElement())->FirstAttribute() ;
	while (Attribute ) {
		if (string(Attribute->Name() ) == "fclk") {
			Attribute->QueryDoubleValue(&Tmp) ;
		} else if (string(Attribute->Name() ) == "vdd") {
			Attribute->QueryDoubleValue(&Tmp) ;
			mV_DD = Tmp ;
		} else if (string(Attribute->Name() ) == "ioff") {
			Attribute->QueryDoubleValue(&Tmp) ;
			mI_off = Tmp ;
		} else if (string(Attribute->Name() ) == "wmin") {
			Attribute->QueryDoubleValue(&Tmp) ;
			mW_min = Tmp ;
		} else if (string(Attribute->Name() ) == "isc") {
			Attribute->QueryDoubleValue(&Tmp) ;
			mI_sc = Tmp ;
		} else if (string(Attribute->Name() ) == "r0") {
			Attribute->QueryDoubleValue(&Tmp) ;
			mR_0 = Tmp ;
		} else if (string(Attribute->Name() ) == "cp") {
			Attribute->QueryDoubleValue(&Tmp) ;
			mC_p = Tmp ;
		} else if (string(Attribute->Name() ) == "c0") {
			Attribute->QueryDoubleValue(&Tmp) ;
			mC_0 = Tmp ;
		} else if (string(Attribute->Name() ) == "nlayers") {
			Attribute->QueryIntValue(&TmpInt) ;
			mR.resize(TmpInt) ;
			mC.resize(TmpInt) ;
			mPitch.resize(TmpInt) ;
		} else {
			cout << "Unknown attribute "<< Attribute->Name()
					<< " for technology"<< endl ;
		}
		Attribute = Attribute->Next() ;
	}
}

void HoOrionModelParser::ParseWire(TiXmlNode* pParent) {
	double C, R, Pitch;
	int Layer;
	TiXmlAttribute* Attribute = ( pParent->ToElement())->FirstAttribute() ;
	while (Attribute ) {
		if (string(Attribute->Name() ) == "type") {
		} else if (string(Attribute->Name() ) == "layer") {
			Attribute->QueryIntValue(&Layer) ;
		} else if (string(Attribute->Name() ) == "r") {
			Attribute->QueryDoubleValue(&R) ;
		} else if (string(Attribute->Name() ) == "c") {
			Attribute->QueryDoubleValue(&C) ;
		} else if (string(Attribute->Name() ) == "pitch") {
			Attribute->QueryDoubleValue(&Pitch) ;
		} else {
			cout << "Unknown attribute "<< Attribute->Name() << " for wire"
					<< endl ;
		}
		Attribute = Attribute->Next() ;
	}
	mR[Layer] = R ;
	mC[Layer] = C ;
	mPitch[Layer] = Pitch ;
}

void HoOrionModelParser::ParseRouter(TiXmlNode* pParent) {
	double TmpDouble;
	int TmpInt;
	string Type;
	stringstream Energy;
	stringstream Area;
	int MaxIn = 0;
	int MaxOut = 0;
	TiXmlAttribute* Attribute = ( pParent->ToElement())->FirstAttribute() ;
	while (Attribute ) {
		if (string(Attribute->Name() ) == "type") {
			Type = string(Attribute->Value() ) ;
		} else if (string(Attribute->Name() ) == "maxin") {
			Attribute->QueryIntValue(&TmpInt) ;
			MaxIn = TmpInt ;
		} else if (string(Attribute->Name() ) == "maxout") {
			Attribute->QueryIntValue(&TmpInt) ;
			MaxOut = TmpInt ;
		} else if (string(Attribute->Name() ) == "maxbw") {
			Attribute->QueryDoubleValue(&TmpDouble) ;
			//mModel->RouterMaxBw = TmpDouble ;
		} else if (string(Attribute->Name() ) == "energy") {
			Energy << Attribute->ValueStr() ;
		} else if (string(Attribute->Name() ) == "area") {
			Area << Attribute->ValueStr() ;
		} else {
			cout << "Unknown attribute "<< Attribute->Name() << " for wire"
					<< endl ;
		}
		Attribute = Attribute->Next() ;
	}

	mMaxIn = MaxIn ;
	mMaxOut = MaxOut ;

	if (Type == "r1ch32") {
		mArouter1ch32.resize(MaxIn + 1) ;
		mErouter1ch32.resize(MaxIn + 1) ;
		mErouterLeak1ch32.resize(MaxIn + 1) ;
		mArouter1ch32[0].resize(MaxOut + 1) ;
		mErouter1ch32[0].resize(MaxOut + 1) ;
		mErouterLeak1ch32[0].resize(MaxOut + 1) ;
		for (int i = 1; i <= MaxIn ; i++) {
			mArouter1ch32[i].resize(MaxOut + 1) ;
			mErouter1ch32[i].resize(MaxOut + 1) ;
			mErouterLeak1ch32[i].resize(MaxOut + 1) ;
			for (int j = 1; j <= MaxOut ; j++) {
				Area >> mArouter1ch32[i][j];
				mArouter1ch32[i][j] = mArouter1ch32[i][j]*1e-12;
				Energy >> mErouter1ch32[i][j];
				Energy >> mErouterLeak1ch32[i][j];
			}
		}

		//handle first rows nad first column
		for (int j = 0; j <= MaxOut ; j++) {
			mArouter1ch32[0][j] = 0;
			mErouter1ch32[0][j] = 0;
			mErouterLeak1ch32[0][j] = 0;
		}
		for (int i = 0; i <= MaxIn ; i++) {
			mArouter1ch32[i][0] = 0;
			mErouter1ch32[i][0] = 0;
			mErouterLeak1ch32[i][0] = 0;
		}

	} else if (Type == "r4ch32") {
		mArouter4ch32.resize(MaxIn + 1) ;
		mErouter4ch32.resize(MaxIn + 1) ;
		mErouterLeak4ch32.resize(MaxIn + 1) ;
		(mArouter4ch32)[0].resize(MaxOut + 1) ;
		(mErouter4ch32)[0].resize(MaxOut + 1) ;
		(mErouterLeak4ch32)[0].resize(MaxOut + 1) ;
		for (int i = 1; i <= MaxIn ; i++) {
			(mArouter4ch32)[i].resize(MaxOut + 1) ;
			(mErouter4ch32)[i].resize(MaxOut + 1) ;
			(mErouterLeak4ch32)[i].resize(MaxOut + 1) ;
			for (int j = 1; j <= MaxOut ; j++) {
				Area >> (mArouter4ch32)[i][j];
				(mArouter4ch32)[i][j] = (mArouter4ch32)[i][j]*1e-12;
				Energy >> (mErouter4ch32)[i][j];
				Energy >> (mErouterLeak4ch32)[i][j];
			}
		}
		//handle first rows nad first column
		for (int j = 0; j <= MaxOut ; j++) {
			mArouter4ch32[0][j] = 0;
			mErouter4ch32[0][j] = 0;
			mErouterLeak4ch32[0][j] = 0;
		}
		for (int i = 0; i <= MaxIn ; i++) {
			mArouter4ch32[i][0] = 0;
			mErouter4ch32[i][0] = 0;
			mErouterLeak4ch32[i][0] = 0;
		}

	} else if (Type == "r1ch128") {
		mArouter1ch128.resize(MaxIn + 1) ;
		mErouter1ch128.resize(MaxIn + 1) ;
		mErouterLeak1ch128.resize(MaxIn + 1) ;
		(mArouter1ch128)[0].resize(MaxOut + 1) ;
		(mErouter1ch128)[0].resize(MaxOut + 1) ;
		(mErouterLeak1ch128)[0].resize(MaxOut + 1) ;
		for (int i = 1; i <= MaxIn ; i++) {
			(mArouter1ch128)[i].resize(MaxOut + 1) ;
			(mErouter1ch128)[i].resize(MaxOut + 1) ;
			(mErouterLeak1ch128)[i].resize(MaxOut + 1) ;
			for (int j = 1; j <= MaxOut ; j++) {
				Area >> (mArouter1ch128)[i][j];
				(mArouter1ch128)[i][j] = (mArouter1ch128)[i][j]*1e-12;
				Energy >> (mErouter1ch128)[i][j];
				Energy >> (mErouterLeak1ch128)[i][j];
			}
		}
		//handle first rows nad first column
		for (int j = 0; j <= MaxOut ; j++) {
			mArouter1ch128[0][j] = 0;
			mErouter1ch128[0][j] = 0;
			mErouterLeak1ch128[0][j] = 0;
		}
		for (int i = 0; i <= MaxIn ; i++) {
			mArouter1ch128[i][0] = 0;
			mErouter1ch128[i][0] = 0;
			mErouterLeak1ch128[i][0] = 0;
		}

	} else if (Type == "r4ch128") {
		mArouter4ch128.resize(MaxIn + 1) ;
		mErouter4ch128.resize(MaxIn + 1) ;
		mErouterLeak4ch128.resize(MaxIn + 1) ;
		(mArouter4ch128)[0].resize(MaxOut + 1) ;
		(mErouter4ch128)[0].resize(MaxOut + 1) ;
		(mErouterLeak4ch128)[0].resize(MaxOut + 1) ;
		for (int i = 1; i <= MaxIn ; i++) {
			(mArouter4ch128)[i].resize(MaxOut + 1) ;
			(mErouter4ch128)[i].resize(MaxOut + 1) ;
			(mErouterLeak4ch128)[i].resize(MaxOut + 1) ;
			for (int j = 1; j <= MaxOut ; j++) {
				Area >> (mArouter4ch128)[i][j];
				(mArouter4ch128)[i][j] = (mArouter4ch128)[i][j]*1e-12;
				Energy >> (mErouter4ch128)[i][j];
				Energy >> (mErouterLeak4ch128)[i][j];
			}
		}
		//handle first rows nad first column
		for (int j = 0; j <= MaxOut ; j++) {
			mArouter4ch128[0][j] = 0;
			mErouter4ch128[0][j] = 0;
			mErouterLeak4ch128[0][j] = 0;
		}
		for (int i = 0; i <= MaxIn ; i++) {
			mArouter4ch128[i][0] = 0;
			mErouter4ch128[i][0] = 0;
			mErouterLeak4ch128[i][0] = 0;
		}

	} else {
		cout << "The model cannot handle type "<< Type << endl ;
	}

}

double HoOrionModelParser::GetWmin() {
	return mW_min ;
}
double HoOrionModelParser::GetR0() {
	return mR_0;
}
double HoOrionModelParser::GetC0() {
	return mC_0 ;
}
double HoOrionModelParser::GetCp() {
	return mC_p ;
}
vector<double> HoOrionModelParser::GetR() {
	return mR ;
}
vector<double> HoOrionModelParser::GetC() {
	return mC ;
}
vector<double> HoOrionModelParser::GetPitch() {
	return mPitch ;
}
double HoOrionModelParser::GetVdd() {
	return mV_DD ;
}
double HoOrionModelParser::GetIoff() {
	return mI_off ;
}

vector<vector<double> > HoOrionModelParser::GetArea1ch32() {
	return mArouter1ch32 ;
}
vector<vector<double> > HoOrionModelParser::GetArea4ch32() {
	return mArouter4ch32 ;
}
vector<vector<double> > HoOrionModelParser::GetArea1ch128() {
	return mArouter1ch128 ;
}
vector<vector<double> > HoOrionModelParser::GetArea4ch128() {
	return mArouter4ch128 ;
}
vector<vector<double> > HoOrionModelParser::GetDynamicEnergy1ch32() {
	return mErouter1ch32 ;
}
vector<vector<double> > HoOrionModelParser::GetDynamicEnergy4ch32() {
	return mErouter4ch32 ;
}
vector<vector<double> > HoOrionModelParser::GetDynamicEnergy1ch128() {
	return mErouter1ch128 ;
}
vector<vector<double> > HoOrionModelParser::GetDynamicEnergy4ch128() {
	return mErouter4ch128 ;
}
vector<vector<double> > HoOrionModelParser::GetStaticEnergy1ch32() {
	return mErouterLeak1ch32 ;
}
vector<vector<double> > HoOrionModelParser::GetStaticEnergy4ch32() {
	return mErouterLeak4ch32 ;
}
vector<vector<double> > HoOrionModelParser::GetStaticEnergy1ch128() {
	return mErouterLeak1ch128 ;
}
vector<vector<double> > HoOrionModelParser::GetStaticEnergy4ch128() {
	return mErouterLeak4ch128 ;
}

int HoOrionModelParser::GetMaxIn() {
	return mMaxIn ;
}

int HoOrionModelParser::GetMaxOut(){
	return mMaxOut;
}


}
}
}
