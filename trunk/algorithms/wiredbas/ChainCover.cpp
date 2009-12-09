////
//// C++ Implementation: ChainCover
////
//// Description:
////
////
//// Author: Alessandro Pinto <apinto@eecs.berkeley.edu>, (C) 2007
////
//// Copyright: See COPYING file that comes with this distribution
////
////
//
//#include "ChainCover.h"
//
//ChainCover::ChainCover() {
//}
//
//void ChainCover::Initialize(map< string , set< Chain > > pChains,
//	Computation_Traffic_Network* pSpecification, Platform* pPlt,
//	map< string, Point > pR) {
//	mChains = pChains ;
//	mSpecification = pSpecification ;
//	mPlt = pPlt ;
//	mRoutingPoint = pR ;
//}
//
//void ChainCover::Run() {
//
//	set< int > Covered;
//	set< int > ToBeCovered;
//
//	map< string , set< Chain > >::iterator RouterNameToSetOfChainIt;
//	set< Chain >::iterator SetOfChainIt;
//	IdGraph::v_iterator Vit;
//	/////////////////////////////////////////////////////////////////////////////
//	//
//	// FIND THE ESSENTIAL NODES
//	//
//	/////////////////////////////////////////////////////////////////////////////
//
//	map< int , set< string > > NodeToRouter;
//	map< int , set< string > >::iterator NodeToRouterIt;
//	map< string , set< int > > RouterToEssentialNode;
//	map< string , set< int > >::iterator RouterToEssentialNodeIt;
//
//	int NumberOfChains = 0;
//	int CoverIndex = 0 ;
//	for (Vit = mSpecification->v_begin() ; Vit != mSpecification->v_end() ; Vit++) {
//		if (mSpecification->GetNodeType( *Vit ) == "sensor"
//				|| mSpecification->GetNodeType( *Vit ) == "actuator") {
//			//Check if this vertex is covered by one router only.
//			ToBeCovered.insert( *Vit ) ;
//			for (RouterNameToSetOfChainIt = mChains.begin() ; RouterNameToSetOfChainIt
//					!= mChains.end() ; RouterNameToSetOfChainIt++) {
//				set< Chain > lSetOfChain = RouterNameToSetOfChainIt->second;
//				for (SetOfChainIt = lSetOfChain.begin() ; SetOfChainIt
//						!= lSetOfChain.end() ; SetOfChainIt++) {
//					Chain lChain = *SetOfChainIt ;
//					NumberOfChains++;
//					if (lChain.Contains( *Vit ) ) {
//						NodeToRouter[ *Vit ].insert(RouterNameToSetOfChainIt->first) ;
//						CoverIndex++ ;
//					}
//				}
//			}
//			if ( CoverIndex == 0 ) {
//				cout << "WARNING: Node " << mSpecification->GetNodeName( *Vit ) << " not covered " << endl ;
//			}
//			CoverIndex = 0 ;
//		} else if (mSpecification->GetNodeType( *Vit ) == "eth-gateway") {
//			mGateway = *Vit ;
//		}
//	}
//
//	//NodeToRouter associates to each node a set of routers that have at least one chain
//	//containing the node.
//
//
//	while ( ToBeCovered != Covered ) {
//
//		RouterToEssentialNode.clear() ;
//
//		//for each router, find the set of essential vertexes that it covers.
//		for (NodeToRouterIt = NodeToRouter.begin() ; NodeToRouterIt
//				!= NodeToRouter.end() ; NodeToRouterIt++) {
//			if (Covered.find(NodeToRouterIt->first) == Covered.end() && ( NodeToRouterIt->second ).size() == 1) {
//				string RouterName = *((NodeToRouterIt->second).begin());
//				RouterToEssentialNode[RouterName].insert(NodeToRouterIt->first) ;
//				cout << "Router "<< RouterName << "  essential for node "
//						<<NodeToRouterIt->first<< endl ;
//			}
//		}
//
//		//RotuerToEssentialNode associates to each router the set of essential nodes that it covers.
//
//		//for each router, find the most cost effective chain that covers all the essential vertexes
//		//if such chain cannot be found, the problem is unfeasible
//		if (RouterToEssentialNode.size() > 0) {
//			cout << "Proceeding with covering essential nodes " << endl ;
//			bool ChainFound = false;
//			for (RouterToEssentialNodeIt = RouterToEssentialNode.begin() ; RouterToEssentialNodeIt
//					!= RouterToEssentialNode.end() ; RouterToEssentialNodeIt++) {
//				string RouterName = RouterToEssentialNodeIt->first;
//				set< Chain > EssentialChains;
//				set< Chain >::iterator EssentialChainsIt;
//				for (SetOfChainIt = mChains[RouterName].begin() ; SetOfChainIt
//						!= mChains[RouterName].end() ; SetOfChainIt++) {
//					//if this chain contains all the essential nodes
//					Chain lChain;
//					lChain = *SetOfChainIt ;
//					if (SizeOfDifference(RouterToEssentialNodeIt->second,
//							lChain.NodeSet() ) == 0) {
//						ChainFound = true;
//						EssentialChains.insert(lChain ) ;
//					}
//				}
//
//				if (ChainFound == false) {
//					CosiLog << "  WARNING : Problem is unfeasible "<< endl ;
//					return;
//				}
//
//				//There are two cases: there are essential chains or there are no essential chains
//				if (EssentialChains.size() != 0) {
//					//Pick the chain with the biggest size
//					int MaxDegree = 0;
//					Chain Longest, lTemp;
//					for (SetOfChainIt = EssentialChains.begin() ; SetOfChainIt
//							!= EssentialChains.end() ; SetOfChainIt++) {
//						lTemp = *SetOfChainIt ;
//						if (lTemp.size() > MaxDegree ) {
//							MaxDegree = lTemp.size() ;
//							Longest = lTemp ;
//						}
//					}
//
//					Chain ToAdd;
//					ToAdd.RemoveRouter() ;
//					list< int >::iterator lListIt;
//					cout << "Router " << RouterName << endl ;
//					for (lListIt = ( Longest.C ).begin() ; lListIt != ( Longest.C ).end() ; lListIt++) {
//						if (Covered.find( *lListIt ) == Covered.end() ) {
//							cout << "Covers " << *lListIt << endl ;
//							(ToAdd.C).push_back( *lListIt ) ;
//						}
//					}
//
//					//Need to update all the fields of the chain
//					//Cost and bandwidth
//					ToAdd.Cost = 0;
//					ToAdd.Length = 0;
//					ToAdd.Delay = 0;
//					ToAdd.Bandwidth = 0;
//					for (lListIt = ToAdd.C.begin() ; lListIt != ToAdd.C.end() ; lListIt++) {
//						if ( *lListIt == -1) {
//							ToAdd.Cost += mPlt->NodePrice("router")
//									+ mPlt->NodeInstallationCost("router") ;
//						} else {
//							if (mSpecification->GetNodeType( *lListIt )
//									== "sensor") {
//								ToAdd.Cost += mPlt->NodePrice("sensor")
//										+ mPlt->NodeInstallationCost("sensor") ;
//								ToAdd.Bandwidth
//										+= 1.0
//												/ (mSpecification->GetTraffic( *lListIt , mGateway )).GetPeriod() ;
//							} else {
//								ToAdd.Cost
//										+= mPlt->NodePrice("actuator")
//												+ mPlt->NodeInstallationCost("actuator") ;
//								ToAdd.Bandwidth
//										+= 1.0
//												/ (mSpecification->GetTraffic( mGateway , *lListIt )).GetPeriod() ;
//							}
//						}
//					}
//
//					//TotalLength
//					double TotalLength = 0;
//					Point CurrentPoint, NextPoint;
//					lListIt = ToAdd.C.begin() ;
//					if ( *lListIt == -1) {
//						CurrentPoint = mRoutingPoint[ RouterName ];
//					} else {
//						CurrentPoint = mSpecification->GetPosition( *lListIt ) ;
//					}
//					lListIt++;
//					while (lListIt != ToAdd.C.end() ) {
//
//						if ( *lListIt == -1) {
//							NextPoint = mRoutingPoint[ RouterName ];
//						} else {
//							NextPoint = mSpecification->GetPosition( *lListIt ) ;
//						}
//						TotalLength += mPlt->MinDistance(CurrentPoint ,
//								NextPoint ) ;
//						CurrentPoint = NextPoint ;
//						lListIt++;
//
//					}
//
//					//Compute delay, length and wiring cost
//					ToAdd.Length = TotalLength ;
//					ToAdd.Delay = mPlt->BusDelay("sensor", "router",
//							"twisted-pair", ToAdd.size() - 1, ToAdd.Length) ;
//					ToAdd.Cost
//							+= TotalLength
//									*(mPlt->LinkPricePerMeter("sensor-2-router")
//											+ mPlt->LinkInstallationCostPerMeter("sensor-2-router") );
//
//					mCover[RouterName] = ToAdd ;
//					set< int > Nodes = ToAdd.NodeSet() ;
//
//					Covered.insert(Nodes.begin() , Nodes.end() ) ;
//					//Remove the router
//					for (NodeToRouterIt = NodeToRouter.begin() ; NodeToRouterIt
//							!= NodeToRouter.end() ; NodeToRouterIt++) {
//						(NodeToRouterIt->second).erase(RouterName ) ;
//					}
//				}
//			}
//		} else {
//			//Pick the most cost effective chain among the ones that belong to non assigned routers
//			cout << "Selecting the most cost effective chain " << endl ;
//			Chain MostCostEffective;
//			string RouterName;
//			double Min = DBL_MAX ;
//			int Dsize;
//
//			for (RouterNameToSetOfChainIt = mChains.begin() ; RouterNameToSetOfChainIt
//					!= mChains.end() ; RouterNameToSetOfChainIt++) {
//				if (mCover.find(RouterNameToSetOfChainIt->first)
//						== mCover.end() ) { //router is available
//					set< Chain > lSetOfChain = RouterNameToSetOfChainIt->second;
//
//					for (SetOfChainIt = lSetOfChain.begin() ; SetOfChainIt
//							!= lSetOfChain.end() ; SetOfChainIt++) {
//						Chain lChain = *SetOfChainIt ;
//						//For all chains
//						if (lChain.size() > 1) {
//							Dsize
//									= SizeOfDifference(lChain.NodeSet() ,
//											Covered ) ;
//
//							double CostEffectiveness = lChain.Cost
//									/ (double) (Dsize );
//							if (Dsize > 0&& CostEffectiveness < Min ) {
//								Min = CostEffectiveness ;
//								MostCostEffective = lChain ;
//								RouterName = RouterNameToSetOfChainIt->first;
//							}
//						}
//					}
//				}
//			}
//			if ( RouterName.size() == 0 )
//				break ;
//			//Remove the already assigned node from the chain
//			Chain ToAdd;
//			ToAdd.RemoveRouter() ;
//			list< int >::iterator lListIt;
//			cout << "Router Name " << RouterName << endl ;
//			for (lListIt = ( MostCostEffective.C ).begin() ; lListIt != ( MostCostEffective.C ).end() ; lListIt++) {
//				if (Covered.find( *lListIt ) == Covered.end() ) {
//					cout << "Covers "<< *lListIt << endl ;
//					(ToAdd.C).push_back( *lListIt ) ;
//				}
//			}
//
//			//Need to update all the fields of the chain
//			//Need to update all the fields of the chain
//			//Cost and bandwidth
//			ToAdd.Cost = 0;
//			ToAdd.Length = 0;
//			ToAdd.Delay = 0;
//			ToAdd.Bandwidth = 0;
//			for (lListIt = ToAdd.C.begin() ; lListIt != ToAdd.C.end() ; lListIt++) {
//				if ( *lListIt == -1) {
//					ToAdd.Cost += mPlt->NodePrice("router")
//							+ mPlt->NodeInstallationCost("router") ;
//				} else {
//					if (mSpecification->GetNodeType( *lListIt ) == "sensor") {
//						ToAdd.Cost += mPlt->NodePrice("sensor")
//								+ mPlt->NodeInstallationCost("sensor") ;
//						ToAdd.Bandwidth += 1.0 / (mSpecification->GetTraffic( *lListIt , mGateway )).GetPeriod() ;
//					} else {
//						ToAdd.Cost += mPlt->NodePrice("actuator")
//								+ mPlt->NodeInstallationCost("actuator") ;
//						ToAdd.Bandwidth += 1.0 / (mSpecification->GetTraffic( mGateway , *lListIt )).GetPeriod() ;
//					}
//				}
//			}
//
//			//TotalLength
//			double TotalLength = 0;
//			Point CurrentPoint, NextPoint;
//			lListIt = ToAdd.C.begin() ;
//			if ( *lListIt == -1) {
//				CurrentPoint = mRoutingPoint[ RouterName ];
//			} else {
//				CurrentPoint = mSpecification->GetPosition( *lListIt ) ;
//			}
//			lListIt++;
//			while (lListIt != ToAdd.C.end() ) {
//				if ( *lListIt == -1) {
//					NextPoint = mRoutingPoint[ RouterName ];
//				} else {
//					NextPoint = mSpecification->GetPosition( *lListIt ) ;
//				}
//				TotalLength += mPlt->MinDistance(CurrentPoint , NextPoint ) ;
//				CurrentPoint = NextPoint ;
//				lListIt++;
//			}
//
//			//Compute delay, length and wiring cost
//			ToAdd.Length = TotalLength ;
//			ToAdd.Delay = mPlt->BusDelay("sensor", "router", "twisted-pair",
//					ToAdd.size() - 1, ToAdd.Length) ;
//			ToAdd.Cost
//					+= TotalLength
//							*(mPlt->LinkPricePerMeter("sensor-2-router")
//									+ mPlt->LinkInstallationCostPerMeter("sensor-2-router") );
//
//			mCover[RouterName] = ToAdd ;
//			set< int > Nodes = ToAdd.NodeSet() ;
//
//			Covered.insert(Nodes.begin() , Nodes.end() ) ;
//			for (NodeToRouterIt = NodeToRouter.begin() ; NodeToRouterIt
//					!= NodeToRouter.end() ; NodeToRouterIt++) {
//				(NodeToRouterIt->second).erase(RouterName ) ;
//			}
//		}
//	}
//
//	map< string , Chain >::iterator Rit;
//	for (Rit = mCover.begin() ; Rit != mCover.end() ; Rit++) {
//		cout << "Chain for "<< Rit->first<< endl ;
//		list<int>::iterator NodeIt;
//		Chain lChain = Rit->second;
//		for (NodeIt = lChain.C.begin() ; NodeIt != lChain.C.end() ; NodeIt++) {
//			cout << *NodeIt << "  ";
//		}
//		cout << endl ;
//	}
//
//}
//
//int ChainCover::SizeOfDifference(set< int > S1, set< int > S2) {
//	set<int>::iterator Sit;
//	int Size = 0;
//	for (Sit = S1.begin() ; Sit != S1.end() ; Sit++) {
//		if (S2.find( *Sit ) == S2.end() )
//			Size++;
//	}
//	return Size ;
//}
//
//map< string, Chain > ChainCover::GetCover() {
//	return mCover ;
//}
//
