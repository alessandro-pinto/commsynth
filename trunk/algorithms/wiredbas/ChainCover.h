////
//// C++ Interface: ChainCover
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
//#include <string>
//#include <map>
//#include <set>
//#include <vector>
//#include <algorithm>
//#include "float.h"
//
//#include "Point.h"
//#include "ChainGenerator.h"
//#include "Computation_Traffic_Network.h"
//#include "Point.h"
//#include "Platform.h"
//
//#ifndef _CHAINCOVER_H_
//#define _CHAINCOVER_H_
//
//
//namespace cosi {
//
//namespace algorithms {
//
//namespace wiredbas {
//
//class ChainCover {
//
//public:
//
//	ChainCover();
//
//	void Initialize(map< string , set< Chain > > pChains,
//			Computation_Traffic_Network* pSpecification, Platform* pPlt,
//			map< string , Point > pR);
//
//	void Run();
//
//	map< string , Chain > GetCover();
//
//private:
//	int SizeOfDifference(set<int> S1, set<int> S2);
//	map< string , set< Chain > > mChains;
//	Computation_Traffic_Network* mSpecification;
//	Platform* mPlt;
//	map< string , Chain > mCover;
//	map< string , Point > mRoutingPoint;
//	int mGateway;
//
//};
//
//}
//}
//#endif
