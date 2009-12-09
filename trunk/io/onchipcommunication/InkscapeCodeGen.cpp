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
#include "InkscapeCodeGen.h"

namespace cosi {

namespace io {

namespace onchipcommunication {

using namespace std;
using namespace cosi;

InkscapeCodeGenerator::InkscapeCodeGenerator(string pName, double W, double H) {

	mF = new ofstream;
	mF->open(pName.c_str() ) ;

	mW = W ; // To micron.
	mH = H ; // To micron.


	mLineWidth = mW / 300.0;
	mDotSize = mLineWidth*2;

	mCurrentLayer = 1;

	SvgOpen() ;

}

InkscapeCodeGenerator::~InkscapeCodeGenerator() {
	//if (mF ) {
	//	mF->close() ;
	//	delete mF;
	//}
}

void InkscapeCodeGenerator::SvgOpen() {

	stringstream RetVal;

	RetVal << "<?xml version=\"1.0\" standalone=\"no\"?> \n";
	RetVal << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \n";
	RetVal << "\"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\"> \n";
	RetVal << "<svg "<< endl ;
	RetVal << "xmlns:dc=\"http://purl.org/dc/elements/1.1/\""<< endl ;
	RetVal << "xmlns:cc=\"http://web.resource.org/cc/\""<< endl ;
	RetVal << "xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\""
			<< endl ;
	RetVal << "xmlns:svg=\"http://www.w3.org/2000/svg\""<< endl ;
	RetVal << "xmlns=\"http://www.w3.org/2000/svg\""<< endl ;
	RetVal
			<< "xmlns:sodipodi=\"http://sodipodi.sourceforge.net/DTD/sodipodi-0.dtd\""
			<< endl ;
	RetVal << "xmlns:inkscape=\"http://www.inkscape.org/namespaces/inkscape\""
			<< endl ;
	RetVal << "sodipodi:version=\"0.32\""<< endl ;
	RetVal << "inkscape:version=\"0.44.1\""<< endl ;
	RetVal << "width=\"600px\" height=\"600px\" viewBox=\"0 0 "<< mW*1.1<< " "
			<< mH*1.1<<"\" version=\"1.0\"> \n";
	RetVal << "\t <defs> \n";
	RetVal << "\t \t <marker \n";
	RetVal << "\t \t \t orient=\"auto\" \n";
	RetVal << "\t \t \t refY=\"0.0\" refX=\"0.0\" \n";
	RetVal << "\t \t \t id=\"Arrow2Lend\" style=\"overflow:visible;\"> \n";
	RetVal << "\t \t \t<path \n";
	RetVal << "\t \t \t \t id=\"path16476\" \n";
	RetVal
			<< "\t \t \t \t style=\"font-size:12.0;fill-rule:evenodd;stroke-width:0.62500000;stroke-linejoin:round;\" \n";
	RetVal
			<< "\t \t \t \t d=\"M 8.7185878,4.0337352 L -2.2072895,0.016013256 L 8.7185884,-4.0017078 C 6.9730900,-1.6296469 6.9831476,1.6157441 8.7185878,4.0337352 z \" \n";
	RetVal
			<< "\t \t \t \t transform=\"scale(0.4) rotate(180) translate(-5,0)\" /> \n";
	RetVal << "\t \t </marker> \n";
	RetVal << "\t </defs> \n";
	(*mF) << RetVal.str() ;

}

void InkscapeCodeGenerator::AddForbidden(PlacementParser* pPl) {

	//open the forbbidden layer
	(*mF) << "<g inkscape:groupmode=\"layer\" id=\"layer"<< mCurrentLayer
			<< "\" inkscape:label=\"IP Cores\" style=\"display:inline\">"
			<< endl ;

	vector< double > BottomLeftX;
	vector< double > TopRightX;
	vector< double > BottomLeftY;
	vector< double > TopRightY;
	vector<string> Name;

	BottomLeftX = pPl->GetBottomLeftX() ;
	TopRightX = pPl->GetTopRightX() ;
	BottomLeftY = pPl->GetBottomLeftY();
	TopRightY = pPl->GetTopRightY();
	Name = pPl->GetName() ;

	for (unsigned int i = 0; i < BottomLeftX.size() ; i++) {

		(*mF) << SvgBox(BottomLeftX[i]*1e6, BottomLeftY[i]*1e6, (TopRightX[i]
				- BottomLeftX[i])*1e6, (TopRightY[i] - BottomLeftY[i])*1e6,
				Name[i]) ;
	}

	(*mF) << "</g>"<< endl ;
	mCurrentLayer++;
}

//void InkscapeCodeGenerator::AddAllowed(NocPlatformRefined* pPlt) {
//
//	(*mF) << "<g inkscape:groupmode=\"layer\" id=\"layer"<< mCurrentLayer
//			<< "\" inkscape:label=\"Allowed\" style=\"opacity:1;display:none\">"
//			<< endl ;
//
//	int V;
//	for (int i = 0; i < pPlt->Nr() ; i++) {
//		V = pPlt->Ns() + pPlt->Nd() + i ;
//		if (pPlt->IsAllowed(V ) ) {
//			(*mF) << SvgCross(pPlt->GetX(V )*1e6, pPlt->GetY(V )*1e6) ;
//		}
//	}
//	(*mF) << "</g>"<< endl ;
//	mCurrentLayer++;
//
//}

//void InkscapeCodeGenerator::AddReachable(int V, NocPlatformRefined* pPlt) {
//
//	(*mF) << "<g inkscape:groupmode=\"layer\" id=\"layer"<< mCurrentLayer
//			<< "\" inkscape:label=\"Reachable("<< V
//			<< ")\" style=\"opacity:1;display:none\">"<< endl ;
//
//	list< int > N;
//	list< int >::iterator U;
//	N = pPlt->Neighbors(V , 0) ;
//
//	for (U = N.begin() ; U != N.end() ; U++) {
//		(*mF) << SvgCross(pPlt->GetX( *U )*1e6, pPlt->GetY( *U )*1e6) ;
//	}
//
//	(*mF) << "</g>"<< endl ;
//	mCurrentLayer++;
//
//}

void InkscapeCodeGenerator::PlotCommStruct(
		cosi::commstruct::onchipcommunication::Specification* pC) {

	stringstream s;
	(*mF) << "<g inkscape:groupmode=\"layer\" id=\"layer"<< mCurrentLayer
			<< "\" inkscape:label=\"CommStruct("<< pC->GetName()
			<< ")\" style=\"opacity:1;display:none\">"<< endl ;

	IdGraph::v_iterator U, V;

	for (U = pC->v_begin() ; U != pC->v_end() ; U++) {
		(*mF) << SvgDot((*pC)[*U].PRJ(Position)GetX() * 1e6, (*pC)[*U].PRJ(Position)GetY() *1e6) ;
		for (V = pC->v_begin() ; V != pC->v_end() ; V++) {
			if (pC->InE( *U , *V ) ) {
				(*mF) << SvgArrow((*pC)[*U].PRJ(Position)GetX() *1e6, (*pC)[*U].PRJ(Position)GetY() *1e6, (*pC)[*V].PRJ(Position)GetX() *1e6, (*pC)[*V].PRJ(Position)GetY() *1e6, mLineWidth ) ;
			}
		}
	}
	(*mF) << "</g>"<< endl ;
	mCurrentLayer++;

}

void InkscapeCodeGenerator::PlotCommStruct(
		cosi::commstruct::onchipcommunication::Implementation* pC) {
	stringstream s;
	(*mF) << "<g inkscape:groupmode=\"layer\" id=\"layer"<< mCurrentLayer
			<< "\" inkscape:label=\"CommStruct("<< pC->GetName()
			<< ")\" style=\"opacity:1;display:inline\">"<< endl ;

	IdGraph::v_iterator U, V;

	for (U = pC->v_begin() ; U != pC->v_end() ; U++) {

		if ((*pC)[*U].PRJ(Type)GetValue() == "Router") {
			double Area = (*pC)[*U].PRJ(IpGeometry)GetArea().GetValue() ;
			double Lsize = sqrt(Area*1e12) ;
			(*mF) << SvgRouter( (*pC)[*U].PRJ(Position)GetX() *1e6, (*pC)[*U].PRJ(Position)GetY() *1e6, Lsize , Lsize ,"" ) ;
		} else {
			(*mF) << SvgDot((*pC)[*U].PRJ(Position)GetX() * 1e6, (*pC)[*U].PRJ(Position)GetY() *1e6) ;
		}
		for (V = pC->v_begin() ; V != pC->v_end() ; V++) {
			if (pC->InE( *U , *V ) ) {
				(*mF) << SvgArrow((*pC)[*U].PRJ(Position)GetX() *1e6, (*pC)[*U].PRJ(Position)GetY() *1e6, (*pC)[*V].PRJ(Position)GetX() *1e6, (*pC)[*V].PRJ(Position)GetY() *1e6, mLineWidth ) ;
			}
		}
	}
	(*mF) << "</g>"<< endl ;
	mCurrentLayer++;
}

//void InkscapeCodeGenerator::PlotIdGraph(Xy__IdGraph* pG) {
//
//	stringstream s;
//	IdGraph::v_iterator Sit, Dit, Nit, Uit, Vit;
//
//	for (Sit = pG->s_begin() ; Sit != pG->s_end() ; Sit++) {
//		(*mF) << SvgSource(pG->GetX( *Sit ) *1e6, pG->GetY( *Sit ) *1e6, "S") ;
//	}
//
//	for (Dit = pG->d_begin() ; Dit != pG->d_end() ; Dit++) {
//		s << (*mF)<< SvgDest(pG->GetX( *Dit ) *1e6, pG->GetY( *Dit ) *1e6, "D") ;
//	}
//
//	for (Nit = pG->r_begin() ; Nit != pG->r_end() ; Nit++) {
//		(*mF) << SvgDot(pG->GetX( *Nit ) *1e6, pG->GetY( *Nit ) *1e6) ;
//	}
//
//	for (Uit = pG->v_begin() ; Uit != pG->v_end() ; Uit++) {
//		for (Vit = pG->v_begin() ; Vit != pG->v_end() ; Vit++) {
//			if (pG->InE( *Uit , *Vit ) ) {
//				(*mF) << SvgArrow(pG->GetX(*Uit) *1e6, pG->GetY(*Uit) *1e6,
//						pG->GetX(*Vit) *1e6, pG->GetY(*Vit)*1e6, mLineWidth ) ;
//			}
//		}
//	}
//
//}

//void InkscapeCodeGenerator::PlotGraph(XyRouting_Flow_Graph* G,
//		NocPlatformRefined* pPlt) {
//
//	(*mF) << "<g inkscape:groupmode=\"layer\" id=\"layer"<< mCurrentLayer
//			<< "\" inkscape:label=\"Sources\" style=\"display:inline\">"<< endl ;
//
//	for (int i = 0; i < G->Ns() ; i++) {
//		(*mF) << SvgSource(G->GetX(i ) *1e6, G->GetY(i ) *1e6, "S") ;
//	}
//
//	(*mF) << "</g>"<< endl ;
//	mCurrentLayer++;
//
//	(*mF) << "<g inkscape:groupmode=\"layer\" id=\"layer"<< mCurrentLayer
//			<< "\" inkscape:label=\"Destinations\" style=\"display:inline\">"
//			<< endl ;
//	for (int i = G->Ns() ; i < G->Ns() + G->Nd() ; i++) {
//		(*mF) << SvgDest(G->GetX(i ) *1e6, G->GetY(i ) *1e6, "D") ;
//	}
//
//	(*mF) << "</g>"<< endl ;
//	mCurrentLayer++;
//
//	(*mF) << "<g inkscape:groupmode=\"layer\" id=\"layer"<< mCurrentLayer
//			<< "\" inkscape:label=\"Routers\" style=\"display:inline\">"<< endl ;
//
//	for (int i = G->Ns() + G->Nd(); i < G->Order() ; i++) {
//		if (G->GetType(i ) ) {
//			stringstream rname;
//			rname << "R"<< i ;
//			double RouterSize;
//			RouterSize = sqrt(pPlt->RouterArea(G->InDegree(i ) ,
//					G->OutDegree(i ) ) *1e12) ;
//			(*mF) << SvgRouter(G->GetX(i ) *1e6- RouterSize/2, G->GetY(i ) *1e6
//					- RouterSize/2, RouterSize , RouterSize , rname.str() ) ;
//		} else {
//			stringstream lname;
//			lname << "L"<< i ;
//			double RouterSize;
//			RouterSize = sqrt(pPlt->RouterArea(G->InDegree(i ) ,
//					G->OutDegree(i ) ) * 1e12) ;
//			(*mF) << SvgLatch(G->GetX(i ) *1e6- RouterSize/2, G->GetY(i ) *1e6
//					- RouterSize/2, RouterSize , RouterSize , lname.str() ) ;
//
//		}
//	}
//
//	(*mF) << "</g>"<< endl ;
//	mCurrentLayer++;
//
//	(*mF) << "<g inkscape:groupmode=\"layer\" id=\"layer"<< mCurrentLayer
//			<< "\" inkscape:label=\"Edges\" style=\"display:inline\">"<< endl ;
//
//	for (int U = 0; U < G->Order() ; U++) {
//		for (int V = 0; V < G->Order() ; V++) {
//			if (G->IsEdge(U , V ) ) {
//				(*mF) << SvgArrow(G->GetX(U) *1e6, G->GetY(U) *1e6, G->GetX(V)
//						*1e6, G->GetY(V)*1e6, pPlt->WirePitch()
//						*pPlt->FlitWidth()*1e6) ;
//			}
//		}
//	}
//
//	(*mF) << "</g>"<< endl ;
//	mCurrentLayer++;
//
//}
//
//void InkscapeCodeGenerator::PlotPaths(Xy_Flow_Graph* pPtP,
//		map<pair<int,int> , int> pOrder, XyRouting_Flow_Graph* G,
//		NocPlatformRefined* pPlt) {
//
//	int CurrentV, NextV, OutPort;
//	map< pair<int,int> ,int> RoutingTable;
//	map< pair<int,int> , int >::iterator Entry;
//	int Nc;
//	Nc = 0;
//	for (int S = 0; S < pPtP->Ns() ; S++) {
//		//forall sources
//		for (int D = pPtP->Ns() ; D < pPtP->Ns() + pPtP->Nd() ; D++) {
//			if (pPtP->IsEdge(S , D ) ) {
//				//Draw path
//				(*mF) << "<g inkscape:groupmode=\"layer\" id=\"layer"
//						<< mCurrentLayer << "\" inkscape:label=\"("
//						<< pPtP->GetName(S ) << ","<< pPtP->GetName(D ) << ") "
//						<< "\" style=\"opacity:1;display:none\">"<< endl ;
//				//verify that there is a path to destination
//				Nc++;
//				CurrentV = S ;
//				//Get routing table
//				while (CurrentV != D ) {
//					if (CurrentV == S )
//						(*mF) << SvgSource(G->GetX(CurrentV ) *1e6,
//								G->GetY(CurrentV ) *1e6, "S") ;
//					else {
//						if (G->GetType(CurrentV ) ) {
//							stringstream rname;
//							rname << "R"<< CurrentV ;
//							double RouterSize;
//							RouterSize = sqrt(pPlt->RouterArea(
//									G->InDegree(CurrentV ) ,
//									G->OutDegree(CurrentV ) ) *1e12) ;
//							(*mF) << SvgRouter(G->GetX(CurrentV ) *1e6
//									- RouterSize/2, G->GetY(CurrentV ) *1e6
//									- RouterSize/2, RouterSize , RouterSize ,
//									rname.str() ) ;
//						} else {
//							stringstream lname;
//							lname << "L"<< CurrentV ;
//							double RouterSize;
//							RouterSize = sqrt(pPlt->RouterArea(
//									G->InDegree(CurrentV ) ,
//									G->OutDegree(CurrentV ) ) * 1e12) ;
//							(*mF) << SvgLatch(G->GetX(CurrentV ) *1e6
//									- RouterSize/2, G->GetY(CurrentV ) *1e6
//									- RouterSize/2, RouterSize , RouterSize ,
//									lname.str() ) ;
//
//						}
//					}
//					RoutingTable = G->GetRoutingTable(CurrentV) ;
//					Entry = RoutingTable.find(pair<int, int>(S, D) );
//					OutPort = Entry->second;
//					//from all edges leaving CurrentV, find the one with that outport
//					int OutV;
//					for (OutV = 0; OutV < G->Order() ; OutV++) {
//						if (G->IsEdge(CurrentV , OutV ) ) {
//							if (G->GetOutputPort(CurrentV, OutV) == OutPort ) {
//								break;
//							}
//						}
//					}
//					(*mF) << SvgArrow(G->GetX(CurrentV) *1e6, G->GetY(CurrentV)
//							*1e6, G->GetX(OutV) *1e6, G->GetY(OutV)*1e6,
//							pPlt->WirePitch()*pPlt->FlitWidth()*1e6) ;
//					CurrentV = OutV ;
//				}
//				//destination
//				(*mF) << SvgDest(G->GetX(D ) *1e6, G->GetY(D ) *1e6, "D") ;
//				//close
//				(*mF) << "</g>"<< endl ;
//				mCurrentLayer++;
//			}
//
//		}
//	}
//
//}
//
//void InkscapeCodeGenerator::AddAssignment(map<int,int> A,
//		NocPlatformRefined* Plt) {
//
//	(*mF) << "<g inkscape:groupmode=\"layer\" id=\"layer"<< mCurrentLayer
//			<< "\" inkscape:label=\"Assignment\" style=\"display:inline\">"
//			<< endl ;
//
//	map<int,int>::iterator It;
//	for (It = A.begin() ; It != A.end() ; It++) {
//		(*mF) << SvgSource(Plt->GetX(It->first) *1e6, Plt->GetY(It->first)*1e6,
//				"S") ;
//		double RouterSize;
//		RouterSize = sqrt(Plt->RouterArea( 5, 5) ) ;
//		(*mF) << SvgRouter(Plt->GetX(It->second) *1e6- RouterSize/2,
//				Plt->GetY(It->second) *1e6- RouterSize/2, RouterSize ,
//				RouterSize , "") ;
//		(*mF) << SvgArrow(Plt->GetX(It->first) *1e6, Plt->GetY(It->first) *1e6,
//								Plt->GetX(It->second) *1e6, Plt->GetY(It->second)*1e6, mLineWidth ) ;
//	}
//	(*mF) << "</g>"<< endl ;
//	mCurrentLayer++;
//
//}

void InkscapeCodeGenerator::Close() {
	(*mF) << "</svg>"<< endl;
	mF->close() ;
	delete mF;
}

string InkscapeCodeGenerator::SvgBox(double x0, double y0, double x1,
		double y1, string Name) {
	stringstream s;
	s << "<rect x=\""<< x0 <<"\" y=\""<< y0 << "\" width=\""<< x1
			<< "\" height=\""<< y1 << "\" stroke=\"black\" stroke-width=\""
			<< mLineWidth << "\" fill=\"none\"/> \n";
	s << "<text x=\""<< x0 + x1/2<< "\" y =\""<< y0 + y1/2<< "\" font-size=\""
			<< mW/40<< "\">"<< Name << "</text>"<< endl ;
	return s.str();
}

string InkscapeCodeGenerator::SvgDot(double x0, double y0) {
	stringstream s;
	s << "<circle cx=\""<< x0 << "\" cy=\""<< y0 << "\" r=\""<< mDotSize
			<< "\" stroke=\"black\" stroke-width=\"1\" fill=\"red\"/>"<< endl ;
	return s.str() ;
}

string InkscapeCodeGenerator::SvgCross(double x0, double y0) {
	stringstream s;
	s << "<path d=\"M "<< x0 - mDotSize << " "<< y0 << " L "<< x0 + mDotSize
			<< " "<< y0 << "\" fill=\"none\" stroke=\"black\" stroke-width=\""
			<< mLineWidth << "\"  /> "<< endl ;
	s << "<path d=\"M "<< x0 << " "<< y0 - mDotSize << " L "<< x0 << " "<< y0
			+ mDotSize << "\" fill=\"none\" stroke=\"black\" stroke-width=\""
			<< mLineWidth << "\"  /> "<< endl ;
	return s.str() ;
}

string InkscapeCodeGenerator::SvgArrow(double x0, double y0, double x1,
		double y1, double width) {
	stringstream s;
	s << "<path "<< "style=\"marker-end:url(#Arrow2Lend)\""<< " d=\"M "<< x0
			<< " "<< y0 << " L "<< x1 << " "<< y1
			<< "\" fill=\"none\" stroke=\"black\" stroke-width=\""<< width
			<< "\"  /> "<< endl ;
	return s.str() ;
}

string InkscapeCodeGenerator::SvgSource(double x0, double y0, string name) {
	stringstream s;
	s << "<circle cx=\""<< x0 << "\" cy=\""<< y0 << "\" r=\""<< mDotSize
			<< "\" stroke=\"black\" stroke-width=\"1\" fill=\"red\"/>"<< endl ;
	//s << "<text x=\"" << x0 - 100 << "\" y=\"" << y0 + 100 << "\"> " << name << " </text>"<< endl ;
	return s.str() ;
}
string InkscapeCodeGenerator::SvgDest(double x0, double y0, string name) {
	stringstream s;
	s << "<circle cx=\""<< x0 << "\" cy=\""<< y0 << "\" r=\""<< mDotSize
			<< "\" stroke=\"black\" stroke-width=\"1\" fill=\"black\"/>"<< endl ;
	//s << "<text x=\"" << x0 - 100 << "\" y=\"" << y0 + 100 << "\"> " << name << " </text>"<< endl ;
	return s.str() ;
}

string InkscapeCodeGenerator::SvgRouter(double x0, double y0, double w,
		double h, string name) {
	stringstream s;
	s << "<rect x=\""<< x0 << "\" y=\""<< y0 << "\" width=\""<< w
			<< "\" height=\""<< h
			<< "\" stroke=\"black\" stroke-width=\"1\" fill=\"green\"/>"<< endl ;
	s << "<text x=\""<< x0 + w << "\" y=\""<< y0 + h << "\" font-size=\""<< mW
			/40<< "\">"<< name << "</text>"<< endl ;
	return s.str() ;
}

string InkscapeCodeGenerator::SvgLatch(double x0, double y0, double w,
		double h, string name) {
	stringstream s;
	s << "<rect x=\""<< x0 << "\" y=\""<< y0 << "\" width=\""<< w
			<< "\" height=\""<< h
			<< "\" stroke=\"black\" stroke-width=\"1\" fill=\"orange\"/>"
			<< endl ;
	s << "<text x=\""<< x0 + w << "\" y=\""<< y0 + h << "\" font-size=\""<< mW
			/40<< "\">"<< name << "</text>"<< endl ;
	return s.str() ;
}
}
}
}
