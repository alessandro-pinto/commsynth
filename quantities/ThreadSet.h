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
#ifndef THREADSET_H_
#define THREADSET_H_

#include <string>
#include <set>
#include <algorithm>
#include "core/Quantity.h"
#include "quantities/Thread.h"

namespace cosi
{

class ThreadSet : public cosi::Quantity
{
	std::set<Thread> mSet ;
public:

	typedef std::set<Thread>::iterator iterator ;

	ThreadSet();

	~ThreadSet();

	ThreadSet Get() ;

	void Set(ThreadSet pSet ) ;

	std::set<Thread> GetValue() ;

	void SetValue(std::set<Thread> pSet) ;

	void AddThread(Thread pT);

	int Size();

	Thread GetLongestMessage() ;

	iterator ThreadSetBegin() ;

	iterator ThreadSetEnd()  ;


	bool operator<(ThreadSet pSet) ;

	bool operator==(ThreadSet pSet) ;

	ThreadSet operator+(ThreadSet pSet) ;

	friend std::ostream& operator <<(std::ostream &pOs , const ThreadSet &pT) ;

	static std::string sQuantityName;
	static std::string sQuantityType;
};

}

#endif /*THREADSET_H_*/
