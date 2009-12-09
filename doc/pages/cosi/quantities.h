/**
 * \page quantities Definition of Quantities
 *
 * Quantities are variables that range over a domain. In COSI a quantity is a class that holds a
 * value of the domain. The class type can be seen as the variable type and one instance of the
 * class is a valuation of the quantity (i.e. the quantity with an assigned value from its
 * domain). We first describe how to define quantities and then we comment on the C++ implementation
 * stategy.
 *
 * \section quantitydefinition Defining a Quantity
 *
 * All quantities should derive from the base class Quantity. This class only implements a few
 * elementary features of a quantity. In particular, it defines three attributes: Any, Bot and Ordered.
 * The attribute Any is true if the value held by the class is "any value", denoting the fact that
 * the quantity instance represents all possible values in the quantity domain. The attribute Bot is true
 * if the class instance hold the special value "no value". The attribute Ordered is true if the class
 * has a ordering relation defined. The ordering relation is defined by the three operators <,>,==.
 *
 * For instance, a quantity that ranges over the reals is defined by the class RealQuantity
 * that contains a private member of type double.
 *
 * A quantity Q with domain D should always define the following methods:
 *
 * 	- Q Get() that returns the quantiy class
 * 	- Set(Q) that sets the fields of the class to match Q
 *	- D GetValue( ) that returns the value stored in the quantity
 * 	- SetValue( D ) that assigns the private member of the class.
 *	- QuantityType, a static string variable that contains the type of the domain
 * 	- QuantityName, a static string varible that contains the name given to the quantity (it is suggested
 *    that this name be the same of the class name).
 * 	- operator == to compare two quantity instances.
 *
 * If the domain is orderd, then the two operators <,> should also be defined.
 * 
 * Quantities are all derived from the base class cosi::Quantity. The base class defines already two values
 * of a quantity: the value \f$ \bot \f$ and the value \f$ \top \f$. The value \f$ \bot \f$ means "no value"
 * and it represents the minimum value of a quantity (i.e. a value that is less than or equal than 
 * any value of the domain D). The value  \f$ \top \f$ means "any value" and it represents the maximum 
 * value of a quantity (i.e. a value that is greater or equal than any value in the domain D). These two 
 * special values are implemented by two boolean flags in the base class cosi::Quantity that are 
 * accessed by accessor functions.
 * 
 * To provide input/outut cabailities that are important for debugging, each quantity should also 
 * define a friend memberd that redefines the operator<<.    
 *  
 * \section labeldefinition Defining a Label
 *
 * A label is a vector of quantity. In COSI, a label is implemented by deriving from all the
 * quantities belonging to the label. The COSI infrastructure provides the definition of labels with
 * up to 10 quantities. The set of labels can be extended if needed. 
 * 
 * A label is a template that takes n paramters where n is the number of quantities. For instnace, 
 * a label with two quantities is defined by using the template class cosi::Label_2. A label offers 
 * other funtions that may be useful for the developer. Since each quantity defines two static members
 * that are its type and its name, the following API can used to retrive information about the 
 * quantities in a label:
 * 
 * 	- cosi::Label::GetQuantityNames returns the list of names of quantity in a label
 * 	- cosi::Label::HasQuantity return true if the quantity belongs to the label
 * 	- cosi::Label::Print prints the value of each quantity.
 *
 * The following examples shows how to define a label
 * 
 * \include networks/wiredbas/WiredBasSpecificaion.h
 * 
 * In this example, the cosi::commstruct::SpecificationLabel class is a vector of quantites that include 
 * the type of a component (i.e. a sensor, an actuator etc.), the ports of a component, the position of a
 * node, the set of send/receive threads running on a node, and the maximum latency associated with an 
 * end-to-end communication constraint.
 *
 *  
 * \section quantityimplementation Implementation Strategy
 *
 * In COSI we are not really interested in a single quantity but in labels that are vectors of quantities.
 * Because a quantity can be of any type, it is not trivial to architect the software such that
 * a label can hold an heterogenoeus vector of quantity.
 *
 * There are two requirements that should be satisfied by the C++ implementation. The first is that a label
 * should be able to hold a vector of heterogenous quantities. The second is that different labels should
 * have differet types depending on the quantities that they bear.
 * 
 * These are the reasons why a label is a template that takes as paramters quantities. In the current 
 * implementation there isn't a strict type checking to make sure that what is passed to the label template
 * is really derived from the base class cosi::Quantity and that the necessary functions are defined.
 * 
 * Since each quantity defines the same functions, how do we select for instance the Get() functions of 
 * a particular one? This behaviour is achieved by the use of a macro cosi::PRJ that takes as argument the 
 * name of the quantity class and selects the namespace of that quantity withing a label.
 * 
 *
 *
 */