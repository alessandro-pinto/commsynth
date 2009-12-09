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
#ifndef PROJECTPARSER_H_
#define PROJECTPARSER_H_

#include <vector>
#include <string>
#include <iostream>
#include "tinyxml.h"

namespace cosi {

namespace io {

namespace onchipcommunication {

using namespace std ;

class ProjectParser {
public:

    ProjectParser( string pFname ) ;

    void Parse( ) ;

    string GetLibraryFileName( ) ;
    string GetConstraintFileName( ) ;
    string GetProjectName( ) ;
    string GetPlacementFileName( ) ;

    bool WireModelDefined( ) ;
    string GetWireFileName( ) ;

    bool BufferModelDefined( ) ;
    string GetBufferFileName( ) ;

    string GetWireLayer( ) ;
    string GetWireSpacing( ) ;
    string GetWireBuffering( ) ;
    bool GetWireShielding( ) ;


    string GetLibraryName( ) ;

    double GetSpareArea( ) ;
    double GetTotalArea( ) ;
    double GetStep( ) ;
    int GetFlitWidth( ) ;
    double GetSwitchingFactor( ) ;
    int GetMaxInDegree( ) ;
    int GetMaxOutDegree( ) ;
    int GetSourceOutDegree( ) ;
    int GetDestInDegree( ) ;
    double GetPowerVsArea( ) ;
    int GetDensity( ) ;
    int GetPruneLines( ) ;
    bool AllowPtP( ) ;
    bool AllowTwoHops( ) ;
    bool UsePlacement( ) ;
    bool RunTimeDeadlockCheck( ) ;
    bool HasHopConstraints( ) ;
    bool HasAreaConstraint( ) ;
    bool UseKmedian() ;
    bool RunTimeBackTrack() ;
    int Kmin() ;
    int Kmax() ;

    bool ReportRequest( ) ;
    bool TabAppendRequest( ) ;
    bool SvgRequest( ) ;
    bool DotRequest( ) ;
    bool SystemCRequest( ) ;
    bool OtterRequest( ) ;


    string GetReportFileName( ) ;
    string GetTabAppendFileName( );
    string GetSvgFileName( ) ;
    string GetDotFileName( ) ;
    string GetSystemCFileName( ) ;
    string GetSystemCcppFileName( ) ;
    string GetSystemCMakeFileName( ) ;
    string GetOtterFileName( ) ;
    string GetCosiRoot( ) ;
    string GetSystemCRoot( ) ;


private:


    void ParseNode( TiXmlNode* Parent ) ;

    void GetProjectAttributes( TiXmlNode* pParent ) ;

    void GetLibraryAttributes( TiXmlNode* pParent ) ;

    void GetConstraintsAttributes( TiXmlNode* pParent ) ;

    void GetParametersAttributes( TiXmlNode* pParent ) ;

    void GetOutputAttributes( TiXmlNode* pParent ) ;

    void GetSvgAttributes( TiXmlNode* pParent ) ;

    void GetSystemCAttributes( TiXmlNode* pParent ) ;

    void GetDotAttributes( TiXmlNode* pParent ) ;

    void GetOtterAttributes( TiXmlNode* pParent ) ;

    void GetReportAttributes( TiXmlNode* pParent ) ;

    void GetTabAppendAttributes( TiXmlNode* pParent ) ;

    string mProjectName ;
    string mFname ;
    string mLibraryFileName ;
    bool mLibraryDefined ;
    string mConstraintFileName ;
    bool mConstraintDefined ;
    string mPlacementFileName ;

    string mWireModelFileName ;
    bool mWireModelDefined ;
    string mBufferModelFileName ;
    bool mBufferModelDefined ;
    string mWireLayer ;
    string mWireSpacing ;
    string mWireBuffering ;
    bool mWireShielding ;

    string mReportFileName ;
    string mTabAppendFileName ;
    string mSvgFileName ;
    string mDotFileName ;
    string mOtterFileName ;
    string mSystemCfileName ;
    string mMakeFileName ;

    string mCosiRoot ;
    string mSystemCroot ;
    string mInputDir ;
    string mOutputDir ;
    string mLibraryName;

    double mSpareA ;
    double mTotalA ;
    double mStep ;
    int mFlitWidth ;
    double mSwitchingFactor ;
    int mMaxInDegree ;
    int mMaxOutDegree ;
    int mSourceOutDegree ;
    int mDestInDegree ;
    double mPowerVsArea ;
    int mPruneLines ;
    int mDensity ;
    bool mAllowPtP ;
    bool mAllowTwoHops ;
    bool mRunTimeDeadlockCheck ;
    bool mHopConstraints ;
    bool mAreaConstraint ;
	bool mUsePlacement ;
	bool mUseKmedian ;
	int mKmin ;
	int mKmax ;
	bool mRunTimeBackTrack ;


    bool mReport ;
    bool mTabAppend ;
    bool mSvg ;
    bool mSystemC ;
    bool mDot ;
    bool mOtter ;




} ;

}
}
}

#endif /*PROJECTPARSER_H_*/
