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
#ifndef LIBRARYPARSER_H_
#define LIBRARYPARSER_H_

#include <string>
#include <sstream>
#include <map>
#include <list>
#include "NodeLinkLibrary.h"
#include "Factory.h"
#include "tinyxml.h"

using namespace std;

namespace cosi {

class LibraryParser {
public:

	LibraryParser(string pFname);
	virtual ~LibraryParser();
	void Parse();
	void Parse(TiXmlNode* pParent);
	NodeLinkLibrary* GetLibrary();

private:

	void ParseDocument(TiXmlNode* pParent);

	// <Library>
	void ParseLibrary(TiXmlNode* pParent);
	void GetLibraryAttributes(TiXmlNode* pParent);

	// <Node>
	void ParseNode(TiXmlNode* pParent);
	void GetNodeAttributes(TiXmlNode* pParent);

	// <Interface>
	void ParseInterface(TiXmlNode* pParent);
	void GetInterfaceAttributes(TiXmlNode* pParent);

	// <Link>
	void ParseLink(TiXmlNode* pParent);
	void GetLinkAttributes(TiXmlNode* pParent);

	//<Source>
	void ParseLinkSource(TiXmlNode* pParent);
	void GetLinkSourceAttributes(TiXmlNode* pParent);

	//<Dest>
	void ParseLinkDest(TiXmlNode* pParent);
	void GetLinkDestAttributes(TiXmlNode* pParent);

	// <BiLink>
	void ParseBiLink(TiXmlNode* pParent);
	void GetBiLinkAttributes(TiXmlNode* pParent);

	string mFname;
	string mName;
	NodeLinkLibrary* mLib;
	Node mCurrentNode;
	Link mCurrentLink;
	BiLink mCurrentBiLink;
	string mCurrentDirection;

};

}

#endif /*LIBRARYPARSER_H_*/
