/**
 * \page commstruct Definition of Communication Structures
 *
 * A communication structure is a labeled graph. Labels are vector of quantities (\ref labeldefinition).
 * COSI defines two basic data structures that implement graphs: cosi::IdGraph for directed graphs and
 * cosi::IdUndirectedGraph for undirected graphs. A communication structure derives this two base
 * classes and adds the label on top of them.
 *
 *
 * Since any label can be attached to the nodes and links of a graph, a communication structure is a
 * template that takes a label as paramter. A communication structure then derives not only the
 * basic graph data structure but also a data structure that attaches labels to the components of a
 * communication structure (i.e. to the nodes and links). This data structure is implemented by the
 * template class cosi::LabelMap. This class implementes an interface to set and get the label
 * attached to a node or a link of a graph. In particular, a useful operator is the operator[].
 * This operator takes a node or a link of a graph and returns a reference to the label. Therefore
 * it can be used to either get or set a label on a component.
 *
 * The following code shows the definition of a communication structure
 * \include core/CommunicationStructure.hpp
 * Notice the redefinition of the functions to add new nodes (i.e. vertexes) and new links (i.e. edges).
 * When a new component is added to the graph, the communication structure will add an empty label
 * to it. An empty label is simply a vector of quantities with no values.
 * The communication strucuture class aslo already defines functions to print the properties
 * of each component which simply prints the component itself (i.e. the id of a node or the
 * pair of ids of a link) and the label attachet to it.
 *
 * For instance the communication strucutre that captures the specification of a communication
 * problem for wired building automation systems looks like the following
 *
 * \code
 *
 * typedef Label_5<Type,Ports,Position,ThreadSet,RealLatency> SpecificationLabel;
 * class Specification : public CommunicationStructure< SpecificationLabel > {
 *   public:
 *     Specification(std::string pName);
 *     ~Specification();
 *     Specification operator+(Specification pC);
 * };
 *
 * \endcode
 *
 * Each communication structure should define a composition operator +. The composition operator 
 * only composes the quantities. Platform dependent composition rules should be
 * defined outside the communication strucuture and checked separately.
 * For an example of such rules see cosi::rules::wiredbas::WiringRule.
 *
 * We report here an example of how to use this data structure. For instance, we can
 * instantiate a specification, add a couple of nodes to it with assigned positions, and add an
 * edge with a latency constraint
 * as follows:
 *
 * \code
 *
 * cosi::commstruct::wiredbas::Specification Spec("My first spec");
 * Spec.AddVertex(0) ;
 * cosi::Position P(0,0,0) ;
 * Spec[0].PRJ(Position)Set(P);
 * //Another node
 * int SecondNodeId = Spec.AddVertex() ;
 * P.SetX(1) ;
 * P.SetY(1) ;
 * P.SetZ(1) ;
 * Spec[SecondNodeId].PRJ(Position)Set(P) ;
 * // Constraint from 0 to SecondNodeId with maximum latency equal to 100ms
 * Spec.AddEdge(0,SecondNodeId) ;
 * Spec[Edge(0,SecondNodeId)].PRJ(RealLatency)SetValue(0.1) ;
 * std::cout << Spec.Print() << std::endl ;
 *
 * \endcode
 * The last line prints out the entire communication structrue.
 *
 *
 *
 */