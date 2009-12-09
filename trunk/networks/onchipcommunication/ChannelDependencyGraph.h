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

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <queue>
#include "core/IdGraph.h"

#ifndef CHANNELDEPENDENCYGRAPH_H_
#define CHANNELDEPENDENCYGRAPH_H_

namespace cosi {

namespace commstruct {

namespace onchipcommunication {

using namespace std ;

/**Basic data structure for representing a Channel Dependency Graph.
 *
 *
 *@author Alessandro Pinto,
 *@author apinto@eecs.berkeley.edu,
 *@author EECS Grad Student,
 *@author 211 Cory Hall #1772,
 *@author Berkeley, CA, 94720-1772
 *@author (510)-6429670

 *@version $Id: ChannelDependencyGraph.h,v 1.3 2008/12/09 23:28:24 apinto Exp $
 */


class ChannelDependencyGraph : public IdGraph
{
public:

  /**\brief Defult Constructor*/
  ChannelDependencyGraph( string Name ) ;
  /**\brief Defult Destructor*/
  ~ChannelDependencyGraph ( ) ;

  /**\brief Union of vertices and edges.*/
  void Union( ChannelDependencyGraph G ) ;

  /**\brief Adds a channel.*/
  void AddChannel( pair<int,int> pChannel );

  /**\brief Returns true if  the vertex is in the graph.*/
  bool InC( pair<int,int> pChannel ) ;

  /**\brief Returns true if the edge (i,j) is in the graph.*/
  bool InD( pair<int,int> pChannel1 , pair<int,int> pChannel2 )  ;

  /**\brief Adds an edge between i and j.*/
  void AddDependency( pair<int,int> pChannel1 , pair<int,int> pChannel2 ) ;

  void RemoveDependency( pair<int,int> pChannel1 , pair<int,int> pChannel2 ) ;

  vector< pair< int , int > > GetChannels( )   ;

  //list_iterator in_begin( pair<int,int> pChannel ) ;
  //list_iterator in_end( pair<int,int> pChannel ) ;
  //list_iterator out_begin( pair<int,int> pChannel ) ;
  //list_iterator out_end( pair<int,int> pChannel ) ;

  bool Reachable( pair<int, int > pChannel1 , pair<int,int> pChannel2 ) ;

  bool Cyclic( ) ;

 private:

  map< pair< int , int > , int > mChannelToVertex ;

} ;
}
}
}


#endif /*CHANNELDEPENDENCYGRAPH_H_*/
