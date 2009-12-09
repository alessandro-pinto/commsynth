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
#include "PlacedIpCore.h"

namespace cosi {

namespace environment {

namespace onchipcommunication {

using namespace std ;

PlacedIpCore::PlacedIpCore( ) : IpCore( )
{
    mXbl = mYbl = mXtr = mYtr = 0.0 ;
}

PlacedIpCore::PlacedIpCore( string Name ) : IpCore( Name )
{
    mXbl = mYbl = mXtr = mYtr = 0.0 ;
}

PlacedIpCore::PlacedIpCore( string Name , double Xbl , double Ybl ,double Xtr , double Ytr) : IpCore( Name , (Xtr - Xbl) * (Ytr - Ybl) )
{
    mXbl = Xbl ;
    mXtr = Xtr ;
    mYbl = Ybl ;
    mYtr = Ytr ;


}

double PlacedIpCore::GetBottomLeftX( )
{
    return mXbl ;
}
void PlacedIpCore::SetBottomLeftX( double X )
{
    mXbl = 	X ;
    SetArea( (mXtr - mXbl) * (mYtr - mYbl) );
}

double PlacedIpCore::GetTopRightX( )
{
    return mXtr ;
}

void PlacedIpCore::SetTopRightX( double X )
{
    mXtr = X ;
    SetArea( (mXtr - mXbl) * (mYtr - mYbl) );
}

double PlacedIpCore::GetBottomLeftY( )
{
    return mYbl ;
}
void PlacedIpCore::SetBottomLeftY( double Y )
{
    mYbl = 	Y ;
    SetArea( (mXtr - mXbl) * (mYtr - mYbl) );
}

double PlacedIpCore::GetTopRightY( )
{
    return mYtr ;
}

void PlacedIpCore::SetTopRightY( double Y )
{
    mYtr = Y ;
    SetArea( (mXtr - mXbl) * (mYtr - mYbl) );
}
}
}
}

