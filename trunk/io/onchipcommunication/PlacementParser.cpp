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
#include "PlacementParser.h"

namespace cosi {

namespace io {

namespace onchipcommunication {

using namespace std;
using namespace cosi::environment::onchipcommunication;
using namespace cosi::quantity::onchipcommunication ;

PlacementParser::PlacementParser() {

}

PlacementParser::PlacementParser(string pFname, double pSpareA) {
	mFname = pFname ;
	mSpareA = pSpareA ;
}

void PlacementParser::Parse() {
	ifstream File;
	File.open(mFname.c_str() ) ;

	double Delta, X, Y, W, H, MinX, MinY, MaxX, MaxY;
	MinX = MinY = DBL_MAX;
	MaxX = MaxY = 0.0;
	char C;
	string Name;
	string Spare;
	double Version;

	File >> Spare;

	File >> Spare;

	File >> Version ;

	while ( !File.eof() ) {
		File >> Name ; // Block name
		if (File.eof() )
			break;
		File >> X >> Y ; // Block Position
		if (X < MinX )
			MinX = X ;
		if (Y < MinY )
			MinY = Y ;
		File >> C ;
		while (C != '('&& !File.eof() )
			File >> C ;
		File >> W >> C >> H ;
		if (X+W > MaxX )
			MaxX = X+W;
		if (Y+H > MaxY )
			MaxY = Y+H ;
		//skip to the end of line
		File >> C ;
		while (C != ':'&& !File.eof() )
			File >> C ;
		File >> Spare ;
		if (Name.find("PAD") != 0&& Name.find("CosiComm") != 0) {

			PlacedIpCore Ip;
			Ip.SetName(Name ) ;

			// We need to remove the area around the block
			// Prortionally to its area
			Delta = (W + H - sqrt( (W+H)*(W+H)- 4*mSpareA*W*H) )/4.0;

			CosiLog << Name << " "<< W << " x "<< H << "  Delta = "<< Delta
					<< endl ;

			Ip.SetBottomLeftX( (X+Delta )*1e-6) ;
			Ip.SetBottomLeftY( (Y+Delta )*1e-6) ;
			Ip.SetTopRightX( (X + W - Delta )*1e-6) ;
			Ip.SetTopRightY( (Y + H - Delta )*1e-6) ;
			Ip.SetArea((W - 2*Delta)*(H - 2*Delta)*1e-12) ;
			mPlacedIp.push_back(Ip ) ;
		}
	}
	// Compute chip dimension
	mChipW = (MaxX - MinX)*1e-6;
	mChipH = (MaxY - MinY)*1e-6;

}

double PlacementParser::GetChipW() {
	return mChipW ;
}

double PlacementParser::GetChipH() {
	return mChipH ;
}

vector< double > PlacementParser::GetBottomLeftX() {
	vector< double > RetVal;
	for (unsigned int i = 0; i < mPlacedIp.size() ; i++) {
		RetVal.push_back(mPlacedIp[i].GetBottomLeftX() ) ;
	}
	return RetVal ;
}

vector< double > PlacementParser::GetBottomLeftY() {
	vector< double > RetVal;
	for (unsigned int i = 0; i < mPlacedIp.size() ; i++) {
		RetVal.push_back(mPlacedIp[i].GetBottomLeftY() ) ;
	}
	return RetVal ;
}

vector< double > PlacementParser::GetTopRightX() {
	vector< double > RetVal;
	for (unsigned int i = 0; i < mPlacedIp.size() ; i++) {
		RetVal.push_back(mPlacedIp[i].GetTopRightX() ) ;
	}
	return RetVal ;
}

vector< double > PlacementParser::GetTopRightY() {
	vector< double > RetVal;
	for (unsigned int i = 0; i < mPlacedIp.size() ; i++) {
		RetVal.push_back(mPlacedIp[i].GetTopRightY() ) ;
	}
	return RetVal ;
}

vector<string> PlacementParser::GetName() {
	vector<string> RetVal;
	for (unsigned int i = 0; i < mPlacedIp.size() ; i++) {
		RetVal.push_back(mPlacedIp[i].GetName() ) ;
	}
	return RetVal ;
}

PlacedIpCore PlacementParser::GetIpCore(string pCoreName) {

	for (unsigned int i = 0; i < mPlacedIp.size() ; i++) {
		if (mPlacedIp[i].GetName() == pCoreName )
			return mPlacedIp[i];
	}
	return PlacedIpCore() ;
}

vector<IpGeometry> PlacementParser::GetIpGeometry() {
	vector<IpGeometry> RetVal;
	for (unsigned int i = 0; i < mPlacedIp.size() ; i++) {
		IpGeometry G;
		G.SetXl(RealQuantity(mPlacedIp[i].GetBottomLeftX()));
		G.SetXr(RealQuantity(mPlacedIp[i].GetTopRightX()));
		G.SetYt(RealQuantity(mPlacedIp[i].GetTopRightY()));
		G.SetYb(RealQuantity(mPlacedIp[i].GetBottomLeftY()));
		G.SetArea(RealQuantity(mPlacedIp[i].GetArea()));
		RetVal.push_back(G) ;
	}
	return RetVal ;
}

IpGeometry PlacementParser::GetChipGeometry(){
	IpGeometry G ;
	G.SetXl(RealQuantity(0)) ;
	G.SetXr(RealQuantity(GetChipW())) ;
	G.SetYt(RealQuantity(GetChipH())) ;
	G.SetYb(RealQuantity(0)) ;
	cout << "Chip geometry " << endl ;
	cout << G << endl;
	return G ;
}


}
}
}

