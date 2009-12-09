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
#include "NetworkParser.h"

namespace cosi {

NetworkParser::NetworkParser(string pFileName) {
	mFileName = pFileName ;
	mF = new Factory ;
}

NetworkParser::~NetworkParser() {
}

void NetworkParser::Parse() {
	TiXmlDocument doc(mFileName.c_str() );
	bool loadSuccess = doc.LoadFile() ;
	if ( !loadSuccess ) {
		cout << "The Xml file "<< mFileName
				<< "  could not be loaded correctly "<< endl ;
		exit( 1) ;
		return;
	} else {
		ParseDocument( &doc ) ;
	}
}

void NetworkParser::Parse(TiXmlNode* pParent) {
	ParseNetwork(pParent) ;
}

IdGraph* NetworkParser::GetNetwork() {
	return mF->GetNetwork() ;
}

void NetworkParser::ParseDocument(TiXmlNode* pParent) {
	if ( !pParent )
		return;

	int Type = pParent->Type() ;
	string Elem;

	if (Type == TiXmlNode::DOCUMENT ) {
		cout << "Parsing the network from file  "<< mFileName << endl ;
		TiXmlNode* Child;
		for (Child = pParent->FirstChild() ; Child != 0; Child
				= Child->NextSibling() ) {
			Elem = Child->ValueStr() ;
			if (Elem == "Network")
				ParseNetwork(Child ) ;
		}
	} else {
		cout << "The document should be parsed..."<< endl ;
	}
	return;
}

//<Network>
void NetworkParser::ParseNetwork(TiXmlNode* pParent) {
	cerr << "Parsing the Network " << endl ;
	GetNetworkAttributes(pParent ) ;
	TiXmlNode* Child;
	string Elem;
	for (Child = pParent->FirstChild() ; Child != 0; Child
			= Child->NextSibling() ) {
		Elem = Child->ValueStr() ;
		if (Elem == "Node") {
			ParseNode(Child ) ;
		} else if (Elem == "Link") {
			ParseLink(Child ) ;
		} else if (Elem == "NodeLabel") {
			ParseNodeLabel(Child ) ;
		} else if (Elem == "LinkLabel") {
			ParseLinkLabel(Child ) ;
		} else {
			cout << "Unknown XML element "<< Elem
					<< " in Network element in file "<< mFileName << endl ;
		}
	}
}

void NetworkParser::GetNetworkAttributes(TiXmlNode* pParent) {
	TiXmlAttribute* Attribute = ( pParent->ToElement())->FirstAttribute() ;
	string Name;
	string Type;
	while (Attribute ) {
		if (string(Attribute->Name() ) == "name") {
			Name = Attribute->ValueStr() ;
		} else if (string(Attribute->Name() ) == "type") {
			Type = Attribute->ValueStr() ;
		}
		Attribute = Attribute->Next() ;
	}
	mF->AllocateNetwork(Type) ;
	mF->SetNetworkName(Name) ;
}

// <Node>
void NetworkParser::ParseNode(TiXmlNode* pParent) {
	GetNodeAttributes(pParent ) ;
}

void NetworkParser::GetNodeAttributes(TiXmlNode* pParent) {
	TiXmlAttribute* Attribute = ( pParent->ToElement())->FirstAttribute() ;
	string Name;
	int Id;
	while (Attribute ) {
		if (string(Attribute->Name() ) == "name") {
			Name = Attribute->ValueStr() ;
		} else if (string(Attribute->Name() ) == "id") {
			Attribute->QueryIntValue(&Id) ;
		}
		Attribute = Attribute->Next() ;
	}
	mF->AddVertex(Id, Name) ;
	mNodes[Name] = Id ;
}

// <Link>
void NetworkParser::ParseLink(TiXmlNode* pParent) {
	GetLinkAttributes(pParent) ;
}
void NetworkParser::GetLinkAttributes(TiXmlNode* pParent) {
	TiXmlAttribute* Attribute = ( pParent->ToElement())->FirstAttribute() ;
	string Name;
	string Src;
	string Dest;
	while (Attribute ) {
		if (string(Attribute->Name() ) == "name") {
			Name = Attribute->ValueStr() ;
		} else if (string(Attribute->Name() ) == "source") {
			Src = Attribute->ValueStr();
		} else if (string(Attribute->Name() ) == "dest") {
			Dest = Attribute->ValueStr();
		}
		Attribute = Attribute->Next() ;
	}
	Edge E(mNodes[Src], mNodes[Dest]);
	mF->AddEdge(E, Name) ;
	mLinks[Name] = E ;
}

// <NodeLabel>
void NetworkParser::ParseNodeLabel(TiXmlNode* pParent) {
	GetNodeLabelAttributes(pParent ) ;
	//depending on the label format, we need to parse a map or a set of maps or othe things.
	if (mLabelFormat == "map") {
		//part a map
		ParseNodeMapLabel(pParent) ;
	}

}
void NetworkParser::GetNodeLabelAttributes(TiXmlNode* pParent) {
	TiXmlAttribute* Attribute = ( pParent->ToElement())->FirstAttribute() ;
	while (Attribute ) {
		if (string(Attribute->Name() ) == "name") {
			mLabelName = Attribute->ValueStr() ;
		} else if (string(Attribute->Name() ) == "format") {
			mLabelFormat = Attribute->ValueStr();
		} else if (string(Attribute->Name() ) == "type") {
			mLabelType.clear( ) ;
			mLabelType << Attribute->ValueStr();
		}
		Attribute = Attribute->Next() ;
	}
	//get the list of types
	string Tmp;
	mTypes.clear();
	while ( !mLabelType.eof() ) {
		mLabelType >> Tmp ;
		mTypes.push_back(Tmp) ;
	}
}

void NetworkParser::ParseNodeMapLabel(TiXmlNode* pParent) {
	TiXmlNode* Child;
	string Elem;
	for (Child = pParent->FirstChild() ; Child != 0; Child
			= Child->NextSibling() ) {
		Elem = Child->ValueStr() ;
		if (Elem == "Entry") {
			ParseNodeMapEntry(Child ) ;
		} else {
			cout << "Unknown XML element "<< Elem
					<< " in Network element in file "<< mFileName << endl ;
		}
	}

}

// <Entry> node
void NetworkParser::ParseNodeMapEntry(TiXmlNode* pParent) {
	GetNodeMapEntryAttributes(pParent) ;
}

void NetworkParser::GetNodeMapEntryAttributes(TiXmlNode* pParent) {
	TiXmlAttribute* Attribute = ( pParent->ToElement())->FirstAttribute() ;
	string Name;
	stringstream S;
	while (Attribute ) {
		if (string(Attribute->Name() ) == "node") {
			Name = Attribute->ValueStr() ;
		} else if (string(Attribute->Name() ) == "value") {
			S << Attribute->ValueStr();
		}
		Attribute = Attribute->Next() ;
	}

	//get type by type...
	list<string> Tstring;
	list<int> Tint;
	list<double> Tdouble;
	list<string>::iterator TypeIt;
	for (TypeIt = mTypes.begin() ; TypeIt != mTypes.end() ; TypeIt++) {
		if ( *TypeIt == "string") {
			string Tmp;
			S >> Tmp ;
			Tstring.push_back(Tmp) ;
		} else if ( *TypeIt == "int") {
			int Tmp;
			S >> Tmp ;
			Tint.push_back(Tmp) ;
		} else if (*TypeIt == "double") {
			double Tmp;
			S >> Tmp ;
			Tdouble.push_back(Tmp) ;
		}
	}
	mF->SetLabelMapEntry(mLabelName , mNodes[Name], Tstring, Tint, Tdouble) ;
}

// <LinkLabel>
void NetworkParser::ParseLinkLabel(TiXmlNode* pParent) {
	GetLinkLabelAttributes(pParent ) ;
	//depending on the label format, we need to parse a map or a set of maps or othe things.
	if (mLabelFormat == "map") {
		//part a map
		ParseLinkMapLabel(pParent) ;
	}
}

void NetworkParser::GetLinkLabelAttributes(TiXmlNode* pParent) {
	TiXmlAttribute* Attribute = ( pParent->ToElement())->FirstAttribute() ;
	while (Attribute ) {
		if (string(Attribute->Name() ) == "name") {
			mLabelName = Attribute->ValueStr() ;
		} else if (string(Attribute->Name() ) == "format") {
			mLabelFormat = Attribute->ValueStr();
		} else if (string(Attribute->Name() ) == "type") {
			mLabelType.clear();
			mLabelType << Attribute->ValueStr();
		}
		Attribute = Attribute->Next() ;
	}
	//get the list of types
	string Tmp;
	mTypes.clear() ;
	while ( !mLabelType.eof() ) {
		mLabelType >> Tmp ;
		mTypes.push_back(Tmp) ;
	}
}

void NetworkParser::ParseLinkMapLabel(TiXmlNode* pParent) {
	TiXmlNode* Child;
	string Elem;
	for (Child = pParent->FirstChild() ; Child != 0; Child
			= Child->NextSibling() ) {
		Elem = Child->ValueStr() ;
		if (Elem == "Entry") {
			ParseLinkMapEntry(Child ) ;
		} else {
			cout << "Unknown XML element "<< Elem
					<< " in Network element in file "<< mFileName << endl ;
		}
	}

}

// <Entry> link
void NetworkParser::ParseLinkMapEntry(TiXmlNode* pParent) {
	GetLinkMapEntryAttributes(pParent) ;
}
void NetworkParser::GetLinkMapEntryAttributes(TiXmlNode* pParent) {
	TiXmlAttribute* Attribute = ( pParent->ToElement())->FirstAttribute() ;
	string Name;
	stringstream S;
	while (Attribute ) {
		if (string(Attribute->Name() ) == "link") {
			Name = Attribute->ValueStr() ;
		} else if (string(Attribute->Name() ) == "value") {
			S << Attribute->ValueStr();
		}
		Attribute = Attribute->Next() ;
	}

	//get type by type...
	list<string> Tstring;
	list<int> Tint;
	list<double> Tdouble;
	list<string>::iterator TypeIt;
	for (TypeIt = mTypes.begin() ; TypeIt != mTypes.end() ; TypeIt++) {
		if ( *TypeIt == "string") {
			string Tmp;
			S >> Tmp ;
			Tstring.push_back(Tmp) ;
		} else if ( *TypeIt == "int") {
			int Tmp;
			S >> Tmp ;
			Tint.push_back(Tmp) ;
		} else if ( (*TypeIt) == "double") {
			double Tmp;
			S >> Tmp ;
			Tdouble.push_back(Tmp) ;
		}
	}
	mF->SetLabelMapEntry(mLabelName , mLinks[Name], Tstring, Tint, Tdouble) ;
}

}

