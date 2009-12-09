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
#ifndef COMMODITY_H_
#define COMMODITY_H_

#include "../core/Quantity.h"
#include "Bandwidth.h"
#include "RealLatency.h"

namespace cosi {

class Commodity : public cosi::Quantity {
	std::string mSourceName;
	std::string mDestName;
	int mSourceId;
	int mDestId;
	Bandwidth mAverageBandwidth;
	RealLatency mMaximumLatency ;
public:
	Commodity();
	virtual ~Commodity();

	void Set(Commodity C);
	Commodity Get();

	void SetValue(std::string SourceName, std::string DestName, int SourceId,
			int DestId, Bandwidth AverageBandwidth , RealLatency L );

	void SetValue(std::string SourceName, std::string DestName, int SourceId,
			int DestId, double AverageBandwidthValue , double Lvalue);

	std::string GetSourceName()const ;
	std::string GetDestName()const ;
	int GetSourceId()const ;
	int GetDestId()const ;
	Bandwidth GetAverageBandwidth()const ;
	RealLatency GetMaximumLatency() const ;
	double GetAverageBandwidthValue();
	double GetMaximumLatencyValue() ;

	bool operator<(const Commodity &pC) const ;

	bool operator==(const Commodity &pC)const ;

	Commodity operator+(Commodity pC);

	friend std::ostream& operator <<(std::ostream &pOs, const Commodity &pC);

	static std::string sQuantityName;
	static std::string sQuantityType;

};

}

#endif /*COMMODITY_H_*/
