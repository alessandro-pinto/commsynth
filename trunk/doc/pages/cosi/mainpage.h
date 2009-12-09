
/** \mainpage COSI
 *
 * 
 * 
 * \image html logo.png 
 * \image latex logo.png 
 *
 * \section Introduction
 *
 * The Communication Synthesis Infrastructure (COSI) is a software environment
 * that can be easily adapted to implement communication synthesis tools
 * for many different applications.
 *
 * Network synthesis has been studied for years by different communities.
 * In computer science (CS) the network is represented by a graph decorated
 * by labels on arcs (usually called capacities and flows).
 * Traditional algorithms for maximum flow, minimum cost flow, Steiner tree etc.
 * have been studied and applied to communication networks
 * (mainly inspired by the Internet). In operations research (OR),
 * many optimization problems on networks are inspired by the potential cost
 * saving that can be achieved in transportation and supply chain management applications.
 * Although some non trivial optimization algorithms have been, and will be,
 * developed in the course of the COSI project, the main contribution is a general methodology.
 * Ideally, such methodology will be general enough to be applied to many application
 * domains where a network, usually physically constrained by the environment,
 * needs to be designed.
 *
 * In this project we are concerned with networks for embedded systems.
 * We address the problem of providing a formal framework for specifying network as
 * communication structures. A communication structure can be used to define a communication
 * problem as end-to-end communication constraints between users.
 * Communication structures can also be used to define a library of communication components
 * together with their performance.
 * In many networked embedded systems, the environment needs to be capture as one more
 * additional constraint. For instance, in an indoor wireless network for building automation,
 * the walls and the floors have an impact on the performances of the network.
 * By the same token, in a System-on-Chip, the silicon area occupied by pre-designed
 * IP cores represents a constraint on the available space to install network components
 * and wires to interconnect the cores.
 *
 * We use COSI for the synthesis of Networked Embedded Systems.
 * We plan many releases of the software depending on the specific application:
 *
 *	- COSI NoC for on chip networks : Version 1.2 (done), Version 1.3 (under development)
 * 	- COSI WNES for wired networked embedded systems: Version 1.0 ( under development)
 * 	- COSI WLESSNES for wireless networked embedded systems : Version 1.0 (under development)
 * 	- COSI WWNES for wireless and wired networked embedded systems : Version 1.0
 *
 *
 * \section organization Organization of this Document
 *
 * This document is a guide to the development of communication analysis and syntheis tools
 * for networked embedded systems using COSI. Plese refer to the following document for the
 * theoretical backgroung that will not be discussed here.
 *
 * <A href="http://www.eecs.berkeley.edu/Pubs/TechRpts/2008/EECS-2008-54.pdf">
 * Alessandro Pinto, 
 * A Platform-Based Approach to Communication Synthesis for Embedded Systems,
 * <I>Ph.D. Thesis, EECS Department, University of California, Berkeley, May 2008</I>
 * </A>
 *
 * There are four setps to follow to set up and and solve a communication synthesis problem.
 * The first step is to define the quantities that are involved in the optimization problem.
 * Bandwidth is one possible quantity. More interestig ones are, for instance, the name and type
 * ports of a node, or the rate and length of messages at the output of a port.
 *
 * The second step is to define the communication structures. A communication structure is a special
 * type of graph. The components of a communication structure are nodes and links. Notice that
 * such components can actually be refined into other communication structures at a lower level of abstraction.
 * A communication structure contains also a set of possible configuration of the components.
 * A configuration is a function that associates to each component a vector of quantities (called
 * label). To define a synthesis (or mapping) step, we need to define the communication structure
 * that captures the specificaion, called the communication specificaion. Then, we need to define the
 * communication strucuture that captures a platform instance. The quantities associated to a platform instance
 * are similar to the
 * ones that characterize the specification but they represent capabilities rather that
 * constraints. For instance, an end-to-end constraint in the specification has an associated
 * minimum bandwidth requirement while a link in  a platform instance has an associated capacity that
 * captures the maximum bandwidth that a link can support. Finally, we need to define the the communciation
 * structure that captures an implementation. Usually, the quantities associated to the components of
 * an implementation are the union of the quantities of the specification and the platform instance.
 *
 * The thirs step is to define the library elements that are used to construct a platform instance and
 * then map the specification on it to obtain an implementation. Each component in the library is again
 * a communication structure. A set of models is attached to each library element to compute
 * derived quantities.
 *
 * Finally, an algorithm can be developed that reads the specification, the library elements and the rules
 * to compose them and derives an optimal implementation.
 *
 * This document covers the installation and configuration of COSI (\subpage installation). Detailed description of each class
 * can found by browsing the software documentation. Deatailed explanation of models and algorithms can be found in the 
 * "COSI Related Pages" section.
 * 
 * COSI provides some communication synthesis flows but this manual describes the
 * general software environemnt that allows to organize synthesis flows. The manual describes how to
 * implement the following four steps:
 *
 * 	- Definition quantities. (\subpage quantities)
 * 	- Definition of the communication structures. (\subpage commstruct)
 * 	- Definition of the library elements. (\subpage components)
 * 	- Algorithmic development (\subpage algorithms)
 *
 * \section release In this release
 *
 * In this release of COSI we have developed packages for the synthesis of on-chip communication architectures
 * and wired networks for building automation systems. The packages provide the definition of the quantities involved, 
 * the definition of library elements and composition rules (i.e. the platform) and synthesis algorithms. A rich set of 
 * input and output plug-ins is also provided. We list the elements that have been developed for each desigh flow. However,
 * some elements are in common and elements defined for one design flow can be also used in other design flows if that
 * makes sense.
 * 
 * For on-chip communication synthesis we provide:
 * 	
 * 	- Quantities
 * 		- Ports and interfaces
 * 		- Geometry of IP cores
 * 		- Implementation parameters for nodes and links
 * 		- Transfer tables (or routing tables)
 * 		- Type and name of components
 *		- Commodity and commodity set
 * 		- Delay
 * 	- Communication structures
 * 		- Specificaiton 
 * 		- Platform instance
 * 		- Implementation 
 * 		- Channel dependency graph
 * 	- Components
 * 		- IP cores, sources and destinations
 * 		- Routers
 * 		- Releay stations
 * 		- Point-to-point interconnect
 * 	- Composition rules
 * 		- Restriction on components positions
 * 		- Unique assignement of id to components
 * 		- Deadlock freedom
 * 	- Environment
 * 		- General 2D surface
 * 		- IP core
 * 		- Placed IP core
 * 		- Installation surface (free space on the chip)
 * 	- Synthesis algorithms
 * 		- Degree constrained iterated shortes path
 * 		- Optimized Mesh topology
 * 		- Start networks
 * 		- Hierarchical: network of star networks.
 * 	- Input and output
 * 		- XML project and specification definition
 * 		- Interface to the PARQUET floorplanner
 * 		- Interface to Hmetis and PatoH graph partitioner
 * 		- Generation of systemc executable simulation
 * 		- Generation of Dot description of the network implementation
 * 		- Generation of Svg graphical representation of the implementation
 * 		- Generation of textual report for area and power
 * 	
 * For building automation wired networks we provide:
 * 
 * 	- Quantities
 * 		- Ports and interfaces
 * 		- Bandwidth
 * 		- Position
 * 		- Name and type of components
 * 		- Thread and thread set
 * 		- Wiring path
 * 		- Delay
 * 	- Communication structures
 * 		- Specification
 * 		- Platform Instance
 * 		- Implementation
 * 	- Components
 * 		- Twisted pair interconnect
 * 		- Ethernet link
 * 		- Arcnet nodes: sensor, actuator, controller
 * 		- Ethernet nodes
 * 		- Arcnet bus
 * 		- Ethernet subnetwork
 * 	- Composition rules
 * 		- Restriction on the position of nodes
 * 		- Wiring rule
 * 	- Environment
 * 		- General 2D face in 3D
 * 		- Wall
 * 		- Wire installation surface
 * 	- Synthesis algorithms
 * 		- Bus-based synthesis for Arcnet networks
 * 		- Two-level network: first and second level implementation technology can be decided as input parameter
 * 	- Input and output
 * 		- Parser of svg specification captured in Inkscape
 * 		- Interface to Hmetis and PatoH graph partitioners
 * 		- Generation of Svg graphical representation of the implementation
 * 		- Generation of textual report for cost, bandwidth, latency and slack
 * 
 * For general graphs we provide
 * 	
 * 	- Quantities
 * 		- Real numbers
 * 	- Communication structures
 * 		- Graphs labeled with real quantities
 * 	- Algorithms
 * 		- Connected components
 * 		- Facility location		
 * 		- Minimum spanning tree
 * 		- Shortest path
 * 		- Travelling salesman problem
 * 		- K-median (need to be revised)
 * 
 * */