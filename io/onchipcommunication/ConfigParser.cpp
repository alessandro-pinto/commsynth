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
#include "ConfigParser.h"

namespace cosi {

namespace io {

namespace onchipcommunication {

ConfigParser::ConfigParser(string pFname) {
	mFname = pFname;
}

void ConfigParser::Parse() {

	TiXmlDocument doc(mFname.c_str() );
	bool loadSuccess = doc.LoadFile() ;
	if ( !loadSuccess) {
		cout << "The Xml file "<< mFname << "  could not be loaded correctly "
				<< endl;
		exit( 1) ;
		return;
	} else {
		ParseNode( &doc) ;

	}
}

void ConfigParser::ParseNode(TiXmlNode* pParent) {
	if ( !pParent)
		return;

	int Type = pParent->Type() ;
	string Elem;

	switch (Type) {

	case TiXmlNode::DOCUMENT:
		cout << "Parsing configuration file  "<< mFname << endl;
		break;
	case TiXmlNode::ELEMENT:
		Elem = pParent->ValueStr() ;
		if (Elem == "Configuration") {
			;
		} else if (Elem == "COSI") {
			ParseCosi(pParent) ;
		} else if (Elem == "SYSTEMC") {
			ParseSystemC(pParent);
		} else if (Elem == "PARQUET") {
			ParseParquet(pParent);
		} else if (Elem == "TMP") {
			ParseTmp(pParent);
		} else if (Elem == "HMETIS") {
			ParseHmetis(pParent);
		} else if (Elem == "KHMETIS") {
			ParseKhmetis(pParent);
		} else if (Elem == "TRACER") {
			ParseTracer(pParent);
		} else if (Elem == "SHMETIS") {
			ParseShmetis(pParent);
		} else {
			cout << "Unknown XML element "<< Elem << " in file "<< mFname
					<< endl;
		}
		break;

	case TiXmlNode::COMMENT:
		//cout << pParent->Value( ) << endl ;
		break;

	case TiXmlNode::TEXT:
		break;

	case TiXmlNode::DECLARATION:
		break;

	default:
		break;

	}

	TiXmlNode* Child;
	for (Child = pParent->FirstChild() ; Child != 0; Child
			= Child->NextSibling() ) {
		ParseNode(Child) ;
	}
}

void ConfigParser::ParseCosi(TiXmlNode* pParent) {

	TiXmlAttribute* Attribute = ( pParent->ToElement())->FirstAttribute() ;
	while (Attribute) {
		if (string(Attribute->Name() ) == "version") {
			mCosiVersion = string(Attribute->Value() ) ;
		} else if (string(Attribute->Name() ) == "root") {
			mCosiRoot = string(Attribute->Value() ) ;
		} else {
			cout << "Unknown attribute "<< Attribute->Name()
					<< " for COSI config"<< endl;
		}
		Attribute = Attribute->Next() ;
	}
}

void ConfigParser::ParseTmp(TiXmlNode* pParent) {

	TiXmlAttribute* Attribute = ( pParent->ToElement())->FirstAttribute() ;
	while (Attribute) {

		if (string(Attribute->Name() ) == "root") {
			mTmp = string(Attribute->Value() ) ;
		} else {
			cout << "Unknown attribute "<< Attribute->Name()
					<< " for TMP config"<< endl;
		}
		Attribute = Attribute->Next() ;
	}
}

void ConfigParser::ParseSystemC(TiXmlNode* pParent) {

	TiXmlAttribute* Attribute = ( pParent->ToElement())->FirstAttribute() ;
	while (Attribute) {

		if (string(Attribute->Name() ) == "root") {
			mSystemCRoot = string(Attribute->Value() ) ;
		} else {
			cout << "Unknown attribute "<< Attribute->Name()
					<< " for SYSTEMC config"<< endl;
		}
		Attribute = Attribute->Next() ;
	}
}

void ConfigParser::ParseParquet(TiXmlNode* pParent) {

	TiXmlAttribute* Attribute = ( pParent->ToElement())->FirstAttribute() ;
	while (Attribute) {
		if (string(Attribute->Name() ) == "exec") {
			mParquetExec = string(Attribute->Value() ) ;
		} else if (string(Attribute->Name() ) == "root") {
			mParquetRoot = string(Attribute->Value() ) ;
		} else {
			cout << "Unknown attribute "<< Attribute->Name()
					<< " for PARQUET config"<< endl;
		}
		Attribute = Attribute->Next() ;
	}
}

void ConfigParser::ParseTracer(TiXmlNode* pParent) {
	TiXmlAttribute* Attribute = ( pParent->ToElement())->FirstAttribute() ;
	while (Attribute) {
		if (string(Attribute->Name() ) == "exec") {
			mTracerExec = string(Attribute->Value() ) ;
		} else if (string(Attribute->Name() ) == "root") {
			mTracerRoot = string(Attribute->Value() ) ;
		} else {
			cout << "Unknown attribute "<< Attribute->Name()
					<< " for TRACER config"<< endl;
		}
		Attribute = Attribute->Next() ;
	}
}

void ConfigParser::ParseHmetis(TiXmlNode* pParent) {
	TiXmlAttribute* Attribute = ( pParent->ToElement())->FirstAttribute() ;
	while (Attribute) {
		if (string(Attribute->Name() ) == "exec") {
			mHmetisExec = string(Attribute->Value() ) ;
		} else if (string(Attribute->Name() ) == "root") {
			mHmetisRoot = string(Attribute->Value() ) ;
		} else {
			cout << "Unknown attribute "<< Attribute->Name()
					<< " for HMETIS config"<< endl;
		}
		Attribute = Attribute->Next() ;
	}
}

void ConfigParser::ParseKhmetis(TiXmlNode* pParent) {
	TiXmlAttribute* Attribute = ( pParent->ToElement())->FirstAttribute() ;
	while (Attribute) {
		if (string(Attribute->Name() ) == "exec") {
			mKhmetisExec = string(Attribute->Value() ) ;
		} else if (string(Attribute->Name() ) == "root") {
			mKhmetisRoot = string(Attribute->Value() ) ;
		} else {
			cout << "Unknown attribute "<< Attribute->Name()
					<< " for KHMETIS config"<< endl;
		}
		Attribute = Attribute->Next() ;
	}
}

void ConfigParser::ParseShmetis(TiXmlNode* pParent) {
	TiXmlAttribute* Attribute = ( pParent->ToElement())->FirstAttribute() ;
	while (Attribute) {
		if (string(Attribute->Name() ) == "exec") {
			mShmetisExec = string(Attribute->Value() ) ;
		} else if (string(Attribute->Name() ) == "root") {
			mShmetisRoot = string(Attribute->Value() ) ;
		} else {
			cout << "Unknown attribute "<< Attribute->Name()
					<< " for SHMETIS config"<< endl;
		}
		Attribute = Attribute->Next() ;
	}
}

string ConfigParser::GetCosiRoot() {
	return mCosiRoot;
}

void ConfigParser::SetCosiRoot(string pRoot) {
	mCosiRoot = pRoot;
}

string ConfigParser::GetTmp() {
	return mTmp;
}

string ConfigParser::GetCosiVersion() {
	return mCosiVersion;
}

string ConfigParser::GetSystemCRoot() {
	return mSystemCRoot;
}

string ConfigParser::GetParquetRoot() {
	return mParquetRoot;
}

string ConfigParser::GetParquetExec() {
	return mParquetExec;
}

string ConfigParser::GetHmetisRoot() {
	return mHmetisRoot;
}

string ConfigParser::GetHmetisExec() {
	return mHmetisExec;
}

string ConfigParser::GetKhmetisRoot() {
	return mKhmetisRoot;
}

string ConfigParser::GetKhmetisExec() {
	return mKhmetisExec;
}

string ConfigParser::GetShmetisRoot() {
	return mShmetisRoot;
}

string ConfigParser::GetShmetisExec() {
	return mShmetisExec;
}

string ConfigParser::GetTracerRoot() {
	return mTracerRoot;
}

string ConfigParser::GetTracerExec() {
	return mTracerExec;
}

}
}
}

