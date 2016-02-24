# Adding Virtual Channels Support in COSI #

Adding virtual channels in COSI requires the following changes:

  * Each node must now have a set of properties for virtual channels. The property set can be as simple as the number of virtual channels, or it can include other properties that are relevant to the synthesis algorithms to be developed. The property set must be defined as a quantity in the quantity directory of COSI and the communication structure label must be extended to include these property set.
  * The library must be extended to include models of performance and cost for routers with virtual channels.
  * Synthesis algorithms can be then developed starting from the ones that are already in COSI. The result of the synthesis algorithms is a network where nodes have virtual channels specified.
  * The result of a synthesis algorithm can be used to generate a number of outputs. Code generators must be capable of generating the right output. In particular the SystemC code generator should instantiate routers with virtual channels (together with their configuration). This also means that the SystemC library should include routers with virtual channels.