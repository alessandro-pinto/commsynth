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

#include "ChannelDependencyGraph.h"

namespace cosi {

namespace commstruct {

namespace onchipcommunication {

ChannelDependencyGraph::ChannelDependencyGraph( string Name ) : IdGraph( Name ) {

}

void ChannelDependencyGraph::Union( ChannelDependencyGraph G ) {

  //Add all channels
  vector< pair< int , int > > Channels ;
  Channels = G.GetChannels( ) ;
  for( int i = 0 ; i < Channels.size( ) ; i++ ) {
    AddChannel( Channels[i] ) ;
  }

  //Add all dependencies
  for( int i = 0 ; i < Channels.size( ) ; i++ ) {
    for( int j = 0 ; j < Channels.size( ) ; j++ ) {
      if ( G.InD( Channels[i] , Channels[j] ) )
	AddDependency( Channels[i] , Channels[j] ) ;
    }
  }
}

ChannelDependencyGraph::~ChannelDependencyGraph ( ) {

}

void ChannelDependencyGraph::AddChannel( pair<int,int> pChannel ){

  //Assigne a new Id to this vertex
  mChannelToVertex[pChannel] = Order( ) ;
  //Add the vertex
  AddVertex( mChannelToVertex[pChannel] ) ;

}

bool ChannelDependencyGraph::InC( pair<int,int> pChannel ) {
  //get the vertex id
  if ( mChannelToVertex.find( pChannel ) != mChannelToVertex.end( ) )
    return true ;
  else
    return false ;
}

vector< pair< int , int > > ChannelDependencyGraph::GetChannels( ) {
  vector< pair< int , int > > RetVal ;
  map< pair< int , int > , int >::iterator It ;
  for( It = mChannelToVertex.begin( ) ; It != mChannelToVertex.end( ) ; It++ ) {
    RetVal.push_back( It->first ) ;
  }
  return RetVal ;
}


bool ChannelDependencyGraph::InD( pair<int,int> pChannel1 , pair<int,int> pChannel2 ) {
  int Id1 , Id2 ;
  if ( mChannelToVertex.find( pChannel1 ) != mChannelToVertex.end( ) )
    Id1 = mChannelToVertex[pChannel1] ;
  else
    return false ;
  if ( mChannelToVertex.find( pChannel2 ) != mChannelToVertex.end( ) )
    Id2 = mChannelToVertex[pChannel2] ;
  else
    return false ;

  return InE(Id1,Id2) ;

}

void ChannelDependencyGraph::AddDependency( pair<int,int> pChannel1 , pair<int,int> pChannel2 ) {
  int Id1 , Id2 ;
  if ( mChannelToVertex.find( pChannel1 ) != mChannelToVertex.end( ) )
    Id1 = mChannelToVertex[pChannel1] ;
  else {
    mChannelToVertex[pChannel1] = Order( ) ;
    AddVertex( mChannelToVertex[pChannel1] ) ;
    Id1 = mChannelToVertex[pChannel1] ;
  }

  if ( mChannelToVertex.find( pChannel2 ) != mChannelToVertex.end( ) )
    Id2 = mChannelToVertex[pChannel2] ;
  else {
    mChannelToVertex[pChannel2] = Order( ) ;
    AddVertex( mChannelToVertex[pChannel2] ) ;
    Id2 = mChannelToVertex[pChannel2] ;
  }

  AddEdge( Id1 , Id2 ) ;

}


void ChannelDependencyGraph::RemoveDependency( pair<int,int> pChannel1 , pair<int,int> pChannel2 ) {
  if ( InD( pChannel1 , pChannel2 ) ) {
    int Id1 , Id2 ;
    Id1 = mChannelToVertex[pChannel1] ;
    Id2 = mChannelToVertex[pChannel2] ;
    DeleteEdge( Id1 , Id2 ) ;
  }
}

/*
IdGraph::list_iterator ChannelDependencyGraph::in_begin( pair<int,int> pChannel ) {
  int Id ;
  Id = mChannelToVertex[ pChannel ] ;
  return in_begin( Id ) ;
}

IdGraph::list_iterator ChannelDependencyGraph::in_end( pair<int,int> pChannel ) {
  int Id ;
  Id = mChannelToVertex[ pChannel ] ;
  return in_end( Id ) ;
}

IdGraph::list_iterator ChannelDependencyGraph::out_begin( pair<int,int> pChannel ) {
  int Id ;
  Id = mChannelToVertex[ pChannel ] ;
  return out_begin( Id ) ;
}

IdGraph::list_iterator ChannelDependencyGraph::out_end( pair<int,int> pChannel ) {
  int Id ;
  Id = mChannelToVertex[ pChannel ] ;
  return out_end( Id ) ;
}
*/

bool ChannelDependencyGraph::Reachable( pair<int, int > pChannel1 , pair<int,int> pChannel2 ) {

  //Check if there is a path between pChannel1 and pChannel2

  if (  ( mChannelToVertex.find( pChannel1 ) == mChannelToVertex.end( ) ) ||
	( mChannelToVertex.find( pChannel2 ) == mChannelToVertex.end( ) ) ) {
    return false ;
  } ;

  int Id1 , Id2 ;

  Id1 = mChannelToVertex[ pChannel1 ] ;
  Id2 = mChannelToVertex[ pChannel2 ] ;

  vector< int > Color( Order() , 0 ) ; //0 = WHITE , 1 = GRAY , 2 = BLACK

  Color[Id1] = 1 ;

  queue<int> Q ;

  Q.push( Id1 ) ;

  int U ;
  list_iterator Adj ;

  while( ! Q.empty( ) ) {

    U = Q.front( ) ;

    for( Adj = out_begin( U ) ; Adj != out_end( U ) ; Adj++ ) {

      if ( *Adj == Id2 )
	return true ;

      if ( Color[*Adj] == 0 ) {
	Color[*Adj] = 1 ;
	Q.push( *Adj ) ;
      }

    }

    Q.pop( ) ;
    Color[U] = 2 ;
  }
  return false ;
}


bool ChannelDependencyGraph::Cyclic( ) {

  //check if any channel can be reached from itself
  map< pair< int , int > , int >::iterator Channel ;
  for( Channel = mChannelToVertex.begin( ) ; Channel != mChannelToVertex.end( ) ; Channel++ ) {
    if ( Reachable( Channel->first , Channel->first ) )  {
      cout << "(" << (Channel->first).first << "," << (Channel->first).second << ") in a cycle " << endl ;
      return true ;
    }
  }

  return false ;


}

}
}
}
