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

#include "NocPlatformInterface.h"

namespace cosi {

namespace platforms {

namespace onchipcommunication {

using namespace std ;
using namespace cosi::rules::onchipcommunication ;

NocPlatformInterface::NocPlatformInterface(ComponentPosition* PositionRule) {
	mPositionRule = PositionRule ;
}

NocPlatformInterface::~NocPlatformInterface() {

}

int NocPlatformInterface::Size() {
	return mPositionRule->Size() ;
}

// Interface with the node position rule

list< int > NocPlatformInterface::Neighbors(int V, int D) {
	return mPositionRule->Neighbors(V,D,WireLst()) ;
}

double NocPlatformInterface::GetX(int V) {
	return mPositionRule->GetX(V);
}

double NocPlatformInterface::GetY(int V) {
	return mPositionRule->GetY(V) ;
}

//vector< InstallationSite > NocPlatformInterface::GetCandidateCenter() {

//}

//vector< int > NocPlatformInterface::GetCandidateCenterId() {

//}

double NocPlatformInterface::Distance(int U, int V) {
	return (fabs(GetX(U) -GetX(V))+ fabs(GetY(U) - GetY(V)) );
}

//Wire metrics
double NocPlatformInterface::WireEnergy(){
	return mWireEnergy ;
}
void NocPlatformInterface::WireEnergy(double WireEnergy){
	mWireEnergy = WireEnergy ;
}
double NocPlatformInterface::WireArea(double l){
	return (WireDeviceArea(l) + WireWiringArea(l) ) ;
}
double NocPlatformInterface::WireDeviceArea(double l){
	return (mWireDeviceArea * l ) ;
}
void NocPlatformInterface::DeviceAreaPerUnitLengthPerBit(double Wa) {
	mWireDeviceArea = Wa ;
}
double NocPlatformInterface::WireWiringArea(double l){
	return (mWireWiringArea * l ) ;
}
void NocPlatformInterface::WiringAreaPerUnitLengthPerBit(double Wa){
	mWireWiringArea = Wa ;
}
double NocPlatformInterface::WireLst(){
	return mWireLst ;
}
void NocPlatformInterface::WireLst(double WireLst){
	mWireLst = WireLst ;
}
double NocPlatformInterface::WirePitch(){
	return mWirePitch ;
}
void NocPlatformInterface::WirePitch(double Pitch){
	mWirePitch = Pitch ;
}
double NocPlatformInterface::WireLeakage(){
	return mWireStaticPower ;
}
void NocPlatformInterface::WireLeakage(double WireStaticPower){
	mWireStaticPower = WireStaticPower ;
}

//Router metrics
double NocPlatformInterface::RouterEnergy(int Nin, int Nout){
	return mRouterEnergy[pair<int,int>(Nin,Nout)] ;
}
void NocPlatformInterface::RouterEnergy(int Nin , int Nout , double E){
	mRouterEnergy[pair<int,int>(Nin,Nout)] = E ;
}
double NocPlatformInterface::RouterMaxBw(){
	return mFclk ;
}
double NocPlatformInterface::RouterLeakage(int i, int j){
	return mRouterLeakage[pair<int,int>(i,j)] ;
}

void NocPlatformInterface::RouterLeakage(int i, int j,double E){
	mRouterLeakage[pair<int,int>(i,j)] = E ;
}
double NocPlatformInterface::RouterArea(int nin, int nout){
	return mRouterArea[pair<int,int>(nin,nout)] ;
}
void NocPlatformInterface::RouterArea(int nin, int nout,double A){
	mRouterArea[pair<int,int>(nin,nout)] = A ;
}

////WIRES
//
//double NocPlatformInterface::WireEnergy() {
//
//	//Fix a unit length and a bandwidth equal to 1
//	//Divide by the flitwidth
//	WiringPath P;
//	P.AppendPosition(Position(0, 0, 0)) ;
//	P.AppendPosition(Position(1, 0, 0)) ;
//	Commodity C;
//	C.SetValue("", "", 0, 0, 1, 1);
//	CommoditySet Cs;
//	Cs.Add(C) ;
//	double RetVal;
//	RetVal = mPlt->GetDynamicPower(Name(mLinkName), P, Cs) / mFlitWidth ;
//	return RetVal ;
//}
//
//double NocPlatformInterface::WireArea(double l) {
//	return (WireDeviceArea(l) + WireWiringArea(l) );
//}
//
//double NocPlatformInterface::WireDeviceArea(double l) {
//	WiringPath P;
//	P.AppendPosition(Position(0, 0, 0)) ;
//	P.AppendPosition(Position(l, 0, 0)) ;
//	double RetVal;
//	RetVal = mPlt->GetDeviceArea(Name(mLinkName), P) / mFlitWidth ;
//	return RetVal ;
//}
//double NocPlatformInterface::WireWiringArea(double l) {
//	WiringPath P;
//	P.AppendPosition(Position(0, 0, 0)) ;
//	P.AppendPosition(Position(l, 0, 0)) ;
//	double RetVal;
//	RetVal = mPlt->GetWiringArea(Name(mLinkName), P) / mFlitWidth ;
//	return RetVal ;
//}
//double NocPlatformInterface::WireLst() {
//
//}
//
//double NocPlatformInterface::WireDelay() {
//	//Need to add the delay model
//}
//
//double NocPlatformInterface::WirePitch() {
//	WiringPath P;
//	P.AppendPosition(Position(0, 0, 0)) ;
//	P.AppendPosition(Position(1, 0, 0)) ;
//	double RetVal;
//	RetVal = mPlt->GetWiringArea(Name(mLinkName), P) / mFlitWidth ;
//	return RetVal ;
//}
//
//double NocPlatformInterface::WireLeakage() {
//	WiringPath P;
//	P.AppendPosition(Position(0, 0, 0)) ;
//	P.AppendPosition(Position(1, 0, 0)) ;
//	double RetVal;
//	RetVal = mPlt->GetStaticPower(Name(mLinkName), P) / mFlitWidth;
//	return RetVal ;
//}
//
////ROUTER
//
//double NocPlatformInterface::RouterEnergy(int Nin, int Nout) {
//
//	Commodity C;
//	C.SetValue("", "", 0, 0, 1, 1);
//	CommoditySet Cs;
//	Cs.Add(C) ;
//	map< std::string , CommoditySet > Commodities;
//	//Note: this is a hack. You should instantiate a
//	//router and check the port name.
//	//Also, this abstract model should be done in a better way
//	//because power consumption might be port-dependent.
//	Commodities["P1"] = Cs ;
//	return mPlt->GetDynamicPower(Name(mNodeName[pair<int,int>(Nin,Nout)]), Commodities);
//}
//
//double NocPlatformInterface::RouterMaxBw() {
//	return mFclk ;
//}
//
//double NocPlatformInterface::RouterLeakage(int i, int j) {
//	//check is the algorithm multiplies by the frequency
//	return mPlt->GetStaticPower(Name(mNodeName[pair<int,int>(Nin,Nout)]));
//}
//
//double NocPlatformInterface::RouterArea(int nin, int nout) {
//	return mPlt->GetArea(Name(mNodeName[pair<int,int>(Nin,Nout)]));
//}

//PARAMTERS

double NocPlatformInterface::OperatingFrequency() {
	return mFclk;
}

void NocPlatformInterface::OperatingFrequency(double Fclk) {
	mFclk = Fclk;
}

int NocPlatformInterface::FlitWidth() {
	return mFlitWidth;
}

void NocPlatformInterface::FlitWidth(int Fw) {
	mFlitWidth = Fw;
}
double NocPlatformInterface::SwitchingFactor() {
	return mSwitchingFactor;
}
void NocPlatformInterface::SwitchingFactor(double Sf) {
	mSwitchingFactor = Sf;
}
int NocPlatformInterface::MaxInDegree() {
	return mMaxInDegree;
}
void NocPlatformInterface::MaxInDegree(int D) {
	mMaxInDegree = D;
}
int NocPlatformInterface::MaxOutDegree() {
	return mMaxOutDegree;
}
void NocPlatformInterface::MaxOutDegree(int D) {
	mMaxOutDegree = D;
}
int NocPlatformInterface::DestInDegree() {
	return mDestInDegree;
}
void NocPlatformInterface::DestInDegree(int D) {
	mDestInDegree = D;
}
int NocPlatformInterface::SourceOutDegree() {
	return mSourceOutDegree;
}
void NocPlatformInterface::SourceOutDegree(int D) {
	mSourceOutDegree = D;
}
double NocPlatformInterface::PowerVsArea() {
	return mPowerArea;
}
void NocPlatformInterface::PowerVsArea(double Pa) {
	mPowerArea = Pa;
}

void NocPlatformInterface::AllowPtP(bool B) {
	mAllowPtP = B;
}
bool NocPlatformInterface::AllowPtP() {
	return mAllowPtP;
}

void NocPlatformInterface::AllowTwoHops(bool B) {
	mAllowTwoHops = B;
}
bool NocPlatformInterface::AllowTwoHops() {
	return mAllowTwoHops;
}

void NocPlatformInterface::AvoidDeadlock(bool B) {
	mAvoidDeadlock = B;
}
bool NocPlatformInterface::AvoidDeadlock() {
	return mAvoidDeadlock;
}

string NocPlatformInterface::GetNodeName() {
	return mNodeName ;
}

void NocPlatformInterface::SetNodeName(string pName){
	mNodeName = pName ;
}
string NocPlatformInterface::GetLinkName(){
	return mLinkName ;
}
void NocPlatformInterface::SetLinkName(string pName){
	mLinkName = pName ;
}

}
}
}
