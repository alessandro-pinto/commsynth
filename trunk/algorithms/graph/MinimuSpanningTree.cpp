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
#include "MinimuSpanningTree.h"

namespace cosi {

namespace algorithms {

namespace graph {

MinimumSpanningTree::MinimumSpanningTree(
		cosi::commstruct::graph::U_RealWeightedGraph* pN) {
	mN = pN ;
}

MinimumSpanningTree::~MinimumSpanningTree() {
}

void MinimumSpanningTree::Run(Vertex pRoot) {
	mRoot = pRoot ;
	//Queue
	std::set< std::pair< double , int > > Q;
	//extracted from the queue
	std::set< std::pair< double , int > >::iterator Extracted;
	//cost
	std::map< int , double > Fp;
	//Element in Q
	std::map< int , bool > InQ;

	//Initialize
	{
		IdUndirectedGraph::v_iterator V;
		for (V = mN->v_begin() ; V != mN->v_end() ; V++) {
			InQ[*V] = true;
			if ( *V == mRoot ) {
				Q.insert(std::pair<double,int>(0,mRoot));
				Fp[mRoot] = 0;
			} else {
				Q.insert( std::pair<double,int>(DBL_MAX,*V) );
				Fp[*V] = DBL_MAX;
			}
		}
		mPi[mRoot] = -1;
	}

	while ( !Q.empty() ) {
		//Extract from the queue
//		std::set< std::pair< double , int > >::iterator It;
//		for( It = Q.begin(); It!= Q.end();It++) {
//			cout << "Cost " << It->first << " node " << It->second << endl ;
//		}
		int V;
		Extracted = Q.begin();
		V = Extracted->second;
		//cout << "Extracted " << V << endl ;
		Q.erase(Extracted);
		InQ[V] = false;
		IdUndirectedGraph::list_iterator U;
		for (U = mN->adj_begin(V); U != mN->adj_end(V); U++) {
			//cout << "Examine " << *U << endl ;
			if ( InQ[*U] && ( (*mN)[Edge(V,*U)].PRJ(RealQuantity)GetValue() < Fp[*U] ) ) {
				//cout << "Updating " << *U << endl ;
				mPi[*U] = V;
				Fp[*U] = (*mN)[Edge(V,*U)].PRJ(RealQuantity)GetValue();
				std::set< std::pair< double , int > >::iterator It;
				for( It = Q.begin(); It!= Q.end();It++) {
					if ( It->second == *U ) {
						Q.erase(It);
						Q.insert(std::pair<double,int>(Fp[*U],*U));
						break;
					}
				}

			}
		}
	}
}

cosi::commstruct::graph::U_RealWeightedGraph* MinimumSpanningTree::Get() {

	cosi::commstruct::graph::U_RealWeightedGraph* RetVal = new cosi::commstruct::graph::U_RealWeightedGraph;

	//for each node, insert the edge between the node and its predecessor
	IdUndirectedGraph::v_iterator V;
	RetVal->AddVertex(mRoot);
	for( V = mN->v_begin(); V != mN->v_end(); V++) {
		if ( *V != mRoot ) {
			if ( ! RetVal->InV(*V)) {
				RetVal->AddVertex(*V);
			}
			int U;
			U = mPi[*V];
			if ( ! RetVal->InV(U)) {
				RetVal->AddVertex(U);
			}
			RetVal->AddEdge(*V,U);
			(*RetVal)[Edge(*V,U)].PRJ(RealQuantity)Set((*mN)[Edge(*V,U)].PRJ(RealQuantity)Get());
		}
	}

	return RetVal;
}

}

}

}
