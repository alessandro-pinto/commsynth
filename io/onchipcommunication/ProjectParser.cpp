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
#include "ProjectParser.h"

namespace cosi {

namespace io {

namespace onchipcommunication {

using namespace std ;

ProjectParser::ProjectParser(string pFname) {
	mFname = pFname ;
	mSpareA = 0.25;
	mTotalA = 0.0;
	mStep = 0.1;
	mFlitWidth = 128;
	mSwitchingFactor = 0.5;
	mMaxInDegree = 2;
	mMaxOutDegree = 2;
	mSourceOutDegree = 1;
	mDestInDegree = 1;
	mPowerVsArea = 0.5;
	mPruneLines = 1;
	mAllowPtP = false;
	mAllowTwoHops = true;
	mUsePlacement = false;
	mRunTimeDeadlockCheck = false;
	mHopConstraints = false;
	mAreaConstraint = false;
	mUseKmedian = false;
	mKmin = 1;
	mKmax = 1;
	mRunTimeBackTrack = false;

	mReport = false;
	mTabAppend = false;
	mSvg = false;
	mSystemC = false;
	mDot = false;
	mOtter = false;

	mLibraryDefined = false;
	mConstraintDefined = false;

	mBufferModelDefined = false;
	mWireModelDefined = false;

	mWireLayer = "global";
	mWireSpacing = "swsp";
	mWireBuffering = "mindelay";
	mWireShielding = false;

}

void ProjectParser::Parse() {

	TiXmlDocument doc(mFname.c_str() );
	bool loadSuccess = doc.LoadFile() ;
	if ( !loadSuccess ) {
		cout << "The Xml file "<< mFname << "  could not be loaded correctly "
				<< endl ;
		exit( 1) ;
		return;
	} else {
		ParseNode( &doc ) ;
		if ( !mLibraryDefined ) {
			cout << "ERROR : A library file must be defined in the Xml project"
					<< endl ;
			exit(1);
		}
		if ( !mConstraintDefined ) {
			cout
					<< "ERROR : A constraint file must be defined in the Xml project"
					<< endl ;
			exit(1);
		}

		if (mInputDir.size() == 0)
			cout << "WARNING : inputdir was not defined in the Xml project"
					<< endl ;
		if (mOutputDir.size() == 0)
			cout << "WARNING : outputdir was not defined in the Xml project"
					<< endl ;

		if (mAreaConstraint && mTotalA == 0.0) {
			cout
					<< "The problem is area constrainted but the total area has not been assigned or is equal to 0.0 "
					<< endl ;
			exit( 1) ;
		}

	}

}

string ProjectParser::GetProjectName() {
	return mProjectName ;
}

string ProjectParser::GetLibraryFileName() {
	return (mInputDir + "/"+ mLibraryFileName );
}

string ProjectParser::GetLibraryName() {
	return mLibraryName ;
}

string ProjectParser::GetConstraintFileName() {
	return (mInputDir + "/"+ mConstraintFileName );
}

string ProjectParser::GetPlacementFileName() {
	return (mInputDir + "/"+ mPlacementFileName );
}

bool ProjectParser::WireModelDefined() {
	return mWireModelDefined ;
}
string ProjectParser::GetWireFileName() {
	return (mInputDir + "/"+ mWireModelFileName );
}

bool ProjectParser::BufferModelDefined() {
	return mBufferModelDefined ;
}

string ProjectParser::GetBufferFileName() {
	return (mInputDir + "/"+ mBufferModelFileName );
}

string ProjectParser::GetWireLayer() {
	return mWireLayer ;
}

string ProjectParser::GetWireSpacing() {
	return mWireSpacing ;
}

string ProjectParser::GetWireBuffering() {
	return mWireBuffering ;
}

bool ProjectParser::GetWireShielding() {
	return mWireShielding ;
}

double ProjectParser::GetSpareArea() {
	return mSpareA ;
}

double ProjectParser::GetTotalArea() {
	return mTotalA ;
}

double ProjectParser::GetStep() {
	return mStep ;
}

int ProjectParser::GetFlitWidth() {
	return mFlitWidth ;
}

double ProjectParser::GetSwitchingFactor() {
	return mSwitchingFactor ;
}

double ProjectParser::GetPowerVsArea() {
	return mPowerVsArea ;
}
int ProjectParser::GetPruneLines() {
	return mPruneLines ;
}

bool ProjectParser::ReportRequest() {
	return mReport ;
}

bool ProjectParser::TabAppendRequest() {
	return mTabAppend ;
}

bool ProjectParser::SvgRequest() {
	return mSvg ;
}

string ProjectParser::GetReportFileName() {
	return (mOutputDir + "/"+ mReportFileName);
}

string ProjectParser::GetTabAppendFileName() {
	return (mOutputDir + "/"+ mTabAppendFileName);
}

string ProjectParser::GetSvgFileName() {
	return (mOutputDir + "/"+ mSvgFileName );
}

bool ProjectParser::DotRequest() {
	return mDot ;
}
bool ProjectParser::SystemCRequest() {
	return mSystemC ;
}
bool ProjectParser::OtterRequest() {
	return mOtter ;
}

string ProjectParser::GetDotFileName() {
	return (mOutputDir + "/"+ mDotFileName );
}

string ProjectParser::GetSystemCFileName() {
	return (mOutputDir + "/"+ mSystemCfileName );
}

string ProjectParser::GetSystemCcppFileName() {
	return (mSystemCfileName );
}

string ProjectParser::GetSystemCMakeFileName() {
	return (mOutputDir + "/"+ mMakeFileName );
}

string ProjectParser::GetOtterFileName() {
	return (mOutputDir + "/"+ mOtterFileName );
}

string ProjectParser::GetCosiRoot() {
	return mCosiRoot ;
}

string ProjectParser::GetSystemCRoot() {
	return mSystemCroot ;
}

int ProjectParser::GetMaxInDegree() {
	return mMaxInDegree ;
}

int ProjectParser::GetMaxOutDegree() {
	return mMaxOutDegree ;
}

int ProjectParser::GetSourceOutDegree() {
	return mSourceOutDegree ;
}

int ProjectParser::GetDestInDegree() {
	return mDestInDegree ;
}

bool ProjectParser::AllowPtP() {
	return mAllowPtP ;
}

bool ProjectParser::AllowTwoHops() {
	return mAllowTwoHops ;
}

bool ProjectParser::UsePlacement() {
	return mUsePlacement ;
}

int ProjectParser::GetDensity() {
	return mDensity ;
}

bool ProjectParser::RunTimeDeadlockCheck() {
	return mRunTimeDeadlockCheck ;
}

bool ProjectParser::HasHopConstraints() {
	return mHopConstraints ;
}

bool ProjectParser::HasAreaConstraint() {
	return mAreaConstraint ;
}

bool ProjectParser::UseKmedian() {
	return mUseKmedian ;
}
bool ProjectParser::RunTimeBackTrack() {
	return mRunTimeBackTrack ;
}
int ProjectParser::Kmin() {
	return mKmin ;
}
int ProjectParser::Kmax() {
	return mKmax ;
}

void ProjectParser::ParseNode(TiXmlNode* pParent) {

	if ( !pParent )
		return;

	int Type = pParent->Type() ;
	string Elem;
	switch (Type ) {

	case TiXmlNode::DOCUMENT :
		cout << "Parsing the project from file  "<< mFname << endl ;
		break;
	case TiXmlNode::ELEMENT :
		Elem = pParent->ValueStr() ;
		if (Elem == "Constraints") {
			GetConstraintsAttributes(pParent ) ;
			mConstraintDefined = true;
		} else if (Elem == "Library") {
			GetLibraryAttributes(pParent ) ;
			mLibraryDefined = true;
		} else if (Elem == "Parameters") {
			GetParametersAttributes(pParent ) ;
		} else if (Elem == "Output") {
			GetOutputAttributes(pParent ) ;
		} else if (Elem == "Svg") {
			GetSvgAttributes(pParent ) ;
		} else if (Elem == "SystemC") {
			GetSystemCAttributes(pParent ) ;
		} else if (Elem == "Dot") {
			GetDotAttributes(pParent ) ;
		} else if (Elem == "Report") {
			GetReportAttributes(pParent ) ;
		} else if (Elem == "TabAppend") {
			GetTabAppendAttributes(pParent ) ;
		} else if (Elem == "Systemc") {
			GetOutputAttributes(pParent ) ;
		} else if (Elem == "Otter") {
			GetOtterAttributes(pParent ) ;
		} else if (Elem == "Project") {
			GetProjectAttributes(pParent ) ;
		} else {
			cout << "Unknown XML element "<< Elem << " in file "<< mFname
					<< endl ;
		}
		break;

	case TiXmlNode::COMMENT :
		//cout << pParent->Value( ) << endl ;
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

void ProjectParser::GetLibraryAttributes(TiXmlNode* pParent) {
	double Tmp;
	int TmpInt;
	TiXmlAttribute* Attribute = ( pParent->ToElement())->FirstAttribute() ;
	while (Attribute ) {
		if (string(Attribute->Name() ) == "name") {
			mLibraryName = Attribute->ValueStr() ;
		} else if (string(Attribute->Name() ) == "file") {
			mLibraryFileName = Attribute->ValueStr() ;
		} else if (string(Attribute->Name() ) == "wirefile") {
			mWireModelDefined = true;
			mWireModelFileName = Attribute->ValueStr() ;
		} else if (string(Attribute->Name() ) == "bufferfile") {
			mBufferModelDefined = true;
			mBufferModelFileName = Attribute->ValueStr() ;
		} else if (string(Attribute->Name() ) == "layer") {
			mWireLayer = Attribute->ValueStr() ;
		} else if (string(Attribute->Name() ) == "spacing") {
			mWireSpacing = Attribute->ValueStr() ;
		} else if (string(Attribute->Name() ) == "buffering") {
			mWireBuffering = Attribute->ValueStr() ;
		} else if (string(Attribute->Name() ) == "shielding") {
			string tmp = Attribute->ValueStr() ;
			if (tmp == "false") {
				mWireShielding = false;
			} else {
				mWireShielding = true;
			}
		} else {
			cout << "Unknown attribute "<< Attribute->Name() << " for Library"
					<< " in "<< mFname << endl ;
		}
		Attribute = Attribute->Next() ;
	}
}

void ProjectParser::GetConstraintsAttributes(TiXmlNode* pParent) {
	double Tmp;
	int TmpInt;
	TiXmlAttribute* Attribute = ( pParent->ToElement())->FirstAttribute() ;
	while (Attribute ) {
		if (string(Attribute->Name() ) == "file") {
			mConstraintFileName = Attribute->ValueStr() ;
		} else if (string(Attribute->Name() ) == "useplacement") {
			mPlacementFileName = Attribute->ValueStr() ;
			mUsePlacement = true;
		} else {
			cout << "Unknown attribute "<< Attribute->Name()
					<< " for ConstraintFile"<< endl ;
		}
		Attribute = Attribute->Next() ;
	}
}

void ProjectParser::GetParametersAttributes(TiXmlNode* pParent) {
	double Tmp;
	int TmpInt;
	TiXmlAttribute* Attribute = ( pParent->ToElement())->FirstAttribute() ;
	while (Attribute ) {
		if (string(Attribute->Name() ) == "sparearea") {
			Attribute->QueryDoubleValue(&Tmp) ;
			mSpareA = Tmp ;

		} else if (string(Attribute->Name() ) == "step") {
			Attribute->QueryDoubleValue(&Tmp) ;
			mStep = Tmp ;

		} else if (string(Attribute->Name() ) == "flitwidth") {
			Attribute->QueryIntValue(&TmpInt) ;
			mFlitWidth = TmpInt ;

		} else if (string(Attribute->Name() ) == "switchingfactor") {
			Attribute->QueryDoubleValue(&Tmp) ;
			mSwitchingFactor = Tmp ;

		} else if (string(Attribute->Name() ) == "maxindegree") {
			Attribute->QueryIntValue(&TmpInt) ;
			mMaxInDegree = TmpInt ;

		} else if (string(Attribute->Name() ) == "maxoutdegree") {
			Attribute->QueryIntValue(&TmpInt) ;
			mMaxOutDegree = TmpInt ;

		} else if (string(Attribute->Name() ) == "destindegree") {
			Attribute->QueryIntValue(&TmpInt) ;
			mDestInDegree = TmpInt ;

		} else if (string(Attribute->Name() ) == "sourceoutdegree") {
			Attribute->QueryIntValue(&TmpInt) ;
			mSourceOutDegree = TmpInt ;

		} else if (string(Attribute->Name() ) == "allowptp") {
			Attribute->QueryIntValue(&TmpInt) ;
			if (TmpInt == 1)
				mAllowPtP = true;
			else
				mAllowPtP = false;
		} else if (string(Attribute->Name() ) == "allowtwohops") {
			Attribute->QueryIntValue(&TmpInt) ;
			if (TmpInt == 1)
				mAllowTwoHops = true;
			else
				mAllowTwoHops = false;

		} else if (string(Attribute->Name() ) == "runtimedeadlockcheck") {
			Attribute->QueryIntValue(&TmpInt) ;
			if (TmpInt == 1)
				mRunTimeDeadlockCheck = true;
			else
				mRunTimeDeadlockCheck = false;

		} else if (string(Attribute->Name() ) == "hopconstraints") {
			Attribute->QueryIntValue(&TmpInt) ;
			if (TmpInt == 1)
				mHopConstraints = true;
			else
				mHopConstraints = false;

		} else if (string(Attribute->Name() ) == "areaconstraint") {
			Attribute->QueryIntValue(&TmpInt) ;
			if (TmpInt == 1)
				mAreaConstraint = true;
			else
				mAreaConstraint = false;

		} else if (string(Attribute->Name() ) == "area") {
			Attribute->QueryDoubleValue(&Tmp) ;
			mTotalA = Tmp ;
		} else if (string(Attribute->Name() ) == "powervsarea") {
			Attribute->QueryDoubleValue(&Tmp) ;
			mPowerVsArea = Tmp ;
		} else if (string(Attribute->Name() ) == "prunelines") {
			Attribute->QueryIntValue(&TmpInt) ;
			mPruneLines = TmpInt ;
		} else if (string(Attribute->Name() ) == "density") {
			Attribute->QueryIntValue(&TmpInt) ;
			mDensity = TmpInt ;
		} else if (string(Attribute->Name() ) == "kmedian") {
			string Val = Attribute->ValueStr() ;
			if (Val == "yes")
				mUseKmedian = true;
			else if (Val == "no")
				mUseKmedian = false;
			else
				cout << "Unknown value  "<< Val << " for parameter kmedian"
						<< endl ;

		} else if (string(Attribute->Name() ) == "runtimebacktrack") {
			string Val = Attribute->ValueStr() ;
			if (Val == "yes")
				mRunTimeBackTrack = true;
			else if (Val == "no")
				mRunTimeBackTrack = false;
			else
				cout << "Unknown value  "<< Val << " for parameter runtimebacktrack"
						<< endl ;

		} else if (string(Attribute->Name() ) == "kmin") {
			Attribute->QueryIntValue(&mKmin) ;

		} else if (string(Attribute->Name() ) == "kmax") {
			Attribute->QueryIntValue(&mKmax);

		} else {
			cout << "Unknown attribute "<< Attribute->Name()
					<< " for Parameters"<< endl ;
		}
		Attribute = Attribute->Next() ;
	}
}

void ProjectParser::GetProjectAttributes(TiXmlNode* pParent) {
	double Tmp;
	int TmpInt;
	TiXmlAttribute* Attribute = ( pParent->ToElement())->FirstAttribute() ;
	while (Attribute ) {
		if (string(Attribute->Name() ) == "name") {
			mProjectName = Attribute->ValueStr() ;
		} else if (string(Attribute->Name() ) == "cosiroot") {
			mCosiRoot = Attribute->ValueStr() ;
		} else if (string(Attribute->Name() ) == "systemc") {
			mSystemCroot = Attribute->ValueStr() ;
		} else if (string(Attribute->Name() ) == "input") {
			mInputDir = Attribute->ValueStr() ;
		} else if (string(Attribute->Name() ) == "output") {
			mOutputDir = Attribute->ValueStr() ;
		} else {
			cout << "Unknown attribute "<< Attribute->Name() << " for Project"
					<< endl ;
		}
		Attribute = Attribute->Next() ;
	}
}

void ProjectParser::GetOutputAttributes(TiXmlNode* pParent) {
}

void ProjectParser::GetSvgAttributes(TiXmlNode* pParent) {
	double Tmp;
	int TmpInt;
	TiXmlAttribute* Attribute = ( pParent->ToElement())->FirstAttribute() ;
	while (Attribute ) {
		if (string(Attribute->Name() ) == "name") {
			mSvgFileName = Attribute->ValueStr() ;
			mSvg = true;
		} else {
			cout << "Unknown attribute "<< Attribute->Name() << " for Output"
					<< endl ;
		}
		Attribute = Attribute->Next() ;
	}
}

void ProjectParser::GetSystemCAttributes(TiXmlNode* pParent) {
	double Tmp;
	int TmpInt;
	TiXmlAttribute* Attribute = ( pParent->ToElement())->FirstAttribute() ;
	while (Attribute ) {
		if (string(Attribute->Name() ) == "name") {
			mSystemCfileName = Attribute->ValueStr() ;
			mSystemC = true;
		} else if (string(Attribute->Name() ) == "mk") {
			mMakeFileName = Attribute->ValueStr() ;
		} else {
			cout << "Unknown attribute "<< Attribute->Name() << " for Output"
					<< endl ;
		}
		Attribute = Attribute->Next() ;
	}
}

void ProjectParser::GetDotAttributes(TiXmlNode* pParent) {
	double Tmp;
	int TmpInt;
	TiXmlAttribute* Attribute = ( pParent->ToElement())->FirstAttribute() ;
	while (Attribute ) {
		if (string(Attribute->Name() ) == "name") {
			mDotFileName = Attribute->ValueStr() ;
			mDot = true;
		} else {
			cout << "Unknown attribute "<< Attribute->Name() << " for Output"
					<< endl ;
		}
		Attribute = Attribute->Next() ;
	}
}

void ProjectParser::GetOtterAttributes(TiXmlNode* pParent) {
	double Tmp;
	int TmpInt;
	TiXmlAttribute* Attribute = ( pParent->ToElement())->FirstAttribute() ;
	while (Attribute ) {
		if (string(Attribute->Name() ) == "name") {
			mOtterFileName = Attribute->ValueStr() ;
			mOtter = true;
		} else {
			cout << "Unknown attribute "<< Attribute->Name() << " for Output"
					<< endl ;
		}
		Attribute = Attribute->Next() ;
	}
}

void ProjectParser::GetReportAttributes(TiXmlNode* pParent) {
	double Tmp;
	int TmpInt;
	TiXmlAttribute* Attribute = ( pParent->ToElement())->FirstAttribute() ;
	while (Attribute ) {
		if (string(Attribute->Name() ) == "name") {
			mReportFileName = Attribute->ValueStr() ;
			mReport = true;
		} else {
			cout << "Unknown attribute "<< Attribute->Name() << " for Output"
					<< endl ;
		}
		Attribute = Attribute->Next() ;
	}
}

void ProjectParser::GetTabAppendAttributes(TiXmlNode* pParent) {
	double Tmp;
	int TmpInt;
	TiXmlAttribute* Attribute = ( pParent->ToElement())->FirstAttribute() ;
	while (Attribute ) {
		if (string(Attribute->Name() ) == "name") {
			mTabAppendFileName = Attribute->ValueStr() ;
			mTabAppend = true;
		} else {
			cout << "Unknown attribute "<< Attribute->Name()<< " for TabAppend"
					<< endl ;
		}
		Attribute = Attribute->Next() ;
	}
}
}
}
}
