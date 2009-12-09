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
#include "ToGraph.h"

namespace cosi {

namespace io {

namespace onchipcommunication {

using namespace cosi::quantity::onchipcommunication;
using namespace std;

cosi::commstruct::onchipcommunication::Specification* ToPtPGraph(
		ConstraintParser* pConstraints, PlacementParser* pPlacement) {

	cosi::commstruct::onchipcommunication::Specification
			* ToReturn = new cosi::commstruct::onchipcommunication::Specification("");

	vector< Constraint > C;
	C = pConstraints->GetConstraints() ;

	//Build the set of sources and destinations
	set< string > Sources;
	set< string > Destinations;
	set< string > IpNames;
	for (unsigned int i = 0; i < C.size() ; i++) {
		Sources.insert(C[i].GetSourceName() ) ;
		Destinations.insert(C[i].GetDestinationName() ) ;
		IpNames.insert(C[i].GetSourceName()) ;
		IpNames.insert(C[i].GetDestinationName()) ;
	}

	CosiLog << "Number of sources "<< Sources.size() << endl ;
	CosiLog << "Number of destinations "<< Destinations.size() << endl ;

	//Set sources and destinations
	set< string >::iterator It;
	map< string , int > IpId;
	int Index;
	Index = 0;
	for (It = IpNames.begin() ; It != IpNames.end() ; It++) {
		ToReturn->AddVertex(Index) ;
		ToReturn->SetName(Index , *It ) ;
		(*ToReturn)[Index].PRJ(Name)SetValue((pConstraints->GetIpCore(*It)).GetComponentName());
		IpId[*It] = Index ;
		Index++;
	}
	//	int D;
	//	D = S ;
	//	for (It = Destinations.begin() ; It != Destinations.end() ; It++) {
	//		ToReturn->AddVertex(D) ;
	//		ToReturn->SetName(D , *It ) ;
	//		(*ToReturn)[D].PRJ(Name)SetValue((pConstraints->GetIpCore(*It)).GetComponentName());
	//		DestId[*It] = D ;
	//		D++;
	//	}

	//Set constraints
	PlacedIpCore Ip;
	int S ,D ;
	for (unsigned int i = 0; i < C.size() ; i++) {
		//We need to add an edges for each constraint
		//Find the source id
		S = IpId[C[i].GetSourceName()];
		D = IpId[C[i].GetDestinationName()];
		Ip = pPlacement->GetIpCore(C[i].GetSourceName() ) ;

			(*ToReturn)[S].PRJ(Position)Set(cosi::Position(Ip.GetBottomLeftX(),
					Ip.GetBottomLeftY(), 0));
			IpGeometry G;
			G.SetArea(RealQuantity(Ip.GetArea())) ;
			G.SetXl(RealQuantity(Ip.GetBottomLeftX()));
			G.SetXr(RealQuantity(Ip.GetTopRightX()));
			G.SetYt(RealQuantity(Ip.GetTopRightY()));
			G.SetYb(RealQuantity(Ip.GetBottomLeftY()));
			(*ToReturn)[S].PRJ(IpGeometry)Set(G);

		Ip = pPlacement->GetIpCore(C[i].GetDestinationName() ) ;

			(*ToReturn)[D].PRJ(Position)Set(cosi::Position(Ip.GetBottomLeftX(),
					Ip.GetBottomLeftY(), 0));

			G.SetArea(RealQuantity(Ip.GetArea())) ;
			G.SetXl(RealQuantity(Ip.GetBottomLeftX()));
			G.SetXr(RealQuantity(Ip.GetTopRightX()));
			G.SetYt(RealQuantity(Ip.GetTopRightY()));
			G.SetYb(RealQuantity(Ip.GetBottomLeftY()));
			(*ToReturn)[D].PRJ(IpGeometry)Set(G);

		ToReturn->AddEdge(S , D ) ;
		cosi::Commodity Flow;
		Flow.SetValue(C[i].GetSourceName(), C[i].GetDestinationName(), S, D,
				C[i].GetBandwidth(), C[i].GetDelay()) ;
		cosi::CommoditySet FlowSet;
		FlowSet.Add(Flow) ;
		(*ToReturn)[Edge(S,D)].PRJ(CommoditySet)Set(FlowSet);
	}

	//	set< int > ExclusionSet;
	//	set< int >::iterator SetIt;
	//	pair<int,int> Comm;
	//	pair<int,int> Comm1;
	//	for (int i = 0; i < C.size() ; i++) {
	//		//We need to add an edges for each constraint
	//		//Find the source id
	//		Comm.first = ToReturn->GetSourceId(C[i].GetSourceName()) ;
	//		Comm.second = ToReturn->GetDestinationId(C[i].GetDestinationName());
	//		CosiLog << C[i].GetSourceName() << " -> "<< C[i].GetDestinationName()
	//				<< " excludes ";
	//		ExclusionSet = pConstraints->GetExclusions(i ) ;
	//		for (SetIt = ExclusionSet.begin() ; SetIt != ExclusionSet.end() ; SetIt++) {
	//			Comm1.first = ToReturn->GetSourceId(C[*SetIt].GetSourceName()) ;
	//			Comm1.second
	//					= ToReturn->GetDestinationId(C[*SetIt].GetDestinationName());
	//			ToReturn->AddExclusion(Comm , Comm1 ) ;
	//			CosiLog << C[*SetIt].GetSourceName() << " -> "
	//					<< C[*SetIt].GetDestinationName() << " , ";
	//		}
	//		CosiLog << endl ;
	//	}

	return ToReturn ;

}

}
}
}

