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

#include "Implementation.h"

namespace cosi {

namespace commstruct {

namespace onchipcommunication {

Implementation::Implementation(std::string pName) :
	CommunicationStructure<ImplementationLabel>(pName) {
}

Implementation::~Implementation() {
}

Implementation Implementation::operator+(Implementation pC) {
	Implementation RetVal("");
	IdGraph::v_iterator U , V;
	for( U = this->v_begin(); U != this->v_end();U++) {
		RetVal.AddVertex(*U);
		RetVal.SetName(*U,GetName(*U));
		if ( pC.InV(*U)) {
			RetVal[*U].PRJ(Name)Set((*this)[*U].PRJ(Name)Get() - pC[*U].PRJ(Name)Get());
			RetVal[*U].PRJ(Type)Set((*this)[*U].PRJ(Type)Get() - pC[*U].PRJ(Type)Get());
			//take the superset
			if ( ! ((*this)[*U].PRJ(Ports)Get() - pC[*U].PRJ(Ports)Get() == pC[*U].PRJ(Ports)Get()) &&
					! ((*this)[*U].PRJ(Ports)Get() - pC[*U].PRJ(Ports)Get() == (*this)[*U].PRJ(Ports)Get()) ) {
				RetVal[*U].PRJ(Ports)Set(cosi::quantity::onchipcommunication::Ports());
			} else {
				RetVal[*U].PRJ(Ports)Set((*this)[*U].PRJ(Ports)Get() + pC[*U].PRJ(Ports)Get());
			}
			RetVal[*U].PRJ(Position)Set((*this)[*U].PRJ(Position)Get() - pC[*U].PRJ(Position)Get());
			RetVal[*U].PRJ(IpGeometry)Set((*this)[*U].PRJ(IpGeometry)Get() - pC[*U].PRJ(IpGeometry)Get());
			RetVal[*U].PRJ(TransferTable)Set((*this)[*U].PRJ(TransferTable)Get() + pC[*U].PRJ(TransferTable)Get());
			RetVal[*U].PRJ(NodeParameter)Set((*this)[*U].PRJ(NodeParameter)Get() - pC[*U].PRJ(NodeParameter)Get());
		} else {
			RetVal[*U].PRJ(Name)Set((*this)[*U].PRJ(Name)Get());
			RetVal[*U].PRJ(Type)Set((*this)[*U].PRJ(Type)Get());
			RetVal[*U].PRJ(Ports)Set((*this)[*U].PRJ(Ports)Get());
			RetVal[*U].PRJ(Position)Set((*this)[*U].PRJ(Position)Get());
			RetVal[*U].PRJ(IpGeometry)Set((*this)[*U].PRJ(IpGeometry)Get());
			RetVal[*U].PRJ(TransferTable)Set((*this)[*U].PRJ(TransferTable)Get());
			RetVal[*U].PRJ(NodeParameter)Set((*this)[*U].PRJ(NodeParameter)Get());

		}

	}

	for( V = pC.v_begin(); V != pC.v_end();V++) {
		if ( ! InV(*V)) {
			RetVal.AddVertex(*V);
			RetVal.SetName(*V,pC.GetName(*V));
			RetVal[*V].PRJ(Name)Set(pC[*V].PRJ(Name)Get());
			RetVal[*V].PRJ(Type)Set(pC[*V].PRJ(Type)Get());
			RetVal[*V].PRJ(Ports)Set(pC[*V].PRJ(Ports)Get());
			RetVal[*V].PRJ(Position)Set(pC[*V].PRJ(Position)Get());
			RetVal[*V].PRJ(IpGeometry)Set(pC[*V].PRJ(IpGeometry)Get());
			RetVal[*V].PRJ(TransferTable)Set(pC[*V].PRJ(TransferTable)Get());
			RetVal[*V].PRJ(NodeParameter)Set(pC[*V].PRJ(NodeParameter)Get());
		}
	}

	for( U = this->v_begin(); U != this->v_end();U++) {
		for( V = this->v_begin(); V != this->v_end();V++) {
			if ( InE(*U,*V)) {
				RetVal.AddEdge(*U,*V);
				Edge E(*U,*V);
				if ( pC.InE(*U,*V)) {
					RetVal[*U].PRJ(Name)Set((*this)[*U].PRJ(Name)Get() - pC[*U].PRJ(Name)Get());
					RetVal[*U].PRJ(Type)Set((*this)[*U].PRJ(Type)Get() - pC[*U].PRJ(Type)Get());
					RetVal[E].PRJ(CommoditySet)Set((*this)[E].PRJ(CommoditySet)Get() + pC[E].PRJ(CommoditySet)Get());
					RetVal[E].PRJ(WiringPath)Set((*this)[E].PRJ(WiringPath)Get() - pC[E].PRJ(WiringPath)Get());
					RetVal[E].PRJ(LinkParameter)Set((*this)[E].PRJ(LinkParameter)Get() - pC[E].PRJ(LinkParameter)Get());
				} else {
					RetVal[E].PRJ(Name)Set((*this)[E].PRJ(Name)Get() );
					RetVal[E].PRJ(Type)Set((*this)[E].PRJ(Type)Get() );
					RetVal[E].PRJ(CommoditySet)Set((*this)[E].PRJ(CommoditySet)Get() );
					RetVal[E].PRJ(WiringPath)Set((*this)[E].PRJ(WiringPath)Get() );
					RetVal[E].PRJ(LinkParameter)Set((*this)[E].PRJ(LinkParameter)Get() );
				}
			}
		}
	}

	for( U = pC.v_begin(); U != pC.v_end();U++) {
		for( V = pC.v_begin(); V != pC.v_end();V++) {
			if ( ! InE(*U,*V) && pC.InE(*U,*V)) {
				Edge E(*U,*V);
				RetVal.AddEdge(*U,*V);
				RetVal[E].PRJ(Name)Set(pC[E].PRJ(Name)Get() );
				RetVal[E].PRJ(Type)Set(pC[E].PRJ(Type)Get() );
				RetVal[E].PRJ(CommoditySet)Set(pC[E].PRJ(CommoditySet)Get() );
				RetVal[E].PRJ(WiringPath)Set(pC[E].PRJ(WiringPath)Get() );
				RetVal[E].PRJ(LinkParameter)Set(pC[E].PRJ(LinkParameter)Get() );
			}
		}
	}

	return RetVal;
}

}

}

}
