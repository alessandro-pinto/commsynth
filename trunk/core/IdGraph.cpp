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
#include "IdGraph.h"

using namespace std;

namespace cosi {

Edge::~Edge() {

}

Edge::Edge(int pU, int pV) {
	mV = pV ;
	mU = pU ;
}

void Edge::U(int pU) {
	mU = pU ;
}

void Edge::V(int pV) {
	mV = pV ;
}

int Edge::U() const {
	return mU ;
}

int Edge::V() const {
	return mV ;
}

bool Edge::operator==(const Edge &pE) const {
	return ( mU == pE.U() && mV == pE.V());
}

bool Edge::operator<(const Edge &pE) const {
	return ( mU < pE.U() || ( mU == pE.U() && mV < pE.V() ));
}

std::ostream& operator<<(std::ostream &pOs, const Edge &pE) {
	pOs << "(" << pE.U() << "," << pE.V() << ")";
	return pOs;
}

IdGraph::IdGraph(string name) {
	mSize = 0;
	mName = name;
	mLastId = 0;
}

IdGraph::~IdGraph() {

	map< int , list<int> >::iterator It;
	for (It = mAdjList.begin(); It != mAdjList.end(); It++) {
		(It->second).clear();
	}
	for (It = mRadjList.begin(); It != mRadjList.end(); It++) {
		(It->second).clear();
	}

	mAdjList.clear();
	mRadjList.clear();

	mS.clear();
	mD.clear();
	mR.clear();
	mV.clear();
}

int IdGraph::InDegree(int V) {
	if (InV(V ) ) {
		map< int , list<int> >::iterator It;
		It = mRadjList.find(V );
		return (It->second).size();
	} else {
		cout << "Vertex "<< V << " is not present in graph "<< mName << endl;
		exit( 1);
	}
}

int IdGraph::OutDegree(int V) {
	if (InV(V ) ) {
		map< int , list<int> >::iterator It;
		It = mAdjList.find(V );
		return (It->second).size();
	} else {
		cout << "Vertex "<< V << " is not present in graph "<< mName << endl;
		exit( 1);
	}
}

int IdGraph::Ns() {
	return mS.size();
}

int IdGraph::Nd() {
	return mD.size();
}

int IdGraph::Nr() {
	return mR.size();
}

void IdGraph::AddVertex(int V) {
	if ( !InV(V ) ) {
		list<int> VadjList;
		mAdjList[V] = VadjList;
		mRadjList[V] = VadjList;
		mR.insert(V );
		mV.insert(V );
		mNodeName[V] = "";
	}
	if ( V > mLastId ) {
		mLastId = V;
	}
}

Vertex IdGraph::AddVertex() {
	int V = mLastId + 1;
	AddVertex(V);
	mLastId++;
	return V;
}

int IdGraph::GetLastId() {
	return mLastId;
}

void IdGraph::DeleteVertex(int V) {
	//delete all edges and update vertices
	map< int , list<int> >::iterator it_V;
	it_V = mAdjList.find(V );
	//for all destination vertices
	//Remove the back edges and update the vertices
	list<int>::iterator it_Edge;
	map<int, list<int> >::iterator it_U;
	int U;
	for (it_Edge = (it_V->second).begin(); it_Edge != (it_V->second).end(); it_Edge++) {
		//Destination vertex of this edge
		U = (*it_Edge);
		//Remove the back edge from V to U
		it_U = mRadjList.find(U );
		(it_U->second).remove(V );
		UpdateVertex(U );
	}
	//Remove all direct edges
	it_V = mRadjList.find(V );
	for (it_Edge = (it_V->second).begin(); it_Edge != (it_V->second).end(); it_Edge++) {
		//Destination vertex of this edge
		U = (*it_Edge);
		//Remove the back edge from V to U
		it_U = mAdjList.find(U );
		(it_U->second).remove(V );
		UpdateVertex(U );
	}
	//Finally remove V from both Adj List representations
	mAdjList.erase(V );
	mRadjList.erase(V );
	mV.erase(V );
	mNodeName.erase(V);
	//std::list< cosi::commstruct::wiredbas::Implementation* > GetChains();

}

bool IdGraph::InV(int V) {
	return (mAdjList.find(V ) != mAdjList.end() );
}

bool IdGraph::InE(int i, int j) {
	if ( !(InV(i ) && InV(j ) ))
	return false;
	map< int , list< int > >::iterator it_U;
	it_U = mAdjList.find(i );
	list<int>::iterator it_V;
	for (it_V = (it_U->second).begin(); it_V != (it_U->second).end(); it_V++) {
		if ( (*it_V) == j )
		return true;
	}
	return false;
}

bool IdGraph::InE(Edge pE) {
	return InE(pE.U(),pE.V());
}

void IdGraph::AddEdge(int i, int j) {
	if (InV(i ) && InV(j ) ) {
		map<int, list<int> >::iterator it_V;
		it_V = mAdjList.find(i );
		(it_V->second).push_back(j );
		it_V = mRadjList.find(j );
		(it_V->second).push_back(i );
		UpdateVertex(i );
		UpdateVertex(j );
		mSize++;
		mEdgeName[pair<int,int>(i,j)] = "";
	} else {
		cout << "WARNING : Trying to add an edge between "<< i << " and "<< j
		<< endl;
		cout << "          "<< "Both vertexes have to be in graph"<< endl;
	}
}

void IdGraph::AddEdge(Edge pE) {
	AddEdge(pE.U(),pE.V());
}
void IdGraph::DeleteEdge(int i, int j) {
	if (InV(i ) && InV(j ) ) {
		if (InE(i, j) ) {
			map<int, list<int> >::iterator it_V;
			it_V = mAdjList.find(i );
			(it_V->second).remove(j );
			it_V = mRadjList.find(j );
			(it_V->second).remove(i );
			UpdateVertex(i );
			UpdateVertex(j );
			mSize--;
			mEdgeName.erase(pair<int,int>(i,j));
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

void IdGraph::DeleteEdge(Edge pE) {
	DeleteEdge(pE.U(),pE.V());
}

void IdGraph::UpdateVertex(int V) {
	//find input and output lists
	map< int , list<int> >::iterator VadjList;
	map< int , list<int> >::iterator VrAdjList;
	VadjList = mAdjList.find(V );
	VrAdjList = mRadjList.find(V );

	//Find and remove the vertex
	set<int>::iterator Vit;
	Vit = mS.find(V );
	if (Vit != mS.end() )
	mS.erase(Vit );
	else {
		Vit = mD.find(V );
		if (Vit != mD.end() )
		mD.erase(Vit );
		else {
			Vit = mR.find(V );
			if (Vit != mR.end() )
			mR.erase(Vit );
		}
	}

	if ((VadjList->second).size() > 0&&(VrAdjList->second).size() == 0) { //SOURCE
		mS.insert(V );
	} else if ((VrAdjList->second).size() > 0&&(VadjList->second).size() == 0) { //DEST
		mD.insert(V );
	} else
	//ROUTER
	mR.insert(V );
}

int IdGraph::Order() {
	return (mV.size() );
}

int IdGraph::Size() {
	return mSize;
}

void IdGraph::Rename(int id1 , int id2) {
	//go through all the nodes and change the
	if ( InV(id2)) {
		CosiLog << "Vertex " << id1 << " cannot be renamed because " << id2 << " is already present in the graph" << endl;
		return;
	} else {
		map<int, list<int> >::iterator It;
		list<int>::iterator Lit;
		map<int, list<int> > AdjList;
		for(It = mAdjList.begin(); It != mAdjList.end(); It++ ) {
			for( Lit = It->second.begin(); Lit != It->second.end(); Lit++) {
				if ( It->first == id1 ) {
					if ( *Lit == id1 ) {
						AdjList[id2].push_back(id2);
					} else {
						AdjList[id2].push_back(*Lit);
					}
				} else {
					if ( *Lit == id1 ) {
						AdjList[It->first].push_back(id2);
					} else {
						AdjList[It->first].push_back(*Lit);
					}
				}

			}
		}
		mAdjList = AdjList;
		AdjList.clear();
		for(It = mRadjList.begin(); It != mRadjList.end(); It++ ) {
			for( Lit = It->second.begin(); Lit != It->second.end(); Lit++) {
				if ( It->first == id1 ) {
					if ( *Lit == id1 ) {
						AdjList[id2].push_back(id2);
					} else {
						AdjList[id2].push_back(*Lit);
					}
				} else {
					if ( *Lit == id1 ) {
						AdjList[It->first].push_back(id2);
					} else {
						AdjList[It->first].push_back(*Lit);
					}
				}

			}
		}
		mRadjList = AdjList;

		set< int >::iterator Vit;
		set<int> Vnew;
		for( Vit = mS.begin(); Vit != mS.end(); Vit++) {
			if ( *Vit == id1) {
				Vnew.insert(id2);
			} else {
				Vnew.insert(*Vit);
			}
		}
		mS = Vnew;
		Vnew.clear();
		for( Vit = mD.begin(); Vit != mD.end(); Vit++) {
			if ( *Vit == id1) {
				Vnew.insert(id2);
			} else {
				Vnew.insert(*Vit);
			}
		}
		mD = Vnew;
		Vnew.clear();
		for( Vit = mR.begin(); Vit != mR.end(); Vit++) {
			if ( *Vit == id1) {
				Vnew.insert(id2);
			} else {
				Vnew.insert(*Vit);
			}
		}
		mR = Vnew;
		Vnew.clear();
		for( Vit = mV.begin(); Vit != mV.end(); Vit++) {
			if ( *Vit == id1) {
				Vnew.insert(id2);
			} else {
				Vnew.insert(*Vit);
			}
		}
		mV = Vnew;
		Vnew.clear();

		map< int , string >::iterator Nit;
		map< int , string > NewNodeName;
		for( Nit = mNodeName.begin(); Nit != mNodeName.end(); Nit++ ) {
			if ( Nit->first == id1 ) {
				NewNodeName[id2] = Nit->second;
			} else {
				NewNodeName[Nit->first] = Nit->second;
			}
		}
		mNodeName = NewNodeName;

		map< pair<int,int> , string >::iterator Eit;
		map< pair<int,int> , string > NewEdgeName;
		for( Eit = mEdgeName.begin(); Eit != mEdgeName.end(); Eit++ ) {
			pair<int,int> E;
			if ( (Eit->first).first == id1 ) {
				E.first = id2;
			} else {
				E.first = (Eit->first).first;
			}
			if ( (Eit->first).second == id1 ) {
				E.second = id2;
			} else {
				E.second = (Eit->first).second;
			}
			NewEdgeName[E] = Eit->second ;
		}
		mEdgeName = NewEdgeName;
	}
}

IdGraph::list_iterator IdGraph::in_begin(int V) {
	map< int , list<int> >::iterator it;
	it = mRadjList.find(V );
	return (it->second).begin();
}

IdGraph::list_iterator IdGraph::in_end(int V) {
	map< int , list<int> >::iterator it;
	it = mRadjList.find(V );
	return (it->second).end();
}

IdGraph::list_iterator IdGraph::out_begin(int V) {
	map< int , list<int> >::iterator it;
	it = mAdjList.find(V );
	return (it->second).begin();
}

IdGraph::list_iterator IdGraph::out_end(int V) {
	map< int , list<int> >::iterator it;
	it = mAdjList.find(V );
	return (it->second).end();
}

IdGraph::v_iterator IdGraph::s_begin() {
	return mS.begin();
}

IdGraph::v_iterator IdGraph::s_end() {
	return mS.end();
}

IdGraph::v_iterator IdGraph::d_begin() {
	return mD.begin();
}

IdGraph::v_iterator IdGraph::d_end() {
	return mD.end();
}

IdGraph::v_iterator IdGraph::r_begin() {
	return mR.begin();
}

IdGraph::v_iterator IdGraph::r_end() {
	return mR.end();
}

IdGraph::v_iterator IdGraph::v_begin() {
	return mV.begin();
}

IdGraph::v_iterator IdGraph::v_end() {
	return mV.end();
}

string IdGraph::GetName() const {
	return mName;
}


void IdGraph::SetName(string pName) {
	mName = pName;
}

string IdGraph::GetName(int V) {
	return mNodeName[V];
}

int IdGraph::GetId(string pName) {
	return mNodeId[pName] ;
}

void IdGraph::SetName(int V, string pName) {
	//TODO: We may want to notify that a name is already taken
	//Names and Id should be unique. While id can change in a renaming
	//the name should not change. The name is calle \lambda in the TCAD paper.
	mNodeName[V] = pName;
	mNodeId[pName] = V ;
}

string IdGraph::GetName(int U,int V) {
	return mEdgeName[pair<int,int>(U,V)];
}

void IdGraph::SetName(int U , int V, string pName) {
	mEdgeName[pair<int,int>(U,V)] = pName;
}

string IdGraph::GetName(Edge pE) {
	return GetName( pE.U() , pE.V());
}

void IdGraph::SetName(Edge pE , string pName) {
	SetName(pE.U() , pE.V(),pName);
}

string IdGraph::Print() {
	stringstream s;
	s
	<< "----------------------------------------------------------------------"
	<< endl;
	s << "Graph "<< mName << endl;
	IdGraph::v_iterator U, V;
	for (V = v_begin(); V != v_end(); V++) {
		s << PrintNode( *V ) << endl;
	}
	for (U = v_begin(); U != v_end(); U++) {
		for (V = v_begin(); V != v_end(); V++) {
			if (InE( *U , *V ) )
			s << PrintEdge( *U , *V ) << endl;
		}
	}
	s
	<< "----------------------------------------------------------------------"
	<< endl;

	return s.str();

}

string IdGraph::PrintNode(int V)
{
	stringstream s;
	s << "Node: id = " << V << "  name = " << GetName(V) << endl;
	return s.str();
}

string IdGraph::PrintEdge(int U, int V) {
	stringstream s;
	s << "Edge (" << U << "," << V << ")" << endl;
	return s.str();
}

string IdGraph::PrintEdge( Edge pE ) {
	return PrintEdge(pE.U() , pE.V());
}

}

