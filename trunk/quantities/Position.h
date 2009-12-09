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
#ifndef POSITION_H_
#define POSITION_H_

#include <iostream>
#include <string>
#include <vector>
#include "math.h"
#include "../core/Quantity.h"

namespace cosi {

class Position : public Quantity {
	std::vector<double> mPosition;
public:
	Position();
	Position(double pX, double pY, double pZ);
	~Position();

	Position Get();
	void Set(Position pP);
	std::vector<double> GetValue();

	double GetX() const;
	double GetY() const;
	double GetZ() const;

	bool operator ==(Position pP);

	Position operator+(Position pT);

	Position operator-(Position pT);

	bool operator<(const Position &pP) const ;

	friend std::ostream& operator <<(std::ostream &pOs, const Position &pP);

	static std::string sQuantityType;
	static std::string sQuantityName;

};

extern double Norm1(Position P1, Position P2);
extern double Norm2(Position P1, Position P2);

}

#endif /*POSITION_H_*/
