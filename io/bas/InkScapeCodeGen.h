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
#ifndef _IO_BAS_INKSCAPECODEGEN_H_
#define _IO_BAS_INKSCAPECODEGEN_H_

//
// C++ Interface: InkScapeCodeGen
//
// Description:
//
//
// Author: Alessandro Pinto <apinto@eecs.berkeley.edu>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <list>
#include "quantities/Position.h"
#include "environment/bas/Wall.h"
#include "environment/bas/InstSurface.h"
#include "math.h"
#include "networks/wiredbas/WiredBasImplementation.h"
#include "networks/wiredbas/WiredBasSpecificaion.h"

namespace cosi {

namespace io {

namespace bas {

class InkScapeCodeGen {

public:

	InkScapeCodeGen(std::string pFname, double pHeight, double pWidth);

	~InkScapeCodeGen();

	void NewLayer(int pLayer, std::string pName);

	void CloseLayer();

	void Add(cosi::environment::bas::Wall pW);

	void Add(cosi::environment::bas::InstSurface pIs);

	void Add(std::list< cosi::Position > pR);

	void Add(cosi::commstruct::wiredbas::Specification* pN);

	void Add(cosi::commstruct::wiredbas::Implementation* pN);

	void Close();

private:

	void SvgOpen();

	std::string SvgBox(double x0, double y0, double x1, double y1,std::string Label);

	std::string SvgBox(std::string Color, double x0, double y0, double x1, double y1,std::string Label);

	std::string SvgLine(double x0, double y0, double x1, double y1,std::string Label);

	std::string SvgPath(std::list<Position> pPath, std::string Label);

	std::string SvgDot(double x0, double y0,std::string Label);

	std::string SvgDot(std::string Color, double x0, double y0,std::string Label);

	std::string SvgText(std::string Txt, double X0, double Y0);


	//string SvgCross( double x0 , double y0 ) ;

	//string SvgArrow( double x0, double y0 , double x1 , double y1 , double width ) ;

	//string SvgSource( double x0 , double y0 , string name );

	//string SvgDest( double x0 , double y0 , string name) ;

	int mLayer;
	string mLayerName;

	ofstream* mF;

	double mWidth;
	double mHeight;
	double mLineWidth;
	double mDotSize;

};

}
}
}

#endif
