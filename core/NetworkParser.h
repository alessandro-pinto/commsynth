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
#ifndef NETWORKPARSER_H_
#define NETWORKPARSER_H_

#include <string>
#include <sstream>
#include <map>
#include <list>
#include "IdGraph.h"
#include "Factory.h"
#include "tinyxml.h"

namespace cosi {

/** This class defines a generic parser for networks.
 *
 *
 *@author Alessandro Pinto,
 *@author The Donald O. Pederson Center for Electronic System Design
 *@author Electrical Engineering Department
 *@author University of California, Berkeley
 *@author 545P Cory Hall, Berkeley, CA 94720
 *@author +1 (510) 642 9670
 *@author http://www.eecs.berkeley.edu/~apinto
 *
 *@version $Id: NetworkParser.h,v 1.4 2008/12/07 23:20:21 apinto Exp $
 */

class NetworkParser {
public:
	NetworkParser(string pFileName);
	virtual ~NetworkParser();
	void Parse();
	void Parse(TiXmlNode* pParent);
	IdGraph* GetNetwork();

private:
	void ParseDocument(TiXmlNode* pParent);

	// <Network>
	void ParseNetwork(TiXmlNode* pParent);
	void GetNetworkAttributes(TiXmlNode* pParent);

	// <Node>
	void ParseNode(TiXmlNode* pParent);
	void GetNodeAttributes(TiXmlNode* pParent);

	// <Link>
	void ParseLink(TiXmlNode* pParent);
	void GetLinkAttributes(TiXmlNode* pParent);

	// <NodeLabel>
	void ParseNodeLabel(TiXmlNode* pParent);
	void GetNodeLabelAttributes(TiXmlNode* pParent);

	// <NodeLabel>
    void ParseNodeMapLabel(TiXmlNode* pParent);
	//void GetNodeMapLabelAttributes(TiXmlNode* pParent);

	void ParseLinkMapLabel(TiXmlNode* pParent);

	// <Entry> node
	void ParseNodeMapEntry(TiXmlNode* pParent);
	void GetNodeMapEntryAttributes(TiXmlNode* pParent);

	// <LinkLabel>
	void ParseLinkLabel(TiXmlNode* pParent);
	void GetLinkLabelAttributes(TiXmlNode* pParent);

	// <Entry> link
	void ParseLinkMapEntry(TiXmlNode* pParent);
	void GetLinkMapEntryAttributes(TiXmlNode* pParent);

	Factory* mF;
	string mFileName;
	string mNetworkType ;
	string mNetworkName ;

	string mLabelName ;
	string mLabelFormat ;
	stringstream mLabelType ;
	list<string> mTypes ;

	//Temporary storage
	map<string,Vertex> mNodes ;
	map<string,Edge> mLinks ;
};

}

#endif /*NETWORKPARSER_H_*/
