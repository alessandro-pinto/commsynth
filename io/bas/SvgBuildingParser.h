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
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <sstream>
#include "include/CosiGlobal.h"
#include "networks/wiredbas/WiredBasSpecificaion.h"
#include "tinyxml.h"
#include "environment/bas/Wall.h"
#include "environment/bas/InstSurface.h"

#ifndef _SVGBUILDINGPARSER_H_
#define _SVGBUILDINGPARSER_H_

namespace cosi {

namespace io {

namespace bas {

using namespace std ;


/** Parser for the building structure
 *
 * This parser reads one floor of a building described in
 * Inkscape svg format. The svg format must have the following
 * layers:
 * - walls. This layer contains a set of rectangles defining
 *           the walls on the floors.
 * - cable_ladders. This layer contains a set of rectangles
 *                   defining the cable ladders
 * - gateways. This layer contains a set of circles (paths)
 *              that define the positions of the gateways
 * - constraints. This layer contains a set of connectors
 *                 that define the end-to-end constraints
 *                 between the nodes and the gateways
 * Any other layer contains nodes (sensors and actuators).

 *@author Alessandro Pinto,
 *@author apinto@eecs.berkeley.edu,
 *@author EECS Grad Student,
 *@author 211 Cory Hall #1772,
 *@author Berkeley, CA, 94720-1772
 *@author (510)-6429670

 *@version $Id: SvgBuildingParser.h,v 1.7 2008/12/09 23:28:21 apinto Exp $
 */



class SvgBuildingParser {

 public:

  SvgBuildingParser( string pFname ) ;

  /** \brief Parses the building xml description.
   *
   * This function parses the XML description of a building
   * and fills up an internal data structure of walls and surfaces
   * grouped by floors.
   *
   */
  void Parse( ) ;

  /** \brief Prints a descripition of the building.
   *
   * @return A string containing the building description in textual format.
   */
  string PrintBuildingDescription( ) ;

  /** \brief Set of walls on a floor.
   *
   * This function returns the set of walls on a floor denoted
   * by a specific level.
   * @param pLevel Floor level
   * @return A map that associates a wall name to a wall data structur
   * @see Wall
   */
  map< string , cosi::environment::bas::Wall > GetWalls( int pLevel ) ;

  /** \brief Set of walls on a floor.
   *
   * This function returns the set of walls on a floor denoted
   * by a specific name.
   * @param pName Floor name
   * @return A map that associates a wall name to a wall data structur
   * @see Wall
   */
  map< string , cosi::environment::bas::Wall > GetWalls( string pName ) ;

  /** \brief Set of installation surfaces on a floor.
   *
   * This function returns the set of installation surfaces on a floor denoted
   * by a specific level.
   * @param pLevel Floor level
   * @return A map that associates an installation surface name to an installation surface data structur
   * @see InstSurface
   */
  map< string , cosi::environment::bas::InstSurface > GetInstallationSurfaces( int pLevel ) ;

  /** \brief Set of installation surfaces on a floor.
   *
   * This function returns the set of installation surfaces on a floor denoted
   * by a specific name.
   * @param pName Floor name
   * @return A map that associates an installation surface name to an installation surface data structur
   * @see InstSurface
   */
  map< string , cosi::environment::bas::InstSurface > GetInstallationSurfaces( string pName ) ;

  //list< Position > GetRoutingPoints( int pLevel ) ;

  list< Position > GetRoutingPoints( string pName ) ;

  cosi::commstruct::wiredbas::Specification* GetSpecification(string pName) ;

 private:

  void ParseDocument( TiXmlNode* pParent ) ;

  // <svg>
  void ParseSvg( TiXmlNode* pParent ) ;
  void GetSvgAttributes( TiXmlNode* pParent ) ;

  // <namedview>
  void ParseNamedView( TiXmlNode* pParent ) ;
  void GetNamedViewAttributes( TiXmlNode* pParent ) ;

  // <g>
  void ParseLayer( TiXmlNode* pParent ) ;
  string GetLayerAttributes( TiXmlNode* pParent ) ;

  // <rect> wall
  void ParseWall( TiXmlNode* pParent ) ;
  void GetWallAttributes( TiXmlNode* pParent ) ;

  // <rect> Cable Ladders
  void ParseCableLadder( TiXmlNode* pParent ) ;
  void GetCableLadderAttributes( TiXmlNode* pParent ) ;

  // <path> Gateway
  void ParseGateway( TiXmlNode* pParent ) ;
  void GetGatewayAttributes( TiXmlNode* pParent ) ;


  // <path> Node
  void ParseNode( TiXmlNode* pParent ) ;
  void GetNodeAttributes( TiXmlNode* pParent ) ;

  // <path> Constraint
  void ParseConstraint( TiXmlNode* pParent ) ;
  void GetConstraintAttributes( TiXmlNode* pParent ) ;

  // <path> RoutingPoint
  void ParseRoutingPoint( TiXmlNode* pParent ) ;
  void GetRoutingPointAttributes( TiXmlNode* pParent ) ;


  string FindNodeType( string pNode ) ;
  cosi::Position FindNodePosition( string pNode ) ;

  string mFname ;

  string mBuildingName ;

  double mHeightPx , mHeightCm ;

  double mWidthPx , mWidthCm ;

  string mCurrentLayerType ;

  string mCurrentApplication ;

  double mCurrentDx , mCurrentDy ;


  map< string , cosi::environment::bas::Wall > mWalls ;

  map< string , cosi::environment::bas::InstSurface >  mSurfaces ;

  //One map for each different application

  map< string , map< string , Position > > mRoutingPoints ;

  map< string , map< string , Position > > mNodes ;

  map< string , map< string , string > > mNodeType ;

  map< string , map< string , Position > > mGateways ;

  map< string , map< string , string > > mGatewayType ;

  map< string , map< pair<string , string> , double > > mPeriods ;

  map< string , map< pair<string , string> , double > > mLengths ;

  map< string , map< pair<string , string> , double > > mLatencies ;

  map< string , map< pair<string , string> , string > > mConstraintId ;

  map<string,int> mNodeId ;

} ;

}
}
}

#endif
