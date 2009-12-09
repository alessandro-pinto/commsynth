/**\page todo To Do List
 *
 * 	-# Write the wiring rule. Needs to know the surfaces. The
 *     first approximation can be simply the start and end point.
 *     A better appprox is the following:
 * 		- find all contact points between surfaces
 * 		- build a graph with the distances between contact points as weights
 * 		- introduce nodes representing the points on each surface (one node per surface)
 * 		- the weights from these nodes to the contact points are paramters
 *        that depends on the point picked on a surface.
 * 		- find the shortest path between two points on two surfaces.
 *	-# Write bus topology rules. There should be two rules for daisychain
 *     and star. The rule should also use the wiring rule to return
 *     the bus instantiated with all the wires.
 * 		- Complete the interface description
 * 		- Derive the base class to define star and daisy chain
 * 	-# Write the wired bus platform. This should take the building
 *     specification and pass it to the rules. It should expose
 *     all necessary functions that can be used by the algorithms.
 * 	-# Test:
 * 		-# Read a specification
 * 		-# Instantiate a platform
 * 		-# Instantiate a bus that connects a few components of the spec
 * 		-# Check latency and cost
 *
 *
 * 	- Revise documentation
 *		- Each package should have the following subpages
 * 			- Quantities
 * 			- Communication structrues
 * 			- Components
 * 			- Composition ruels
 * 			- Environment
 * 			- Synthesis Algorithms
 * 			- Input/output
 * 		- The following sections should be in the documentation
 * 			- Graphs
 * 			- On-Chip communication
 * 			- Wired bas
 * 			- Wireless bas (to be done also in code).
 * 		- Code should be documented and commented.
 *
 * */