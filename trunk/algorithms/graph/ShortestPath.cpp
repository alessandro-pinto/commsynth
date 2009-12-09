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
#include "ShortestPath.h"

namespace cosi {

namespace algorithms {

namespace graph {

ShortestPath::ShortestPath(cosi::commstruct::graph::RealWeightedGraph* pN) {
	mN = pN ;
}

ShortestPath::~ShortestPath() {
}

void ShortestPath::Run(int Src, int Dest) {
	mSrc = Src ;
	mDest = Dest ;
	//Queue
	std::set< std::pair< double , int > > Q;
	//extracted from the queue
	std::set< std::pair< double , int > >::iterator Extracted;
	//cost
	std::map< int , double > Fp;
	//vertexes already considered
	std::map< int , bool > Found;

	Q.insert(std::pair<double, int>( 0, Src ) );
	Found[Src] = true;
	Fp[Src] = 0;
	mPi[Src] = -1;
	while ( !Q.empty() ) {

		//Extract from the queue
		int V;
		Extracted = Q.begin();
		V = Extracted->second;
		Q.erase(Extracted );
		if (V == Dest ) {
			return;
		}

		IdGraph::list_iterator U;

		for (U = mN->out_begin(V); U != mN->out_end(V); U++) {
			double D;
			D = (*mN)[Edge(V,*U)].PRJ(RealQuantity)GetValue() ;
			if ( (Fp.find( *U ) == Fp.end() ) || (Fp[*U] > Fp[V] + D)) {
				if (Found.find(*U) == Found.end() ) {
					Q.insert(pair<double, int>(Fp[V] + D , *U ) );
					Found[*U] = true;
				}
				Fp[*U] = Fp[V] + D;
				mPi[*U] = V;
			}
		}
	}
	cout << "WARNING : A path cannot be found " << endl ;
}

std::list<Vertex> ShortestPath::Get() {

	std::list<Vertex> RetVal ;
	RetVal.push_back(mDest) ;
	int CurrentNode;
	CurrentNode = mDest;
	std::cout << CurrentNode << "<-" ;
	while (CurrentNode != mSrc ) {
		CurrentNode = mPi[CurrentNode];
		RetVal.push_front(CurrentNode);
		std::cout << CurrentNode << "<-"  ;
	}
	std::cout << std::endl ;
	return RetVal;
}

}

}

}
