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
#include "Tsp.h"

namespace cosi {

namespace algorithms {

namespace graph {

Tsp::Tsp(cosi::commstruct::graph::U_RealWeightedGraph* pN) {
	mN = pN ;
	//check that the graph is complete
	if (mN->Size() != mN->Order() * (mN->Order() - 1)) {
		//std::cout << "WARNING : The graph is not a complete graph"<< std::endl ;
	}
}

Tsp::~Tsp() {
}

void Tsp::Run() {
	cosi::commstruct::graph::U_RealWeightedGraph* M;
	MinimumSpanningTree* Mst = new MinimumSpanningTree(mN);
	IdUndirectedGraph::v_iterator V;
	V = mN->v_begin() ;
	Mst->Run(*V) ;
	M = Mst->Get() ;

	//Find the Euler tour
	mTour.clear() ;
	mTour.push_back(*V) ;
	DfsInit(M) ;
	DfsVisit(*V) ;
	delete mMd ;
	delete M ;
}

std::list<int> Tsp::Get() {
	return mTour ;
}

void Tsp::DfsVisit(int Root) {
	mColor[Root] = GRAY ;
	IdUndirectedGraph::list_iterator Adj ;
	for( Adj = mMd->adj_begin(Root) ; Adj != mMd->adj_end(Root) ; Adj++ ) {
		if ( mColor[*Adj] == WHITE ) {
			mTour.push_back(*Adj) ;
			DfsVisit(*Adj) ;
		}
	}
	mColor[Root] = BLACK ;
}

void Tsp::DfsInit(cosi::commstruct::graph::U_RealWeightedGraph* pM) {
	//double all edges in the graph
	mMd = new cosi::commstruct::graph::U_RealWeightedGraph;
	IdUndirectedGraph::v_iterator U, V;
	for (U = pM->v_begin() ; U != pM->v_end() ; U++) {
		for (V = pM->v_begin() ; V != pM->v_end() ; V++) {
			if ( !mMd->InV(*U)) {
				mMd->AddVertex(*U) ;
				mColor[*U] = WHITE ;
			}
			if ( !mMd->InV(*V)) {
				mMd->AddVertex(*V) ;
				mColor[*V] = WHITE ;
			}
			if (pM->InE(*U, *V) && !mMd->InE(*U, *V)) {
				mMd->AddEdge(*U, *V) ;
				mMd->AddEdge(*U, *V) ;
			}
		}
	}
}

}

}

}
