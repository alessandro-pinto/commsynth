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
#ifndef SYSCCODEGEN_H_
#define SYSCCODEGEN_H_

#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include "networks/onchipcommunication/Specification.h"
#include "networks/onchipcommunication/PlatformInstance.h"
#include "networks/onchipcommunication/Implementation.h"
#include "include/onchipcommunication/CosiGlobal.h"

namespace cosi {

namespace io {

namespace onchipcommunication {

using namespace std;

class SyscCodeGen {
public:
	SyscCodeGen(string FileName, string MakeFileName,
	cosi::commstruct::onchipcommunication::Implementation* I, cosi::commstruct::onchipcommunication::Specification* PtP, int pFlitWidth,
	double pClockPeriod);
	~SyscCodeGen();
	void Run();
private:
	void SyscMakeFileGen();

	void SyscOpen();

	void SyscScMain();

	string SyscTrace() ;

	void SyscPrintStat(ofstream* f,
			cosi::commstruct::onchipcommunication::Implementation* G,
			cosi::commstruct::onchipcommunication::Specification* PtP);

	void SyscClose();

	string SyscSource( int S, string P);

	string SyscDest( int D, string P);

	string SyscRouter(int V);

	string SyscSourceNetworkInterface( int S , string P);

	string SyscDestNetworkInterface(int D , string P);

	string SyscConnect(
			cosi::commstruct::onchipcommunication::Implementation* G, int U,
			int V);

	int InputIndex(int, string);
	int OutputIndex(int, string);
	void InputIndex(int, string , int );
	void OutputIndex(int, string , int );

	std::map< std::pair<int,std::string> , int > mInputPortIndex;
	std::map< std::pair<int,std::string> , int > mOutputPortIndex;


	string mFileName ;
	string mMakefileName ;
	cosi::commstruct::onchipcommunication::Specification* mSpec ;
	cosi::commstruct::onchipcommunication::Implementation* mI ;
	int mFlitWidth ;
	double mClockPeriod;

	ofstream* mF ;

};

}
}
}

#endif /*SYSCCODEGEN_H_*/
