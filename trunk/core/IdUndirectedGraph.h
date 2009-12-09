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
#ifndef IDUNDIRECTEDGRAPH_H_
#define IDUNDIRECTEDGRAPH_H_

#include <string>
#include <sstream>
#include <list>
#include <set>
#include "core/IdGraph.h"

namespace cosi {

class IdUndirectedGraph {

public:

	IdUndirectedGraph(std::string pName);

	virtual ~IdUndirectedGraph();

	int Degree(int r);

	Vertex AddVertex() ;

	int GetLastId( ) ;

	void AddVertex(int Id);

	void DeleteVertex(int Id);

	bool InV(int V);

	bool InE(int i, int j);

	bool InE(Edge pE);

	void AddEdge(int i, int j);

	void AddEdge(Edge pE);

	void DeleteEdge(int i, int j);

	void DeleteEdge(Edge pE);

	int Order();

	int Size();

	typedef std::list<int>::iterator list_iterator;
	typedef std::set<int>::iterator v_iterator;

	/**\brief Returns the begin iterator to the list of vertices
	 connected to V.*/
	list_iterator adj_begin(int V);
	/**\brief Returns the end iterator to the list of vertices
	 connected to V.*/
	list_iterator adj_end(int V);

	/**\brief Returns the begin iterator to the set of vertexes.*/
	v_iterator v_begin();
	/**\brief Returns the end iterator to the set of vertexes.*/
	v_iterator v_end();

	/**\brief Returns the name of the graph.*/
	std::string GetName() const;

	/**\brief Sets the name of the graph.*/
	void SetName(std::string pName);

	/**\brief Sets the name of a vertex.*/
	void SetName(int V, std::string pName);

	/**\brief Sets the name of an edge.*/
	void SetName(int U, int V, std::string pName);

	void SetName(Edge pE, std::string pName);

	/**\brief Get the name of the a vertex.*/
	std::string GetName(int V);

	/**\brief Get the name of the an edge.*/
	std::string GetName(int U, int V);

	std::string GetName(Edge pE);

	virtual string Print();

	virtual string PrintNode(int V);

	virtual string PrintEdge(int U, int V);

	virtual string PrintEdge(Edge pE);

private:
	/**\brief Adj List*/
	std::map< int , std::list<int> > mAdjList;

	std::set<int> mV;
	/**\brief Number of edges in the graph.*/
	int mSize;
	/**\brief Name of the graph.*/
	string mName;

	std::map< int , std::string > mNodeName;
	std::map< std::pair<int,int> , std::string > mEdgeName;

	int mLastId ;

};

}

#endif /*IDUNDIRECTEDGRAPH_H_*/
