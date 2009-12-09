/**
 * \page components Library Elements
 *
 * A basic library elements can be a node or a link. We consider more complex components like
 * entire subnetwork as special kind of nodes. In fact the COSI infrastructure allows the
 * refinement of components by allowing a quantities to be entire communication structures.

 *
 * \image html node_link.png "Representation of node and a link"
 * \image latex node_link.png "Representation of node and a link" width=0.8\textwidth
 *
 * A node can be seen as the superposition of a beasic graph element like a vertex (that is
 * simply characterized by a name and an id) and a set of ports. These ports will be attached
 * as a quantity to the instantiated library element. When defining a node as library element the
 * ports are simply a member of the component class. The definition of the node base class can be
 * found in cosi::library::Node, while the definition of the ports of a node can be found in
 * cosi::Ports. The ports of a node are pairs of a unique name, that identifies a port, and a cosi::PortInterface
 * that defines the interface that that port supports. The interface has a name, that encodes the
 * type of the interface, and a direction that can be input (IN), output (OUT) or bidirection (INOUT).
 * Notice that the id is not assigned a priori but will be assigned at instantiation time.
 *
 * Similarly, a link is the superposition of a bear link (i.e an edge between two nodes) and the
 * interfaces that the link can connect to. Notice that the source and destination node of a link
 * are defined by to quantities of type cosi::Name. They refer to the name of library elements.
 * In this way we model the case where a link can be instantiated only between two specific
 * library nodes. If a link can connect any two libary elements, then the value of the source
 * and destination name will be \f$ \top \f$. A link is characterized also by the source an destination
 * interfaces. It means that it can be only connected to a specific interface at the source node,
 * and a specific interface at the destination node. The specific port name is decided at
 * instantiation time.
 *
 * To define a library component, the user of COSI does not extend cosi::libray::Node or cosi::library::Link
 * directly. In fact, these two base classes are in common to any node and link component. What is missing
 * in this base classes are functions that allow the instantiation of a component.
 * The instantiation of a component is a function that, given the component paramters, returns
 * a communication structure. Therefore, the instantiation function depends on the
 * application domain becasue different applications require different labels to be attached to
 * the components of a communication structure.
 * For instance, for wired BAS, the communication structure that well models a platform instance is
 * cosi::commstruct::wiredbas::PlatformInstance, and the communication structure that well models
 * an implementation is cosi::commstruct::wiredbas::Implementation.
 *
 * For each application domain, the used defines an interface that an instantiable component must
 * implement. For instance the following code defines an instantiable node for wired BAS:
 * \include libraries/wiredbas/common/InstantiableNode.h
 * This is the base class that COSI users should derive from to define a new node component.
 *
 * \section cosi_library_element Definition of a Component
 *
 * A component can be either a node, a link or a an entire network (e.g a bus).
 * To define a component, say a node, the COSI user should extend an instantiable interface (Section \ref components)
 * that already extends the base class cosi::library::Node.
 * A must define the necessary functions for instantiations and may define other functions
 * to compute quantities that are relative to the component (like for instance the
 * computation of the cost):
 *
 *	- \e Instantiation. Each component provide an instantiation function that given the names
 *    of nodes and links and the their configuration returns a communication structure.
 * 	-  \e Quantity \e Computation. A component exposes functions to compute derived quantities. For instance,
 *    a twisted pair wire exposes a function that takes as parameter a wiring path (cosi::WiringPath) and returns the delay
 *    of the wire. Another fuction takes the wiring path and returns the cost of the wire. A component
 *    usually exposes such interface but the actual computation is done by models that are
 *    attached to the component.
 *
 * We report here the definition of an Arcnet sensor as an example to clarify the modeling philosophy.
 * \code
 * namespace cosi {
 * namespace library {
 * namespace wiredbas {
 * class ArcnetNode : public Node {
 *  protected:
 *   cosi::models::wiredbas::ArcnetNodeCost* mCostModel ;
 *  public:
 *   ArcnetNode();
 *   virtual ~ArcnetNode();
 *   void SetCostModel(cosi::models::wiredbas::ArcnetNodeCost* pCostModel ) ;
 *   virtual cosi::commstruct::wiredbas::PlatformInstance*
 *                GetPlatformInstance(int V , Position pP ) = 0 ;
 *   virtual cosi::commstruct::wiredbas::Implementation*
 *                GetImplementationInstance(int V , Position pP ,
 *                                          ThreadSet pThreads,
 *                                          TransferTable pT ) = 0 ;
 *   virtual double GetCost() = 0 ;
 * }; } } }
 * \endcode
 *  An Arcnet node simply derives cosi::library::wiredbas::Node that derives cosi::library::Node and
 *  cosi::library::wiredbas::InstantiableNode. It provides only a way of setting up the cost
 *  model for an Arcnet node and leave all other functions are pure virtual functions. It meas that
 *  when we derive this class to implement an Arcnet sensor, these functions must be
 *  implemented. It also means that it is not possible to instantiate a cosi::library::wiredbas::Arcnetnode.
 *
 * The definition of a sensor is not difficut now. Here is a code snippen for the header
 * and source file:
 * \include libraries/wiredbas/node/ArcnetSensor.h
 * \include libraries/wiredbas/node/ArcnetSensor.cpp
 *
 * A few this must be observed. A component has two static string members that define the type of the
 * component and the name. Notice that the type is used also to specify the type of requested
 * component in the specification, therefore these strings must match. In oder wods, if a sensor
 * in the specification has a type "Sensor" then a sensor component should also have a
 * type "Sensor" otherwire there would be no components able to implement a node in the specificaion.
 *
 * The ports of the components are defined in the constructor. The ports must be defined
 * because they are going to be used in the instantiation function. A sensor in this case has to
 * bidirection ports that are meant to connect this component on a daisy chain bus.
 *
 * The instantiation function returns a communication structure. The integer id of the component
 * must be give as a paramter. This correspond to rename the component. The rest of the instantiation
 * function is easy to understand.
 *
 * The cost is computed by invoking the cost model that given the type and name of the component
 * and its configuration returns the cost. The cost is always assumed to be a real number.
 *
 * \section cosi_library_models Definition of Models
 *
 * A model is a function that maps a vector of quantities into another quantity that is called derived.
 * If the computation of a derived quantity only required to know the configurations (i.e. the labels) of
 * a communication structure, then the quantity is called \e directly \e derived and a model for it is
 * called a \e direct \e model. In some cases, the computation of a quantity relative to a
 * component requires to access information on other components. For instance, the access latency
 * experienced from a node attached to a bus depends on the entire bus configuration.
 * This quantity are colled \e indirectly \e derived and a modelfor them is called and \e indirect \e model.
 *
 * Typically, direct models' interface requires the label of a component to compute the derived
 * quantity. Indirect models' interface, instead, take a communication structure and the id of a
 * component and return the derived quantity of that component.
 * See cosi::models::wiredbas::ArcnetNodeCost for an example of direct model and cosi::models::wiredbas::ArcnetBusDelay
 * for an example of indirect model.
 *
 *
 */