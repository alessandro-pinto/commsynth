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
#ifndef PORTS_H_
#define PORTS_H_

#include <iostream>
#include <string>
#include <map>
#include "core/IdGraph.h"
#include "Bandwidth.h"

namespace cosi {

class PortInterface {
public:
	enum tDirection {IN,OUT,INOUT};
	PortInterface();
	~PortInterface();
	tDirection GetDirection() const;
	void SetDirection(tDirection pDir);
	std::string GetName() const;
	void SetName(std::string pName);
	bool operator==(const PortInterface &pP) const;
	bool operator!=(const PortInterface &pP) const;
	friend std::ostream& operator<<(std::ostream &pOs, const PortInterface &pP);
private:
	std::string mName;
	tDirection mDir;
};

class Ports : public Quantity {
	std::map<std::string,PortInterface> mPorts;
	std::map<std::string,Edge> mBound;
public:
	Ports();
	~Ports();
	static std::string GetQuantityType();
	static std::string GetQuantityName();

	Ports Get();
	void Set(Ports pP);
	std::map<std::string,PortInterface> GetValue();

	void AddPort(std::string pName, std::string pType,
			PortInterface::tDirection pDir);
	void AddPort(std::string pName, PortInterface pIf);
	bool HasPortType(std::string pType);
	bool HasPort(std::string pName);

	bool IsBound(std::string pName);
	Edge BoundTo(std::string pName);
	void BoundTo(std::string pName, Edge pE);
	string IsBoundTo(Edge pE , PortInterface::tDirection pDir = PortInterface::OUT) ;

	typedef std::map<string,PortInterface>::iterator iterator;

	iterator Begin();
	iterator End();
	string Name(iterator It);
	PortInterface If(iterator It);

	string GetInputPortNotBound();
	string GetOutputPortNotBound();
	string GetInputOutputPortNotBound();

	std::vector<std::string> GetNotBoundIn();
	std::vector<std::string> GetNotBoundOut();
	std::vector<std::string> GetNotBoundInOut();

	bool operator==(Ports pP);

	Ports operator+(Ports pT);
	Ports operator-(Ports pT);

	friend std::ostream& operator<<(std::ostream &pOs, const Ports &pP);
	PortInterface operator[](std::string pName);

	static std::string sQuantityType;
	static std::string sQuantityName;
};

class PortRates : public Quantity {
	std::map<std::string,Bandwidth> mPortRates;
public:
	PortRates();
	virtual ~PortRates();

	PortRates Get();
	void Set(PortRates pP);

	std::map<std::string,Bandwidth> GetValue();

	bool HasPort(std::string pName);

	void AddRate(std::string pName, Bandwidth pB);
	void AddRate(std::string pName, double pValue);

	Bandwidth GetRate(std::string pName);

	bool operator==(PortRates pP);

	PortRates operator+(PortRates pP);

	void operator<<(PortRates pP);

	static std::string sQuantityType;
	static std::string sQuantityName;
};

class PortCapacities : public PortRates {
public:
	PortCapacities();

	~PortCapacities();

	PortCapacities Get();

	PortRates operator+(PortRates pP);

	static std::string sQuantityType;
	static std::string sQuantityName;
};

}

#endif /*PORTS_H_*/
