/**\page  wired_bas_package_commstruct Communication Structures for Wired Building Automation Network Design
 *
 * The specification of a communication problem is captured by end-to-end requirements among
 * sensors, actuators and controllers. A specificaiton contains nodes and links representing
 * end-to-end constraints.
 * Section \ref wired_bas_spec define a precise model to capture a specification.
 *
 * A platform instance is captured by a communication structure that includes the information
 * of the component name and type. Links are actual wires and they are annotated wiht a cosi::WiringPath.
 * A wire in the platform can accomodate many different wiring path, therefore, when instantiated,
 * a wire has an "any value" wiring path. Section \ref wired_bas_plt_instance describes the
 * communication structure that is used to capture a platform instance.
 * The wiring path is then assigned by an algorithm that computes the actual layout of a wire.
 *
 * The implementation is captured by a communication structure that combines the
 * properties of the specification and the platform instance. Nodes are labeled by threads that bear the
 * information on period and length of messages,
 * and links are labeled by wiring paths. Section \ref wired_bas_implementation describe the communication
 * structure that is used to capture and implementation.
 *
 * \section wired_bas_spec Communication Specification
 *
 * The specification of a wired building automation system synthesis  problem is a communication structure
 * labeled by cosi::commstruct::wiredbas::SpecificationLabel. The meaning of the labels is the following:
 *
 * 	- cosi::Type is a string that defines the type of a component (e.g. "Sensor" or "Controller").
 * 	- cosi::Position contains the value of the three coordinates x, y, z associated with a node.
 * 	- cosi::Ports is the required set of ports for each node. A port is a pair of two strings
 *    that representing the name and the type of a componenent. This ports are the ones that are required
 *    by specification and do not represent the actual ports that a component has. The same label
 *    is used to specify the connection between links and nodes.
 * 	- cosi::ThreadSet is a set of communication thread that run on the node. A communication thread is
 *    defiened by the class cosi::Thread and contains the following information:
 * 		- ThreadName is the name of the thread.
 * 		- DestinationNodeName is the name of the destination node to which messages must be sent.
 * 		- DestinationThreadName is the name of the thread running on the destination node that
 *        receives the messages.
 * 		- Period is the interval of time between two messages.
 * 		- MsgLength is the length of messages in bits.
 * 	- cosi::RealLatency is the maximum latency that is accepted for an end-to-end requirement.
 *
 * \image latex bas_spec_example.png "Example of a specification of a communication problem" width=0.8\textwidth
 *
 * <IMG SRC="../images/bas_spec_example.png" width="70%" align="middle" >
 * <CENTER> <B> Figure_bas_spec_example : Example of end-to-end requirement specificaiton for building autoamtion systems</B> </CENTER>
 * \anchor Figure_bas_spec_example
 *
 * \ref Figure_bas_spec_example shows an example of specification. The specification contains three nodes:
 * a sensor named TempSens1, a controller names Ctrl and an actuatuator names Damper1. The sensor is a source
 * of messages that are generated by a thread with name Thread1. This thread sends messages every 40 seconds to
 * thread GetTempSens1 running on the controller. Each message is 8 bit long. The sensor is in position (0,0,1)
 * and has one output port only. The maximum lantency of the massages is 2 second and it is specified on the
 * link between the sensor and the controller. The link is bound to output port P1 of the sensor and input
 * port P1 of the controller. Notice that the same label cosi::Ports is used to specify the ports of a
 * node and the ports to which a link is bound.
 *
 * Notice that the links are only logical end-to-end connections between nodes. They represent connectivity
 * requirements.
 *
 * \subsection wired_bas_spec_composition Composition of Specifications
 *
 * The composition code can be found in cosi::commstruct::wiredbas::Specification::operator+. The composition of two
 * specifications relies on the definition of the composition of the single quantities and is defined as follows:
 *
 *	-# For each node in common, if they have the same type, name and position
 * 		- Assing name, type and position
 * 		- Take the union of the ports (defined in cosi::Ports::operator+)
 * 		- Take the union of Threads (defined in cosi::ThreadSet::operator+)
 * 	-# For each link in common
 * 		- Take the minimum of the two latencies (defined in cosi::RealLatency::operator+)
 * 	-# For components that are not in common
 * 		- Inherit the label of the communication structure the component belongs to.
 *
 *
 * \section wired_bas_plt_instance Communication Platform Instance
 *
 * Components of a platform instance are labeled by cosi::commstruct::wiredbas::PlatformLabel.
 * We recall that the platform is a container, therefore nodes do not have threads assigned to them. Also,
 * since the latency of a wire depends on the wiring path, protocol etc, there is no latency attached
 * to a platform instance. On the other hand, the platform must expose the capabilities of the components.
 * Therefore, each component should be labeled with quantities like:
 *
 * 	- Largest set of ports
 * 	- Maximum numeber of threads
 *  - Maximum length of a wiring path, maximum number of turns of a wiring path etc.
 *
 * These limitations are captured by models. Here, we define a minimum set of labels that are
 * required for a platform instance. The labels are:
 * 	- cosi::Name is the component name. Each library element has a component name associated
 *    with it that identifies the library element. For instance, a specific temperature sensor
 *    may be labeled with "InfraSense50".
 * 	- cosi::Type is the components type. Each library element has a component type associated
 *    with it. For instnace, a temperature sensor may be labeld with type "Sensor".
 * 	- cosi::Ports is the set of ports offered by a component. A component may come in different
 *    configurations with a different set of available ports. This label should capture the
 *    largest set of ports. This label is also used to specify the connection between links and nodes
 *    (i.e. the output port of a source node and the input port of a destination port that represent
 *    the extremities of a link).
 *	- cosi::WiringPath is the path followed by a wired from one source point to a destination point.
 *
 * \subsection wired_bas_plt_instance_composition Composition of Platform Instances
 *
 * Platform instances can be component to form other platform instances. The composition is not
 * constrainted by any rule at the moment. This is still to be defined.
 *
 * The composition is defined similarly to the composition of specifications with the important
 * difference that the ports of the nodes in common between the two communication structures
 * must also be the same (containement may be considered instead of equality).
 *
 *	-# For each node in common, if they have the same type, name, position and <B>ports</B>
 * 		- Assign ports, type, name and position.
 *	-# For each link in common
 * 		- If the wiring path has the same starting and ending points, take the shortest wiring path.
 * 	-# For components that are not in common
 * 		- Inherit the label of the communication structure the component belongs to.
 *
 * \section wired_bas_implementation Communication Implementation
 *
 * The implementation is the result of mapping the specification on a platform instance. It contains some
 * quantities from the specification and some from the platform instance. In particular it contains the following:
 *
 *	- cosi::Name, cosi::Type, cosi::Position, cosi::Ports
 * 	- cosi::ThreadSet
 *	- cosi::WiringPath that is essential for the computation of cost and performance.
 *	- cosi::TransferTable is a set of 3-tuple. Each entry of the tranfer table contains the follwing fields
 * 		- SourceName is the name of the node that originated the message
 * 		- DestName is the name of the node that is the recipient of the message
 * 		- PortName is the name of the port to which the message has to be sent
 *
 *
 * \section wired_bas_implementation_composition Composition of Implementations
 *
 * The composition of two implementations inherits its semantics from the composition of specificatio and palatform
 * instances:
 *
 *	-# For each node in common, if they have the same type, name, position and <B>ports</B>
 * 		- Assign ports, type, name and position.
 * 		- Take the union of Threads (defined in cosi::ThreadSet::operator+)
 *	-# For each link in common
 * 		- If the wiring path has the same starting and ending points, take the shortest wiring path.
 * 	-# For components that are not in common
 * 		- Inherit the label of the communication structure the component belongs to.
 *
 *
 * */