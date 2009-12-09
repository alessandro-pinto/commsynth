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
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <sstream>
#include "tinyxml.h"

#ifndef COMMON_CONFIGPARSER_H_
#define COMMON_CONFIGPARSER_H_

namespace cosi {

namespace io {

using namespace std;

class ConfigParser {
public:

	ConfigParser(string pFname);

	void Parse();

	string GetCosiRoot();

	void SetCosiRoot(string pRoot);

	string GetCosiVersion();

	string GetSystemCRoot();

	string GetParquetRoot();

	string GetParquetExec();

	string GetHmetisRoot();

	string GetHmetisExec();

	string GetKhmetisRoot();

	string GetKhmetisExec();

	string GetShmetisRoot();

	string GetShmetisExec();

	string GetPatohRoot();

	string GetPatohExec();

	string GetTracerRoot();

	string GetTracerExec();

	string GetPbSolverRoot();

	string GetPbSolverExec();

	string GetTmp();

private:

	void ParseNode(TiXmlNode* pParent);

	void ParseCosi(TiXmlNode* pParent);

	void ParseTmp(TiXmlNode* pParent);

	void ParseSystemC(TiXmlNode* pParent);

	void ParseParquet(TiXmlNode* pParent);

	void ParseHmetis(TiXmlNode* pParent);

	void ParseKhmetis(TiXmlNode* pParent);

	void ParseShmetis(TiXmlNode* pParent);

	void ParsePatoh(TiXmlNode* pParent);

	void ParseTracer(TiXmlNode* pParent);

	void ParsePbSolver(TiXmlNode* pParent);

	string mFname;

	string mCosiRoot;

	string mCosiVersion;

	string mSystemCRoot;

	string mParquetRoot;

	string mTracerRoot;

	string mTracerExec;

	string mParquetExec;

	string mHmetisRoot;
	string mKhmetisRoot;
	string mShmetisRoot;

	string mPatohRoot;

	string mHmetisExec;
	string mKhmetisExec;
	string mShmetisExec;

	string mPatohExec;

	string mPbSolverRoot ;
	string mPbSolverExec ;

	string mTmp;

};
}
}

#endif
