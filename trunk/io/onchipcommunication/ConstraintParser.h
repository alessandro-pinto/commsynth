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
#include "../../include/CosiGlobal.h"

#ifndef CONSTRAINTPARSER_H_
#define CONSTRAINTPARSER_H_

#include <set>
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include "Constraint.h"
#include "../../environment/onchipcommunication/IpCore.h"
#include "../../environment/onchipcommunication/PlacedIpCore.h"
#include "tinyxml.h"

namespace cosi {

namespace io {

namespace onchipcommunication {

using namespace std ;
using namespace cosi::environment::onchipcommunication ;

class ConstraintParser {
public:

    /**\brief Constructor by file name.
     * This constructor takes as parameter the name of the file
     * that contains the description of all constraints.
     * */
    ConstraintParser( string pFname ) ;

	/**\brief This function parses the constraint file.
	 * */
    void Parse( ) ;

	/**\brief This function returns the set of placed IPs.
	 * This function returns the set of IPs that have already
	 * a position on the chip.
	 * */
    vector< PlacedIpCore > GetPlacedIpCore( ) ;


    PlacedIpCore  GetPlacedIpCore( string pName ) ;

    vector< IpCore > GetIpCore( ) ;

    IpCore GetIpCore(string pName) ;


    vector< Constraint > GetConstraints( ) ;

    set<int> GetExclusions( int i ) ;

    bool AllPlaced( ) ;

    void WritePlacement( string pFname) ;


private:

    void ParseNode( TiXmlNode* pParent ) ;

    void AddConstraint( TiXmlNode* pParent );

    void AddCore( TiXmlNode* pParent ) ;

    void AddExclusion( TiXmlNode* pParent ) ;


    string mFname ;

    vector< Constraint > mConstraint ;

    map< int , string > mIntToName ;
    map< string , int > mNameToInt ;

    //Exclusion sets
    map< string , set< string > > mExclusions ;

    bool mPlaced ;

    map< string , PlacedIpCore > mPlacedIp ;

    map< string , IpCore > mIp ;

} ;
}
}
}
#endif /*CONSTRAINTPARSER_H_*/
