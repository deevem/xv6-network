## Transmit descriptor(SDM.pdf page 35)
a structure to help **adapter** track packages received in the system memory. Three types: "legacy" and two extended ones, one called "TCP/IP Data Descriptor", similar to the legacy in that it points to a block of packed data. It is a replacement for legacy, offering new offloading capabilities. The second one don't point to packed data, but contains control information which is loaded into registers of the controller and affect the processing of future packets.

