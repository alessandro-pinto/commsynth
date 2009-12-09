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
#include "HmetisInterface.h"

namespace cosi {
namespace io {
namespace onchipcommunication {

using namespace std;

vector<set<int> > ShmetisPartition(
		cosi::commstruct::onchipcommunication::Specification* Spec, int Nparts,
		int Unbalance) {

	ofstream F;

	//We assume simply that the weight of the hyperedges is the
	//bandwidth and that the weight of the ip is 1 but it woudl be better to have
	//the area as a weight

	map<int,int> VertexId;
	map<int,int> IdVertex;

	string FileName;
	FileName = Configuration->GetTmp() + "/graph.hgr";
	F.open(FileName.c_str()) ;
	F << Spec->Size() << " "<< Spec->Order() << " 1"<< endl ;
	IdGraph::v_iterator U, V;

	int Id = 1;
	for (U = Spec->v_begin() ; U != Spec->v_end() ; U++) {
		VertexId[*U] = Id ;
		IdVertex[Id] = *U ;
		Id++;
	}

	for (U = Spec->v_begin() ; U != Spec->v_end() ; U++) {
		for (V = Spec->v_begin() ; V != Spec->v_end() ; V++) {
			if (Spec->InE(*U, *V)) {
				F << (int)((*Spec)[Edge(*U,*V)].PRJ(CommoditySet)GetAggregateBandwidthValue()/(double)1000)
						<< " "<< VertexId[*U]<< " "<< VertexId[*V]<< endl ;
			}
		}
	}

	F.close() ;

	stringstream CmdLine;
	CmdLine << Configuration->GetShmetisRoot() << "/"
			<< Configuration->GetShmetisExec() << " "<< Configuration->GetTmp()
			<< "/graph.hgr "<< Nparts << " "<< Unbalance ;
	system((CmdLine.str()).c_str());

	//parse the result back
	vector<set<int> > Partitions(Nparts);
	ifstream If;
	stringstream Fname;
	Fname << Configuration->GetTmp() << "/graph.hgr.part."<< Nparts ;
	If.open((Fname.str()).c_str());
	int Token = 0;
	for (Id = 1; Id <= Spec->Order() ; Id ++) {
		If >> Token;
		Partitions[Token].insert(IdVertex[Id]);
		cout << "Vertex "<< IdVertex[Id]<< " in partition "<< Token << endl;
	}
	If.close();
	return Partitions ;

}

}
}
}
