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
#ifndef PBCODEGEN_H_
#define PBCODEGEN_H_

#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include "include/bas/CosiGlobal.h"

namespace cosi {

namespace io {

namespace bas {

class PbCodeGen {
public:
	PbCodeGen(std::string pFileName);
	virtual ~PbCodeGen();
	void AddVariable(int pIndex);
	void AddToCostFunction(int pCoefficinet, int pIndex);
	void AddLessThanOrEqualToConstraint(std::map<int,int> pVarList, int pRhs);
	void AddGreaterThanOrEqualToConstraint(std::map<int,int> pVarList, int pRhs);
	void AddEqualityConstraint(std::map<int,int> pVarList, int pRhs);
	bool Run();
	bool GetValue(int Index);
private:
	std::map<int,std::string> mVariables;
	std::map< std::string , int > mName2Variable;
	std::string mFileName;
	std::map<int,int> mCostFuntion;
	std::vector<std::map<int,int> > mLeqLhs;
	std::vector< int > mLeqRhs;
	std::vector<std::map<int,int> > mGeqLhs;
	std::vector< int > mGeqRhs;
	std::vector<std::map<int,int> > mEqLhs;
	std::vector< int > mEqRhs;
	std::map<int,bool> mResult ;
};

}
}
}

#endif /*PBCODEGEN_H_*/
