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
//COMMSTRUCT
#include "networks/onchipcommunication/Specification.h"
#include "networks/onchipcommunication/PlatformInstance.h"
#include "networks/onchipcommunication/Implementation.h"

//PLATFORMS
#include "platforms/onchipcommunication/Platform.h"
#include "platforms/onchipcommunication/NocPlatformInterface.h"

//ALGORITHMS
#include "algorithms/onchipcommunication/StarTopologyGeneration.h"
#include "algorithms/onchipcommunication/MeshTopology.h"
#include "algorithms/onchipcommunication/MFDg.h"
#include "algorithms/onchipcommunication/HierarchicalNetwork.h"
#include "algorithms/onchipcommunication/NetworkCost.h"



//IO
#include "io/onchipcommunication/ConstraintParser.h"
#include "io/onchipcommunication/PlacementParser.h"
#include "io/onchipcommunication/ProjectParser.h"
#include "io/onchipcommunication/ConfigParser.h"
#include "io/onchipcommunication/ToGraph.h"
#include "io/onchipcommunication/SyscCodeGen.h"
#include "io/onchipcommunication/ParquetCodeGen.h"
//#include "io/onchipcommunication/Output.h"
//#include "io/onchipcommunication/DotCodeGen.h"
//#include "io/onchipcommunication/OtterCodeGen.h"
#include "io/onchipcommunication/Report.h"
#include "io/onchipcommunication/InkscapeCodeGen.h"
//#include "io/onchipcommunication/CplexCodeGen.h"

//ENVIRONMENT

