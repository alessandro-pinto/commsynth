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
#ifndef LABEL_HPP_
#define LABEL_HPP_

#include <string>
#include <sstream>
#include <list>
#include "Label.h"

namespace cosi {

template<class T1> class Label_1 : public Label, public T1 {

public:
	Label_1() {
		AddQuantityName(T1::sQuantityName) ;
	}

	std::string Print() {
		std::stringstream s;
		s << T1::sQuantityName << " of type "<< T1::sQuantityType << std::endl
				<< "Value = ";
		if (T1::IsBot()) {
			s << "No value"<< std::endl ;
		} else if (T1::IsAny()) {
			s << "Any value"<< std::endl;
		} else {
			s << T1::Get() << std::endl ;
		}
		return s.str() ;
	}
	;
};

template<class T1, class T2> class Label_2 : public Label, public T1, public T2 {

public:
	Label_2() {
		AddQuantityName(T1::sQuantityName) ;
		AddQuantityName(T2::sQuantityName) ;
	}

	std::string Print() {
		std::stringstream s;
		s << T1::sQuantityName << " of type "<< T1::sQuantityType << std::endl
				<< "Value = ";
		if (T1::IsBot()) {
			s << "No value"<< std::endl ;
		} else if (T1::IsAny()) {
			s << "Any value"<< std::endl;
		} else {
			s << T1::Get() << std::endl ;
		}
		s << T2::sQuantityName << " of type "<< T2::sQuantityType << std::endl
				<< "Value = ";
		if (T2::IsBot()) {
			s << "No value"<< std::endl ;
		} else if (T2::IsAny()) {
			s << "Any value"<< std::endl;
		} else {
			s << T2::Get() << std::endl ;
		}

		return s.str();
	}
	;
};

template<class T1, class T2, class T3> class Label_3 : public Label, public T1,
	public T2, public T3 {

public:
	Label_3() {
		AddQuantityName(T1::sQuantityName) ;
		AddQuantityName(T2::sQuantityName) ;
		AddQuantityName(T3::sQuantityName) ;
	}
	;
	~Label_3() {
	}
	;
	std::string Print() {
		std::stringstream s;
		s << T1::sQuantityName << " of type "<< T1::sQuantityType << std::endl
				<< "Value = ";
		if (T1::IsBot()) {
			s << "No value"<< std::endl ;
		} else if (T1::IsAny()) {
			s << "Any value"<< std::endl;
		} else {
			s << T1::Get() << std::endl ;
		}
		s << T2::sQuantityName << " of type "<< T2::sQuantityType << std::endl
				<< "Value = ";
		if (T2::IsBot()) {
			s << "No value"<< std::endl ;
		} else if (T2::IsAny()) {
			s << "Any value"<< std::endl;
		} else {
			s << T2::Get() << std::endl ;
		}
		s << T3::sQuantityName << " of type "<< T3::sQuantityType << std::endl
				<< "Value = ";
		if (T3::IsBot()) {
			s << "No value"<< std::endl ;
		} else if (T3::IsAny()) {
			s << "Any value"<< std::endl;
		} else {
			s << T3::Get() << std::endl ;
		}

		return s.str();
	}
	;

};

template<class T1, class T2, class T3, class T4> class Label_4 : public Label,
	public T1, public T2, public T3, public T4 {
public:
	Label_4() {
		AddQuantityName(T1::sQuantityName) ;
		AddQuantityName(T2::sQuantityName) ;
		AddQuantityName(T3::sQuantityName) ;
		AddQuantityName(T4::sQuantityName) ;
	}

	std::string Print() {
		std::stringstream s;
		s << T1::sQuantityName << " of type "<< T1::sQuantityType << std::endl
				<< "Value = ";
		if (T1::IsBot()) {
			s << "No value"<< std::endl ;
		} else if (T1::IsAny()) {
			s << "Any value"<< std::endl;
		} else {
			s << T1::Get() << std::endl ;
		}
		s << T2::sQuantityName << " of type "<< T2::sQuantityType << std::endl
				<< "Value = ";
		if (T2::IsBot()) {
			s << "No value"<< std::endl ;
		} else if (T2::IsAny()) {
			s << "Any value"<< std::endl;
		} else {
			s << T2::Get() << std::endl ;
		}
		s << T3::sQuantityName << " of type "<< T3::sQuantityType << std::endl
				<< "Value = ";
		if (T3::IsBot()) {
			s << "No value"<< std::endl ;
		} else if (T3::IsAny()) {
			s << "Any value"<< std::endl;
		} else {
			s << T3::Get() << std::endl ;
		}
		s << T4::sQuantityName << " of type "<< T4::sQuantityType << std::endl
				<< "Value = ";
		if (T4::IsBot()) {
			s << "No value"<< std::endl ;
		} else if (T4::IsAny()) {
			s << "Any value"<< std::endl;
		} else {
			s << T4::Get() << std::endl ;
		}

		return s.str();
	}
	;

};

template<class T1, class T2, class T3, class T4, class T5> class Label_5 :
	public Label, public T1, public T2, public T3, public T4, public T5 {
public:
	Label_5() {
		AddQuantityName(T1::sQuantityName) ;
		AddQuantityName(T2::sQuantityName) ;
		AddQuantityName(T3::sQuantityName) ;
		AddQuantityName(T4::sQuantityName) ;
		AddQuantityName(T5::sQuantityName) ;
	}

	std::string Print() {
		std::stringstream s;
		s << T1::sQuantityName << " of type "<< T1::sQuantityType << std::endl
				<< "Value = ";
		if (T1::IsBot()) {
			s << "No value"<< std::endl ;
		} else if (T1::IsAny()) {
			s << "Any value"<< std::endl;
		} else {
			s << T1::Get() << std::endl ;
		}
		s << T2::sQuantityName << " of type "<< T2::sQuantityType << std::endl
				<< "Value = ";
		if (T2::IsBot()) {
			s << "No value"<< std::endl ;
		} else if (T2::IsAny()) {
			s << "Any value"<< std::endl;
		} else {
			s << T2::Get() << std::endl ;
		}
		s << T3::sQuantityName << " of type "<< T3::sQuantityType << std::endl
				<< "Value = ";
		if (T3::IsBot()) {
			s << "No value"<< std::endl ;
		} else if (T3::IsAny()) {
			s << "Any value"<< std::endl;
		} else {
			s << T3::Get() << std::endl ;
		}
		s << T4::sQuantityName << " of type "<< T4::sQuantityType << std::endl
				<< "Value = ";
		if (T4::IsBot()) {
			s << "No value"<< std::endl ;
		} else if (T4::IsAny()) {
			s << "Any value"<< std::endl;
		} else {
			s << T4::Get() << std::endl ;
		}
		s << T5::sQuantityName << " of type "<< T5::sQuantityType << std::endl
				<< "Value = ";
		if (T5::IsBot()) {
			s << "No value"<< std::endl ;
		} else if (T5::IsAny()) {
			s << "Any value"<< std::endl;
		} else {
			s << T5::Get() << std::endl ;
		}

		return s.str();

	}
	;

};

template<class T1, class T2, class T3, class T4, class T5, class T6> class Label_6 :
	public Label, public T1, public T2, public T3, public T4, public T5,
	public T6 {
public:
	Label_6() {
		AddQuantityName(T1::sQuantityName) ;
		AddQuantityName(T2::sQuantityName) ;
		AddQuantityName(T3::sQuantityName) ;
		AddQuantityName(T4::sQuantityName) ;
		AddQuantityName(T5::sQuantityName) ;
		AddQuantityName(T6::sQuantityName) ;
	}

	std::string Print() {
		std::stringstream s;
		s << T1::sQuantityName << " of type "<< T1::sQuantityType << std::endl
				<< "Value = ";
		if (T1::IsBot()) {
			s << "No value"<< std::endl ;
		} else if (T1::IsAny()) {
			s << "Any value"<< std::endl;
		} else {
			s << T1::Get() << std::endl ;
		}
		s << T2::sQuantityName << " of type "<< T2::sQuantityType << std::endl
				<< "Value = ";
		if (T2::IsBot()) {
			s << "No value"<< std::endl ;
		} else if (T2::IsAny()) {
			s << "Any value"<< std::endl;
		} else {
			s << T2::Get() << std::endl ;
		}
		s << T3::sQuantityName << " of type "<< T3::sQuantityType << std::endl
				<< "Value = ";
		if (T3::IsBot()) {
			s << "No value"<< std::endl ;
		} else if (T3::IsAny()) {
			s << "Any value"<< std::endl;
		} else {
			s << T3::Get() << std::endl ;
		}
		s << T4::sQuantityName << " of type "<< T4::sQuantityType << std::endl
				<< "Value = ";
		if (T4::IsBot()) {
			s << "No value"<< std::endl ;
		} else if (T4::IsAny()) {
			s << "Any value"<< std::endl;
		} else {
			s << T4::Get() << std::endl ;
		}
		s << T5::sQuantityName << " of type "<< T5::sQuantityType << std::endl
				<< "Value = ";
		if (T5::IsBot()) {
			s << "No value"<< std::endl ;
		} else if (T5::IsAny()) {
			s << "Any value"<< std::endl;
		} else {
			s << T5::Get() << std::endl ;
		}
		s << T6::sQuantityName << " of type "<< T6::sQuantityType << std::endl
				<< "Value = ";
		if (T6::IsBot()) {
			s << "No value"<< std::endl ;
		} else if (T6::IsAny()) {
			s << "Any value"<< std::endl;
		} else {
			s << T6::Get() << std::endl ;
		}
		return s.str();

	}
	;

};

template<class T1, class T2, class T3, class T4, class T5, class T6, class T7> class Label_7 :
	public Label, public T1, public T2, public T3, public T4, public T5,
	public T6, public T7 {
public:
	Label_7() {
		AddQuantityName(T1::sQuantityName) ;
		AddQuantityName(T2::sQuantityName) ;
		AddQuantityName(T3::sQuantityName) ;
		AddQuantityName(T4::sQuantityName) ;
		AddQuantityName(T5::sQuantityName) ;
		AddQuantityName(T6::sQuantityName) ;
		AddQuantityName(T7::sQuantityName) ;
	}

	std::string Print() {
		std::stringstream s;
		s << T1::sQuantityName << " of type "<< T1::sQuantityType << std::endl
				<< "Value = ";
		if (T1::IsBot()) {
			s << "No value"<< std::endl ;
		} else if (T1::IsAny()) {
			s << "Any value"<< std::endl;
		} else {
			s << T1::Get() << std::endl ;
		}
		s << T2::sQuantityName << " of type "<< T2::sQuantityType << std::endl
				<< "Value = ";
		if (T2::IsBot()) {
			s << "No value"<< std::endl ;
		} else if (T2::IsAny()) {
			s << "Any value"<< std::endl;
		} else {
			s << T2::Get() << std::endl ;
		}
		s << T3::sQuantityName << " of type "<< T3::sQuantityType << std::endl
				<< "Value = ";
		if (T3::IsBot()) {
			s << "No value"<< std::endl ;
		} else if (T3::IsAny()) {
			s << "Any value"<< std::endl;
		} else {
			s << T3::Get() << std::endl ;
		}
		s << T4::sQuantityName << " of type "<< T4::sQuantityType << std::endl
				<< "Value = ";
		if (T4::IsBot()) {
			s << "No value"<< std::endl ;
		} else if (T4::IsAny()) {
			s << "Any value"<< std::endl;
		} else {
			s << T4::Get() << std::endl ;
		}
		s << T5::sQuantityName << " of type "<< T5::sQuantityType << std::endl
				<< "Value = ";
		if (T5::IsBot()) {
			s << "No value"<< std::endl ;
		} else if (T5::IsAny()) {
			s << "Any value"<< std::endl;
		} else {
			s << T5::Get() << std::endl ;
		}
		s << T6::sQuantityName << " of type "<< T6::sQuantityType << std::endl
				<< "Value = ";
		if (T6::IsBot()) {
			s << "No value"<< std::endl ;
		} else if (T6::IsAny()) {
			s << "Any value"<< std::endl;
		} else {
			s << T6::Get() << std::endl ;
		}
		s << T7::sQuantityName << " of type "<< T7::sQuantityType << std::endl
				<< "Value = ";
		if (T7::IsBot()) {
			s << "No value"<< std::endl ;
		} else if (T7::IsAny()) {
			s << "Any value"<< std::endl;
		} else {
			s << T7::Get() << std::endl ;
		}
		return s.str();

	}
	;

};

template<class T1, class T2, class T3, class T4, class T5, class T6, class T7,
		class T8> class Label_8 : public Label, public T1, public T2,
	public T3, public T4, public T5, public T6, public T7, public T8 {
public:
	Label_8() {
		AddQuantityName(T1::sQuantityName) ;
		AddQuantityName(T2::sQuantityName) ;
		AddQuantityName(T3::sQuantityName) ;
		AddQuantityName(T4::sQuantityName) ;
		AddQuantityName(T5::sQuantityName) ;
		AddQuantityName(T6::sQuantityName) ;
		AddQuantityName(T7::sQuantityName) ;
		AddQuantityName(T8::sQuantityName) ;
	}

	std::string Print() {
		std::stringstream s;
		s << T1::sQuantityName << " of type "<< T1::sQuantityType << std::endl
				<< "Value = ";
		if (T1::IsBot()) {
			s << "No value"<< std::endl ;
		} else if (T1::IsAny()) {
			s << "Any value"<< std::endl;
		} else {
			s << T1::Get() << std::endl ;
		}
		s << T2::sQuantityName << " of type "<< T2::sQuantityType << std::endl
				<< "Value = ";
		if (T2::IsBot()) {
			s << "No value"<< std::endl ;
		} else if (T2::IsAny()) {
			s << "Any value"<< std::endl;
		} else {
			s << T2::Get() << std::endl ;
		}
		s << T3::sQuantityName << " of type "<< T3::sQuantityType << std::endl
				<< "Value = ";
		if (T3::IsBot()) {
			s << "No value"<< std::endl ;
		} else if (T3::IsAny()) {
			s << "Any value"<< std::endl;
		} else {
			s << T3::Get() << std::endl ;
		}
		s << T4::sQuantityName << " of type "<< T4::sQuantityType << std::endl
				<< "Value = ";
		if (T4::IsBot()) {
			s << "No value"<< std::endl ;
		} else if (T4::IsAny()) {
			s << "Any value"<< std::endl;
		} else {
			s << T4::Get() << std::endl ;
		}
		s << T5::sQuantityName << " of type "<< T5::sQuantityType << std::endl
				<< "Value = ";
		if (T5::IsBot()) {
			s << "No value"<< std::endl ;
		} else if (T5::IsAny()) {
			s << "Any value"<< std::endl;
		} else {
			s << T5::Get() << std::endl ;
		}
		s << T6::sQuantityName << " of type "<< T6::sQuantityType << std::endl
				<< "Value = ";
		if (T6::IsBot()) {
			s << "No value"<< std::endl ;
		} else if (T6::IsAny()) {
			s << "Any value"<< std::endl;
		} else {
			s << T6::Get() << std::endl ;
		}
		s << T7::sQuantityName << " of type "<< T7::sQuantityType << std::endl
				<< "Value = ";
		if (T7::IsBot()) {
			s << "No value"<< std::endl ;
		} else if (T7::IsAny()) {
			s << "Any value"<< std::endl;
		} else {
			s << T7::Get() << std::endl ;
		}
		s << T8::sQuantityName << " of type "<< T8::sQuantityType << std::endl
				<< "Value = ";
		if (T8::IsBot()) {
			s << "No value"<< std::endl ;
		} else if (T8::IsAny()) {
			s << "Any value"<< std::endl;
		} else {
			s << T8::Get() << std::endl ;
		}
		return s.str();

	}
	;

};

template<class T1, class T2, class T3, class T4, class T5, class T6, class T7,
		class T8, class T9> class Label_9 : public Label, public T1, public T2,
	public T3, public T4, public T5, public T6, public T7, public T8, public T9 {
public:
	Label_9() {
		AddQuantityName(T1::sQuantityName) ;
		AddQuantityName(T2::sQuantityName) ;
		AddQuantityName(T3::sQuantityName) ;
		AddQuantityName(T4::sQuantityName) ;
		AddQuantityName(T5::sQuantityName) ;
		AddQuantityName(T6::sQuantityName) ;
		AddQuantityName(T7::sQuantityName) ;
		AddQuantityName(T8::sQuantityName) ;
		AddQuantityName(T9::sQuantityName) ;
	}

	std::string Print() {
		std::stringstream s;
		s << T1::sQuantityName << " of type "<< T1::sQuantityType << std::endl
				<< "Value = ";
		if (T1::IsBot()) {
			s << "No value"<< std::endl ;
		} else if (T1::IsAny()) {
			s << "Any value"<< std::endl;
		} else {
			s << T1::Get() << std::endl ;
		}
		s << T2::sQuantityName << " of type "<< T2::sQuantityType << std::endl
				<< "Value = ";
		if (T2::IsBot()) {
			s << "No value"<< std::endl ;
		} else if (T2::IsAny()) {
			s << "Any value"<< std::endl;
		} else {
			s << T2::Get() << std::endl ;
		}
		s << T3::sQuantityName << " of type "<< T3::sQuantityType << std::endl
				<< "Value = ";
		if (T3::IsBot()) {
			s << "No value"<< std::endl ;
		} else if (T3::IsAny()) {
			s << "Any value"<< std::endl;
		} else {
			s << T3::Get() << std::endl ;
		}
		s << T4::sQuantityName << " of type "<< T4::sQuantityType << std::endl
				<< "Value = ";
		if (T4::IsBot()) {
			s << "No value"<< std::endl ;
		} else if (T4::IsAny()) {
			s << "Any value"<< std::endl;
		} else {
			s << T4::Get() << std::endl ;
		}
		s << T5::sQuantityName << " of type "<< T5::sQuantityType << std::endl
				<< "Value = ";
		if (T5::IsBot()) {
			s << "No value"<< std::endl ;
		} else if (T5::IsAny()) {
			s << "Any value"<< std::endl;
		} else {
			s << T5::Get() << std::endl ;
		}
		s << T6::sQuantityName << " of type "<< T6::sQuantityType << std::endl
				<< "Value = ";
		if (T6::IsBot()) {
			s << "No value"<< std::endl ;
		} else if (T6::IsAny()) {
			s << "Any value"<< std::endl;
		} else {
			s << T6::Get() << std::endl ;
		}
		s << T7::sQuantityName << " of type "<< T7::sQuantityType << std::endl
				<< "Value = ";
		if (T7::IsBot()) {
			s << "No value"<< std::endl ;
		} else if (T7::IsAny()) {
			s << "Any value"<< std::endl;
		} else {
			s << T7::Get() << std::endl ;
		}
		s << T8::sQuantityName << " of type "<< T8::sQuantityType << std::endl
				<< "Value = ";
		if (T8::IsBot()) {
			s << "No value"<< std::endl ;
		} else if (T8::IsAny()) {
			s << "Any value"<< std::endl;
		} else {
			s << T8::Get() << std::endl ;
		}
		s << T9::sQuantityName << " of type "<< T9::sQuantityType << std::endl
				<< "Value = ";
		if (T9::IsBot()) {
			s << "No value"<< std::endl ;
		} else if (T9::IsAny()) {
			s << "Any value"<< std::endl;
		} else {
			s << T9::Get() << std::endl ;
		}
		return s.str();

	}
	;

};

template<class T1, class T2, class T3, class T4, class T5, class T6, class T7,
		class T8, class T9, class T10> class Label_10 : public Label,
	public T1, public T2, public T3, public T4, public T5, public T6,
	public T7, public T8, public T9, public T10 {
public:
	Label_10() {
		AddQuantityName(T1::sQuantityName) ;
		AddQuantityName(T2::sQuantityName) ;
		AddQuantityName(T3::sQuantityName) ;
		AddQuantityName(T4::sQuantityName) ;
		AddQuantityName(T5::sQuantityName) ;
		AddQuantityName(T6::sQuantityName) ;
		AddQuantityName(T7::sQuantityName) ;
		AddQuantityName(T8::sQuantityName) ;
		AddQuantityName(T9::sQuantityName) ;
		AddQuantityName(T10::sQuantityName) ;
	}

	std::string Print() {
		std::stringstream s;
		s << T1::sQuantityName << " of type "<< T1::sQuantityType << std::endl
				<< "Value = ";
		if (T1::IsBot()) {
			s << "No value"<< std::endl ;
		} else if (T1::IsAny()) {
			s << "Any value"<< std::endl;
		} else {
			s << T1::Get() << std::endl ;
		}
		s << T2::sQuantityName << " of type "<< T2::sQuantityType << std::endl
				<< "Value = ";
		if (T2::IsBot()) {
			s << "No value"<< std::endl ;
		} else if (T2::IsAny()) {
			s << "Any value"<< std::endl;
		} else {
			s << T2::Get() << std::endl ;
		}
		s << T3::sQuantityName << " of type "<< T3::sQuantityType << std::endl
				<< "Value = ";
		if (T3::IsBot()) {
			s << "No value"<< std::endl ;
		} else if (T3::IsAny()) {
			s << "Any value"<< std::endl;
		} else {
			s << T3::Get() << std::endl ;
		}
		s << T4::sQuantityName << " of type "<< T4::sQuantityType << std::endl
				<< "Value = ";
		if (T4::IsBot()) {
			s << "No value"<< std::endl ;
		} else if (T4::IsAny()) {
			s << "Any value"<< std::endl;
		} else {
			s << T4::Get() << std::endl ;
		}
		s << T5::sQuantityName << " of type "<< T5::sQuantityType << std::endl
				<< "Value = ";
		if (T5::IsBot()) {
			s << "No value"<< std::endl ;
		} else if (T5::IsAny()) {
			s << "Any value"<< std::endl;
		} else {
			s << T5::Get() << std::endl ;
		}
		s << T6::sQuantityName << " of type "<< T6::sQuantityType << std::endl
				<< "Value = ";
		if (T6::IsBot()) {
			s << "No value"<< std::endl ;
		} else if (T6::IsAny()) {
			s << "Any value"<< std::endl;
		} else {
			s << T6::Get() << std::endl ;
		}
		s << T7::sQuantityName << " of type "<< T7::sQuantityType << std::endl
				<< "Value = ";
		if (T7::IsBot()) {
			s << "No value"<< std::endl ;
		} else if (T7::IsAny()) {
			s << "Any value"<< std::endl;
		} else {
			s << T7::Get() << std::endl ;
		}
		s << T8::sQuantityName << " of type "<< T8::sQuantityType << std::endl
				<< "Value = ";
		if (T8::IsBot()) {
			s << "No value"<< std::endl ;
		} else if (T8::IsAny()) {
			s << "Any value"<< std::endl;
		} else {
			s << T8::Get() << std::endl ;
		}
		s << T9::sQuantityName << " of type "<< T9::sQuantityType << std::endl
				<< "Value = ";
		if (T9::IsBot()) {
			s << "No value"<< std::endl ;
		} else if (T9::IsAny()) {
			s << "Any value"<< std::endl;
		} else {
			s << T9::Get() << std::endl ;
		}
		s << T10::sQuantityName << " of type "<< T10::sQuantityType << std::endl
				<< "Value = ";
		if (T10::IsBot()) {
			s << "No value"<< std::endl ;
		} else if (T10::IsAny()) {
			s << "Any value"<< std::endl;
		} else {
			s << T10::Get() << std::endl ;
		}
		return s.str();
	}
	;

};

}

#endif /*LABEL_HPP_*/
