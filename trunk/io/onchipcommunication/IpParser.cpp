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
#include "IpParser.h"
namespace cosi {

namespace io {

namespace onchipcommunication {

using namespace std ;
using namespace cosi::environment::onchipcommunication ;

IpParser::IpParser( string Fname )
{
    mFname = Fname ;
}

void IpParser::Parse( )
{

    TiXmlDocument doc( mFname.c_str( ) ) ;
    bool loadSuccess = doc.LoadFile( ) ;
    if ( ! loadSuccess )
    {
        cout << "The Xml file " << mFname << "  could not be loaded correctly " << endl ;
        return ;
    }
    else
    {
        ParseNode( &doc ) ;
    }
}

void IpParser::ParseNode( TiXmlNode* Parent )
{

    if ( ! Parent )
        return ;

    int Type = Parent->Type( ) ;
    string Elem ;
    string IpName ;

    switch( Type )
    {

    case TiXmlNode::DOCUMENT :
        cout <<  "Parsing the description of the cores ... " << endl ;
        break ;

    case TiXmlNode::ELEMENT :
        Elem = Parent->ValueStr( ) ;
        //if the element is the filetype then set the type
        if ( Elem == "FileType" )
        {
            SetFileType( Parent ) ;

        }
        else if ( Elem == "Core" )
        {
            if ( mPlaced )
            {
                AddPlacedIpCore( Parent ) ;
            }
            else
            {
                AddIpCore( Parent ) ;
            }
        } else {
        //	cout << "Unknown XML element " << Elem << " in file " << mFname << endl ;
        }
        break ;

    case TiXmlNode::COMMENT :
        break ;

    case TiXmlNode::TEXT :

        break ;

    case TiXmlNode::DECLARATION :
        break ;

    default:
        break ;

    }

    TiXmlNode* Child ;
    for( Child = Parent->FirstChild( ) ; Child != 0 ; Child = Child->NextSibling( ) )
    {
        ParseNode( Child ) ;
    }

}


void IpParser::AddIpCore( TiXmlNode* Node )
{

	//parse the area attibute
    IpCore Ip ;
    double A  ;
    string IpName ;
    TiXmlText* Text ;
    TiXmlNode* Child;
    Child = ( Node->FirstChild( ) )->FirstChild( ) ;
    Text = Child->ToText( ) ;
    IpName = Text->ValueStr( ) ;

    //Get the name of the I
    Ip.SetName( IpName ) ;
    //parse all attibutes
    TiXmlAttribute* Attribute = ( Node->ToElement())->FirstAttribute( ) ;
    while ( Attribute )
    {

        if ( string( Attribute->Name( ) ) == "area" )
        {
            Attribute->QueryDoubleValue(&A) ;
            Ip.SetArea( A ) ;
        }
        else
        {
            cout << "Unknown attribute " << Attribute->Name( ) << " for " << IpName << endl ;
        }
        cout << Ip.GetName( ) << "   " << A << endl ;
        Attribute = Attribute->Next( ) ;
    }
    mIp.push_back( Ip ) ;
}

void IpParser::AddPlacedIpCore( TiXmlNode* Node )
{

    PlacedIpCore Ip ;
    double Xbl , Ybl , Xtr , Ytr ;
    string IpName ;
    TiXmlText* Text ;
    TiXmlNode* Child;
    Child = Node->FirstChild( ) ;
    Text = Child->ToText( ) ;
    IpName = Text->ValueStr( ) ;

    //Get the name of the Ip
    Ip.SetName( IpName ) ;
    //parse all attibutes
    TiXmlAttribute* Attribute = (Node->ToElement())->FirstAttribute( ) ;
    while ( Attribute )
    {

        if ( Attribute->ValueStr( ) == "xlb" )
        {
            Attribute->QueryDoubleValue(&Xbl) ;
            Ip.SetBottomLeftX( Xbl ) ;
        }
        else if ( Attribute->ValueStr( ) == "ylb" )
        {
            Attribute->QueryDoubleValue(&Ybl) ;
            Ip.SetBottomLeftY( Ybl ) ;
        }
        else if ( Attribute->ValueStr( ) == "xtr" )
        {
            Attribute->QueryDoubleValue(&Xtr) ;
            Ip.SetTopRightX( Xtr ) ;
        }
        else if ( Attribute->ValueStr( ) == "ytr" )
        {
            Attribute->QueryDoubleValue(&Ytr) ;
            Ip.SetTopRightY( Ytr ) ;
        }
        else
        {
            cout << "Unknown attribute " << Attribute->Value( ) << " for " << IpName << endl ;
        }
        cout << Ip.GetName( ) << "  (" << Xbl << "," << Ybl << ")   (" <<Xtr << "," << Ytr << ")" << endl ;
        Attribute = Attribute->Next( ) ;
    }
    mPlacedIp.push_back( Ip ) ;

}


void IpParser::SetFileType( TiXmlNode* Node)
{
	TiXmlText* Text ;
	TiXmlNode* Child ;
	Child = Node->FirstChild( ) ;
	Text = Child->ToText( ) ;
	cout << Text->Value( ) << endl ;
    if ( Text->ValueStr( ) == "Placed" )
    {
        cout << "Ips are placed " << endl ;
        mPlaced = true ;
    }
    else if ( Text->ValueStr( ) == "Area" )
    {
        cout << "Ips are not placed " << endl ;
        mPlaced = false ;
    }
    else
    {
        cout << "Unknown core file tyle, assuming Area" << endl ;
        mPlaced = false ;
    }
}

vector< IpCore > IpParser::GetIpCores( )
{
    return mIp ;
}


vector< PlacedIpCore > IpParser::GetPlacedIpCores( )
{
    return mPlacedIp ;
}

bool IpParser::IsPlaced( )
{
    return 	mPlaced ;
}
}
}
}


