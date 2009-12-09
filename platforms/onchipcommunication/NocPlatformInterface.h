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

#ifndef NOCPLATFORMINTERFACE_H_
#define NOCPLATFORMINTERFACE_H_

#include <string>
#include <map>
#include "networks/onchipcommunication/Specification.h"
#include "networks/onchipcommunication/PlatformInstance.h"
#include "networks/onchipcommunication/Implementation.h"
#include "rules/onchipcommunication/ComponentPosition.h"

namespace cosi {

namespace platforms {

namespace onchipcommunication {

using namespace std ;
using namespace cosi::rules::onchipcommunication ;

class NocPlatformInterface {
public:
	NocPlatformInterface(ComponentPosition* PositionRule);
	virtual ~NocPlatformInterface();

	int Size() ;

	//Position rule
	list< int > Neighbors(int V, int D);
	double GetX(int V);
	double GetY(int V);
	double Distance(int U, int V);

	//Wire metrics
	double WireEnergy();
	void WireEnergy(double mWireEnergy);
	double WireArea(double l);
	double WireDeviceArea(double l);
	void DeviceAreaPerUnitLengthPerBit(double WireDeviceArea) ;
	double WireWiringArea(double l);
	void WiringAreaPerUnitLengthPerBit(double WireDeviceArea) ;
	double WireLst();
	void WireLst(double Lst) ;
	double WirePitch();
	void WirePitch(double Pitch) ;
	double WireLeakage();
	void WireLeakage(double WireStaticPower) ;

	//Router metrics
	double RouterEnergy(int Nin, int Nout);
	void RouterEnergy(int Nin , int Nout , double E) ;
	double RouterMaxBw();
	double RouterLeakage(int i, int j);
	void RouterLeakage(int i, int j,double E);
	double RouterArea(int nin, int nout);
	void RouterArea(int nin, int nout,double A);

	//Global network paramters
	double OperatingFrequency() ;
	void OperatingFrequency(double Fclk) ;
	int FlitWidth();
	void FlitWidth(int Fw);
	double SwitchingFactor();
	void SwitchingFactor(double Sf);
	int MaxInDegree();
	void MaxInDegree(int D);
	int MaxOutDegree();
	void MaxOutDegree(int D);
	int DestInDegree();
	void DestInDegree(int D);
	int SourceOutDegree();
	void SourceOutDegree(int D);
	string GetNodeName() ;
	void SetNodeName(string pName);
	string GetLinkName();
	void SetLinkName(string pName);


	//Synthesis paramters
	double PowerVsArea();
	void PowerVsArea(double Pa) ;
	void AllowPtP(bool B);
	bool AllowPtP();
	void AllowTwoHops(bool B);
	bool AllowTwoHops();
	void AvoidDeadlock(bool B);
	bool AvoidDeadlock();

private:

	double mWireEnergy ;
	double mWireDeviceArea ;
	double mWireWiringArea ;
	double mWireLst ;
	double mWirePitch ;
	double mWireStaticPower ;

	map<pair<int,int>,double > mRouterEnergy ;
	map<pair<int,int>,double > mRouterLeakage ;
	map<pair<int,int>,double > mRouterArea ;

	double mH , mW ;
    int mFlitWidth ;
    double mFclk ;
    double mSwitchingFactor ;
    double mPowerArea ;
    int mMaxInDegree ;
    int mMaxOutDegree ;
    int mDestInDegree ;
    int mSourceOutDegree ;
    bool mAllowPtP ;
    bool mAllowTwoHops;
    bool mAvoidDeadlock ;
    ComponentPosition* mPositionRule ;

    string mNodeName ;
    string mLinkName ;


};

}

}

}

#endif /*NOCPLATFORMINTERFACE_H_*/
