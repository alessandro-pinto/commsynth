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
#include "IdUndirectedGraph.h"

namespace cosi {

IdUndirectedGraph::IdUndirectedGraph(std::string pName) {
	mName = pName ;
	mLastId = 0;
	mSize = 0 ;
}

IdUndirectedGraph::~IdUndirectedGraph() {
}

int IdUndirectedGraph::Degree(int V) {
	if (InV(V ) ) {
		std::map< int , std::list<int> >::iterator It;
		It = mAdjList.find(V );
		return (It->second).size();
	} else {
		cout << "Vertex "<< V << " is not present in graph "<< mName << endl;
		exit( 1);
	}
}

void IdUndirectedGraph::AddVertex(int V) {
	if ( !InV(V ) ) {
		std::list<int> VadjList;
		mAdjList[V] = VadjList;
		mV.insert(V );
		mNodeName[V] = "";
	}
	if (V > mLastId ) {
		mLastId = V ;
	}
}

Vertex IdUndirectedGraph::AddVertex() {
	int V = mLastId + 1;
	AddVertex(V) ;
	mLastId++;
	return V ;
}

int IdUndirectedGraph::GetLastId() {
	return mLastId ;
}

void IdUndirectedGraph::DeleteVertex(int V) {
	//delete all edges and update vertices
	std::map< int , std::list<int> >::iterator it_V;
	it_V = mAdjList.find(V );
	//for all destination vertices
	//Remove the back edges and update the vertices
	std::list<int>::iterator it_Edge;
	std::map<int, std::list<int> >::iterator it_U;
	int U;
	for (it_Edge = (it_V->second).begin(); it_Edge != (it_V->second).end(); it_Edge++) {
		//Destination vertex of this edge
		U = (*it_Edge);
		//Remove the back edge from V to U
		it_U = mAdjList.find(U );
		(it_U->second).remove(V );
	}
	mAdjList.erase(V );
	mV.erase(V );
	mNodeName.erase(V) ;
}

bool IdUndirectedGraph::InV(int V) {
	return (mAdjList.find(V ) != mAdjList.end() );
}

bool IdUndirectedGraph::InE(int i, int j) {
	if ( !(InV(i ) && InV(j ) ))
		return false;
	std::map< int , std::list< int > >::iterator it_U;
	it_U = mAdjList.find(i );
	list<int>::iterator it_V;
	for (it_V = (it_U->second).begin(); it_V != (it_U->second).end(); it_V++) {
		if ( (*it_V) == j )
			return true;
	}
	return false;
}

bool IdUndirectedGraph::InE(Edge pE) {
	return InE(pE.U(), pE.V());
}

void IdUndirectedGraph::AddEdge(int i, int j) {
	if (InV(i ) && InV(j ) ) {
		std::map<int, std::list<int> >::iterator it_V;
		it_V = mAdjList.find(i );
		(it_V->second).push_back(j );
		it_V = mAdjList.find(j );
		(it_V->second).push_back(i );
		mSize++;
		mEdgeName[pair<int,int>(i,j)] = "";
		mEdgeName[pair<int,int>(j,i)] = "";
	} else {
		cout << "WARNING : Trying to add an edge between "<< i << " and "<< j
				<< endl;
		cout << "          "<< "Both vertexes have to be in graph"<< endl;
	}
}

void IdUndirectedGraph::AddEdge(Edge pE) {
	AddEdge(pE.U(), pE.V());
}

void IdUndirectedGraph::DeleteEdge(int i, int j) {
	if (InV(i ) && InV(j ) ) {
		if (InE(i, j) ) {
			std::map<int, std::list<int> >::iterator it_V;
			it_V = mAdjList.find(i );
			(it_V->second).remove(j );
			it_V = mAdjList.find(j );
			(it_V->second).remove(i );
			mSize--;
			mEdgeName.erase(pair<int, int>(i, j)) ;
			mEdgeName.erase(pair<int, int>(j, i)) ;
		} else {
			cout << "WARNING : Trying to remove an edge between "<< i<< " and "
					<< j << endl;
			cout << "          but the edge is not in the graph"<< endl;
		}
	} else {
		cout << "WARNING : Trying to remove an edge between "<< i << " and "
				<< j << endl;
		cout << "          Both vertexes have to be in the graph"<< endl;
	}
}

void IdUndirectedGraph::DeleteEdge(Edge pE) {
	DeleteEdge(pE.U(), pE.V());
}

int IdUndirectedGraph::Order() {
	return (mV.size() );
}

int IdUndirectedGraph::Size() {
	return mSize ;
}

IdUndirectedGraph::list_iterator IdUndirectedGraph::adj_begin(int V) {
	std::map< int , std::list<int> >::iterator it;
	it = mAdjList.find(V );
	return (it->second).begin();
}

IdUndirectedGraph::list_iterator IdUndirectedGraph::adj_end(int V) {
	std::map< int , std::list<int> >::iterator it;
	it = mAdjList.find(V );
	return (it->second).end();
}

IdUndirectedGraph::v_iterator IdUndirectedGraph::v_begin() {
	return mV.begin();
}

IdUndirectedGraph::v_iterator IdUndirectedGraph::v_end() {
	return mV.end();
}

std::string IdUndirectedGraph::GetName() const {
	return mName;
}

void IdUndirectedGraph::SetName(std::string pName) {
	mName = pName ;
}

void IdUndirectedGraph::SetName(int V, std::string pName) {
	mNodeName[V] = pName;
}

void IdUndirectedGraph::SetName(int U, int V, std::string pName) {
	mEdgeName[std::pair<int,int>(U,V)] = pName;
}

void IdUndirectedGraph::SetName(Edge pE, std::string pName) {
	SetName(pE.U() , pE.V(), pName);
}

std::string IdUndirectedGraph::GetName(int V) {
	return mNodeName[V];
}

std::string IdUndirectedGraph::GetName(int U, int V) {
	return mEdgeName[std::pair<int,int>(U,V)];
}

std::string IdUndirectedGraph::GetName(Edge pE) {
	return GetName(pE.U() , pE.V());
}

std::string IdUndirectedGraph::Print() {
	std::stringstream s;
	s
			<< "----------------------------------------------------------------------"
			<< std::endl;
	s << "Graph "<< mName << std::endl;
	IdUndirectedGraph::v_iterator U, V;
	for (V = v_begin(); V != v_end(); V++) {
		s << PrintNode( *V ) << std::endl;
	}
	for (U = v_begin(); U != v_end(); U++) {
		for (V = U; V != v_end(); V++) {
			if (InE( *U , *V ) )
				s << PrintEdge( *U , *V ) << std::endl;
		}
	}
	s
			<< "----------------------------------------------------------------------"
			<< std::endl;

	return s.str();

}

std::string IdUndirectedGraph::PrintNode(int V) {
	std::stringstream s;
	s << "Node: id = "<< V << "  name = "<< GetName(V) << std::endl;
	return s.str();
}

std::string IdUndirectedGraph::PrintEdge(int U, int V) {
	std::stringstream s;
	s << "Edge ("<< U << ","<< V << ") undirected "<< std::endl;
	return s.str();
}

std::string IdUndirectedGraph::PrintEdge(Edge pE) {
	return PrintEdge(pE.U() , pE.V());
}

}
