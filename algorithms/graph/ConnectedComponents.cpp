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
#include "ConnectedComponents.h"

namespace cosi {

namespace algorithms {

namespace graph {

using namespace std;

ConnectedComponents::ConnectedComponents(
		cosi::commstruct::graph::U_RealWeightedGraph* G) {
	mG = G ;
}
void ConnectedComponents::Run() {
	//Insert one set for each vertex
	IdGraph::v_iterator U, V;
	for (U = mG->v_begin() ; U != mG->v_end() ; U++) {
		set<Vertex> S;
		S.insert(*U) ;
		mComponents.insert(S) ;
	}

	for (U = mG->v_begin() ; U != mG->v_end() ; U++) {
		for (V = mG->v_begin() ; V != mG->v_end() ; V++) {
			if (mG->InE(*U, *V)) {
				if (Disjoint(*U, *V)) {
					Union(*U, *V);
				}
			}
		}
	}
}

bool ConnectedComponents::Disjoint(Vertex U, Vertex V) {
	set<set<Vertex> >::iterator It;
	for (It = mComponents.begin() ; It != mComponents.end() ; It++) {
		if (It->find(U) != It->end() && It->find(V) != It->end() )
			return false;
	}
	return true;
}

void ConnectedComponents::Union(Vertex U, Vertex V) {
	set< Vertex > New;
	set<set<Vertex> >::iterator It , ItU , ItV;
	for (It = mComponents.begin() ; It != mComponents.end() ; It++) {
		if (It->find(U) != It->end()) {
			ItU = It ;
		}
		if (It->find(V) != It->end()) {
			ItV = It ;
		}
	}
	New.insert(ItU->begin() , ItU->end()) ;
	New.insert(ItV->begin() , ItV->end()) ;
	mComponents.erase(ItU) ;
	mComponents.erase(ItV) ;
	mComponents.insert(New) ;
}

std::set< std::set<Vertex> > ConnectedComponents::GetComponents() {
	return mComponents ;
}

ConnectedComponents::~ConnectedComponents() {

}

}

}

}
