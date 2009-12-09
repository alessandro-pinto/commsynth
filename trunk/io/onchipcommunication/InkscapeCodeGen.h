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

#ifndef _INKSCAPE_CODE_GEN_H_
#define _INKSCAPE_CODE_GEN_H_

#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include "math.h"
#include "include/onchipcommunication/cosi.h"

namespace cosi {

namespace io {

namespace onchipcommunication {

using namespace std ;


class InkscapeCodeGenerator {

 public:

  /**brief Class constructor.
   *
   * The constructor takes the file and the chip size in micron.
   * It simply opens the file.
   */
  InkscapeCodeGenerator( string pName , double W , double H ) ;

  /**\brief Class destructor.
   *
   * The destructor closes the file and deletes the file pointer.
   */
  ~InkscapeCodeGenerator( );

  /**\brief Adds the forbidden blocks to the svg file.
   *
   * This method add a set of rectangular boxes to the svg file.
   * Each box represents an IP core.
   */
  void AddForbidden( PlacementParser* pPl) ;

  /**\brief Adds the set of installation sites to the svg file.
   *
   * This method ass a dot in the svg file for each admissible installation
   * site. The user should be aware of the fact that the number of installation
   * sites can be big and could cause a long rendering time for standard
   * browser like Firefox. Sgv viewers should not be affected by
   * the length of the sgv file.
   */
  //void AddAllowed( NocPlatformRefined* pPlt ) ;

  /**\brief Add a graph to the svg file.
   *
   * This method adds a graph to the svg file. Each source is
   * represented by red dot and each destination by a blue dot. If
   * a source and a destination have the same position, this method
   * adds a white dot in that position. Routers additional nodes are
   * represented by gree dots and edges by arrows.
   */
  void PlotCommStruct( cosi::commstruct::onchipcommunication::Specification* pC ) ;
  void PlotCommStruct( cosi::commstruct::onchipcommunication::Implementation* pC ) ;

  /**\brief Add a graph with routing information to the svg file.
   *
   * This method adds a graph to the svg file. Each source is
   * represented by red dot and each destination by a blue dot. If
   * a source and a destination have the same position, this method
   * adds a white dot in that position. Routers are represented by
   * dark gree rectangles and latches by orange rectangles.
   */
  //void PlotGraph( XyRouting_Flow_Graph* pG , NocPlatformRefined* pPlt ) ;

  //void PlotPaths( Xy_Flow_Graph* pPtP , map<pair<int,int> , int> pOrder , XyRouting_Flow_Graph* pG , NocPlatformRefined* pPlt ) ;

  //void AddReachable( int V  , NocPlatformRefined* pPlt ) ;

  //void AddAssignment(map<int,int> A , NocPlatformRefined* Plt) ;

  /**\brief Adds the tails of svg file, closes the file and deletes the file pointer.*/
  void Close( ) ;

 private:

  void SvgOpen( ) ;

  string SvgBox( double x0, double y0 , double x1 , double y1 , string Name ) ;

  string SvgDot( double x0 , double y0 ) ;

  string SvgCross( double x0 , double y0 ) ;

  string SvgArrow( double x0, double y0 , double x1 , double y1 , double width ) ;

  string SvgSource( double x0 , double y0 , string name );

  string SvgDest( double x0 , double y0 , string name) ;

  string SvgRouter( double x0 , double y0 , double w , double h , string name) ;

  string SvgLatch( double x0 , double y0 , double w , double h , string name) ;


  ofstream* mF ;

  double mW ;
  double mH ;
  double mLineWidth ;
  double mDotSize ;

  int mCurrentLayer ;


} ;

}
}
}

#endif
