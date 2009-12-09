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

#include "Specification.h"

namespace cosi {

namespace commstruct {

namespace onchipcommunication {

using namespace cosi::quantity::onchipcommunication ;

Specification::Specification(std::string pName) :
	CommunicationStructure<SpecificationLabel>(pName) {
}

Specification::~Specification() {
}

Specification Specification::operator+(Specification pC) {
	//The name has to be decided after composition
	Specification RetVal("");

	IdGraph::v_iterator U , V;
	for( U = this->v_begin(); U != this->v_end();U++) {
		RetVal.AddVertex(*U);
		RetVal.SetName(*U,GetName(*U));
		if ( pC.InV(*U)) {
			RetVal[*U].PRJ(Name)Set((*this)[*U].PRJ(Name)Get() + pC[*U].PRJ(Name)Get());
			RetVal[*U].PRJ(Ports)Set((*this)[*U].PRJ(Ports)Get() + pC[*U].PRJ(Ports)Get());
			RetVal[*U].PRJ(Position)Set((*this)[*U].PRJ(Position)Get() + pC[*U].PRJ(Position)Get());
			RetVal[*U].PRJ(IpGeometry)Set((*this)[*U].PRJ(IpGeometry)Get() + pC[*U].PRJ(IpGeometry)Get());
		} else {
			RetVal[*U].PRJ(Name)Set((*this)[*U].PRJ(Name)Get());
			RetVal[*U].PRJ(Ports)Set((*this)[*U].PRJ(Ports)Get());
			RetVal[*U].PRJ(Position)Set((*this)[*U].PRJ(Position)Get());
			RetVal[*U].PRJ(IpGeometry)Set((*this)[*U].PRJ(IpGeometry)Get());
		}

	}

	for( V = pC.v_begin(); V != pC.v_end();V++) {
		if ( ! InV(*V)) {
			RetVal.AddVertex(*V);
			RetVal.SetName(*V,pC.GetName(*V));
			RetVal[*V].PRJ(Name)Set(pC[*V].PRJ(Name)Get());
			RetVal[*V].PRJ(Ports)Set(pC[*V].PRJ(Ports)Get());
			RetVal[*V].PRJ(Position)Set(pC[*V].PRJ(Position)Get());
			RetVal[*V].PRJ(IpGeometry)Set(pC[*V].PRJ(IpGeometry)Get());
		}
	}

	for( U = this->v_begin(); U != this->v_end();U++) {
		for( V = this->v_begin(); V != this->v_end();V++) {
			if ( InE(*U,*V)) {
				RetVal.AddEdge(*U,*V);
				Edge E(*U,*V);
				if ( pC.InE(*U,*V)) {
					RetVal[E].PRJ(CommoditySet)Set((*this)[E].PRJ(CommoditySet)Get() + pC[E].PRJ(CommoditySet)Get());
				} else {
					RetVal[E].PRJ(CommoditySet)Set((*this)[E].PRJ(CommoditySet)Get() );
				}
			}
		}
	}

	for( U = pC.v_begin(); U != pC.v_end();U++) {
		for( V = pC.v_begin(); V != pC.v_end();V++) {
			if ( ! InE(*U,*V) && pC.InE(*U,*V)) {
				Edge E(*U,*V);
				RetVal.AddEdge(*U,*V);
				RetVal[E].PRJ(CommoditySet)Set(pC[E].PRJ(CommoditySet)Get() );
			}
		}
	}

	return RetVal;
}

}

}

}
