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


#ifndef _FACE_H_
#define _FACE_H_

#include <iostream>
#include <vector>
#include "include/CosiGlobal.h"
#include "quantities/Position.h"

/** A face is a convex 2D set with four vertexes and four edges.
 *  The face class contains four points in the euclidean space.
 *  Point V1 is the closest to he origin and the other
 *  are in counterclockwise order with respect to the positive
 *  direction of z.
 *
 *@author Alessandro Pinto,
 *@author apinto@eecs.berkeley.edu,
 *@author EECS Grad Student,
 *@author 211 Cory Hall #1772,
 *@author Berkeley, CA, 94720-1772
 *@author (510)-6429670

 *@version $Id: Face.h,v 1.3 2008/12/07 23:21:47 apinto Exp $
 */

namespace cosi {

namespace environment {

namespace bas {

class Face {

 public:

  /**\brief Name of the Face.*/
  std::string Name ;

  /**\brief Floor level*/
  int Level ;

  /**\brief Default constructor.*/
  Face( ) ;

  /**\brief Assigns the four points.
   *
   * This function takes care of assigning the four
   * points in the right order. In particular the four
   * paramters of this function don't have to be in nay
   * specific order.
   */
  void SetPoints( Position pV1 , Position pV2 , Position pV3 , Position pV4 ) ;

  /**\brief Sets the value of of point V1 of the face.*/
  void SetV1( Position pV1 ) ;

  /**\brief Sets the value of of point V2 of the face.*/
  void SetV2( Position pV2 ) ;

  /**\brief Sets the value of of point V3 of the face.*/
  void SetV3( Position pV3 ) ;

  /**\brief Sets the value of of point V4 of the face.*/
  void SetV4( Position pV4 ) ;

  /**\brief Returns the value of of point V1 of the face.*/
  Position GetV1( ) ;

  /**\brief Returns the value of of Position V2 of the face.*/
  Position GetV2( ) ;

  /**\brief Returns the value of of point V3 of the face.*/
  Position GetV3( ) ;

  /**\brief Returns the value of of point V4 of the face.*/
  Position GetV4( ) ;

  /**\brief Returns true if the four points are on the same surface.*/
  bool CheckFace( ) ;


 private:

  std::vector< Position > mV ;


} ;

}
}
}
#endif
