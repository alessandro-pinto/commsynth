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
#ifndef WIRINGRULE_H_
#define WIRINGRULE_H_

#include <iostream>
#include <string>
#include <map>
#include <set>
#include "newmat.h"
#include "float.h"
#include "core/CommunicationStructure.hpp"
#include "label/Label.hpp"
#include "core/Quantity.h"
#include "quantities/WiringPath.h"
#include "quantities/Position.h"
#include "environment/bas/InstSurface.h"
#include "networks/wiredbas/WiredBasImplementation.h"

namespace cosi {

namespace rules {

namespace wiredbas {

class WiringRule {
public:

	class SurfacePair : public Quantity {
		std::pair<std::string ,std::string> mPair;
public:
		SurfacePair() {
		}
		SurfacePair(std::string pFirst, std::string pSecond) {
			mPair.first = pFirst ;
			mPair.second = pSecond ;
		}
		;
		~SurfacePair() {
		}
		;

		SurfacePair Get() {
			return (*this);
		}

		std::string GetFirst() {
			return mPair.first;
		}
		;
		void SetFirst(std::string pFirst) {
			mPair.first = pFirst ;
		}
		;
		std::string GetSecond() {
			return mPair.second;
		}
		;
		void SetSecond(std::string pSecond) {
			mPair.second = pSecond ;
		}
		;
		bool Contains(std::string pS) {
			return ( (mPair.first == pS) || (mPair.second==pS));
		}
		;
		bool Intersects(SurfacePair pP) {
			return ( (GetFirst() == pP.GetFirst()) ||(GetFirst()
					== pP.GetSecond())||(GetSecond() == pP.GetFirst())
					||(GetSecond() == pP.GetSecond()));
		}
		;

		bool operator==(SurfacePair pP) {
			return ( (GetFirst() == pP.GetFirst() && GetSecond()
					== pP.GetSecond()) ||(GetFirst() == pP.GetSecond()
					&& GetSecond() == pP.GetFirst()));
		}
		;

		friend std::ostream& operator <<(std::ostream &pOs,
				const SurfacePair &pS);

		static std::string sQuantityName;
		static std::string sQuantityType;

	};
			WiringRule(
					std::map< std::string , cosi::environment::bas::InstSurface > pInstallationSurfaces);
	virtual ~WiringRule();
	virtual WiringPath GetWiringPath(Position pStart, Position pEnd);
	cosi::commstruct::wiredbas::Implementation* GetConnectivityGraph() ;
private:
	std::map< std::string , cosi::environment::bas::InstSurface >
			mInstallationSurfaces;

	CommunicationStructure< Label_2<Position,SurfacePair> > mConnectivityGraph;

	void BuildSurfaceConnectivityGraph();

	Position FindIntersectionPoint(std::string pS1, std::string pS2);

	Position LineIntersection(ColumnVector a1, ColumnVector b1, double Ub1 ,
			ColumnVector a2, ColumnVector b2,double Ub2);

	Position ClosestPoint(Position pP, std::string pSurface);

	int mLastId;

};

}
}
}
#endif /*WIRINGRULE_H_*/
