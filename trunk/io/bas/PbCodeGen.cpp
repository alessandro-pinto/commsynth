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
#include "PbCodeGen.h"

namespace cosi {

namespace io {

namespace bas {

using namespace std;

PbCodeGen::PbCodeGen(std::string pFileName) {
	mFileName = pFileName ;
}

PbCodeGen::~PbCodeGen() {

}

void PbCodeGen::AddVariable(int pIndex) {
	//The variable name is simply the the letter v followed by the index
	stringstream s;
	s << "v"<< pIndex ;
	mVariables[pIndex] = s.str() ;
	mName2Variable[s.str()] = pIndex ;
}

void PbCodeGen::AddToCostFunction(int pCoefficinet, int pIndex) {
	//If there is already such variable in the cost function,
	//simply add the coefficients
	if (mCostFuntion.find(pIndex) == mCostFuntion.end() )
		mCostFuntion[pIndex] = pCoefficinet ;
	else
		mCostFuntion[pIndex] = mCostFuntion[pIndex] + pCoefficinet ;
}

void PbCodeGen::AddLessThanOrEqualToConstraint(std::map<int,int> pVarList,
		int pRhs) {
	mLeqLhs.push_back(pVarList) ;
	mLeqRhs.push_back(pRhs) ;
}

void PbCodeGen::AddGreaterThanOrEqualToConstraint(std::map<int,int> pVarList,
		int pRhs) {
	mGeqLhs.push_back(pVarList) ;
	mGeqRhs.push_back(pRhs) ;
}

void PbCodeGen::AddEqualityConstraint(std::map<int,int> pVarList, int pRhs) {
	mEqLhs.push_back(pVarList) ;
	mEqRhs.push_back(pRhs) ;
}

bool PbCodeGen::Run() {

	cout << "(IO) -- Pseudo-Boolean code generatotion  (pb)... " << endl ;

	cout << "(pb) Number of variables : " << mVariables.size() << endl ;
	cout << "(pb) Number of constraints : " << mLeqLhs.size() + mGeqLhs.size() + mEqLhs.size() << endl ;

	//Generate the file in opb format
	cout << "(pb) Writing opb file " << mFileName << endl ;

	ofstream F;
	F.open(mFileName.c_str()) ;

	//write the cost function first
	F << "min: ";
	map<int,int>::iterator It;
	for (It = mCostFuntion.begin() ; It != mCostFuntion.end() ; It++) {
		if (It->second > 0)
			F << "+";
		F << It->second << "*"<< mVariables[It->first]<< " ";
	}

	F << ";" << endl ;

	//Write the inequality constraints
	for (unsigned int i = 0; i < mLeqLhs.size() ; i++) {
		for (It = mLeqLhs[i].begin(); It != mLeqLhs[i].end() ; It++) {
			if (It->second > 0)
				F << "+";
			F << It->second<< "*"<< mVariables[It->first]<< " ";
		}
		F << " <= ";
		F << mLeqRhs[i]<< ";" <<endl ;
	}
	for (unsigned int i = 0; i < mGeqLhs.size() ; i++) {
		for (It = mGeqLhs[i].begin(); It != mGeqLhs[i].end() ; It++) {
			if (It->second > 0)
				F << "+";
			F << It->second<< "*"<< mVariables[It->first]<< " ";
		}
		F << " >= ";
		F << mGeqRhs[i]<< ";" << endl ;
	}
	for (unsigned int i = 0; i < mEqLhs.size() ; i++) {
		for (It = mEqLhs[i].begin(); It != mEqLhs[i].end() ; It++) {
			if (It->second > 0)
				F << "+";
			F << It->second << "*"<< mVariables[It->first]<< " ";
		}
		F << " = ";
		F << mEqRhs[i]<< ";" << endl ;
	}
	F << endl ;
	F.close();

	//Run the PB solver
	stringstream CmdLine;
	CmdLine << Configuration->GetPbSolverRoot() << "/"
			<< Configuration->GetPbSolverExec() << " "<< mFileName << " "
			<< Configuration->GetTmp()<< "/pb_solver_result.txt ";
	cout << "(pb) Runnign minisat+ : " << CmdLine.str() << endl ;
	system((CmdLine.str()).c_str());

	//Read results
	ifstream If;
	string Fname;
	Fname = Configuration->GetTmp() + "/pb_solver_result.txt";
	If.open(Fname.c_str()) ;
	cout << "(pb) Parsint result file " << Fname << endl ;
	for (unsigned int i = 0; i < mVariables.size() ; i ++) {
		string Name;
		int Value;
		If >> Name ;
		If >> Value ;
		if (Value == 1) {
			mResult[mName2Variable[Name]] = true;
		} else {
			mResult[mName2Variable[Name]] = false;
		}
	}
	If.close() ;

	cout << "(IO) -- Pseudo-Boolean code generation done" << endl ;

}

bool PbCodeGen::GetValue(int Index) {
	return mResult[Index];
}

}
}
}
