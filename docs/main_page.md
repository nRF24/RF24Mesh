# Mesh Networking Layer for RF24 Radios

@tableofcontents

This class intends to provide a simple and seamless 'mesh' layer for sensor networks,
allowing automatic and dynamic configuration that can be customized to suit many scenarios.
It is currently designed to interface directly with with the
[RF24Network library](http://nRF24.github.io/RF24Network), an
[OSI Network Layer](http://en.wikipedia.org/wiki/Network_layer) using nRF24L01(+) radios
driven by the newly optimized [RF24 library](http://nRF24.github.io/RF24) fork.

## Purpose/Goals

- Provide a simple user interface for creating dynamic sensor networks with the RF24 and RF24Network libraries.
- Create stable, fully automated/self-managed networks

## News

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
- [Setup and Configuration](md_docs_setup_config.html)
- [Usage & Overview](md_docs_general_usage.html)
- [RF24Mesh Class Documentation](classRF24Mesh.html)
- [RF24 Network Class Documentation](http://nRF24.github.io/RF24Network/)
- [RF24Ethernet: TCP/IP based Mesh over RF24](http://nRF24.github.io/RF24Ethernet/)
- [RF24Gateway: A TCP/IP and RF24 Gateway for RF24 nodes](http://nRF24.github.io/RF24Gateway/)
- [All Documentation and Downloads](https://tmrh20.github.io)
- [Source Code](https://github.com/nRF24/RF24Mesh)
