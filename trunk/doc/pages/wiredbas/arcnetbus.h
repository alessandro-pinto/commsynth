/** \page arcnetbusmodels ARCNET Bus Models
 *
 *	\section arcnetnodemodel ARCNET Node Model
 *
 * 	\subsection arcnetnodecostmodel ARCNET Node Cost Model
 *  \subsection arcnetnodedelaymodel ARCNET Node Delay Model
 *
 *
 * 	\section arcnetbusmodel ARCNET Bus Model
 *
 * 	\subsection arcnetbuscostmodel ARCNET Bus Cost Model
 *
 *  \subsection arcnetbusdelaymodel ARCNET Bus Delay Model
 *
 * 	The delay model for an archnet bus computes the toke rotation time.
 *  This delay is the worst case delay for the bus. An introduction to ARCNET can
 *  be found <A href="../../libraries/wiredbas/doc/Tutorial.pdf">here</A>.
 *
 * 	<IMG SRC="../images/daisychainbus.png" align="middle">
 * <CENTER> <B> Figure_arcnettokenring : The ARCNET token ring protocol.</B> </CENTER>
 * \anchor Figure_arcnettokenring
 *
 * \ref Figure_arcnettokenring shows the ARCNET token ring protocol. Node are logically
 * organized in a ring with an order that depends on the address of the nodes. Before
 * sending a message, a node needs to wait for an invitation to transmit (IIT) that is
 * sent by the previous node in the ring. The, a node sends a free buffer enquiry (FBE)
 * message and waits for an acknoledge form the destination node. The node, then. sends
 * the packet whose length can vary and waits for the final acknoledge from the
 * destination node.
 *
 * The token rotation time can be computed knowing the nodes connected on the bus,
 * the message period and the message length for each of them. This information
 * is stored in the cosi::ThreadSet of each node. Each node on the bus
 * can have multiple threads. Clearly, only sending threads (i.e. those threads
 * with a non empty destination) contribute to the bus traffic. For a node, we can
 * have the following cases:
 *
 * 	- A node is a source. In this case the node only has sending threads. To compute
 *    the worst case token rotation time, only the thread with longest messages
 *    needs to be taken into account.
 * 	- A node is a destination. In this case the node only has receiving threads.
 *    This node contributes to the total delay but a token passing message only
 *    (i.e. the length of the IIT).
 * 	- A node is a source of some messages and a destination of some others. This is
 *    the case for controllers that receive messages from sources (i.e. sensors), and
 *    send messages to destinations (i.e. actuators). To compute the worst case
 *    token rotation time, only the sending thread with longest messages needs to be
 *    taken into account.
 *
 * Given a communication structure \f$ N(C,Q,L) \f$ capturing a bus,
 * the bus delay model can compute the worst case token rotation time,
 * and the worst case delay for a thread to access the bus. The model does not
 * check that the communication structure is actually a bus. This must be ensured by
 * the algorithm.
 *
 * The token rotation time can be computed as follows:
 *
 * 	-# \f$ N \leftarrow 0 \f$
 * 	-# for each arcnet node \f$ v \in C \f$
 * 		-# let \f$ \hat{l}(v)\f$ be the maximum message length among all threads
 * 		-# \f$ N \leftarrow N + \textrm{ComputeMsgLength}(\hat{l}(v)) \f$
 * 	-# return \f$ N * T_{bit} \f$
 *
 * The algorithm computes the total number of bits sent during a token rotation.
 * This is worst case analysis because the longest messages only are considered.
 * The total number of bits is divided by \f$T_{bit}\f$ which is the bit time
 * compute as the inverse of the bust speed.
 *
 * Procedure \f$\textrm{ComputeMsgLength}(l)\f$ computes the length of a bus message
 * that is the sum of IIT, FBE, ACK, PAC and final ACK. The length of PAC depends on
 * the parameter \f$l\f$.
 *
 *
 * */