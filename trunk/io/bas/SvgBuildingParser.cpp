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
#include "SvgBuildingParser.h"

namespace cosi {

namespace io {

namespace bas {

SvgBuildingParser::SvgBuildingParser(string pFname) {
	mFname = pFname ;
}

void SvgBuildingParser::Parse() {
	TiXmlDocument doc(mFname.c_str() );
	bool loadSuccess = doc.LoadFile() ;
	if ( !loadSuccess ) {
		cout << "The Xml file "<< mFname << "  could not be loaded correctly "
				<< endl ;
		exit( 1) ;
		return;
	} else {
		ParseDocument( &doc ) ;
	}
}

void SvgBuildingParser::ParseDocument(TiXmlNode* pParent) {
	if ( !pParent )
		return;

	int Type = pParent->Type() ;
	string Elem;

	if (Type == TiXmlNode::DOCUMENT ) {
		cout << "Parsing the building from file  "<< mFname << endl ;
		TiXmlNode* Child;
		for (Child = pParent->FirstChild() ; Child != 0; Child
				= Child->NextSibling() ) {
			Elem = Child->ValueStr() ;
			if (Elem == "svg")
				ParseSvg(Child ) ;
		}
	} else {
		cout << "The document should be parsed..."<< endl ;
	}
	return;
}

void SvgBuildingParser::ParseSvg(TiXmlNode* pParent) {
	GetSvgAttributes(pParent ) ;
	TiXmlNode* Child;
	string Elem;
	for (Child = pParent->FirstChild() ; Child != 0; Child
			= Child->NextSibling() ) {
		Elem = Child->ValueStr() ;
		if (Elem == "sodipodi:namedview") {
			ParseNamedView(Child ) ;
		} else if (Elem == "g") {
			ParseLayer(Child ) ;
		} else {
			cout << "Unknown XML element "<< Elem
					<< " in Building element in file "<< mFname << endl ;
		}
	}
	return;
}

void SvgBuildingParser::GetSvgAttributes(TiXmlNode* pParent) {
	TiXmlAttribute* Attribute = ( pParent->ToElement())->FirstAttribute() ;
	while (Attribute ) {
		if (string(Attribute->Name() ) == "height") {
			Attribute->QueryDoubleValue(&mHeightPx) ;;
		} else if (string(Attribute->Name() ) == "width") {
			Attribute->QueryDoubleValue(&mWidthPx) ;;
		}
		Attribute = Attribute->Next() ;
	}

	CosiLog << "height : "<< mHeightPx << " px   Width : "<< mWidthPx << "  px"
			<< endl ;
}

void SvgBuildingParser::ParseNamedView(TiXmlNode* pParent) {
	GetNamedViewAttributes(pParent ) ;
}

void SvgBuildingParser::GetNamedViewAttributes(TiXmlNode* pParent) {
	TiXmlAttribute* Attribute = ( pParent->ToElement())->FirstAttribute() ;
	while (Attribute ) {
		if (string(Attribute->Name() ) == "height") {
			Attribute->QueryDoubleValue(&mHeightCm) ;;
		} else if (string(Attribute->Name() ) == "width") {
			Attribute->QueryDoubleValue(&mWidthCm) ;;
		}
		Attribute = Attribute->Next() ;
	}

	CosiLog << "height : "<< mHeightCm << " cm   Width : "<< mWidthCm << "  cm"
			<< endl ;
}

void SvgBuildingParser::ParseLayer(TiXmlNode* pParent) {

	GetLayerAttributes(pParent ) ;

	TiXmlNode* Child;
	string Elem;
	for (Child = pParent->FirstChild() ; Child != 0; Child
			= Child->NextSibling() ) {
		Elem = Child->ValueStr() ;
		if (mCurrentLayerType == "walls"&& Elem == "rect") {

			ParseWall(Child ) ;

		} else if (mCurrentLayerType == "cable_ladders"&& Elem == "rect") {

			ParseCableLadder(Child ) ;

		} else if (mCurrentLayerType =="gateways"&& Elem == "path") {

			ParseGateway(Child ) ;

		} else if (mCurrentLayerType =="constraints"&& Elem == "path") {

			ParseConstraint(Child ) ;

		} else if (mCurrentLayerType =="nodes"&& Elem == "path") {

			ParseNode(Child ) ;
		} else if (mCurrentLayerType =="routing_points"&& Elem == "path") {
			ParseRoutingPoint(Child ) ;
		}
	}
	return;
}

string SvgBuildingParser::GetLayerAttributes(TiXmlNode* pParent) {
	string Layer, Transform;
	TiXmlAttribute* Attribute = ( pParent->ToElement())->FirstAttribute() ;
	while (Attribute ) {
		if (string(Attribute->Name() ) == "inkscape:label") {
			Layer = Attribute->ValueStr() ;
		} else if (string(Attribute->Name() ) == "transform") {
			Transform = Attribute->ValueStr() ;
		}
		Attribute = Attribute->Next() ;
	}

	//analyze the layer type and the application
	string::size_type loc = Layer.find(".", 0) ;
	if (loc != string::npos ) {
		mCurrentLayerType = string(Layer , 0, loc) ;
		mCurrentApplication = string(Layer, loc+1, Layer.size() ) ;
	} else {
		//either walls or cable_ladders
		mCurrentLayerType = Layer ;
	}

	mCurrentDx=0;
	mCurrentDy=0;
	loc = Transform.find("translate(", 0) ;
	stringstream s;
	if (loc != string::npos ) {
		string::size_type loc1 = Transform.find(",", 0);
		string number1(Transform, 10, loc1-11);
		s << number1.c_str() << " ";
		string::size_type loc2 = Transform.find(")", 0);
		string number2(Transform, loc1+1, loc2-loc1-1);
		s << number2.c_str() ;
		s >> mCurrentDx >> mCurrentDy ;
	}

	return "";

}

void SvgBuildingParser::ParseWall(TiXmlNode* pParent) {

	GetWallAttributes(pParent ) ;

}

void SvgBuildingParser::GetWallAttributes(TiXmlNode* pParent) {

	double W, H, X, Y;
	string O, Id;

	TiXmlAttribute* Attribute = ( pParent->ToElement())->FirstAttribute() ;
	while (Attribute ) {
		if (string(Attribute->Name() ) == "width") {
			Attribute->QueryDoubleValue(&W) ;
		} else if (string(Attribute->Name() ) == "height") {
			Attribute->QueryDoubleValue(&H) ;
		} else if (string(Attribute->Name() ) == "x") {
			Attribute->QueryDoubleValue(&X) ;
		} else if (string(Attribute->Name() ) == "y") {
			Attribute->QueryDoubleValue(&Y) ;
		} else if (string(Attribute->Name() ) == "orientation") {
			O = Attribute->ValueStr() ;
		} else if (string(Attribute->Name() ) == "id") {
			Id = Attribute->ValueStr() ;
		}

		Attribute = Attribute->Next() ;
	}

	//to meters
	W = W * mWidthCm / mWidthPx ;
	H = H * mHeightCm / mHeightPx ;
	X = (mCurrentDx + X ) * mWidthCm / mWidthPx ;
	Y = (mHeightPx - mCurrentDy - Y) * mHeightCm / mHeightPx ;

	//Add a new wall
	cosi::environment::bas::Wall Wl;
	Wl.Name = Id ;
	Wl.Level = 1;

	if (O == "H")
		Wl.Thickness = H ;
	else if (O == "V")
		Wl.Thickness = W ;

	//Assume that the height is 3 meters

	double X1, Y1, Z1, X2, Y2, Z2, X3, Y3, Z3, X4, Y4, Z4;
	X1=Y1=Z1=X2=Y2=Z2=X3=Y3=Z3=X4=Y4=Z4=0;
	if (O == "H") {
		Z1 = 0;
		X1 = X ;
		Y1 = Y ;
		Z2 = 3;
		X2 = X ;
		Y2 = Y ;
		Z3 = 3;
		X3 = X + W;
		Y3 = Y ;
		Z4 = 0;
		X4 = X + W ;
		Y4 = Y ;
	} else if (O == "V") {
		Z1 = 0;
		X1 = X ;
		Y1 = Y -H;
		Z2 = 3;
		X2 = X ;
		Y2 = Y -H;
		Z3 = 3;
		X3 = X ;
		Y3 = Y;
		Z4 = 0;
		X4 = X ;
		Y4 = Y;
	}

	Position v1(X1 , Y1 , Z1 ); //cout << X1 << " " << Y1 << " " << Z1 << endl ;
	Position v2(X2 , Y2 , Z2 ); //cout << X2 << " " << Y2 << " " << Z2 << endl ;
	Position v3(X3 , Y3 , Z3 ); //cout << X3 << " " << Y3 << " " << Z3 << endl ;
	Position v4(X4 , Y4 , Z4 ); //cout << X4 << " " << Y4 << " " << Z4 << endl ;
	Wl.SetPoints(v1, v4, v3, v2);
	mWalls[Id] = Wl ;

}

void SvgBuildingParser::ParseCableLadder(TiXmlNode* pParent) {

	GetCableLadderAttributes(pParent ) ;

}

void SvgBuildingParser::GetCableLadderAttributes(TiXmlNode* pParent) {

	double W, H, X, Y;
	string Id;

	TiXmlAttribute* Attribute = ( pParent->ToElement())->FirstAttribute() ;
	while (Attribute ) {
		if (string(Attribute->Name() ) == "width") {
			Attribute->QueryDoubleValue(&W) ;
		} else if (string(Attribute->Name() ) == "height") {
			Attribute->QueryDoubleValue(&H) ;
		} else if (string(Attribute->Name() ) == "x") {
			Attribute->QueryDoubleValue(&X) ;
		} else if (string(Attribute->Name() ) == "y") {
			Attribute->QueryDoubleValue(&Y) ;
		} else if (string(Attribute->Name() ) == "id") {
			Id = Attribute->ValueStr() ;
		}

		Attribute = Attribute->Next() ;
	}

	//to meters
	W = W * mWidthCm / mWidthPx ;
	H = H * mHeightCm / mHeightPx ;
	X = (mCurrentDx + X ) * mWidthCm / mWidthPx ;
	Y = (mHeightPx - mCurrentDy - Y) * mHeightCm / mHeightPx ;

	//Set the new surface
	cosi::environment::bas::InstSurface If;
	If.Name = Id ;
	If.Level = 1;
	double X1, Y1, Z1, X2, Y2, Z2, X3, Y3, Z3, X4, Y4, Z4;
	X1 = X;
	Y1 = Y - H;
	Z1 = 3;
	X2 = X + W;
	Y2 = Y - H;
	Z2 = 3;
	X3 = X+W;
	Y3 = Y;
	Z3 = 3;
	X4 = X;
	Y4 = Y;
	Z4 = 3;

	Position v1(X1 , Y1 , Z1 );
	Position v2(X2 , Y2 , Z2 );
	Position v3(X3 , Y3 , Z3 );
	Position v4(X4 , Y4 , Z4 );
	If.SetPoints(v1 , v2 , v3 , v4);
	mSurfaces[Id] = If ;

}

void SvgBuildingParser::ParseGateway(TiXmlNode* pParent) {
	GetGatewayAttributes(pParent ) ;
}

void SvgBuildingParser::GetGatewayAttributes(TiXmlNode* pParent) {
	double Cx, Cy, Rx, Ry, Dx, Dy;
	string Transform, Id, Type;
	stringstream s;
	TiXmlAttribute* Attribute = ( pParent->ToElement())->FirstAttribute() ;
	while (Attribute ) {
		if (string(Attribute->Name() ) == "sodipodi:cx") {
			Attribute->QueryDoubleValue(&Cx) ;
		} else if (string(Attribute->Name() ) == "sodipodi:cy") {
			Attribute->QueryDoubleValue(&Cy) ;
		} else if (string(Attribute->Name() ) == "sodipodi:rx") {
			Attribute->QueryDoubleValue(&Rx) ;
		} else if (string(Attribute->Name() ) == "sodipodi:ry") {
			Attribute->QueryDoubleValue(&Ry) ;
		} else if (string(Attribute->Name() ) == "transform") {
			Transform = Attribute->ValueStr() ;
		} else if (string(Attribute->Name() ) == "id") {
			Id = Attribute->ValueStr() ;
		} else if (string(Attribute->Name() ) == "type") {
			Type = Attribute->ValueStr() ;
		}

		Attribute = Attribute->Next() ;
	}

	//parse the transformation
	string::size_type loc = Transform.find("translate(", 0) ;
	if (loc != string::npos ) {
		string::size_type loc1 = Transform.find(",", 0);
		string number1(Transform, 10, loc1-11);
		s << number1.c_str() << " ";
		string::size_type loc2 = Transform.find(")", 0);
		string number2(Transform, loc1+1, loc2-loc1-1);
		s << number2.c_str() ;
		s >> Dx >> Dy ;
	} else {
		cout
				<< "WARNING : only the translate transformation is supported by the svg parser"
				<< endl ;
	}

	//set up the gateway position
	double X, Y, Z;
	X = (mCurrentDx + Cx + Dx) * mWidthCm / mWidthPx ;
	Y = (mHeightPx - mCurrentDy - (Cy+Dy)) * mHeightCm / mHeightPx ;
	Z = 1;
	Position V(X, Y, Z);
	mGateways[mCurrentApplication][Id] = V ;
	mGatewayType[mCurrentApplication][Id] = Type ;
}

void SvgBuildingParser::ParseNode(TiXmlNode* pParent) {
	GetNodeAttributes(pParent ) ;
}

void SvgBuildingParser::GetNodeAttributes(TiXmlNode* pParent) {
	double Cx, Cy, Rx, Ry, Dx, Dy;
	string Transform, Id, Type;
	stringstream s;
	TiXmlAttribute* Attribute = ( pParent->ToElement())->FirstAttribute() ;
	while (Attribute ) {
		if (string(Attribute->Name() ) == "sodipodi:cx") {
			Attribute->QueryDoubleValue(&Cx) ;
		} else if (string(Attribute->Name() ) == "sodipodi:cy") {
			Attribute->QueryDoubleValue(&Cy) ;
		} else if (string(Attribute->Name() ) == "sodipodi:rx") {
			Attribute->QueryDoubleValue(&Rx) ;
		} else if (string(Attribute->Name() ) == "sodipodi:ry") {
			Attribute->QueryDoubleValue(&Ry) ;
		} else if (string(Attribute->Name() ) == "transform") {
			Transform = Attribute->ValueStr() ;
		} else if (string(Attribute->Name() ) == "id") {
			Id = Attribute->ValueStr() ;
		} else if (string(Attribute->Name() ) == "type") {
			Type = Attribute->ValueStr() ;
		}

		Attribute = Attribute->Next() ;
	}

	//parse the transformation
	string::size_type loc = Transform.find("translate(", 0) ;
	if (loc != string::npos ) {
		string::size_type loc1 = Transform.find(",", 0);
		string number1(Transform, 10, loc1-11);
		s << number1.c_str() << " ";
		string::size_type loc2 = Transform.find(")", 0);
		string number2(Transform, loc1+1, loc2-loc1-1);
		s << number2.c_str() ;
		s >> Dx >> Dy ;
	} else {
		cout
				<< "WARNING : only the translate transformation is supported by the svg parser"
				<< endl ;
	}

	//set up the gateway position
	double X, Y, Z;
	X = (mCurrentDx + Cx + Dx) * mWidthCm / mWidthPx ;
	Y = (mHeightPx - mCurrentDy - Cy - Dy ) * mHeightCm / mHeightPx ;
	Z = 1;
	Position V(X, Y, Z);
	mNodes[mCurrentApplication][Id] = V ;
	mNodeType[mCurrentApplication][Id] = Type ;
}

void SvgBuildingParser::ParseRoutingPoint(TiXmlNode* pParent) {
	GetRoutingPointAttributes(pParent ) ;
}

void SvgBuildingParser::GetRoutingPointAttributes(TiXmlNode* pParent) {
	double Cx, Cy, Rx, Ry, Dx, Dy;
	string Transform, Id, Type;
	stringstream s;
	TiXmlAttribute* Attribute = ( pParent->ToElement())->FirstAttribute() ;
	while (Attribute ) {
		if (string(Attribute->Name() ) == "sodipodi:cx") {
			Attribute->QueryDoubleValue(&Cx) ;
		} else if (string(Attribute->Name() ) == "sodipodi:cy") {
			Attribute->QueryDoubleValue(&Cy) ;
		} else if (string(Attribute->Name() ) == "sodipodi:rx") {
			Attribute->QueryDoubleValue(&Rx) ;
		} else if (string(Attribute->Name() ) == "sodipodi:ry") {
			Attribute->QueryDoubleValue(&Ry) ;
		} else if (string(Attribute->Name() ) == "transform") {
			Transform = Attribute->ValueStr() ;
		} else if (string(Attribute->Name() ) == "id") {
			Id = Attribute->ValueStr() ;
		} else if (string(Attribute->Name() ) == "type") {
			Type = Attribute->ValueStr() ;
		}

		Attribute = Attribute->Next() ;
	}

	//parse the transformation
	string::size_type loc = Transform.find("translate(", 0) ;
	if (loc != string::npos ) {
		string::size_type loc1 = Transform.find(",", 0);
		string number1(Transform, 10, loc1-11);
		s << number1.c_str() << " ";
		string::size_type loc2 = Transform.find(")", 0);
		string number2(Transform, loc1+1, loc2-loc1-1);
		s << number2.c_str() ;
		s >> Dx >> Dy ;
	} else {
		cout
				<< "WARNING : only the translate transformation is supported by the svg parser"
				<< endl ;
	}

	//set up the gateway position
	double X, Y, Z;
	X = (mCurrentDx + Cx + Dx) * mWidthCm / mWidthPx ;
	Y = (mHeightPx - mCurrentDy - Cy - Dy) * mHeightCm / mHeightPx ;
	Z = 1;
	Position V(X, Y, Z);
	mRoutingPoints[mCurrentApplication][Id] = V ;
}

void SvgBuildingParser::ParseConstraint(TiXmlNode* pParent) {
	GetConstraintAttributes(pParent ) ;

}

void SvgBuildingParser::GetConstraintAttributes(TiXmlNode* pParent) {
	double Period;
	double Length;
	double Latency;
	string Start;
	string End;
	string Id;
	TiXmlAttribute* Attribute = ( pParent->ToElement())->FirstAttribute() ;
	while (Attribute ) {
		if (string(Attribute->Name() ) == "period") {
			Attribute->QueryDoubleValue(&Period) ;
		} else if (string(Attribute->Name() ) == "length") {
			Attribute->QueryDoubleValue(&Length) ;
		} else if (string(Attribute->Name() ) == "latency") {
			Attribute->QueryDoubleValue(&Latency) ;
		} else if (string(Attribute->Name() ) == "inkscape:connection-start") {
			Start = Attribute->ValueStr() ;
		} else if (string(Attribute->Name() ) == "inkscape:connection-end") {
			End = Attribute->ValueStr() ;
		} else if (string(Attribute->Name() ) == "id") {
			Id = Attribute->ValueStr() ;
		}
		Attribute = Attribute->Next() ;
	}

	Start = string(Start, 1, Start.size() - 1);
	End = string(End , 1, End.size() - 1);
	pair<string,string> C(Start, End);
	mPeriods[mCurrentApplication][C] = Period ;
	mLengths[mCurrentApplication][C] = Length ;
	mLatencies[mCurrentApplication][C] = Latency ;
	mConstraintId[mCurrentApplication][C] = Id ;
}

map< string, cosi::environment::bas::Wall > SvgBuildingParser::GetWalls(int pLevel) {
	return mWalls ;
}

map< string, cosi::environment::bas::Wall > SvgBuildingParser::GetWalls(string pName) {
	return mWalls ;
}

list< Position > SvgBuildingParser::GetRoutingPoints(string pName) {
	map<string,Position>::iterator It ;
	list<Position> RetVal ;
	for( It = mRoutingPoints[pName].begin() ; It != mRoutingPoints[pName].end() ; It++) {
		RetVal.push_back(It->second) ;
	}
	return RetVal ;
}

map< string, cosi::environment::bas::InstSurface > SvgBuildingParser::GetInstallationSurfaces(int pLevel) {
	return mSurfaces ;
}

map< string, cosi::environment::bas::InstSurface > SvgBuildingParser::GetInstallationSurfaces(
		string pName) {
	return mSurfaces ;
}

cosi::commstruct::wiredbas::Specification* SvgBuildingParser::GetSpecification(
		string pName) {

	//Gets all constraints relative a group
	cosi::commstruct::wiredbas::Specification
			* Net = new cosi::commstruct::wiredbas::Specification("");
	Net->SetName(pName) ;

	int Id = 0;
	map< pair<string , string> , double >::iterator Period;
	map< pair<string , string> , double >::iterator Length;

	string Start, End;
	int U, V;
	for (Period = (mPeriods[pName]).begin() ; Period != (mPeriods[pName]).end() ; Period++) {
		Start = (Period->first).first;
		End = (Period->first).second;
		//Check if the node has been assigned already
		if (mNodeId.find(Start ) == mNodeId.end() ) {
			mNodeId[Start] = Id ;
			//Add the node to the graph
			Net->AddVertex(Id ) ;
			Net->SetName(Id , Start ) ;
			(*Net)[Id].PRJ(Type)SetValue(FindNodeType(Start ) );
			(*Net)[Id].PRJ(Position)Set(FindNodePosition(Start ) );
			//Net->SetComputation( Id , mComputations[It->first] ) ;
			Id ++;
		}
		if (mNodeId.find(End ) == mNodeId.end() ) {
			mNodeId[End] = Id ;
			Net->AddVertex(Id ) ;
			Net->SetName(Id , End ) ;
			(*Net)[Id].PRJ(Type)SetValue(FindNodeType(End ) );
			(*Net)[Id].PRJ(Position)Set(FindNodePosition(End ) );
			Id ++;
		}
		//Add the thread to the source node
		cosi::Thread T;
		T.SetThreadName(Start + "2"+ End + "_Send") ;
		T.SetDestinationNodeName(End ) ;
		T.SetDestinationNodeId(mNodeId[End]) ;
		T.SetDestinationThreadName(Start + "2"+ End + "_Receieve") ;
		T.SetPeriod(mPeriods[pName][pair<string,string>(Start,End)]) ;
		T.SetMessageLength(mLengths[pName][pair<string,string>(Start,End)]) ;
		(*Net)[mNodeId[Start]].PRJ(ThreadSet)AddThread(T);
		//Add a thread to the destination node
		T.SetThreadName(Start + "2"+ End + "_Receive") ;
		T.SetDestinationNodeName("") ;
		T.SetDestinationThreadName("") ;
		T.SetDestinationNodeId(-128) ; //no id
		T.SetPeriod(0) ;
		T.SetMessageLength(0) ;
		(*Net)[mNodeId[End]].PRJ(ThreadSet)AddThread(T);
		//Add latency on the edge.
		Net->AddEdge(mNodeId[Start], mNodeId[End]) ;
		(*Net)[Edge(mNodeId[Start],mNodeId[End])].PRJ(RealLatency)SetValue(mLatencies[pName][pair<string,string>(Start,End)]);

	}
	return Net ;
}

string SvgBuildingParser::FindNodeType(string pNode) {
	//for all node groups
	map< string , map< string , string > >::iterator GroupIt;
	map< string , string >::iterator NodeIt;
	for (GroupIt = mNodeType.begin() ; GroupIt != mNodeType.end() ; GroupIt++) {
		if ( (NodeIt = (GroupIt->second).find(pNode ) ) != (GroupIt->second).end() ) {
			return NodeIt->second;
		}
	}
	//look for the gateway
	for (GroupIt = mGatewayType.begin() ; GroupIt != mGatewayType.end() ; GroupIt++) {
		if ( (NodeIt = (GroupIt->second).find(pNode ) ) != (GroupIt->second).end() ) {
			return NodeIt->second;
		}
	}

	cout << "Node not found "<< endl ;
	exit( 1) ;
}

Position SvgBuildingParser::FindNodePosition(string pNode) {
	//for all node groups
	//for all node groups
	map< string , map< string , Position > >::iterator GroupIt;
	map< string , Position >::iterator NodeIt;
	for (GroupIt = mNodes.begin() ; GroupIt != mNodes.end() ; GroupIt++) {
		if ( (NodeIt = (GroupIt->second).find(pNode ) ) != (GroupIt->second).end() ) {
			return NodeIt->second;
		}
	}

	for (GroupIt = mGateways.begin() ; GroupIt != mGateways.end() ; GroupIt++) {
		if ( (NodeIt = (GroupIt->second).find(pNode ) ) != (GroupIt->second).end() ) {
			return NodeIt->second;
		}
	}

	cout << "Node not found "<< endl ;
	exit( 1) ;
}

string SvgBuildingParser::PrintBuildingDescription() {
	stringstream s;
	s << endl ;
	s
			<< "______________________________________________________________________"
			<< endl ;
	s << "Building "<< mBuildingName << "   File name "<< mFname << endl ;
	s
			<< "----------------------------------------------------------------------"
			<< endl ;
	s << endl ;

	//Print Walls
	s << "Walls "<< endl ;
	map< string , cosi::environment::bas::Wall >::iterator Wit;
	for (Wit = mWalls.begin() ; Wit != mWalls.end() ; Wit++) {
		s << "  "<< "Wall "<< Wit->first<< " "<< ((Wit->second).GetV1( )).GetX() << "  "<< ((Wit->second).GetV1( )).GetY() << "  "<< ((Wit->second).GetV1( )).GetZ() <<"  Thickness "<< (Wit->second).Thickness<< endl ;
	}
	s<< endl ;
	s << "Cable ladders "<< endl ;
	map< string,cosi::environment::bas::InstSurface >::iterator Iit;
	for (Iit = mSurfaces.begin() ; Iit != mSurfaces.end() ; Iit++) {
		s << "  "<< "Surface "<< (Iit->first)<< " "<< ((Iit->second).GetV1( )).GetX() << " "<< ((Iit->second).GetV1( )).GetY() << " "<< ((Iit->second).GetV1( )).GetZ() << endl ;
	}
	s << endl ;
	s << "Nodes  "<< endl ;
	map< string , map< string , Position > >::iterator Nit;

	s
			<< "______________________________________________________________________"
			<< endl ;
	return s.str() ;

}

}
}
}

