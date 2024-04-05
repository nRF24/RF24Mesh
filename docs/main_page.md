# Mesh Networking Layer for RF24 Radios

@tableofcontents

This class intends to provide a simple and seamless 'mesh' layer for sensor networks,
allowing automatic and dynamic configuration that can be customized to suit many scenarios.
It is currently designed to interface directly with with the
[RF24Network library](http://nRF24.github.io/RF24Network), an
[OSI Network Layer](http://en.wikipedia.org/wiki/Network_layer) using nRF24L01(+) or NRF52x radios
driven by the [RF24 library](http://nRF24.github.io/RF24) or [nrf_to_nrf library](https://github.com/TMRh20/nrf_to_nrf).

## Purpose/Goals

- Provide a simple user interface for creating dynamic sensor networks with the RF24 and RF24Network libraries.
- Create stable, fully automated/self-managed networks

## News - 2023 API Changes

Introducing **RF24Network & RF24Mesh v2.0** with some *significant API changes*, adding the use of [C++ Templates](https://cplusplus.com/doc/oldtutorial/templates/)
in order to support a range of ESB enabled radios, most recently NRF52x radios.

**Important Notes:**
- Any network layer that uses v2 needs to have RF24Network/RF24Mesh dependencies of v2 or newer. RF24 v1.x is an exception here.
- General usage should remain backward compatible, see the included examples of the related libraries for more info
- Any third party libs that extend the network/mesh layer may also need to be updated to incorporate the new templated class prototypes:
```cpp
template<class radio_t>
class ESBNetwork;
  
template<class network_t, class radio_t>
class ESBMesh;
```
- Third party libs should also be able to use the backward-compatible typedef in their template:
  - ESBGateway.h:
  ```cpp
  template<typename network_t, typename mesh_t>
  class ESBGateway
  ```
  and inform the compiler what types they intend to support:
  - ESBGateway.cpp:
  ```cpp
  template class ESBGateway<RF24Network, RF24Mesh>;
  ```  
- The auto installers do not perform a version check like package managers, so having the correct versions of the software is important.
- We *will* be maintaining the v1.x versions with bugfixes etc for those who cannot or do not wish to migrate to the newer template approach.

See a the list of changes on [the Github releases page](https://github.com/nRF24/RF24Mesh/releases/)

## RF24Mesh Overview

The RF24Network library provides a system of addressing and routing for RF24 radio modules
that allows large wireless sensor networks to be constructed.

RF24Mesh provides extended features, including automatic addressing and dynamic configuration
of wireless sensors.

### How does it work?

Nodes are assigned a unique number ranging from 1 to 255, and just about everything else, addressing, routing, etc. is managed by the library.

The unique identifier is like an IP address, used to communicate at a high level within the
RF24 communication stack and will generally remain static. At the network layer, the physical
radio addresses, similar to MAC addresses, are allocated as nodes move around and establish
connections within the network.

The 'master' node keeps track of the unique nodeIDs and the assigned RF24Network addresses.
When a node is moved physically, or just loses its connection to the network,
it can automatically re-join the mesh and reconfigure itself within the network.

In the mesh configuration sensors/nodes can move around physically, far from the 'master
node' using other nodes to route traffic over extended distances. Addressing and
topology is reconfigured as connections are broken and re-established within different areas
of the network.

RF24Mesh takes advantage of functionality and features within the RF24 and RF24Network
libraries, so everything from addressing, routing, fragmentation/re-assembly
(very large payloads) are handled automatically with processes designed to support a
multi-node radio network.

## How to learn more

- Try it out!
- [Setup and Configuration](setup_config.md)
- [Usage & Overview](general_usage.md)
- [RF24Mesh Class Documentation](classRF24Mesh.html)
- [RF24 Network Class Documentation](http://nRF24.github.io/RF24Network/)
- [RF24Ethernet: TCP/IP based Mesh over RF24](http://nRF24.github.io/RF24Ethernet/)
- [RF24Gateway: A TCP/IP and RF24 Gateway for RF24 nodes](http://nRF24.github.io/RF24Gateway/)
- [All Documentation and Downloads](https://tmrh20.github.io)
- [Source Code](https://github.com/nRF24/RF24Mesh)
