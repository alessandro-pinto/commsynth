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
#include "ConstraintParser.h"

namespace cosi {

namespace io {

namespace onchipcommunication {

using namespace std;
using namespace cosi::environment::onchipcommunication;

ConstraintParser::ConstraintParser(string Fname) {
	mFname = Fname ;
}

void ConstraintParser::Parse() {

	TiXmlDocument doc(mFname.c_str() );
	bool loadSuccess = doc.LoadFile() ;
	if ( !loadSuccess ) {
		cout << "The Xml file "<< mFname << "  could not be loaded correctly "
				<< endl ;
		exit(1);
		return;
	} else {
		ParseNode( &doc ) ;

	}
}

void ConstraintParser::ParseNode(TiXmlNode* Parent) {

	if ( !Parent )
		return;

	int Type = Parent->Type() ;
	string Elem;
	string IpName;

	switch (Type ) {

	case TiXmlNode::DOCUMENT :
		cout << "Parsing the communicaiton constraints from file  "<< mFname
				<< endl ;
		break;

	case TiXmlNode::ELEMENT :
		Elem = Parent->ValueStr() ;
		//if the element is the filetype then set the type
		if (Elem == "Constraints") {
		} else if (Elem == "Constraint") {
			AddConstraint(Parent ) ;
		} else if (Elem == "Core") {
			AddCore(Parent ) ;
		} else if (Elem == "Exclusion") {
			AddExclusion(Parent ) ;
		} else {
			cout << "Unknown XML element "<< Elem << " in file "<< mFname
					<< endl ;
		}
		break;

	case TiXmlNode::COMMENT :
		//cout << Parent->Value( ) << endl ;
		break;

	case TiXmlNode::TEXT :
		break;

	case TiXmlNode::DECLARATION :
		break;

	default:
		break;

	}

	TiXmlNode* Child;
	for (Child = Parent->FirstChild() ; Child != 0; Child
			= Child->NextSibling() ) {
		ParseNode(Child ) ;
	}

}

void ConstraintParser::AddConstraint(TiXmlNode* Node) {

	Constraint C;
	double Bw = 0;
	double T = 0;
	string Sourse, Dest, Name;
	//parse all the attributes
	TiXmlAttribute* Attribute = ( Node->ToElement())->FirstAttribute() ;
	while (Attribute ) {

		if (string(Attribute->Name() ) == "source") {
			C.SetSourceName(string(Attribute->Value() ) ) ;
		} else if (string(Attribute->Name() ) == "dest") {
			C.SetDestinationName(string(Attribute->Value() ) ) ;
		} else if (string(Attribute->Name() ) == "bw") {
			Attribute->QueryDoubleValue(&Bw) ;
			C.SetBandwidth(Bw) ;
		} else if (string(Attribute->Name() ) == "T") {
			Attribute->QueryDoubleValue(&T) ;
			C.SetDelay(T ) ;
		} else if (string(Attribute->Name() ) == "name") {
			Name = Attribute->ValueStr() ;
		} else {
			cout << "Unknown attribute "<< Attribute->Name()
					<< " for constraint"<< endl ;
		}
		Attribute = Attribute->Next() ;
	}

	CosiLog << C.GetSourceName() << " to "<< C.GetDestinationName()
			<< "  Bw = "<< C.GetBandwidth() << "  Delay = "<< C.GetDelay()
			<< endl ;

	mNameToInt[ Name ] = mConstraint.size() ;
	mIntToName[ mConstraint.size( ) ] = Name ;
	mConstraint.push_back(C ) ;
}

void ConstraintParser::AddCore(TiXmlNode* Node) {

	IpCore Ip;
	PlacedIpCore PlacedIp;
	string Name , ComponentName;
	bool Placed = false;
	double Xbl, Ybl, Xtr, Ytr, Area;
	Xbl = Ybl = Xtr = Ytr = Area = 0.0;
	Name = "NoName";
	ComponentName = "NoName";
	TiXmlAttribute* Attribute = (Node->ToElement())->FirstAttribute() ;
	while (Attribute ) {

		if (string(Attribute->Name() ) == "placed") {
			if (Attribute->ValueStr() == "yes")
				Placed = true;
			else
				Placed = false;
		} else if (string(Attribute->Name() ) == "name") {
			Name = Attribute->ValueStr() ;
		} else if (string(Attribute->Name() ) == "componentname") {
					ComponentName = Attribute->ValueStr() ;
		} else if (string(Attribute->Name() ) == "area") {
			Attribute->QueryDoubleValue(&Area) ;
		} else if (string(Attribute->Name()) == "xbl") {
			Attribute->QueryDoubleValue(&Xbl) ;

		} else if (string(Attribute->Name()) == "ybl") {
			Attribute->QueryDoubleValue(&Ybl) ;

		} else if (string(Attribute->Name() ) == "xtr") {
			Attribute->QueryDoubleValue(&Xtr) ;

		} else if (string(Attribute->Name() ) == "ytr") {
			Attribute->QueryDoubleValue(&Ytr) ;

		} else {
			cout << "Unknown attribute "<< Attribute->Name() << " for  core "
					<< endl ;
		}
		Attribute = Attribute->Next() ;
	}

	if (Placed ) {
		PlacedIp.SetName(Name ) ;
		PlacedIp.SetComponentName(ComponentName);
		PlacedIp.SetBottomLeftX(Xbl ) ;
		PlacedIp.SetBottomLeftY(Ybl ) ;
		PlacedIp.SetTopRightX(Xtr ) ;
		PlacedIp.SetTopRightY(Ytr ) ;
		PlacedIp.SetArea((Xtr - Xbl)*(Ytr - Ybl)) ;
		mPlacedIp[ Name ] = PlacedIp ;
		CosiLog << Name << " is placed in "<< " ("<< Xbl << ","<< Ybl
				<< ")   ("<<Xtr << ","<< Ytr << ")"<< endl ;
	} else {
		Ip.SetName(Name ) ;
		Ip.SetComponentName(ComponentName);
		Ip.SetArea(Area ) ;
		mIp[ Name ] = Ip ;
		CosiLog << Name << " is not placed "<< "  has area  "<< Area << endl ;

	}
}

void ConstraintParser::AddExclusion(TiXmlNode* Node) {
	stringstream s;
	string C, C1;
	TiXmlAttribute* Attribute = (Node->ToElement())->FirstAttribute() ;
	while (Attribute ) {

		if (string(Attribute->Name() ) == "set") {
			s << Attribute->ValueStr() ;
			//Bild a set of all constraints
			set< string > Eset;
			while ( !s.eof() ) {
				s >> C ;
				Eset.insert(C ) ;
			}
			set<string>::iterator It1;
			set<string>::iterator It2;
			for (It1 = Eset.begin() ; It1 != Eset.end() ; It1++) {
				It2 = It1 ;
				It2++;
				for (; It2 != Eset.end() ; It2++) {
					if (mExclusions.find( *It1 ) == mExclusions.end() ) {
						mExclusions[*It1] = set<string>() ;
						mExclusions[*It1].insert( *It2 ) ;
					} else {
						mExclusions[*It1].insert( *It2 ) ;
					}

					if (mExclusions.find( *It2 ) == mExclusions.end() ) {
						mExclusions[*It2] = set<string>() ;
						mExclusions[*It2].insert( *It1 ) ;
					} else {
						mExclusions[*It2].insert( *It1 ) ;
					}
				}
			}
		} else if (string(Attribute->Name() ) == "pair") {
			s << Attribute->ValueStr() ;
			s >> C ;
			s >> C1 ;
			if (mExclusions.find(C ) == mExclusions.end() ) {
				mExclusions[C] = set<string>() ;
				mExclusions[C].insert(C1 ) ;
			} else {
				mExclusions[C].insert(C1 ) ;
			}

			if (mExclusions.find(C1 ) == mExclusions.end() ) {
				mExclusions[C1] = set<string>() ;
				mExclusions[C1].insert(C ) ;
			} else {
				mExclusions[C1].insert(C ) ;
			}
		} else {
			cout << "Unknown attribute "<< Attribute->Name() << " for  core "
					<< endl ;
		}
		Attribute = Attribute->Next() ;
	}

}

void ConstraintParser::WritePlacement(string pFname) {

	ofstream f;
	f.open(pFname.c_str() ) ;

	f << "UMICH blocks 1.0"<< endl ;
	f << endl ;

	map< string , PlacedIpCore >::iterator It;
	vector< PlacedIpCore > ToReturn;
	for (It = mPlacedIp.begin() ; It != mPlacedIp.end() ; It++) {
		f << endl ;
		f << (It->second).GetName() ;
		f << "\t";
		f << (It->second).GetBottomLeftX() << "  "<< (It->second).GetBottomLeftY() ;
		f << "\t";
		f << "DIMS = ("<< (It->second).GetTopRightX() - (It->second).GetBottomLeftX() << " , "<< (It->second).GetTopRightY() - (It->second).GetBottomLeftY() << ")";
		f << "\t";
		f << ": N";
	}

	f.close() ;

}

vector< PlacedIpCore > ConstraintParser::GetPlacedIpCore() {
	map< string , PlacedIpCore >::iterator It;
	vector< PlacedIpCore > ToReturn;
	for (It = mPlacedIp.begin() ; It != mPlacedIp.end() ; It++) {
		ToReturn.push_back(It->second) ;
	}
	return ToReturn ;
}

PlacedIpCore ConstraintParser::GetPlacedIpCore(string pName) {
	return mPlacedIp[pName];
}

IpCore ConstraintParser::GetIpCore(string pName) {
	return mIp[pName] ;
}

vector< IpCore > ConstraintParser::GetIpCore() {
	map< string , IpCore >::iterator It;
	vector< IpCore > ToReturn;
	for (It = mIp.begin() ; It != mIp.end() ; It++) {
		ToReturn.push_back(It->second) ;
	}
	return ToReturn ;
}

vector< Constraint > ConstraintParser::GetConstraints() {
	return mConstraint ;
}

set< int > ConstraintParser::GetExclusions(int i) {
	//Get all constraints that are in mutual exclusion with this.
	//Get the constraint name
	set<int> RetVal;
	string C;
	C = mIntToName[ i ];
	set<string> StringSet;
	if (mExclusions.find(C ) != mExclusions.end() ) {
		StringSet = mExclusions[C];
		set<string>::iterator It;

		for (It = StringSet.begin() ; It != StringSet.end() ; It++) {
			RetVal.insert(mNameToInt[*It]) ;
		}
	}
	return RetVal ;
}

bool ConstraintParser::AllPlaced() {
	return (mIp.size() == 0);
}
}
}
}
