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
#include "WiringRule.h"

namespace cosi {

namespace rules {

namespace wiredbas {

std::string WiringRule::SurfacePair::sQuantityName = "SurfacePair";
std::string WiringRule::SurfacePair::sQuantityType = "A pair of Surfaces";

std::ostream& operator <<(std::ostream &pOs, const WiringRule::SurfacePair &pS) {
	return pOs ;
}

WiringRule::WiringRule(
		std::map< std::string , cosi::environment::bas::InstSurface > pInstallationSurfaces) {
	mInstallationSurfaces = pInstallationSurfaces;
	mLastId = 0;

	BuildSurfaceConnectivityGraph();

}

WiringRule::~WiringRule() {

}

WiringPath WiringRule::GetWiringPath(Position pStart, Position pEnd) {

	//shortest path on a modified graph where the positions are also node

	//Find closest surface and point to Start
	std::string ClosestStart, ClosestEnd;
	double MinDistanceStart= DBL_MAX;
	Position ClosestPstart;
	double MinDistanceEnd= DBL_MAX;
	Position ClosestPend;
	std::map< std::string , cosi::environment::bas::InstSurface >::iterator S1;
	for (S1 = mInstallationSurfaces.begin(); S1 != mInstallationSurfaces.end(); S1++) {
		Position P = ClosestPoint(pStart , S1->first);
		if (Norm2(P, pStart) < MinDistanceStart) {
			MinDistanceStart =Norm2(P, pStart);
			ClosestStart = S1->first;
			ClosestPstart = P;
		}
		P = ClosestPoint(pEnd, S1->first);
		if (Norm2(P, pEnd) < MinDistanceEnd) {
			MinDistanceEnd =Norm2(P, pEnd);
			ClosestEnd = S1->first;
			ClosestPend = P;
		}
	}


	if ( ClosestStart == ClosestEnd ) {
		WiringPath Path ;
		Path.AppendPosition(pStart) ;
		Path.AppendPosition(ClosestPstart) ;
		Path.AppendPosition(ClosestPend) ;
		Path.AppendPosition(pEnd) ;
		return Path ;
	}


	CommunicationStructure< Label_2<Position,SurfacePair> >
			ConnectivityGraph("");
	ConnectivityGraph = mConnectivityGraph;
	int Id = mLastId;
	int StartId, EndId;
	ConnectivityGraph.AddVertex(Id );
	ConnectivityGraph.SetName(Id , "Start");
	ConnectivityGraph[Id].PRJ(Position)Set(ClosestPstart);
	StartId = Id ;
	Id++;
	ConnectivityGraph.AddVertex(Id );
	ConnectivityGraph.SetName(Id , "End");
	ConnectivityGraph[Id].PRJ(Position)Set(ClosestPend);
	EndId = Id ;
	//add edges from the start node to all nodes containing
	//the closeest surface
	IdGraph::v_iterator V;
	for (V = ConnectivityGraph.v_begin(); V != ConnectivityGraph.v_end(); V++) {
		if (ConnectivityGraph[*V].PRJ(SurfacePair)Contains(ClosestStart) ) {
			ConnectivityGraph.AddEdge(StartId , *V );
		}
		if (ConnectivityGraph[*V].PRJ(SurfacePair)Contains(ClosestEnd) ) {
			ConnectivityGraph.AddEdge(*V , EndId );
		}
	}

	//find the shortest path
	//Shortest path on the set of installation sites
	std::set< std::pair< double , int > > Q;
	std::set< std::pair< double , int > >::iterator Extracted;
	std::map< int , double > Fp;
	std::map< int , int > Pi;
	std::map< int , bool > Found;
	Q.insert(std::pair<double, int>( 0, StartId ) );
	Found[StartId] = true;
	Fp[StartId] = 0;
	Pi[StartId] = -1;
	while ( !Q.empty() ) {

		//Extract from the queue
		int V;
		Extracted = Q.begin();
		V = Extracted->second;
		Q.erase(Extracted );

		if (V == EndId ) {
			break;
		}

		IdGraph::list_iterator U;

		for (U = ConnectivityGraph.out_begin(V); U != ConnectivityGraph.out_end(V); U++) {
			double D;
			D = Norm2(ConnectivityGraph[V].PRJ(Position)Get(),ConnectivityGraph[*U].PRJ(Position)Get());
			if ( (Fp.find( *U ) == Fp.end() ) || (Fp[*U] > Fp[V] + D)) {
				if (Found.find(*U) == Found.end() ) {
					Q.insert(pair<double, int>(Fp[V] + D , *U ) );
					Found[*U] = true;
				}
				Fp[*U] = Fp[V] + D;
				Pi[*U] = V;
			}
		}
	}

	//create the path
	WiringPath Path;
	Path.AppendPosition(pEnd);
	int CurrentNode;
	CurrentNode = EndId;
	Path.PrependPosition(ConnectivityGraph[CurrentNode].PRJ(Position)Get());
	while (CurrentNode != StartId ) {
		CurrentNode = Pi[CurrentNode];
		Path.PrependPosition(ConnectivityGraph[CurrentNode].PRJ(Position)Get());
	}

	Path.PrependPosition(pStart);

	return Path;
}

void WiringRule::BuildSurfaceConnectivityGraph() {

	std::map< std::string , cosi::environment::bas::InstSurface >::iterator S1,
	S2;

	//set vertexes

	for (S1 = mInstallationSurfaces.begin(); S1 != mInstallationSurfaces.end(); S1++) {
		for (S2 = S1; S2!= mInstallationSurfaces.end(); S2++) {
			if (S1 != S2 ) {
				Position P = FindIntersectionPoint(S1->first, S2->first);
				if ( ! P.IsBot() && ! P.IsAny()) {
					mConnectivityGraph.AddVertex(mLastId);
					mConnectivityGraph.SetName(S1->first + S2->first);
					mConnectivityGraph[mLastId].PRJ(Position)Set(P);
					mConnectivityGraph[mLastId].PRJ(SurfacePair)SetFirst(S1->first);
					mConnectivityGraph[mLastId].PRJ(SurfacePair)SetSecond(S2->first);
					//std::cout << "Adding vertex " << mLastId << " s1 = " << S1->first << " s2 = " << S2->first << std::endl;
					mLastId++;
				}
			}
		}
	}

	//set edges
	IdGraph::v_iterator U, V;
	for (U = mConnectivityGraph.v_begin(); U != mConnectivityGraph.v_end(); U++) {
		for (V = mConnectivityGraph.v_begin(); V != mConnectivityGraph.v_end(); V++) {
			if ( *U != *V ) {
				if ( mConnectivityGraph[*U].PRJ(SurfacePair)Intersects( mConnectivityGraph[*V].PRJ(SurfacePair)Get() )) {
					mConnectivityGraph.AddEdge(*U,*V);
					//std::cout << "Adding edge (" << *U << "," << *V << ")" << std::endl;
				}
			}
		}
	}
}

Position WiringRule::FindIntersectionPoint(std::string pS1, std::string pS2) {
	cosi::environment::bas::InstSurface S1, S2;
	S1 = mInstallationSurfaces[pS1];
	S2 = mInstallationSurfaces[pS2];
	//return bot if there is no intersection point
	//we have four combinations to try
	//Let a11, b11 be one line in the V11, V12 direction

	ColumnVector a11(3);
	ColumnVector b11(3);

	a11(1) = S1.GetV1().GetX() + 0.5*(S1.GetV4().GetX() - S1.GetV1().GetX());
	a11(2) = S1.GetV1().GetY() + 0.5*(S1.GetV4().GetY() - S1.GetV1().GetY());
	a11(3) = S1.GetV1().GetZ() + 0.5*(S1.GetV4().GetZ() - S1.GetV1().GetZ());

	b11(1) = S1.GetV2().GetX() - S1.GetV1().GetX() + S1.GetV3().GetX() - S1.GetV4().GetX();
	b11(2) = S1.GetV2().GetY() - S1.GetV1().GetY() + S1.GetV3().GetY() - S1.GetV4().GetY();
	b11(3) = S1.GetV2().GetZ() - S1.GetV1().GetZ() + S1.GetV3().GetZ() - S1.GetV4().GetZ();

	b11 = b11 / b11.NormFrobenius();

	//Let a12, b12 be one line in the V11, V14 direction

	ColumnVector a12(3);
	ColumnVector b12(3);

	a12(1) = S1.GetV1().GetX() + 0.5*(S1.GetV2().GetX() - S1.GetV1().GetX());
	a12(2) = S1.GetV1().GetY() + 0.5*(S1.GetV2().GetY() - S1.GetV1().GetY());
	a12(3) = S1.GetV1().GetZ() + 0.5*(S1.GetV2().GetZ() - S1.GetV1().GetZ());

	b12(1) = S1.GetV4().GetX() - S1.GetV1().GetX() + S1.GetV3().GetX() - S1.GetV2().GetX();
	b12(2) = S1.GetV4().GetY() - S1.GetV1().GetY() + S1.GetV3().GetY() - S1.GetV2().GetY();
	b12(3) = S1.GetV4().GetZ() - S1.GetV1().GetZ() + S1.GetV3().GetZ() - S1.GetV2().GetZ();

	b12 = b12 / b12.NormFrobenius();

	//Let a21, b21 be one line in the V21, V22 direction
	ColumnVector a21(3);
	ColumnVector b21(3);

	a21(1) = S2.GetV1().GetX() + 0.5*(S2.GetV4().GetX() - S2.GetV1().GetX());
	a21(2) = S2.GetV1().GetY() + 0.5*(S2.GetV4().GetY() - S2.GetV1().GetY());
	a21(3) = S2.GetV1().GetZ() + 0.5*(S2.GetV4().GetZ() - S2.GetV1().GetZ());

	b21(1) = S2.GetV2().GetX() - S2.GetV1().GetX() + S2.GetV3().GetX() - S2.GetV4().GetX();
	b21(2) = S2.GetV2().GetY() - S2.GetV1().GetY() + S2.GetV3().GetY() - S2.GetV4().GetY();
	b21(3) = S2.GetV2().GetZ() - S2.GetV1().GetZ() + S2.GetV3().GetZ() - S2.GetV4().GetZ();

	b21 = b21 / b21.NormFrobenius();

	//Let a22, b22 be one line in the V21, V24 direction
	ColumnVector a22(3);
	ColumnVector b22(3);

	a22(1) = S2.GetV1().GetX() + 0.5*(S2.GetV2().GetX() - S2.GetV1().GetX());
	a22(2) = S2.GetV1().GetY() + 0.5*(S2.GetV2().GetY() - S2.GetV1().GetY());
	a22(3) = S2.GetV1().GetZ() + 0.5*(S2.GetV2().GetZ() - S2.GetV1().GetZ());

	b22(1) = S2.GetV4().GetX() - S2.GetV1().GetX() + S2.GetV3().GetX() - S2.GetV2().GetX();
	b22(2) = S2.GetV4().GetY() - S2.GetV1().GetY() + S2.GetV3().GetY() - S2.GetV2().GetY();
	b22(3) = S2.GetV4().GetZ() - S2.GetV1().GetZ() + S2.GetV3().GetZ() - S2.GetV2().GetZ();

	b22 = b22 / b22.NormFrobenius();

	//Check if one of the 4 combination gives one intersection point
	//Stop as soon as one if found

	Position P;
	P = LineIntersection(a11, b11, cosi::Norm2(S1.GetV1(),S1.GetV2()) , a21, b21 , cosi::Norm2(S2.GetV1(),S2.GetV2()) );
	if ( !P.IsBot() )
	return P;
	P = LineIntersection(a11, b11,cosi::Norm2(S1.GetV1(),S1.GetV2()) , a22, b22,cosi::Norm2(S2.GetV1(),S2.GetV4()) );
	if ( !P.IsBot() )
	return P;
	P = LineIntersection(a12, b12,cosi::Norm2(S1.GetV1(),S1.GetV4()) , a21, b21,cosi::Norm2(S2.GetV1(),S2.GetV2()));
	if ( !P.IsBot() )
	return P;
	P = LineIntersection(a12, b12,cosi::Norm2(S1.GetV1(),S1.GetV4()) , a22, b22,cosi::Norm2(S2.GetV1(),S2.GetV4()));

	return P;

}

Position WiringRule::LineIntersection(ColumnVector a1, ColumnVector b1, double Ub1,
ColumnVector a2, ColumnVector b2,double Ub2) {

	Matrix A(3, 2);
	A(1, 1) = b1(1);
	A(2, 1) = b1(2);
	A(3, 1) = b1(3);
	A(1, 2) = -b2(1);
	A(2, 2) = -b2(2);
	A(3, 2) = -b2(3);

	//Check if the two lines are the same line
	if ( ( (A(1, 1)-A(1, 2))/(b2(1) - b1(1))== (A(2, 1)-A(2, 2))
			/(b2(2) - b1(2))) &&( (A(2, 1)-A(2, 2))/(b2(2) - b1(2))== (A(3, 1)
					-A(3, 2))/(b2(3) - b1(3)))) {
		Position P;
		P.IsAny(true);
		return P;
	}

	//Otherwise, find the solution by projecting on X and Y
	Matrix B(2, 2);
	B(1, 1) = A(1, 1);
	B(1, 2) = A(1, 2);
	B(2, 1) = A(2, 1);
	B(2, 2) = A(2, 2);
	//check if the two lines are parallel in X and Y
	if (B.Determinant() == 0) {
		Position P;
		P.IsBot(true);
		return P;
	}

	//Find the solution

	ColumnVector t(2);
	ColumnVector a2_(2);
	ColumnVector a1_(2);
	a1_(1) = a1(1);
	a1_(2) = a1(2);
	a2_(1) = a2(1);
	a2_(2) = a2(2);
	t = B.i() * (a2_ - a1_);

	if (A(3, 1)*t(1) + A(3, 2)*t(2) != a2(3) - a1(3)) {
		Position P;
		P.IsBot(true);
		return P;
	} else if ( t(1) >=0 && t(1) <= Ub1 && t(2) >=0 && t(2) <= Ub2) {
		double X = a1(1) + b1(1)*t(1);
		double Y = a1(2) + b1(2)*t(1);
		double Z = a1(3) + b1(3)*t(1);

		Position P(X, Y, Z);
		return P;
	}
	return Position();

}

Position WiringRule::ClosestPoint(Position pP, std::string pSurface) {

	double MinDistance= DBL_MAX;
	ColumnVector p(3), minp(3);
	p(1) = pP.GetX();
	p(2) = pP.GetY();
	p(3) = pP.GetZ();

	ColumnVector b12(3);
	b12(1) = mInstallationSurfaces[pSurface].GetV2().GetX() - mInstallationSurfaces[pSurface].GetV1().GetX();
	b12(2) = mInstallationSurfaces[pSurface].GetV2().GetY() - mInstallationSurfaces[pSurface].GetV1().GetY();
	b12(3) = mInstallationSurfaces[pSurface].GetV2().GetZ() - mInstallationSurfaces[pSurface].GetV1().GetZ();
	b12 = b12 / b12.NormFrobenius();
	ColumnVector a12(3);
	a12(1) = mInstallationSurfaces[pSurface].GetV1().GetX();
	a12(2) = mInstallationSurfaces[pSurface].GetV1().GetY();
	a12(3) = mInstallationSurfaces[pSurface].GetV1().GetZ();
	double u12 = Norm2(mInstallationSurfaces[pSurface].GetV1(),
	mInstallationSurfaces[pSurface].GetV2());

	double t12;
	t12 = DotProduct(p-a12, b12);

	if (t12 < 0)
	t12 = 0;
	else if (t12 > u12)
	t12 = u12;

	ColumnVector p12(3);
	p12 = a12 + b12*t12;

	if ((p-p12).NormFrobenius() < MinDistance ) {
		minp = p12;
		MinDistance = (p-p12).NormFrobenius();
	}

	ColumnVector b14(3);
	b14(1) = mInstallationSurfaces[pSurface].GetV4().GetX() - mInstallationSurfaces[pSurface].GetV1().GetX();
	b14(2) = mInstallationSurfaces[pSurface].GetV4().GetY() - mInstallationSurfaces[pSurface].GetV1().GetY();
	b14(3) = mInstallationSurfaces[pSurface].GetV4().GetZ() - mInstallationSurfaces[pSurface].GetV1().GetZ();
	b14 = b14 / b14.NormFrobenius();
	ColumnVector a14(3);
	a14 = a12;
	double u14 = Norm2(mInstallationSurfaces[pSurface].GetV1(),
	mInstallationSurfaces[pSurface].GetV4());

	double t14;
	t14 = DotProduct(p-a14, b14);

	if (t14 < 0)
	t14 = 0;
	else if (t14 > u14)
	t14 = u14;

	ColumnVector p14(3);
	p14 = a14 + b14*t14;

	if ((p-p14).NormFrobenius() < MinDistance ) {
		minp = p14;
		MinDistance = (p-p14).NormFrobenius();
	}

	ColumnVector b23(3);
	b23(1) = mInstallationSurfaces[pSurface].GetV3().GetX() - mInstallationSurfaces[pSurface].GetV2().GetX();
	b23(2) = mInstallationSurfaces[pSurface].GetV3().GetY() - mInstallationSurfaces[pSurface].GetV2().GetY();
	b23(3) = mInstallationSurfaces[pSurface].GetV3().GetZ() - mInstallationSurfaces[pSurface].GetV2().GetZ();
	b23 = b23 / b23.NormFrobenius();
	ColumnVector a23(3);
	a23(1) = mInstallationSurfaces[pSurface].GetV2().GetX();
	a23(2) = mInstallationSurfaces[pSurface].GetV2().GetY();
	a23(3) = mInstallationSurfaces[pSurface].GetV2().GetZ();
	double u23 = Norm2(mInstallationSurfaces[pSurface].GetV2(),
	mInstallationSurfaces[pSurface].GetV3());

	double t23;
	t23 = DotProduct(p-a23, b23);

	if (t23 < 0)
	t23 = 0;
	else if (t23 > u23)
	t23 = u23;

	ColumnVector p23(3);
	p23 = a23 + b23*t23;

	if ((p-p23).NormFrobenius() < MinDistance ) {
		minp = p23;
		MinDistance = (p-p23).NormFrobenius();
	}

	ColumnVector b43(3);
	b43(1) = mInstallationSurfaces[pSurface].GetV3().GetX() - mInstallationSurfaces[pSurface].GetV4().GetX();
	b43(2) = mInstallationSurfaces[pSurface].GetV3().GetY() - mInstallationSurfaces[pSurface].GetV4().GetY();
	b43(3) = mInstallationSurfaces[pSurface].GetV3().GetZ() - mInstallationSurfaces[pSurface].GetV4().GetZ();
	b43 = b43 / b43.NormFrobenius();
	ColumnVector a43(3);
	a43(1) = mInstallationSurfaces[pSurface].GetV4().GetX();
	a43(2) = mInstallationSurfaces[pSurface].GetV4().GetY();
	a43(3) = mInstallationSurfaces[pSurface].GetV4().GetZ();
	double u43 = Norm2(mInstallationSurfaces[pSurface].GetV4(),
	mInstallationSurfaces[pSurface].GetV3());
	double t43;
	t43 = DotProduct(p-a43, b43);

	if (t43 < 0)
	t43 = 0;
	else if (t43 > u43)
	t43 = u43;

	ColumnVector p43(3);
	p43 = a43 + b43*t43;

	if ((p-p43).NormFrobenius() < MinDistance ) {
		minp = p43;
		MinDistance = (p-p43).NormFrobenius();
	}

	return Position(minp(1), minp(2), minp(3));

}

cosi::commstruct::wiredbas::Implementation* WiringRule::GetConnectivityGraph() {
	cosi::commstruct::wiredbas::Implementation* C = new cosi::commstruct::wiredbas::Implementation("");
	IdGraph::v_iterator U,V;
	for( U = mConnectivityGraph.v_begin(); U!= mConnectivityGraph.v_end(); U++) {
		for( V = mConnectivityGraph.v_begin(); V!= mConnectivityGraph.v_end(); V++) {
			if ( !C->InV(*U)) {
				C->AddVertex(*U);
				(*C)[*U].PRJ(Position)Set(mConnectivityGraph[*U].PRJ(Position)Get());
			}
			if ( !C->InV(*V)) {
				C->AddVertex(*V);
				(*C)[*V].PRJ(Position)Set(mConnectivityGraph[*V].PRJ(Position)Get());
			}
			if ( ! C->InE(*U,*V) && mConnectivityGraph.InE(*U,*V) ) {
				C->AddEdge(*U,*V);
				WiringPath Path;
				Path.AppendPosition(mConnectivityGraph[*U].PRJ(Position)Get());
				Path.AppendPosition(mConnectivityGraph[*V].PRJ(Position)Get());
				(*C)[Edge(*U,*V)].PRJ(WiringPath)Set(Path);
			}
		}
	}
	return C;
}

}
}
}
