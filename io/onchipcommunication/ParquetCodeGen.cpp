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
#include "ParquetCodeGen.h"

namespace cosi {

namespace io {

namespace onchipcommunication {

using namespace std;
using namespace cosi::environment::onchipcommunication;

void ParquetBlocksFile(string pFname, vector< IpCore > pIp,
		vector< PlacedIpCore> pPlacedIp) {
	ofstream File;
	File.open(pFname.c_str() ) ;

	File << "UCSC blocks 1.0"<< endl ;
	File << "# Created      : today"<< endl ;
	File << "# User         : cosinoc"<< endl ;
	File << "# Platform     : Linux"<< endl ;

	File << "NumSoftRectangularBlocks : "<< pIp.size() << endl ;
	File << "NumHardRectilinearBlocks : "<< pPlacedIp.size() << endl ;
	File << "NumTerminals : "<< pPlacedIp.size() << endl ;

	for (unsigned int Index = 0; Index < pIp.size() ; Index++) {
		File << pIp[Index].GetName() << " softrectangular "
				<< pIp[Index].GetArea() << " 1.0 1.0 "<< endl ;
	}
	for (unsigned int Index = 0; Index < pPlacedIp.size() ; Index++) {
		File << pPlacedIp[Index].GetName() << " hardrectilinear 4 ";

		File << "(0,0) ";
		File << "(0,"<< pPlacedIp[Index].GetTopRightY()
				- pPlacedIp[Index].GetBottomLeftY() << ") ";
		File << "("<< pPlacedIp[Index].GetTopRightX()
				- pPlacedIp[Index].GetBottomLeftX() << ","
				<< pPlacedIp[Index].GetTopRightY()
						- pPlacedIp[Index].GetBottomLeftY() << ") ";
		File << "("<< pPlacedIp[Index].GetTopRightX()
				- pPlacedIp[Index].GetBottomLeftX() << ",0)"<< endl ;

	}
	//Add the pads
	for (unsigned int Index = 0; Index < pPlacedIp.size() ; Index++) {
		File << "PAD"<< pPlacedIp[Index].GetName() << " terminal "<< endl ;
	}
	File.close() ;
}

void ParquetNetsFile(string pFname, vector< Constraint > pConstraints,
		vector<PlacedIpCore> pPlacedIp) {
	ofstream File;
	File.open(pFname.c_str() ) ;
	File << "UCLA nets 1.0"<< endl ;
	File << "# Created      : today"<< endl ;
	File << "# User         : socgen"<< endl ;
	File << "# Platform     : Linux"<< endl ;
	File << "NumNets : "<< pConstraints.size() + pPlacedIp.size() << endl ;
	File << "NumPins : "<< 2* (pConstraints.size() + pPlacedIp.size() )<< endl ;

	//Connect the pads
	for (unsigned int Index = 0; Index < pPlacedIp.size() ; Index++) {
		File << "NetDegree : 2 "<< pPlacedIp[Index].GetName() << "2PAD"
				<< pPlacedIp[Index].GetName() << endl ;
		File << "PAD"<< pPlacedIp[Index].GetName() << " B"<< endl ;
		File << pPlacedIp[Index].GetName() << " B : \%-50"<< " \%-50"<< endl ;
	}

	for (unsigned int Index = 0; Index < pConstraints.size() ; Index++) {
		File << "NetDegree : 2 "<< pConstraints[Index].GetSourceName() << "2"
				<< pConstraints[Index].GetDestinationName() << endl ;
		File << pConstraints[Index].GetSourceName() << " O : \%50"<< " \%50"
				<< endl ;
		File << pConstraints[Index].GetDestinationName() << " I : \%50"
				<< " \%50"<< endl ;
	}

	File.close() ;
}

void ParquetWightsFile(string pFname, vector< Constraint > pConstraints,
		vector<PlacedIpCore> pPlacedIp) {
	ofstream File;
	File.open(pFname.c_str() ) ;
	File << "UCLA wts 1.0"<< endl ;
	File << "# Created      : today"<< endl ;
	File << "# User         : socgen"<< endl ;
	File << "# Platform     : Linux"<< endl ;

	for (unsigned int Index = 0; Index < pPlacedIp.size() ; Index++) {
		File << pPlacedIp[Index].GetName() << "2PAD"
				<< pPlacedIp[Index].GetName() << "  1e20"<< endl ;
	}

	for (unsigned int Index = 0; Index < pConstraints.size() ; Index++) {
		File << pConstraints[Index].GetSourceName() << "2"
				<< pConstraints[Index].GetDestinationName() << "  "
				<< pConstraints[Index].GetBandwidth() << endl ;
	}
	File.close() ;
}

void ParquetPlFile(string pFname, vector< IpCore > pIp,
		vector< PlacedIpCore> pPlacedIp) {
	ofstream File;
	File.open(pFname.c_str() ) ;

	File << "UCSC pl 1.0"<< endl ;
	File << "# Created      : today"<< endl ;
	File << "# User         : cosinoc"<< endl ;
	File << "# Platform     : Linux"<< endl ;

	for (unsigned int Index = 0; Index < pIp.size() ; Index++) {
		File << pIp[Index].GetName() << " 0 0"<< endl ;
	}

	for (unsigned int Index = 0; Index < pPlacedIp.size() ; Index++) {
		File << pPlacedIp[Index].GetName() << " 0 0"<< endl ;
	}

	for (unsigned int Index = 0; Index < pPlacedIp.size() ; Index++) {
		File << "PAD"<< pPlacedIp[Index].GetName() << " "
				<< pPlacedIp[Index].GetBottomLeftX() << " "
				<< pPlacedIp[Index].GetBottomLeftY() << endl ;
	}
	File.close() ;
}

void ParquetInputGen( string pBlockFname , string pNetFname , string pWeightsFname, string pPlFname,
		Specification* pSpec ) {
	//generate an ip core for each core and generate a constraint for each constraint
	vector<IpCore> Cores;
	vector<PlacedIpCore> PlacedCores;
	vector<Constraint> PtP;

	IdGraph::v_iterator U , V;
	for( U = pSpec->v_begin(); U != pSpec->v_end(); U++) {
		//add an IP core
		IpCore Core;
		Core.SetName(pSpec->GetName(*U));
		Core.SetArea(((*pSpec)[*U].PRJ(IpGeometry)GetArea()).GetValue());
		Cores.push_back(Core);
		for( V = pSpec->v_begin(); V != pSpec->v_end(); V++) {
			if(pSpec->InE(*U,*V)) {
				Constraint C;
				//compute bandwidth and latency
				CommoditySet::iterator It;
				double L = DBL_MAX;
				double B = 0;
				for( It = (*pSpec)[Edge(*U,*V)].PRJ(CommoditySet)Begin(); It != (*pSpec)[Edge(*U,*V)].PRJ(CommoditySet)End(); It++ ) {
					Commodity Cmm = *It ;
					B += Cmm.GetAverageBandwidthValue();
					if ( Cmm.GetMaximumLatencyValue() < L ) {
						L = Cmm.GetMaximumLatencyValue();
					}
				}
				C.SetBandwidth(B);
				C.SetDelay(L);
				C.SetSourceName(pSpec->GetName(*U));
				C.SetDestinationName(pSpec->GetName(*V));
				PtP.push_back(C);
			}
		}
	}
	ParquetBlocksFile(pBlockFname,
			Cores, PlacedCores);
	ParquetPlFile(pPlFname,
			Cores , PlacedCores );
	ParquetNetsFile(pNetFname,
			PtP,
			PlacedCores );
	ParquetWightsFile(pWeightsFname,
			PtP,
			PlacedCores);
}

void ParquetInputGen( string pBlockFname , string pNetFname , string pWeightsFname, string pPlFname,
		PlatformInstance* pP ) {
	//here we should be carefull with busses

	//generate an ip core for each core and generate a constraint for each constraint
	vector<IpCore> Cores;
	vector<PlacedIpCore> PlacedCores;
	vector<Constraint> PtP;

	IdGraph::v_iterator U , V;
	for( U = pP->v_begin(); U != pP->v_end(); U++) {
		//add an IP core
		IpCore Core;
		Core.SetName(pP->GetName(*U));
		Core.SetArea(((*pP)[*U].PRJ(IpGeometry)GetArea()).GetValue());
		Cores.push_back(Core);
		for( V = pP->v_begin(); V != pP->v_end(); V++) {
			if(pP->InE(*U,*V)) {
				Constraint C;
				C.SetBandwidth(1);
				C.SetDelay(1);
				C.SetSourceName(pP->GetName(*U));
				C.SetDestinationName(pP->GetName(*V));
				PtP.push_back(C);
			}
		}
	}
	ParquetBlocksFile(pBlockFname,
			Cores, PlacedCores);
	ParquetPlFile( pPlFname,
			Cores , PlacedCores );
	ParquetNetsFile( pNetFname,
			PtP,
			PlacedCores );
	ParquetWightsFile( pWeightsFname,
			PtP,
			PlacedCores);

}

void ParquetInputGen( string pBlockFname , string pNetFname , string pWeightsFname, string pPlFname,
		Implementation* pI ) {
	//generate an ip core for each core and generate a constraint for each constraint
	vector<IpCore> Cores;
	vector<PlacedIpCore> PlacedCores;
	vector<Constraint> PtP;

	IdGraph::v_iterator U , V;
	for( U = pI->v_begin(); U != pI->v_end(); U++) {
		//add an IP core
		IpCore Core;
		Core.SetName(pI->GetName(*U));
		Core.SetArea(10e12*((*pI)[*U].PRJ(IpGeometry)GetArea()).GetValue());
		Cores.push_back(Core);
		for( V = pI->v_begin(); V != pI->v_end(); V++) {
			if(pI->InE(*U,*V)) {
				Constraint C;
				//compute bandwidth and latency
				CommoditySet::iterator It;
				double L = DBL_MAX;
				double B = 0;
				for( It = (*pI)[Edge(*U,*V)].PRJ(CommoditySet)Begin(); It != (*pI)[Edge(*U,*V)].PRJ(CommoditySet)End(); It++ ) {
					Commodity Cmm = *It ;
					B += Cmm.GetAverageBandwidthValue();
					if ( Cmm.GetMaximumLatencyValue() < L ) {
						L = Cmm.GetMaximumLatencyValue();
					}
				}
				C.SetBandwidth(B);
				C.SetDelay(L);
				C.SetSourceName(pI->GetName(*U));
				C.SetDestinationName(pI->GetName(*V));
				PtP.push_back(C);
			}
		}
	}
	ParquetBlocksFile(pBlockFname,
			Cores, PlacedCores);
	ParquetPlFile(pPlFname,
			Cores , PlacedCores );
	ParquetNetsFile( pNetFname,
			PtP,
			PlacedCores );
	ParquetWightsFile( pWeightsFname,
			PtP,
			PlacedCores);
}

}
}
}

