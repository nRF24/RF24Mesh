# Setup And Config

@tableofcontents

The initial testing version of RF24Mesh is built as a simple overlay for RF24Network. Users
currently need to be familiar with the basics of sending and receiving data via
RF24Network, but do not need to understand the topology, routing or addressing systems.
RF24Mesh will attempt to construct and maintain a mesh network, keeping all nodes
connected together.

## Requirements

### Hardware Requirements

- 1 Raspberry Pi or Arduino to act as the Master Node
- 1 or more Arduino, Raspberry Pi, etc. (Sensor Nodes)
- 2 or more NRF24L01+ radio modules
- 1 or more various sensors for your sensor nodes

### Software Requirements

- [Download RF24 Core Radio Library](https://github.com/TMRh20/RF24/archive/master.zip)
- [Download RF24Network Library](https://github.com/TMRh20/RF24Network/archive/master.zip)
- [Download RF24Mesh - Dynamic Mesh Library](https://github.com/TMRh20/RF24Mesh/archive/master.zip)

## Installation

1. Use the Arduino Library Manager. Selecting RF24Mesh should also install RF24Network and RF24 Core libraries
2. Configure and test the hardware using examples from RF24 and RF24Network prior to attempting to use RF24Mesh
   - In Arduino IDE
      - File > Examples > RF24 > GettingStarted
        @see [Arduino Support page](http://nRF24.github.io/RF24/md_docs_arduino.html)
   - For a Raspberry Pi
      - An installer is provided: [Linux Installation](http://nRF24.github.io/RF24/md_docs_linux_install.html)
        @see [General Linux/RPi setup and configuration page](http://nRF24.github.io/RF24/md_docs_rpi_general.html)
3. Once testing is complete:
   - In Arduino IDE
      - File > Examples > RF24Mesh > RF24Mesh_Example
   - For a Raspberry Pi
      - Run `make` from the examples directory. Then `sudo ./RF24Mesh_Example_Master` to begin an example as the master node
4. Once configured and running, the Master Node will begin to assign addresses to the sensor nodes, which will find their way onto the network, and
   display incoming data from the sensor examples. Usage is very much the same as RF24Network, except for address assignment and network management.

## Configuration

As per the examples, nodes are configured with a unique value between 1 and 253. This allows
them to change positions on the network while still being identified.

For pre-configuration of the mesh, some options are available by editing RF24Mesh_config.h
prior to compiling:

### Restrict number of children

```cpp
#define MESH_MAX_CHILDREN 4
```

The @ref MESH_MAX_CHILDREN option restricts the maximum number of child nodes/node and limits the number of available addresses on the network. Max: 4

### Reduce resource consumption

```cpp
#define MESH_NOMASTER
```

The MESH_NOMASTER macro optionally reduces program space and memory usage. Can be used on any node except for the master (nodeID 0)

@see [General Usage](md_docs_general_usage.html) for information on how to work with the mesh once connected
