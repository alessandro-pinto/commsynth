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
#include "Factory.h"

namespace cosi {

Factory::Factory() {
}

Factory::~Factory() {
}

//Define the object to generate
void Factory::AllocateNetwork(string pType) {
	if (pType=="NocSpecification") {
		mNetwork = new NocSpecification("");
	} else if (pType =="") {

	} else if (pType =="") {

	}
}
void Factory::SetNetworkName(string pName) {
	mNetwork->SetName(pName) ;
}

//Fill in the graph
void Factory::AddVertex(Vertex pV) {
	mNetwork->AddVertex(pV) ;
}

void Factory::AddVertex(Vertex pV, string pName) {
	mNetwork->AddVertex(pV) ;
	mNetwork->SetName(pV, pName) ;
}

void Factory::AddEdge(Edge pE) {
	mNetwork->AddEdge(pE);
}

void Factory::AddEdge(Edge pE, string pName) {
	mNetwork->AddEdge(pE);
	mNetwork->SetName(pE, pName) ;
}

void Factory::SetLabelMapEntry(string pMapName, Vertex pV,
		list<string> pString, list<int> pInt, list<double> pDouble) {

	//Depends on the name of map
	if (pMapName == "Area") {
		double Area = *(pDouble.begin()) ;
		dynamic_cast<NocSpecification*>(mNetwork)->MapArea::Set(pV, Area ) ;
	} else if (pMapName == "InterfaceType") {
		((MapInterfaceType*) mNetwork)->Set(pV,*(pString.begin())) ;
	}

}
void Factory::SetLabelMapEntry(string pMapName, Edge pE, list<string> pString,
		list<int> pInt, list<double> pDouble) {

	if (pMapName == "Bandwidth") {
		double Tmp = *(pDouble.begin()) ;
		cerr << Tmp << endl ;
		dynamic_cast<NocSpecification*>(mNetwork)->MapBandwidth::Set(pE,Tmp) ;
	} else if (pMapName == "HopConstraint") {
		int Tmp = *(pInt.begin()) ;
		cerr << Tmp << endl ;
		dynamic_cast<NocSpecification*>(mNetwork)->MapHopConstraint::Set(pE, Tmp ) ;
	}

}

//Return the object
IdGraph* Factory::GetNetwork() {
	return mNetwork ;
}

}
