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
#ifndef COMMUNICATIONSTRUCTURE_HPP_
#define COMMUNICATIONSTRUCTURE_HPP_

#include <string>
#include "IdGraph.h"
#include "IdUndirectedGraph.h"
#include "../label/LabelMap.hpp"

namespace cosi {

#define PRJ(x) x::

template<class L> class CommunicationStructure : public IdGraph,
	public LabelMap<L> {
public:

	CommunicationStructure() :
		IdGraph("Unknown") {
	}
	;

	CommunicationStructure(std::string pName) :
		IdGraph(pName) {
	}
	;

	int AddVertex() {
		int Id = IdGraph::AddVertex();
		LabelMap<L>::Set(Id, L()) ;
		return Id ;
	}
	;

	/**\brief Adds a vertex.*/
	void AddVertex(int Id) {
		IdGraph::AddVertex(Id);
		LabelMap<L>::Set(Id, L()) ;
	}
	;

	/**\brief Removes a vertex.*/
	void DeleteVertex(int Id) {
		IdGraph::DeleteVertex(Id);
		LabelMap<L>::Erase(Id);
	}
	;

	/**\brief Adds an edge between i and j.*/
	void AddEdge(int i, int j) {
		IdGraph::AddEdge(i, j);
		LabelMap<L>::Set(Edge(i, j), L());
	}
	;

	/**\brief Adds an edge.*/
	void AddEdge(Edge pE) {
		IdGraph::AddEdge(pE);
		LabelMap<L>::Set(pE, L());
	}
	;

	/**\brief Removes an edge between i and j.*/
	void DeleteEdge(int i, int j) {
		IdGraph::DeleteEdge(i, j);
		LabelMap<L>::Erase(Edge(i, j));
	}
	;

	void DeleteEdge(Edge pE) {
		IdGraph::DeleteEdge(pE);
		LabelMap<L>::Erase(pE);
	}
	;

	L& operator[](Vertex V) {
		return LabelMap<L>::Get(V) ;
	}
	;

	L& operator[](Edge E) {
		return LabelMap<L>::Get(E) ;
	}
	;

	std::string PrintNode(int V) {
		std::string s;
		s += IdGraph::PrintNode(V) ;
		s += (*this)[V].Print() ;
		return s ;
	}

	std::string PrintEdge(int U, int V) {
		std::string s;
		s+= IdGraph::PrintEdge(U, V) ;
		s+= (*this)[Edge(U,V)].Print() ;
		return s ;
	}

	std::string PrintEdge(Edge pE) {
		std::string s;
		s+= IdGraph::PrintEdge(pE) ;
		s+= (*this)[pE].Print() ;
		return s ;
	}

};

template<class L> class U_CommunicationStructure : public IdUndirectedGraph,
	public LabelMap<L> {
public:

	U_CommunicationStructure() :
		IdUndirectedGraph("Unknown") {
	}
	;

	U_CommunicationStructure(std::string pName) :
		IdUndirectedGraph(pName) {
	}
	;

	/**\brief Adds a vertex.*/
	void AddVertex(int Id) {
		IdUndirectedGraph::AddVertex(Id);
		LabelMap<L>::Set(Id, L()) ;
	}
	;

	/**\brief Removes a vertex.*/
	void DeleteVertex(int Id) {
		IdUndirectedGraph::DeleteVertex(Id);
		LabelMap<L>::Erase(Id);
	}
	;

	/**\brief Adds an edge between i and j.*/
	void AddEdge(int i, int j) {
		IdUndirectedGraph::AddEdge(i, j);
		LabelMap<L>::Set(Edge(i, j), L());
		LabelMap<L>::Set(Edge(j, i), L());
	}
	;

	/**\brief Adds an edge.*/
	void AddEdge(Edge pE) {
		IdUndirectedGraph::AddEdge(pE);
		LabelMap<L>::Set(pE, L());
		Edge E(pE.V() , pE.U());
		LabelMap<L>::Set(E, L());
	}
	;

	/**\brief Removes an edge between i and j.*/
	void DeleteEdge(int i, int j) {
		IdUndirectedGraph::DeleteEdge(i, j);
		LabelMap<L>::Erase(Edge(i, j));
		LabelMap<L>::Erase(Edge(j, i));
	}
	;

	void DeleteEdge(Edge pE) {
		IdUndirectedGraph::DeleteEdge(pE);
		LabelMap<L>::Erase(pE);
		Edge E(pE.V() , pE.U());
		LabelMap<L>::Erase(E);
	}
	;

	L& operator[](Vertex V) {
		return LabelMap<L>::Get(V) ;
	}
	;

	L& operator[](Edge E) {
		if ( !(LabelMap<L>::Get(E)).IsBot() )
			return LabelMap<L>::Get(E) ;
		Edge Re ;
		Re.U(E.V()) ;
		Re.V(E.U()) ;
		if ( !(LabelMap<L>::Get(Re)).IsBot() )
			return LabelMap<L>::Get(Re) ;
		return  LabelMap<L>::Get(E);
	}
	;

	std::string PrintNode(int V) {
		std::string s;
		s += IdUndirectedGraph::PrintNode(V) ;
		s += (*this)[V].Print() ;
		return s ;
	}

	std::string PrintEdge(int U, int V) {
		std::string s;
		s+= IdUndirectedGraph::PrintEdge(U, V) ;
		s+= (*this)[Edge(U,V)].Print() ;
		return s ;
	}

	std::string PrintEdge(Edge pE) {
		std::string s;
		s+= IdUndirectedGraph::PrintEdge(pE) ;
		s+= (*this)[pE].Print() ;
		return s ;
	}

};

}

#endif /*COMMUNICATIONSTRUCTURE_HPP_*/
