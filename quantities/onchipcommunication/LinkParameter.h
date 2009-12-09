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
#ifndef LINKPARAMETER_H_
#define LINKPARAMETER_H_

#include "core/Quantity.h"
#include "include/onchipcommunication/typedefs.h"

namespace cosi {

namespace quantity {

namespace onchipcommunication {

class LinkParameter : public cosi::Quantity {

public:

	LinkParameter();

	LinkParameter(cosi::library::onchipcommunication::Technology pTech);

	virtual ~LinkParameter();

	LinkParameter Get();

	void Set(LinkParameter pLp);

	void SetTechnology(cosi::library::onchipcommunication::Technology pTech) ;

	cosi::library::onchipcommunication::Technology GetTechnology() const ;

	bool operator ==(const LinkParameter &pP) const;

	bool operator <(const LinkParameter &pP) const;

	LinkParameter operator+(LinkParameter pT);
	LinkParameter operator-(LinkParameter pT);

	friend std::ostream
			& operator <<(std::ostream &pOs, const LinkParameter &pP);

	static std::string sQuantityType;
	static std::string sQuantityName;
private:
	cosi::library::onchipcommunication::Technology mTech;

};

}

}

}

#endif /*LINKPARAMTER_H_*/
