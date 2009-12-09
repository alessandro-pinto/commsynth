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
#ifndef _HOORIONMODELPARSER_H_
#define _HOORIONMODELPARSER_H_

#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <sstream>
#include "tinyxml.h"
#include "models/onchipcommunication/link/HoAreaModel.h"
#include "include/CosiGlobal.h"

namespace cosi {

namespace io {

namespace onchipcommunication {

using namespace std;

class HoOrionModelParser {
public:

	HoOrionModelParser(string pFname);

	void Parse();

	double GetWmin();
	double GetR0();
	double GetC0();
	double GetCp();
	double GetVdd();
	double GetIoff();
	vector<double> GetR();
	vector<double> GetC();
	vector<double> GetPitch();
	vector<vector<double> > GetArea1ch32();
	vector<vector<double> > GetArea4ch32();
	vector<vector<double> > GetArea1ch128();
	vector<vector<double> > GetArea4ch128();
	vector<vector<double> > GetDynamicEnergy1ch32();
	vector<vector<double> > GetDynamicEnergy4ch32();
	vector<vector<double> > GetDynamicEnergy1ch128();
	vector<vector<double> > GetDynamicEnergy4ch128();
	vector<vector<double> > GetStaticEnergy1ch32();
	vector<vector<double> > GetStaticEnergy4ch32();
	vector<vector<double> > GetStaticEnergy1ch128();
	vector<vector<double> > GetStaticEnergy4ch128();

	int GetMaxIn();
	int GetMaxOut() ;

	//void SetUpPowerModel(HoPowerModelModel* pModel )

private:

	void ParseNode(TiXmlNode* pParent);

	void ParseModel(TiXmlNode* pParent);

	void ParseTechnology(TiXmlNode* pParent);

	void ParseWire(TiXmlNode* pParent);

	void ParseRouter(TiXmlNode* pParent);

	string mFname;
	string mModelName;

	/**\brief Number of metal layers*/
	int mNlayer;
	/**\brief Output resistance of a minimum sized transitor.*/
	double mR_0;
	/**\brief Output capacitance of the minimum sized transitor.*/
	double mC_p;
	/**\brief Input capacitance of the minimum sized transitor.*/
	double mC_0;
	/**\brief Capacitance per unit lenght. One value for each metal layer.*/
	vector<double> mC;
	/**\brief Resistance per unit lenght. One value for each metal layer.*/
	vector<double> mR;
	/**\brief wire pitch. */
	vector<double> mPitch;

	/**\brief Supply voltage.*/
	double mV_DD;
	/**\brief Transistor's off current.*/
	double mI_off;
	/**\brief Minimum transistor width.*/
	double mW_min;
	/**\brief Transistor's short-circuit current.*/
	double mI_sc;
	/**\brief Router's energy consumption
	 * Erouter[i][j] is the energy consumption
	 * of a router with i+1 input ports and j+1 output ports.
	 * This values are taken from Orion.
	 * */
	vector<vector<double> > mErouter1ch32;
	vector<vector<double> > mErouter4ch32;
	vector<vector<double> > mErouter1ch128;
	vector<vector<double> > mErouter4ch128;

	vector<vector<double> > mErouterLeak1ch32;
	vector<vector<double> > mErouterLeak4ch32;
	vector<vector<double> > mErouterLeak1ch128;
	vector<vector<double> > mErouterLeak4ch128;

	vector<vector<double> > mArouter1ch32;
	vector<vector<double> > mArouter4ch32;
	vector<vector<double> > mArouter1ch128;
	vector<vector<double> > mArouter4ch128;

	int mMaxIn ;
	int mMaxOut ;

};

}
}
}

#endif /*HOORIONMODELPARSER_H_*/
