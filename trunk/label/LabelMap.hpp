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

#ifndef LABELMAP_H_
#define LABELMAP_H_

#include <map>
#include <set>
#include <string>
#include "../core/IdGraph.h"

using namespace std;
namespace cosi {

/** This template class defines a map between two objects.
 * In COSI, this template class is used to attach labels to
 * nodes and links.
 *
 *@author Alessandro Pinto,
 *@author The Donald O. Pederson Center for Electronic System Design
 *@author Electrical Engineering Department
 *@author University of California, Berkeley
 *@author 545P Cory Hall, Berkeley, CA 94720
 *@author +1 (510) 642 9670
 *@author http://www.eecs.berkeley.edu/~apinto
 *
 *@version $Id: LabelMap.hpp,v 1.12 2008/12/09 23:28:22 apinto Exp $
 */

template<class T2> class LabelMap {
public:

	/**\brief Default constructor.*/
	LabelMap() {
	}
	;

	/**\brief Destructor.
	 *
	 * The destructor clears the intermal map.*/
	~LabelMap() {
		mV.clear();
		mE.clear();
	}
	;

	/**\brief Assigns the value pD to pK.*/

	void Set(Vertex V, T2 pD) {
		mV[V] = pD;
	}
	;

	void Set(Edge E, T2 pD) {
		mE[E] = pD;
	}
	;

	void Erase(Vertex V) {
		mV.erase(V);
	}
	;

	void Erase(Edge E) {
		mE.erase(E);
	}
	;

	/**\brief Returns the value associated to pK.
	 *
	 * If no value has been associated to pK, this
	 * function returns the default object T2(). A default
	 * constructur should always be availalbe for type T2.
	 * */

	T2& Get(Vertex V) {
		if (mV.find(V) != mV.end() )
			return mV[V];
		else {
			mV[V] = T2() ;
			return mV[V];
		}
	}
	;

	T2& Get(Edge E) {
		if (mE.find(E) != mE.end() )
			return mE[E];
		else {
			mE[E] = T2() ;
			return mE[E] ;
		}
	}
	;

	/**\brief Returns false if no value has been
	 * previously assigned to pK (by an appropiate call
	 * to the Set function).
	 *
	 * @see Set
	 * */
	bool Assigned(Vertex V) {
		if (mV.find(V) != mV.end() )
			return false;
		else
			return true;
	}
	;

	bool Assigned(Edge E) {
		if (mE.find(E) != mE.end() )
			return false;
		else
			return true;
	}
	;

	multiset<string> GetQuantityNames() {
		T2 Dummy;
		return Dummy.GetQuantityNames() ;
	}

private:

	/**\brief The internal map storage.*/
	map<Vertex,T2> mV;
	map<Edge,T2> mE;

};

}

#endif /*LABELMAP_H_*/

