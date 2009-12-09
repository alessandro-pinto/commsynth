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

#ifndef QUANTITY_H_
#define QUANTITY_H_

#include <iostream>
#include <string>

namespace cosi {

/** This class defines the basic quantity data type. This basic class defines the
 * following three attributes:
 * 	-Ordered that specifies if the quantity is ordered or not (i.e. the ordering is flat)
 * 	-Any that specifies if the value held by the quantity is "any value"
 * 	-Bot that specifies if the value held by the quantity is "no value"
 *
 * All quantities should be derived from this base class.
 *
 *@author Alessandro Pinto,
 *@author The Donald O. Pederson Center for Electronic System Design
 *@author Electrical Engineering Department
 *@author University of California, Berkeley
 *@author 545P Cory Hall, Berkeley, CA 94720
 *@author +1 (510) 642 9670
 *@author http://www.eecs.berkeley.edu/~apinto
 *
 *@version $Id: Quantity.h,v 1.10 2008/12/07 23:20:21 apinto Exp $
 */

class Quantity {
	/**\brief This member is true if value associated with the quantity is "any value".*/
	bool mAny;
	/**\brief This member is true if value associated with the quantity is "no value".*/
	bool mBot;
	/**\brief This member is true if domain of the quantity is ordered and false otherwise
	 * (this field may not be needed).*/
	bool mOrdered;
public:
	/**\brief Defailt constructor
	 *
	 * By default this constructor sets the fields such that the quantity is not
	 * ordered, and holds no value.
	 * */
	Quantity();
	/**\brief Default destructor.*/
	virtual ~Quantity();
	/**\brief Returns true if the quantity holds "any value".*/
	bool IsAny() const;
	/**\brief If the paramter pAny is true, the quantity holds "any value".*/
	void IsAny(bool pAny);
	/**\brief Returns true if the quantity holds "no value".*/
	bool IsBot() const;
	/**\brief If the paramter pAny is true, the quantity holds "no value".*/
	void IsBot(bool pBot);
	/**\brief Returns true if the domain of the quantity is ordered.*/
	bool IsOrdered() const ;
	/**\brief If the paramter pAny is true, the domain of the quantity is ordered (and an order should be
	 * define by three operators: <, >, ==.*/
	void IsOrdered(bool pOrdered);


};


/**This class defines a quantity that takes on value in \f$R\f$.
 * This class has a private member that is a of type double.
 *
 *@author Alessandro Pinto,
 *@author The Donald O. Pederson Center for Electronic System Design
 *@author Electrical Engineering Department
 *@author University of California, Berkeley
 *@author 545P Cory Hall, Berkeley, CA 94720
 *@author +1 (510) 642 9670
 *@author http://www.eecs.berkeley.edu/~apinto
 *
 *@version $Id: Quantity.h,v 1.10 2008/12/07 23:20:21 apinto Exp $
 */

class RealQuantity : public Quantity {
protected:
	double mValue;
public:
	RealQuantity();
	RealQuantity(double pValue);
	static std::string QuantityType();
	RealQuantity Get() ;
	void Set(RealQuantity pValue) ;
	double GetValue() const;
	void SetValue(double pValue);
	friend std::ostream& operator<<(std::ostream &pOs , const RealQuantity &pL) ;

	static std::string sQuantityType;
	static std::string sQuantityName;
};



class IntQuantity : public Quantity {
protected:
	int mValue;
public:
	IntQuantity();
	IntQuantity(int pValue);
	static std::string QuantityType();
	int GetValue() const;
	void SetValue(int pValue);
	static std::string sQuantityType;
};



class StringQuantity : public Quantity {
protected:
	std::string mValue;
public:
	StringQuantity();
	StringQuantity(std::string pValue);
	static std::string QuantityType();
	std::string GetValue() const;
	void SetValue(std::string pValue);
	static std::string sQuantityType;
};



}
#endif /*QUANTITY_H_*/
