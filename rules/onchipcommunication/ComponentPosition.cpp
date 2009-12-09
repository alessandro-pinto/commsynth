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
#include "ComponentPosition.h"

namespace cosi {

namespace rules {

namespace onchipcommunication {

using namespace cosi::quantity::onchipcommunication;

using namespace cosi::environment::onchipcommunication;

using namespace std;

ComponentPosition::ComponentPosition(std::vector< IpGeometry > Ip,
		IpGeometry ChipArea, double Density) {

	//Interface with the old model in Noc 1.2
	SetUpCoordinates(Ip, ChipArea);

	mIp = Ip;

	mDensity = Density;

	//Area purified from IPs
	mA = mH * mW;
	cout << "Slicing Platform set up :"<< endl;
	cout << "  "<< "Chip area : "<< mA << ",  Number of IPs : "<< mXbl.size()
			<< "  Required density : "<< mDensity;

	for (unsigned int i = 0; i < mXbl.size(); i++) {
		mY.insert(mYbl[i]);
		mY.insert(mYtr[i]);
		mA = mA - (mXtr[i] - mXbl[i])*(mYtr[i] - mYbl[i]);
	}
	cout << ",  IP area : "<< mH * mW - mA << ",  Remaining area = "<< mA
			<< endl;

	//mStep = sqrt( mA / (double) N ) ;
	mStep = sqrt( 1.0/( (double)mDensity*1e6));
	cout << "Step = "<< mStep << endl;

	int Vid, Aid;
	double X, Y;
	set < pair< double , double > > Xintervals;
	set < pair< double , double > >::iterator Iit;
	set< double >::iterator ItY;

	//Find all the free boxes
	double PrevY = 0.0;
	Aid = 0;
	vector< InstallationFace > InstallationFaces(mXbl.size() * mXbl.size() );
	for (ItY = mY.begin(); ItY != mY.end(); ItY++) {
		Xintervals = IntervalsX(PrevY);
		//cout << "Processing Y : " << *ItY << endl ;
		for (Iit = Xintervals.begin(); Iit != Xintervals.end(); Iit++) {
			//Extend all intervals to the next line
			InstallationFaces[Aid].SetXl(Iit->first);
			InstallationFaces[Aid].SetXr(Iit->second);
			InstallationFaces[Aid].SetYb(PrevY );
			InstallationFaces[Aid].SetYt(*ItY );
			//cout << "Face : (" << Iit->first << "," << Iit->second << ") (" << PrevY << "," << *ItY << endl ;
			//add the middle point
			mCandidateCenters.resize(mCandidateCenters.size() + 1);
			mCandidateCenters[Aid].SetX( (InstallationFaces[Aid].GetXr().GetValue() + InstallationFaces[Aid].GetXl().GetValue() )/2);
			mCandidateCenters[Aid].SetY((InstallationFaces[Aid].GetYt().GetValue()+ InstallationFaces[Aid].GetYb().GetValue())/2);
			mCandidateCenters[Aid].SetSurfaceIndex(Aid );
			Aid++;
		}
		PrevY = *ItY;
	}
	InstallationFaces.resize(Aid );
	//Merge the boxes if it can be done
	vector< int > ToErase(InstallationFaces.size() , 0);
	for (unsigned int i = 0; i < InstallationFaces.size(); i++) {
		for (unsigned int j = 0; j < InstallationFaces.size(); j++) {
			if (InstallationFaces[i].GetXl().GetValue() == InstallationFaces[j].GetXl().GetValue()&&InstallationFaces[i].GetXr().GetValue()== InstallationFaces[j].GetXr().GetValue()&&InstallationFaces[i].GetYt().GetValue()== InstallationFaces[j].GetYb().GetValue()) {
				//Extend j and delete i
				InstallationFaces[j].SetYb(InstallationFaces[i].GetYb());
				for (unsigned int k = 0; k < mCandidateCenters.size(); k++) {
					if (mCandidateCenters[k].GetSurfaceIndex() == i )
						mCandidateCenters[k].SetSurfaceIndex(j);
				}
				ToErase[i] = 1;
				break;
			}
		}
	}

	Aid = 0;
	mInstallationFaces.resize(InstallationFaces.size() );
	for (unsigned int i = 0; i < InstallationFaces.size(); i++) {
		if ( !ToErase[i]) {
			mInstallationFaces[Aid].SetXl(InstallationFaces[i].GetXl());
			mInstallationFaces[Aid].SetXr(InstallationFaces[i].GetXr());
			mInstallationFaces[Aid].SetYt(InstallationFaces[i].GetYt());
			mInstallationFaces[Aid].SetYb(InstallationFaces[i].GetYb());
			for (unsigned int k = 0; k < mCandidateCenters.size(); k++) {
				if (mCandidateCenters[k].GetSurfaceIndex() == i )
					mCandidateCenters[k].SetSurfaceIndex(Aid );
			}
			Aid++;
		}
	}

	mInstallationFaces.resize(Aid );
	//Sample each surface and generate the installation sites but don't forget
	//the sources and destinations!!!
	Vid = 0;
	mInstallationSites.resize(mSXpos.size() + mDXpos.size() );
	for (unsigned int i = 0; i < mSXpos.size(); i ++) {
		//find the closes area
		double MinDistance;
		int MinId;
		MinDistance = DBL_MAX;
		for (int j = 0; j < mInstallationFaces.size(); j++) {
			double Distance = VtoAdist(mSXpos[i], mSYpos[i], j );
			if (Distance <= MinDistance ) {
				MinDistance = Distance;
				MinId = j;
			}
		}
		mInstallationSites[i].SetSurfaceIndex(MinId);
		mInstallationSites[i].SetX(mSXpos[i]);
		mInstallationSites[i].SetY(mSYpos[i]);
	}
	for (int i = mSXpos.size(); i < mSXpos.size() + mDXpos.size(); i ++) {
		double MinDistance;
		int MinId;
		MinDistance = DBL_MAX;
		for (int j = 0; j < mInstallationFaces.size(); j++) {
			double Distance = VtoAdist(mDXpos[i], mDYpos[i], j );
			if (Distance <= MinDistance ) {
				MinDistance = Distance;
				MinId = j;
			}
		}
		mInstallationSites[i].SetSurfaceIndex(MinId );
		mInstallationSites[i].SetX(mDXpos[i - mSXpos.size()]);
		mInstallationSites[i].SetY(mDYpos[i - mSXpos.size()]);
	}

	Vid = mSXpos.size() + mDXpos.size();
	for (int i = 0; i < mInstallationFaces.size(); i ++) {
		mInstallationFaces[i].SetMinId(Vid );
		if ( (mInstallationFaces[i].GetXr().GetValue() - mInstallationFaces[i].GetXl().GetValue()<= mStep)&&(mInstallationFaces[i].GetYt().GetValue()- mInstallationFaces[i].GetYb().GetValue()<= mStep)) {
			//Install one node
			X = mInstallationFaces[i].GetXl().GetValue() + (mInstallationFaces[i].GetXr().GetValue()- mInstallationFaces[i].GetXl().GetValue()) / 2.0;
			Y = mInstallationFaces[i].GetYb().GetValue() + (mInstallationFaces[i].GetYt().GetValue()- mInstallationFaces[i].GetYb().GetValue()) / 2.0;
			mInstallationSites.resize(mInstallationSites.size() + 1);
			mInstallationSites[Vid].SetSurfaceIndex(i);
			mInstallationSites[Vid].SetX(X);
			mInstallationSites[Vid].SetY(Y);
			Vid++;
		} else if (mInstallationFaces[i].GetXr().GetValue()- mInstallationFaces[i].GetXl().GetValue()<= mStep ) {
			//Install a vertical line of nodes
			X = mInstallationFaces[i].GetXl().GetValue() + (mInstallationFaces[i].GetXr().GetValue()- mInstallationFaces[i].GetXl().GetValue()) / 2.0;
			Y = mInstallationFaces[i].GetYb().GetValue() + mStep/2;
			while (Y <= mInstallationFaces[i].GetYt().GetValue() - mStep/2) {
				//Add a node
				mInstallationSites.resize(mInstallationSites.size() + 1);
				mInstallationSites[Vid].SetSurfaceIndex(i);
				mInstallationSites[Vid].SetX(X );
				mInstallationSites[Vid].SetY(Y );
				Vid++;
				Y = Y + mStep;
			}
		} else if (mInstallationFaces[i].GetYt().GetValue()- mInstallationFaces[i].GetYb().GetValue()<= mStep ) {
			//Install an horizontal line of nodes
			Y = mInstallationFaces[i].GetYb().GetValue() + (mInstallationFaces[i].GetYt().GetValue()- mInstallationFaces[i].GetYb().GetValue()) / 2.0;
			X = mInstallationFaces[i].GetXl().GetValue();
			while (X <= mInstallationFaces[i].GetXr().GetValue()) {
				//Add a node
				mInstallationSites.resize(mInstallationSites.size() + 1);
				mInstallationSites[Vid].SetSurfaceIndex(i );
				mInstallationSites[Vid].SetX(X);
				mInstallationSites[Vid].SetY(Y);
				Vid++;
				X = X + mStep;
			}
		} else {
			Y = mInstallationFaces[i].GetYb().GetValue() + mStep/2;
			while (Y <= mInstallationFaces[i].GetYt().GetValue() - mStep/2) {
				X = mInstallationFaces[i].GetXl().GetValue();
				while (X <= mInstallationFaces[i].GetXr().GetValue()) {
					//Add a node
					mInstallationSites.resize(mInstallationSites.size() + 1);
					mInstallationSites[Vid].SetSurfaceIndex(i);
					mInstallationSites[Vid].SetX(X);
					mInstallationSites[Vid].SetY(Y);
					Vid++;
					X = X + mStep;
				}
				Y = Y + mStep;
			}
		}
		mInstallationFaces[i].SetMaxId(Vid - 1);
	}

	mNr = mInstallationSites.size();

	//map the centers id

	int CenterIndex = 0;
	for (int i = 0; i < mCandidateCenters.size(); i++) {
		//find the closest point to the center
		list<int> N = LocalSearch(mCandidateCenters[i].GetSurfaceIndex(),
				mCandidateCenters[i].GetX(), mCandidateCenters[i].GetY(), 1.2
						*mStep);
		if (N.size() > 0) {
			mCentersId.resize(CenterIndex + 1);
			mCentersId[CenterIndex] = *(N.begin());
			CenterIndex++;
		} else {
			cout << "Isolated center at ("<< mCandidateCenters[i].GetX()<< ","
					<< mCandidateCenters[i].GetY()<< ")"<< endl;
		}
	}

	cout << "  "<< "Number of installation sites : "<< mNr << ",  Density : "
			<< mNr*1e-6/ mA << " Nodes/mm^2"<< endl;
}

ComponentPosition::~ComponentPosition() {
}

set< pair< double , double > > ComponentPosition::IntervalsX(double pY) {
	double X = 0;
	pair< double , double > Interval;
	set< pair<double,double> > OccupiedIntervals, RetVal;
	set< pair<double,double> >::iterator It;
	for (int i = 0; i < mXbl.size(); i++) {
		if (pY < mYtr[i]&& pY >= mYbl[i]) {
			OccupiedIntervals.insert(pair<double, double>(mXbl[i], mXtr[i]) );
		}
	}

	OccupiedIntervals.insert(pair<double, double>(mW , mW ) );

	Interval.first = 0;
	for (It = OccupiedIntervals.begin(); It != OccupiedIntervals.end(); It++) {
		Interval.second = It->first;
		RetVal.insert(Interval );
		Interval.first = It->second;
	}
	return RetVal;
}

double ComponentPosition::VtoAdist(double X, double Y, int A) {

	if (X <= mInstallationFaces[A].GetXl().GetValue() && Y <= mInstallationFaces[A].GetYb().GetValue())
		return Distance(X , Y, mInstallationFaces[A].GetXl().GetValue(), mInstallationFaces[A].GetYb().GetValue());

	if (X >= mInstallationFaces[A].GetXr().GetValue() && Y <= mInstallationFaces[A].GetYb().GetValue())
		return Distance(X , Y , mInstallationFaces[A].GetXr().GetValue(), mInstallationFaces[A].GetYb().GetValue());

	if (X >= mInstallationFaces[A].GetXl().GetValue() && X <= mInstallationFaces[A].GetXr().GetValue() && Y<= mInstallationFaces[A].GetYb().GetValue() )
		return fabs(Y - mInstallationFaces[A].GetYb().GetValue());

	if (X <= mInstallationFaces[A].GetXl().GetValue()&& Y >= mInstallationFaces[A].GetYt().GetValue())
		return Distance(X , Y , mInstallationFaces[A].GetXl().GetValue(), mInstallationFaces[A].GetYt().GetValue());

	if (X >= mInstallationFaces[A].GetXr().GetValue()&& Y >= mInstallationFaces[A].GetYt().GetValue())
		return Distance(X , Y , mInstallationFaces[A].GetXr().GetValue(), mInstallationFaces[A].GetYt().GetValue());

	if (X >= mInstallationFaces[A].GetXl().GetValue()&& X <= mInstallationFaces[A].GetXr().GetValue()&& Y>= mInstallationFaces[A].GetYt().GetValue())
		return fabs(Y - mInstallationFaces[A].GetYt().GetValue());

	if (X <= mInstallationFaces[A].GetXl().GetValue()&& Y <= mInstallationFaces[A].GetYt().GetValue()&& Y>= mInstallationFaces[A].GetYb().GetValue())
		return fabs(X - mInstallationFaces[A].GetXl().GetValue());

	if (X >= mInstallationFaces[A].GetXr().GetValue()&& Y <= mInstallationFaces[A].GetYt().GetValue()&& Y>= mInstallationFaces[A].GetYb().GetValue())
		return fabs(X - mInstallationFaces[A].GetXr().GetValue());

}

double ComponentPosition::VtoAdist(int V, int A) {
	return VtoAdist(GetX(V),GetY(V),A) ;
}

list< int > ComponentPosition::LocalSearch(int A, double X, double Y, double D) {
	list< int > N;
	for (int i = mInstallationFaces[A].GetMinId(); i
			<= mInstallationFaces[A].GetMaxId(); i++) {
		if (Norm1(Position(GetX(i ) , GetY(i ), 0), Position(X , Y , 0) ) <= D ) {
			N.push_back(i );
		}
	}

	return N;
}

double ComponentPosition::GetX(int V) {
	return mInstallationSites[V].GetX();
}
double ComponentPosition::GetY(int V) {
	return mInstallationSites[V].GetY();
}

std::vector<InstallationSite > ComponentPosition::GetInstallationSites() {
	return mInstallationSites;
}

double ComponentPosition::Distance(double X1, double Y1, double X2, double Y2) {
	return (fabs(X1 - X2)+ fabs(Y1 - Y2) );
}

double ComponentPosition::Distance(int U , int V) {
	return Distance(GetX(U),GetY(U),GetX(V),GetY(V));
}


list< int > ComponentPosition::Neighbors(int V, int D, double Span) {

	list< int > N, ToAdd;

	list< int >::iterator Lit;

	ToAdd = LocalSearch(mInstallationSites[V].GetSurfaceIndex(), mInstallationSites[V].GetX(),
			mInstallationSites[V].GetY(), Span );
	for (Lit = ToAdd.begin(); Lit != ToAdd.end(); Lit++) {
		if ( *Lit != V )
			N.push_back( *Lit );
	}

	//for all other areas different frim V.A
	for (unsigned int i = 0; i < mInstallationFaces.size(); i++) {
		if ( (i != mInstallationSites[V].GetSurfaceIndex()) && (VtoAdist(V , i ) < Span )) {
			Position P;
			double MaxDist;
			MaxDist = Span - VtoAdist(V , i );
			P = Closest(i , V );
			ToAdd = LocalSearch(i , P.GetX() , P.GetY(), MaxDist );
			for (Lit = ToAdd.begin(); Lit != ToAdd.end(); Lit++) {
				if ( *Lit != V )
					N.push_back( *Lit );
			}
		}
	}

	if (Distance(V , D ) < Span )
		N.push_back(D );

	//verify N
	//Each point in N must have a distance that is less then lst
	for (Lit = N.begin(); Lit != N.end(); Lit++) {
		if (Distance( *Lit , V ) > Span )
			cout << "WARNING: slicing platform returning an unreachable point "
					<< endl;
	}

	return N;
}



Position ComponentPosition::Closest(int A, int V) {

	double X, Y;
	X = GetX(V );
	Y = GetY(V );
	if (X <= mInstallationFaces[A].GetXl().GetValue()&& Y <= mInstallationFaces[A].GetYb().GetValue())
		return Position(mInstallationFaces[A].GetXl().GetValue(),
				mInstallationFaces[A].GetYb().GetValue(),0);

	if (X >= mInstallationFaces[A].GetXr().GetValue()&& Y <= mInstallationFaces[A].GetYb().GetValue())
		return Position(mInstallationFaces[A].GetXr().GetValue(),
				mInstallationFaces[A].GetYb().GetValue(),0);

	if (X >= mInstallationFaces[A].GetXl().GetValue() && X <= mInstallationFaces[A].GetXr().GetValue() && Y
			<= mInstallationFaces[A].GetYb().GetValue())
		return Position(X , mInstallationFaces[A].GetYb().GetValue(),0);

	if (X <= mInstallationFaces[A].GetXl().GetValue()&& Y >= mInstallationFaces[A].GetYt().GetValue())
		return Position(mInstallationFaces[A].GetXl().GetValue(),
				mInstallationFaces[A].GetYt().GetValue(),0);

	if (X >= mInstallationFaces[A].GetXr().GetValue()&& Y >= mInstallationFaces[A].GetYt().GetValue())
		return Position(mInstallationFaces[A].GetXr().GetValue(),
				mInstallationFaces[A].GetYt().GetValue(),0);

	if (X >= mInstallationFaces[A].GetXl().GetValue()&& X <= mInstallationFaces[A].GetXr().GetValue()&& Y
			>= mInstallationFaces[A].GetYt().GetValue())
		return Position(X , mInstallationFaces[A].GetYt().GetValue(),0);

	if (X <= mInstallationFaces[A].GetXl().GetValue()&& Y <= mInstallationFaces[A].GetYt().GetValue()&& Y
			>= mInstallationFaces[A].GetYb().GetValue())
		return Position(mInstallationFaces[A].GetXl().GetValue(), Y ,0);

	if (X >= mInstallationFaces[A].GetXr().GetValue()&& Y <= mInstallationFaces[A].GetYt().GetValue()&& Y
			>= mInstallationFaces[A].GetYb().GetValue())
		return Position(mInstallationFaces[A].GetXr().GetValue(), Y ,0);
}

void ComponentPosition::SetUpCoordinates(std::vector< IpGeometry > Ip,
		IpGeometry ChipArea) {

	mSXpos.resize(Ip.size() );
	mSYpos.resize(Ip.size() );
	mDXpos.resize(Ip.size() );
	mDYpos.resize(Ip.size() );
	mXbl.resize(Ip.size() );
	mYbl.resize(Ip.size() );
	mXtr.resize(Ip.size() );
	mYtr.resize(Ip.size() );

	for (unsigned int i = 0; i < Ip.size(); i ++) {
		mSXpos[i] = Ip[i].GetXl().GetValue();
		mSYpos[i] = Ip[i].GetYb().GetValue();
		mDXpos[i] = Ip[i].GetXl().GetValue();
		mDYpos[i] = Ip[i].GetYb().GetValue();
		mXtr[i] = Ip[i].GetXr().GetValue();
		mYtr[i] = Ip[i].GetYt().GetValue();
	}

	mXbl = mSXpos;
	mYbl = mSYpos;

	mW = ChipArea.GetXr().GetValue() - ChipArea.GetXl().GetValue();
	mH = ChipArea.GetYt().GetValue() - ChipArea.GetYb().GetValue();

}

int ComponentPosition::Size() {
	return mInstallationSites.size() ;
}

}
}
}
