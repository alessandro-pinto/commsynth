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
#include "InkScapeCodeGen.h"

namespace cosi {

namespace io {

namespace bas {

InkScapeCodeGen::InkScapeCodeGen(std::string pFname, double pHeight,
		double pWidth) {
	mF = new std::ofstream;
	mF->open(pFname.c_str() ) ;
	mWidth = pWidth*100; //to centimeter
	mHeight = pHeight*100; //to centimeter
	mLineWidth = 10;
	mDotSize = 30;
	mLayer = 1;
	SvgOpen() ;

}

InkScapeCodeGen::~InkScapeCodeGen() {

}

void InkScapeCodeGen::SvgOpen() {
	std::stringstream RetVal;

	RetVal << "<?xml version=\"1.0\" standalone=\"no\"?> \n";
	RetVal << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \n";
	RetVal << "\"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\"> \n";
	RetVal << "<svg "<< std::endl ;
	RetVal << "xmlns:dc=\"http://purl.org/dc/elements/1.1/\""<< std::endl ;
	RetVal << "xmlns:cc=\"http://web.resource.org/cc/\""<< std::endl ;
	RetVal << "xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\""
			<< std::endl ;
	RetVal << "xmlns:svg=\"http://www.w3.org/2000/svg\""<< std::endl ;
	RetVal << "xmlns=\"http://www.w3.org/2000/svg\""<< std::endl ;
	RetVal
			<< "xmlns:sodipodi=\"http://sodipodi.sourceforge.net/DTD/sodipodi-0.dtd\""
			<< std::endl ;
	RetVal << "xmlns:inkscape=\"http://www.inkscape.org/namespaces/inkscape\""
			<< std::endl ;
	RetVal << "xmlns:cosi=\"http://embedded.eecs.berkeley.edu/cosi\""
			<< std::endl ;
	RetVal << "sodipodi:version=\"0.32\""<< std::endl ;
	RetVal << "inkscape:version=\"0.44.1\""<< std::endl ;
	RetVal << "width=\"600px\" height=\"600px\" viewBox=\"0 0 "<< mWidth*1.1
			<< " "<< mHeight*1.1<<"\" version=\"1.0\"> \n";
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

void InkScapeCodeGen::NewLayer(int pLayer, std::string pName) {
	mLayer = pLayer ;
	mLayerName = pName ;
	(*mF) << "<g inkscape:groupmode=\"layer\" id=\"layer"<< pLayer
			<< "\" inkscape:label=\""<< pName
			<< "\" style=\"opacity:1;display:none\">"<< std::endl ;

}

void InkScapeCodeGen::CloseLayer() {
	(*mF) << "</g>"<< std::endl ;
}

void InkScapeCodeGen::Add(cosi::environment::bas::Wall pW) {
	//project the wall on x and y

	//Add a box from the vertex closer to the origin to the one that is farthest
	cosi::Position V1, V2, V3, V4;

	V1 = pW.GetV1() ;
	V2 = pW.GetV2() ;
	V3 = pW.GetV3() ;
	V4 = pW.GetV4() ;

	//compute normal vector
	double Dx, Dy;
	Dx = (V3.GetX() - V1.GetX() ) / Norm2(V1 , V3 ) ;
	Dy = (V3.GetY() - V1.GetY()) / Norm2(V1 , V3 ) ;
	//compute the orthogonal vector
	double Dxo, Dyo;
	Dxo = Dyo = 0;
	if (Dx == 0) {
		Dx = -1;
		Dy = 0;
	} else if (Dy == 0) {
		Dy = 1;
		Dx = 0;
	} else {
		Dx = -1/Dx ;
		Dy = 1/Dy ;
	}
	//Normalize the vector
	double Norm;
	Norm = sqrt(pow(Dx, 2) + pow(Dy, 2) ) ;
	Dx = Dx / Norm ;
	Dy = Dy / Norm ;

	(*mF) << SvgLine( (V1.GetX() - Dx*pW.Thickness/2.0)*100, (V1.GetY() - Dy
			*pW.Thickness/2.0)*100, (V3.GetX() - Dx*pW.Thickness/2.0)*100,
			(V3.GetY() - Dy*pW.Thickness/2.0)*100, "") ;
	(*mF) << SvgLine( (V3.GetX() - Dx*pW.Thickness/2.0)*100, (V3.GetY() - Dy
			*pW.Thickness/2.0)*100, (V3.GetX() + Dx*pW.Thickness/2.0)*100,
			(V3.GetY() + Dy*pW.Thickness/2.0)*100, "") ;
	(*mF) << SvgLine( (V3.GetX() + Dx*pW.Thickness/2.0)*100, (V3.GetY() + Dy
			*pW.Thickness/2.0)*100, (V1.GetX() + Dx*pW.Thickness/2.0)*100,
			(V1.GetY() + Dy*pW.Thickness/2.0)*100, "") ;
	(*mF) << SvgLine( (V1.GetX() + Dx*pW.Thickness/2.0)*100, (V1.GetY() + Dy
			*pW.Thickness/2.0)*100, (V1.GetX() - Dx*pW.Thickness/2.0)*100,
			(V1.GetY() - Dy*pW.Thickness/2.0)*100, "") ;

}

void InkScapeCodeGen::Add(cosi::environment::bas::InstSurface pIs) {
	//Add a box from the vertex closer to the origin to the one that is farthest
	cosi::Position V1, V2, V3, V4;

	V1 = pIs.GetV1() ;
	V2 = pIs.GetV2() ;
	V3 = pIs.GetV3() ;
	V4 = pIs.GetV4() ;

	(*mF)<< SvgLine(V1.GetX()*100, V1.GetY()*100, V2.GetX()*100, V2.GetY()*100,
			"") ;
	(*mF)<< SvgLine(V2.GetX()*100, V2.GetY()*100, V3.GetX()*100, V3.GetY()*100,
			"") ;
	(*mF)<< SvgLine(V3.GetX()*100, V3.GetY()*100, V4.GetX()*100, V4.GetY()*100,
			"") ;
	(*mF)<< SvgLine(V4.GetX()*100, V4.GetY()*100, V1.GetX()*100, V1.GetY()*100,
			"") ;
}

void InkScapeCodeGen::Add(std::list< Position > pR) {
	std::list< Position >::iterator Rit;
	for (Rit = pR.begin() ; Rit != pR.end() ; Rit++) {
		(*mF) << SvgDot((*Rit).GetX() * 100, (*Rit).GetY() * 100, "") ;
	}
}

void InkScapeCodeGen::Add(cosi::commstruct::wiredbas::Specification* pN) {
	IdGraph::v_iterator U, V;
	for (U = pN->v_begin() ; U != pN->v_end() ; U ++) {
		//get the label
		std::string Nl = (*pN)[*U].Print();
		double Xu, Yu;
		Xu = (*pN)[*U].PRJ(Position)GetX() ;
		Yu = (*pN)[*U].PRJ(Position)GetY() ;
		if (pN->InDegree( *U ) == 0) { //source
			(*mF) << SvgDot("green", Xu * 100, Yu * 100, Nl) ;
		} else {
			(*mF) << SvgDot("red", Xu * 100, Yu * 100, Nl) ;
		}
		for (V = pN->v_begin() ; V != pN->v_end() ; V ++) {
			if (pN->InE( *U , *V ) ) {
				std::string El = (*pN)[Edge(*U,*V)].Print();
				double Xv, Yv;
				Xv = (*pN)[*V].PRJ(Position)GetX() ;
				Yv = (*pN)[*V].PRJ(Position)GetY() ;
				(*mF) << SvgLine(Xu*100, Yu*100, Xv*100, Yv*100, El) ;
			}
		}
	}

}

void InkScapeCodeGen::Add(cosi::commstruct::wiredbas::Implementation* pN) {
	IdGraph::v_iterator U, V;
	for (U = pN->v_begin() ; U != pN->v_end() ; U ++) {
		//get the label
		std::string Nl = (*pN)[*U].Print();
		double Xu, Yu;
		Xu = (*pN)[*U].PRJ(Position)GetX() ;
		Yu = (*pN)[*U].PRJ(Position)GetY() ;
		if (pN->InDegree( *U ) == 0) { //source
			(*mF) << SvgDot("green", Xu * 100, Yu * 100, Nl) ;
		} else {
			(*mF) << SvgDot("red", Xu * 100, Yu * 100, Nl) ;
		}
		for (V = pN->v_begin() ; V != pN->v_end() ; V ++) {
			if (pN->InE( *U , *V ) ) {
				(*mF) << SvgPath( (*pN)[Edge(*U,*V)].PRJ(WiringPath)GetValue(),"") ;
			}
		}
	}
}

std::string InkScapeCodeGen::SvgBox(double x0, double y0, double x1, double y1,
		std::string Label) {
	std::stringstream s;
	s << "<rect x=\""<< x0 <<"\" y=\""<< y0 << "\" width=\""<< x1
			<< "\" height=\""<< y1 << "\" stroke=\"black\" stroke-width=\""
			<< mLineWidth << "\" fill=\"none\"/> \n";

	return s.str();
}

string InkScapeCodeGen::SvgBox(std::string Color, double x0, double y0,
		double x1, double y1, std::string Label) {
	std::stringstream s;
	s << "<rect x=\""<< x0 <<"\" y=\""<< y0 << "\" width=\""<< x1
			<< "\" height=\""<< y1 << "\" stroke=\"none\" stroke-width=\""
			<< mLineWidth << "\" fill=\""<< Color << "\"/> \n";
	return s.str();

}

string InkScapeCodeGen::SvgLine(double x0, double y0, double x1, double y1,
		std::string Label) {
	std::stringstream s;
	s << "<path  d=\"M "<< x0 << " "<< y0 << " L "<< x1 << " "<< y1
			<< "\" fill=\"none\" stroke=\"black\" stroke-width=\""<< mLineWidth
			<< "\" cosi:label=\""<< Label << "\"/> "<< std::endl ;
	return s.str() ;

}

std::string InkScapeCodeGen::SvgPath(std::list<Position> pPath, std::string Label) {
	std::list<Position>::iterator It;
	std::stringstream s;
	It = pPath.begin() ;
	s << "<path  d=\"M "<< It->GetX() * 100  << " "<< It->GetY() * 100 ;
	It++;
	for (; It != pPath.end() ; It++) {
		s << " L "<< It->GetX()* 100 << " "<< It->GetY()* 100 ;
	}
	s << "\" fill=\"none\" stroke=\"black\" stroke-width=\""<< mLineWidth
			<< "\" cosi:label=\""<< Label << "\"/> "<< std::endl ;
	return s.str();
}

void InkScapeCodeGen::Close() {
	(*mF) << "</svg>"<< std::endl;
	mF->close() ;
	delete mF;

}

string InkScapeCodeGen::SvgDot(double x0, double y0, std::string Label) {
	stringstream s;
	s << "<circle cx=\""<< x0 << "\" cy=\""<< y0 << "\" r=\""<< mDotSize
			<< "\" stroke=\"black\" stroke-width=\"1\" fill=\"red\" cosi:label=\""
			<< Label << "\"/>"<< std::endl ;
	return s.str() ;

}

string InkScapeCodeGen::SvgDot(std::string Color, double x0, double y0,
		std::string Label) {
	stringstream s;
	s << "<circle cx=\""<< x0 << "\" cy=\""<< y0 << "\" r=\""<< mDotSize
			<< "\" stroke=\"none\" stroke-width=\"0\" fill=\""<< Color
			<< "\" cosi:label=\""<< Label << "\" />"<< std::endl ;
	return s.str() ;

}

string InkScapeCodeGen::SvgText(std::string Txt, double X0, double Y0) {
	stringstream s;
	s << "<text x=\""<< X0 << "\" y =\""<< Y0 << "\" font-size=\""<< 70<< "\">"
			<< Txt << "</text>"<< std::endl ;
	return s.str() ;
}

}
}
}

