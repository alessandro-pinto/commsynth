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
#include "LibraryParser.h"

namespace cosi {

LibraryParser::LibraryParser(string pFname) {
	mFname = pFname ;
	mLib = new NodeLinkLibrary;
}

LibraryParser::~LibraryParser() {
}

NodeLinkLibrary* LibraryParser::GetLibrary() {
	return mLib ;
}

void LibraryParser::Parse() {
	TiXmlDocument doc(mFname.c_str() );
	bool loadSuccess = doc.LoadFile() ;
	if ( !loadSuccess ) {
		cout << "The Xml file "<< mFname<< "  could not be loaded correctly "
				<< endl ;
		exit( 1) ;
		return;
	} else {
		ParseDocument( &doc ) ;
	}
}

void LibraryParser::Parse(TiXmlNode* pParent) {
	ParseLibrary(pParent) ;
}

void LibraryParser::ParseDocument(TiXmlNode* pParent) {
	if ( !pParent )
		return;

	int Type = pParent->Type() ;
	string Elem;

	if (Type == TiXmlNode::DOCUMENT ) {
		cout << "Parsing the library from file  "<< mFname << endl ;
		TiXmlNode* Child;
		for (Child = pParent->FirstChild() ; Child != 0; Child
				= Child->NextSibling() ) {
			Elem = Child->ValueStr() ;
			if (Elem == "Library")
				ParseLibrary(Child ) ;
		}
	} else {
		cout << "The document should be parsed..."<< endl ;
	}
	return;
}

//<Library>
void LibraryParser::ParseLibrary(TiXmlNode* pParent) {
	cerr << "Parsing the Library "<< endl ;
	GetLibraryAttributes(pParent ) ;
	TiXmlNode* Child;
	string Elem;
	for (Child = pParent->FirstChild() ; Child != 0; Child
			= Child->NextSibling() ) {
		Elem = Child->ValueStr() ;
		if (Elem == "Node") {
			ParseNode(Child ) ;
		} else if (Elem == "Link") {
			ParseLink(Child ) ;
		} else if (Elem == "BiLink") {
			ParseBiLink(Child ) ;
		} else {
			cout << "Unknown XML element "<< Elem<< " in file "<< mFname<< endl ;
		}
	}
}

void LibraryParser::GetLibraryAttributes(TiXmlNode* pParent) {
	TiXmlAttribute* Attribute = ( pParent->ToElement())->FirstAttribute() ;
	string Name;
	while (Attribute ) {
		if (string(Attribute->Name() ) == "name") {
			Name = Attribute->ValueStr() ;
		}
		Attribute = Attribute->Next() ;
	}
	mLib->SetName(Name ) ;
}

//<Node>
void LibraryParser::ParseNode(TiXmlNode* pParent) {
	GetNodeAttributes(pParent ) ;
	TiXmlNode* Child;
	string Elem;
	for (Child = pParent->FirstChild() ; Child != 0; Child
			= Child->NextSibling() ) {
		Elem = Child->ValueStr() ;
		if (Elem == "Interface") {
			ParseInterface(Child ) ;
		} else {
			cout << "Unknown XML element "<< Elem << " in file "<< mFname
					<< endl ;
		}
	}

	//Add node to the library
	mLib->AddNode(mCurrentNode) ;
}

void LibraryParser::GetNodeAttributes(TiXmlNode* pParent) {
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
	Node N;
	N.SetName(Name);
	N.SetType(Type);
	mCurrentNode = N ;
}

//<Interface>
void LibraryParser::ParseInterface(TiXmlNode* pParent) {
	GetInterfaceAttributes(pParent ) ;
	TiXmlNode* Child;
	string Elem;
	for (Child = pParent->FirstChild() ; Child != 0; Child
			= Child->NextSibling() ) {
		Elem = Child->ValueStr() ;
		if (Elem == "") {
			Parse(Child ) ;
		} else {
			cout << "Unknown XML element "<< Elem << " in file "<< mFname
					<< endl ;
		}
	}
}

void LibraryParser::GetInterfaceAttributes(TiXmlNode* pParent) {
	TiXmlAttribute* Attribute = ( pParent->ToElement())->FirstAttribute() ;
	string Name;
	string Type;
	int Count = 1;
	while (Attribute ) {
		if (string(Attribute->Name() ) == "name") {
			Name = Attribute->ValueStr() ;
		} else if (string(Attribute->Name() ) == "type") {
			Type = Attribute->ValueStr() ;
		} else if (string(Attribute->Name() ) == "count") {
			Attribute->QueryIntValue(&Count) ;
		}
		Attribute = Attribute->Next() ;
	}
	//Add the interface to the current node
	mCurrentNode.Add(Name, Type, Count) ;
}

//<Link>
void LibraryParser::ParseLink(TiXmlNode* pParent) {
	GetLinkAttributes(pParent ) ;
	TiXmlNode* Child;
	string Elem;
	for (Child = pParent->FirstChild() ; Child != 0; Child
			= Child->NextSibling() ) {
		Elem = Child->ValueStr() ;
		if (Elem == "Source") {
			ParseLinkSource(Child ) ;
		} else if (Elem == "Dest") {
			ParseLinkDest(Child ) ;
		} else {
			cout << "Unknown XML element "<< Elem << " in file "<< mFname
					<< endl ;
		}
	}
	//Add link to the library
	mLib->AddLink(mCurrentLink) ;
}

void LibraryParser::GetLinkAttributes(TiXmlNode* pParent) {
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
	Link L;
	L.SetName(Name);
	L.SetType(Type);
	mCurrentLink = L ;
}

//<Source>
void LibraryParser::ParseLinkSource(TiXmlNode* pParent) {
	GetLinkSourceAttributes(pParent ) ;
}

void LibraryParser::GetLinkSourceAttributes(TiXmlNode* pParent) {
	TiXmlAttribute* Attribute = ( pParent->ToElement())->FirstAttribute() ;
	string Node;
	string Interface;
	while (Attribute ) {
		if (string(Attribute->Name() ) == "node") {
			Node = Attribute->ValueStr() ;
		} else if (string(Attribute->Name() ) == "interface") {
			Interface = Attribute->ValueStr() ;
		}
		Attribute = Attribute->Next() ;
	}
	mCurrentLink.SetSource(Node);
	mCurrentLink.SetSourceInterface(Interface);
}

//<Dest>
void LibraryParser::ParseLinkDest(TiXmlNode* pParent) {
	GetLinkDestAttributes(pParent ) ;
}

void LibraryParser::GetLinkDestAttributes(TiXmlNode* pParent) {
	TiXmlAttribute* Attribute = ( pParent->ToElement())->FirstAttribute() ;
	string Node;
	string Interface;
	while (Attribute ) {
		if (string(Attribute->Name() ) == "node") {
			Node = Attribute->ValueStr() ;
		} else if (string(Attribute->Name() ) == "interface") {
			Interface = Attribute->ValueStr() ;
		}
		Attribute = Attribute->Next() ;
	}
	mCurrentLink.SetDestination(Node);
	mCurrentLink.SetDestinationInterface(Interface) ;
}

//<BiLink>
void LibraryParser::ParseBiLink(TiXmlNode* pParent) {
	GetBiLinkAttributes(pParent ) ;
	TiXmlNode* Child;
	string Elem;
	BiLink L;
	int Lcount = 0;
	for (Child = pParent->FirstChild() ; Child != 0; Child
			= Child->NextSibling() ) {
		Elem = Child->ValueStr() ;
		if (Elem == "Link") {
			ParseLink(Child ) ;
			if (Lcount == 0) {
				L.L1(mCurrentLink) ;
				Lcount++;
			} else if (Lcount==1) {
				L.L2(mCurrentLink) ;
			}
		} else {
			cout << "Unknown XML element "<< Elem << " in file "<< mFname
					<< endl ;
		}

	}
	mCurrentBiLink = L ;
	mLib->AddBiLink(mCurrentBiLink) ;
}

void LibraryParser::GetBiLinkAttributes(TiXmlNode* pParent) {
	TiXmlAttribute* Attribute = ( pParent->ToElement())->FirstAttribute() ;
	string Name;
	string Type ;
	while (Attribute ) {
		if (string(Attribute->Name() ) == "name") {
			Name = Attribute->ValueStr() ;
		}else if (string(Attribute->Name() ) == "type") {
			Type = Attribute->ValueStr() ;
		}
		Attribute = Attribute->Next() ;
	}
	BiLink L ;
	L.SetName(Name );
	L.SetType( Type);
	mCurrentBiLink = L ;
}


////<>
//void LibraryParser::Parse(TiXmlNode* pParent) {
//	GetAttributes(pParent ) ;
//	TiXmlNode* Child;
//	string Elem;
//	for (Child = pParent->FirstChild() ; Child != 0; Child
//			= Child->NextSibling() ) {
//		Elem = Child->ValueStr() ;
//		if (Elem == "") {
//			Parse(Child ) ;
//		} else {
//			cout << "Unknown XML element "<< Elem << " in file "<< mFname
//					<< endl ;
//		}
//	}
//}
//
//void LibraryParser::GetAttributes(TiXmlNode* pParent) {
//	TiXmlAttribute* Attribute = ( pParent->ToElement())->FirstAttribute() ;
//	string Name;
//	while (Attribute ) {
//		if (string(Attribute->Name() ) == "") {
//			Name = Attribute->ValueStr() ;
//		}
//		Attribute = Attribute->Next() ;
//	}
//}

}

