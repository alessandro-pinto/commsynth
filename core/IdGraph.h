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

/**Basic data structure for representing graphs.
 * Each vertex has an Id that distinguish one vertex from another.
 * Two vertexes with the same integer Id are considered the same
 * vertex.
 *
 *@author Alessandro Pinto,
 *@author The Donald O. Pederson Center for Electronic System Design
 *@author Electrical Engineering Department
 *@author University of California, Berkeley
 *@author 545P Cory Hall, Berkeley, CA 94720
 *@author +1 (510) 642 9670
 *@author http://www.eecs.berkeley.edu/~apinto
 *
 *@version $Id: IdGraph.h,v 1.12 2008/12/07 23:20:21 apinto Exp $
 */

#ifndef IDGRAPH_H_
#define IDGRAPH_H_

#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <string>
#include <sstream>
#include "include/CosiGlobal.h"

using namespace std;

namespace cosi {

typedef int Vertex;

class Edge {
public:
	Edge() {
	}
	;
	Edge(int pU, int pV);
	~Edge();
	void U(int pU);
	int U() const;
	void V(int pV);
	int V() const;
	bool operator==(const Edge &pE) const;
	bool operator<(const Edge &pE) const;
	friend std::ostream& operator<<(std::ostream &pOs, const Edge &pE);
private:
	int mU, mV;
};

class IdGraph {
public:

	/**\brief Defult Constructor*/
	IdGraph(string Name);

	/**\brief Defult Destructor*/
	virtual ~IdGraph();

	/** \brief Returns the input degree of node r.*/
	int InDegree(int r);

	/** \brief Returns the output degree of node r.*/
	int OutDegree(int r);

	/**\brief Returns the nuber of sources*/
	int Ns();

	/**\brief Returns the nuber of destinations*/
	int Nd();

	/**\brief Returns the nuber of additional nodes*/
	int Nr();

	Vertex AddVertex();

	/**\brief Adds a vertex.*/
	void AddVertex(int Id);

	/**\brief Removes a vertex.*/
	void DeleteVertex(int Id);

	/**\brief Returns true if  the vertex is in the graph.*/
	bool InV(int V);

	/**\brief Returns true if the edge (i,j) is in the graph.*/
	bool InE(int i, int j);

	/**\brief Returns true if the edge is in the graph.*/
	bool InE(Edge pE);

	/**\brief Adds an edge between i and j.*/
	void AddEdge(int i, int j);

	/**\brief Adds an edge.*/
	void AddEdge(Edge pE);

	/**\brief Removes an edge between i and j.*/
	void DeleteEdge(int i, int j);

	void DeleteEdge(Edge pE);

	/**\brief Returns the number of vertices.*/
	int Order();

	/**\brief Returns the number of edges.*/
	int Size();

	void Rename(int id1, int id2);

	typedef list<int>::iterator list_iterator;
	typedef set<int>::iterator v_iterator;

	/**\brief Returns the begin iterator to the list of vertices
	 connected to V.*/
	list_iterator in_begin(int V);
	/**\brief Returns the end iterator to the list of vertices
	 connected to V.*/
	list_iterator in_end(int V);
	/**\brief Returns the begin iterator to the list of vertices
	 V is connected to.*/
	list_iterator out_begin(int V);
	/**\brief Returns the end iterator to the list of vertices
	 V is connected to.*/
	list_iterator out_end(int V);
	/**\brief Returns the begin iterator to the set of sources.*/
	v_iterator s_begin();
	/**\brief Returns the end iterator to the set of sources.*/
	v_iterator s_end();
	/**\brief Returns the begin iterator to the set of destinations.*/
	v_iterator d_begin();
	/**\brief Returns the end iterator to the set of destinations.*/
	v_iterator d_end();
	/**\brief Returns the begin iterator to the set of routers.*/
	v_iterator r_begin();
	/**\brief Returns the end iterator to the set of routers.*/
	v_iterator r_end();

	/**\brief Returns the begin iterator to the set of vertexes.*/
	v_iterator v_begin();
	/**\brief Returns the end iterator to the set of vertexes.*/
	v_iterator v_end();

	/**\brief Returns the name of the graph.*/
	string GetName() const;

	/**\brief Sets the name of the graph.*/
	void SetName(string pName);

	/**\brief Sets the name of a vertex.*/
	void SetName(int V, string pName);

	/**\brief Sets the name of an edge.*/
	void SetName(int U, int V, string pName);

	void SetName(Edge pE, string pName);

	/**\brief Get the name of the a vertex.*/
	string GetName(int V);

	/**\brief Get the name of the an edge.*/
	string GetName(int U, int V);

	string GetName(Edge pE);

	int GetId(string Name ) ;


	virtual string Print();

	virtual string PrintNode(int V);

	virtual string PrintEdge(int U, int V);

	virtual string PrintEdge(Edge pE);

	int GetLastId();

private:

	void UpdateVertex(int V);
	/**\brief Adj List*/
	map< int , list<int> > mAdjList;
	/**\brief Reversed Adj List*/
	map< int , list<int> > mRadjList;
	/**\brief Set of sources.*/
	set< int > mS;
	/**\brief Set of destinations.*/
	set< int > mD;
	/**\brief Set of routers.*/
	set< int > mR;
	/**\brief Set of all vertices.*/
	set<int> mV;
	/**\brief Number of edges in the graph.*/
	int mSize;
	/**\brief Name of the graph.*/
	string mName;

	map< int , string > mNodeName;
	map< pair<int,int> , string > mEdgeName;
	map< string , int > mNodeId;
	map< string , pair<int,int> > mEdgeIdPair;

	int mLastId;

};
}
#endif /*IDGRAPH_H_*/
