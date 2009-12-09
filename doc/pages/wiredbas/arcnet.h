/**
 * \page wired_bas_package_arcnet ARCNET COSI library
 *
 *
 * The ARCNET protocol is a token ring protocol. The components that are availalbe 
 * in the library are links and nodes at different abstraction levels. The ARCNET library
 * provides two special nodes that are the BUS node (that models an entire ARCNET BUS) 
 * and the cross-layer node that models the Medium Access Control, the Physical and
 * the Electrical level. This node is the one used by the synthesis algorithm. 
 *
 * \section wired_bas_arcnet_nodes ARCNET Nodes
 *
 * The ARCNET library contains the following types of nodes
 *
 * 	- Sensors
 * 	- Actuators
 * 	- Controllers
 * 	- Routers
 * 	- Gateway
 * 	- Bus (this node models an entire bus)
 *
 * The base class cosi::library::ArcnetNode is used to derive all other nodes. \ref Figure_arcnetnode
 * shows the different kind of nodes that are modeled by this library. Notice that this figure shows 
 * the different abstraction levels but does not specialize the nodes to sensors, actuators etc.
 * 
 * <IMG SRC="../images/arcnetnode.png" align="middle" width="400">
 * <CENTER> <B> Figure_arcnetnode : An ARCNET node</B> </CENTER>
 * \anchor Figure_arcnetnode
 *
 * At the medium access control (MAC) level, a nodes has an input and an output interface. 
 * These two interfaces are of type ARCNET. The physical layer (PHY) connected to the MAC layer 
 * on one side and to actual wires on the other side. The electrical connection is modeled by 
 * another node that connects ot the ARCNET physical layer and has two bidirectional ports. This 
 * node represents a "T" connection that models the electrical connection in a daisy chain bus.
 * Other electrical connections are possible that possibly have more than two bidirectional ports.
 * 
 * \subsection wired_bas_arcnet_node_sensor ARCNET Sensor
 *
 *	An ARCNET sensor is an ARCNET node that sends data of a certain length periodically.
 * The messages back to the MAC layer are the acknowledges that the sensor receives from 
 * other nodes. A sensor is characterized by the maximum capacity of its output port.
 * 
 * Notice that the capacity limitation is imposed at the PHY layer considering the 
 * PHY overhead. 
 * 
 * \subsection wired_bas_arcnet_node_actuator ARCNET Actuator
 *
 * An ARCNET actuator is an ARCNET node that receives messages with a certain period and sends 
 * acknowledges back to the sender. An actuator is characterized by the maximum capacity of its 
 * input port.
 * 
 * Notice that the capacity limitation is imposed at the PHY layer considering the 
 * PHY overhead. 
 * 
 * \subsection wired_bas_arcnet_node_controller ARCNET Controller
 * 
 * An ARCNET actuator is an ARCNET node that receives data from sensors and sends acknoledges back, and send data to actuators 
 * and waits for their acknoledges. In the current status, we do not model any computation on the nodes.
 * An ARCNET controller is characterizes by the maximum output and maximum input capacities. 
 * 
 * Notice that the capacity limitation is imposed at the PHY layer considering the 
 * PHY overhead. 
 * 
 * \subsection wired_bas_arcnet_node_router ARCNET Router
 * 
 * TBD
 *
 * \section wired_bas_arcnet_links ARCNET Links
 *
 * The ARCNET library contains the following types of links
 *
 * <IMG SRC="../images/arcnetlinks.png" align="middle" width="400">
 * <CENTER> <B> Figure_arcnetlinks : The set of ARCNET links</B> </CENTER>
 * \anchor Figure_arcnetlinks
 *
 * 	- A Physical link is a bidirectional link connecting EIA-485 interfaces. 
 *    It has two associated quantities, cosi::WiringPath and cosi::RealDelay.
 * 	- Logical links
 * 		- A unidirectional link between two ARCNET interfaces that is used to connect ARCNET nodes 
 *        at the MAC level.
 * 		- A unidirectional link between two EIA-485 interfaces that is used to connect two ARCNET nodes at the 
 *        PHY level or a nodes that the PHY level to an electrical connection.
 *
 * \subsection wired_bas_arcnet_node_bus ARCNET BUS
 *
 * A bus can be modeled at different levels of abstractions. At the MAC level, the details of the physical 
 * implementation of the bus are abstracted. This model is a good abstraction when:
 * 	
 * 	- Only performances have to be evaluated. Delay and bandwidth are determined by the MAC protocol and by the
 *    BUS speed that can be abstraction to the bit-time. 
 *	- The delay is not really impacted by the wiring 
 *    because the maximum length of the bus is constrained in such a way that message delivery is 
 *    basically synchronous.
 * 
 * <IMG SRC="../images/arcnetbus.png" align="middle" width="400">
 * <CENTER> <B> Figure_arcnetbus : The ARCNET BUS</B> </CENTER>
 * \anchor Figure_arcnetbus
 *
 * \ref Figure_arcnetbus shows a model of the ARCNET BUS as a node with many input-output port pairs.
 * In this model, an ARCNET BUS has a maximum number of ports and a maximum capacity. The sum of the
 * bandwidth generates by the messages (including the protocol overhead) and the acknoledges must be 
 * less than the total bus capacity.
 * 
 * When also cost is a concern, then the physical layout of the bus cannot be abstracted. In fact, 
 * about half of the cost of a network is in wiring!!! To account for the wiring cost, we need to 
 * consider a refined view of an ARCNET BUS shown in \ref Figure_arcnetbusrefinement. 
 *
 * <IMG SRC="../images/arcnetbusrefinement.png" align="middle" width="400">
 * <CENTER> <B> Figure_arcnetbusrefinement : The refinement of an ARCNET BUS used for synthesis</B> </CENTER>
 * \anchor Figure_arcnetbusrefinement
 * 
 * Each actual node is comprised of three layers: MAC, PHY and Electrical. Nodes are connected
 * in daisy chain configuration (this should be enforced by ports and rules). The ARCNET library provides
 * already a cross-layer node that can readly used by a synthesis algorithm. The cross-layer node is an
 * abstraction of all the layers and only exposes the bidirectional electrical interfaces. Each node is labeled with 
 * the properties of the MAC and PHY layer.
 * 
 * \subpage arcnet_cost_model
 * \subpage arcnet_delay_model
 * \subpage arcnetbusmodels
 * 
 * */