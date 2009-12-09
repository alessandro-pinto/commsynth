/**
 * \page wired_bas_package_ethernet Ethernet COSI library
 * 
 * The ethernet model in COSI is divided in two layers: PHY and MAC.
 * At the physical level, the delay is due to the propagation delay of the wires and the dealy due to the 
 * signaling speed. Notice that this contribution is a per-bit contribution.
 * A the mac level, the protocol overhead is also taken into account. Sensors and actuators are connected 
 * to switches. Each sensor has a dedicated port on a switch therefore there is not bus contention.
 * However, when multiple inputs want to reach the same ouptut, the switch uses a round-robin arbitration 
 * scheme among them. Therefore, the worst case delay for a packet arriving at a switch depends on the 
 * traffic conditions at the other inputs.
 * 
 * 
 * 
* */