





#ifndef __RF24MESH_H__
#define __RF24MESH_H__

#include <stdio.h>
#include <stdlib.h>

/** 
 * Network/Mesh Response Types  
 * The network will determine whether to automatically acknowledge payloads based on their type 
 * RF24Mesh uses pre-defined system types for interacting with RF24Network at the system level
 * 
 */

// Network ACK types 
#define MESH_ADDR_CONFIRM 129
// No Network ACK types
#define MESH_ADDR_LOOKUP 196
#define MESH_ADDR_RELEASE 197
#define MESH_ID_LOOKUP 198
 
#define MESH_BLANK_ID 65535
 
/**
 * @file RF24Mesh.h
 *
 * Class declaration for RF24Mesh
 */

#if defined (__AVR_ATtiny85__) || defined (__AVR_ATtiny84__)
	#define RF24_TINY
#endif


#include "RF24Mesh_config.h"

#if defined (__linux) && !defined(__ARDUINO_X86__)
  #include <RF24/RF24.h>
  #include <RF24Network/RF24Network.h>
  #define RF24_LINUX
#else
  #include <RF24.h>
  #include <RF24Network.h>
#endif

  #include <stddef.h>
  #include <stdint.h>



class RF24;
class RF24Network;


class RF24Mesh
{
  /**@}*/
  /**
   * @name RF24Mesh
   *
   *  The mesh library and class documentation is currently in active development and usage may change.
   */
  /**@{*/
public:

  /**
   * Construct the mesh:
   *
   * @code
   * RF24 radio(7,8);
   * RF24Network network(radio);
   * RF24Mesh mesh(radio,network);
   * @endcode
   * @param _radio The underlying radio driver instance
   * @param _network The underlying network instance
   */

  RF24Mesh( RF24& _radio,RF24Network& _network );
  
  /**
   * Call this in setup() to configure the mesh and request an address.  <br>
   *  
   * @code mesh.begin(); @endcode
   * This may take a few moments to complete. 
   * The radio channel and data-rate can be specified optionally as well
   */
  bool begin(uint8_t channel = MESH_DEFAULT_CHANNEL, rf24_datarate_e data_rate = RF24_1MBPS );
  
  /**
   * Very similar to network.update(), it needs to be called regularly to keep the network
   * and the mesh going.
   */   
  uint8_t update();
  
  /**
   * Automatically construct a header and send a payload to the 'master' node.
   * Very similar to the standard network.write() function, which can be used directly.
   * @param data Send any type of data of any length (Very large payloads will be more error prone)
   * @param msg_type The user-defined (1-127) message header_type to send. Used to distinguish between different types of data being transmitted.
   * @param size The size of the data being sent
   * @param nodeID Optional: The nodeID of the recipient if not sending to master
   * @return True if success, False if failed
   */
  bool write(const void* data, uint8_t msg_type, size_t size, uint8_t nodeID=0);
  
  /**
   * Set a unique nodeID for this node. This value, if changed, will be written to EEPROM on AVR so it will remain set, even after loss of power or code changes.  
   * Currently not saved on Due or RPi, so should be manually configured.
   * This can generally be called before mesh.begin(), or set via serial connection or other methods if configuring a large number of nodes...  
   * @param nodeID Can be any unique value ranging from 1 to 255. 
   */
  void setNodeID(uint8_t nodeID);
  
 /**
  * Only to be used on the master node. Provides automatic configuration for sensor nodes, similar to DHCP.
  * Call immediately after calling network.update() to ensure address requests are handled appropriately
  */  
  void DHCP();
  
  /**@}*/
  /**
   * @name Advanced Operation
   *
   *  For advanced configuration and usage of the mesh
   */
  /**@{*/
  
  /**
   * Convert an RF24Network address into a nodeId.
   * When called on any node but the master node, this will result in a lookup request being sent to the master node
   * 
   * @return Returns the unique identifier (1-255) or -1 if not found.
   */
  int getNodeID(uint16_t address=MESH_BLANK_ID);
  
  /**
   * Tests connectivity of this node to the mesh.
   * @return Return 1 if connected, 0 if mesh not responding after up to 1 second
   */
  
  bool checkConnection();
  
  /**
  * Reconnect to the mesh and renew the current RF24Network address. Used to re-establish a connection to the mesh if physical location etc. has changed, or
  * a routing node goes down.
  * @note Currently blocks until a connection is established and an address is received.
  * @return Returns the newly assigned RF24Network address
  */
  uint16_t renewAddress(uint32_t timeout=3000);
  
  /**
   * Releases the currently assigned address lease. Useful for nodes that will be sleeping etc.
   * @return Returns 1 if successfully released, 0 if not
   */
  bool releaseAddress();
  
  /**
   * The assigned RF24Network (Octal) address of this node
   * @return Returns an unsigned 16-bit integer containing the RF24Network address in octal format
   */
  uint16_t mesh_address; 
  
  /**
   * Convert a nodeID into an RF24Network address
   * @note If printing or displaying the address, it needs to be converted to octal format.
   *
   * When called on any node but the master node, this will result in a name lookup request being sent to the master node, and a response
   * returned containing the address corresponding to the included nodeID.
   * @param nodeID - The unique identifier (1-255) of the node
   * @return Returns the RF24Network address of the node or 0 if not found or lookup failed.
   */
  uint16_t getAddress(uint8_t nodeID);

  /**
   * Write to a specific node by RF24Network address.
   *
   */
  bool write(uint16_t to_node, const void* data, uint8_t msg_type, size_t size );
  
  /**
  * Change the active radio channel after the mesh has been started.
  */
  void setChannel(uint8_t _channel);
  
  /**
  * Set a static nodeID/RF24Network Address pair.
  * Mainly for use with nodes not using RF24Mesh, but RF24Network only.
  * Set a static address assignment, that will not be updated or re-assigned.
  * @code
  * Set a static address for node 02, with nodeID 23, since it will just be a static routing node for example
  * running on an ATTiny chip.
  * 
  * mesh.setStaticAddress(23,02);
  * @endcode
  */
  void setStaticAddress(char nodeID, uint16_t address);
  
  void saveDHCP();
  void loadDHCP();
  
  /**@}*/
  /**
   * @name Address list struct
   *
   *  See the list struct class reference
   */
  /**@{*/
  
#if !defined RF24TINY  
  typedef struct{
	uint8_t nodeID;       /**< NodeIDs and addresses are stored in the addrList array using this structure */
	uint16_t address;  /**< NodeIDs and addresses are stored in the addrList array using this structure */
  }addrListStruct;
  
  // Pointer used for dynamic memory allocation of address list
  addrListStruct *addrList;  /**< See the addrListStruct class reference */
  uint8_t addrListTop;       /**< The number of entries in the assigned address list */
#endif
  
  private:
  RF24& radio;
  RF24Network& network;  
  bool findNodes(RF24NetworkHeader& header, uint8_t level, uint16_t *address); /**< Broadcasts to all multicast levels to find available nodes **/
  bool requestAddress(uint8_t level); /**< Actual requesting of the address once a contact node is discovered or supplied **/
  bool waitForAvailable(uint32_t timeout); /**< Waits for data to become available */
  bool doDHCP; /**< Indicator that an address request is available */
  uint32_t lastSaveTime;
  uint32_t lastFileSave;
  uint8_t radio_channel;

  public:

  
  //#if (defined (ARDUINO_SAM_DUE) || defined (__linux) || defined(RF24_TINY) || defined (CORE_TEENSY)) && !defined(__ARDUINO_X86__)
	uint8_t _nodeID;
  //#endif
 };
 
 #endif
 
 
 /**
  * @example RF24Mesh_Example.ino
  * <b> Arduino Example Sketch </b><br>
  * This example sketch shows how to manually configure a node via RF24Mesh, and send data to the
  * master node.
  * The nodes will refresh their network address as soon as a single write fails. This allows the
  * nodes to change position in relation to each other and the master node.
  */
  
 /**
  * @example RF24Mesh_Example_Master.ino
  * <b> Arduino Example Sketch </b><br>
  * @note This sketch only functions on -Arduino Due-
  *
  * This example sketch shows how to manually configure a node via RF24Mesh as a master node, which
  * will receive all data from sensor nodes.
  *
  * The nodes can change physical or logical position in the network, and reconnect through different
  * routing nodes as required. The master node manages the address assignments for the individual nodes
  * in a manner similar to DHCP.
  *
  */

  /**
  * @example RF24Mesh_Example_Node2Node.ino
  * Example of node to node communication using RF24Mesh
  */
  
  /**
  * @example RF24Mesh_Example_Node2NodeExtra.ino
  * Extended Example of node to node communication using RF24Mesh
  */
  
 /**
  * @example RF24Mesh_SerialConfig.ino
  * 
  * This example sketch shows how the same sketch can be written to a large number of devices, which are
  * configured later via Serial input. 
  *
  */
 
  /**
  * @example RF24Mesh_Example.cpp
  * 
  * <b> Raspberry Pi Example Sketch </b><br>
  * This example sketch shows how to manually configure a node via RF24Mesh, and send data to the
  * master node.
  * The nodes will refresh their network address as soon as a single write fails. This allows the
  * nodes to change position in relation to each other and the master node.
  */
  
 /**
  * @example RF24Mesh_Example_Master.cpp
  *
  * <b> Raspberry Pi Example Sketch </b><br>
  *
  * This example sketch shows how to manually configure a node via RF24Mesh as a master node, which
  * will receive all data from sensor nodes.
  *
  * The nodes can change physical or logical position in the network, and reconnect through different
  * routing nodes as required. The master node manages the address assignments for the individual nodes
  * in a manner similar to DHCP.
  *
  */
  
  /**
  * @example RF24Mesh_Ncurses_Master.cpp
  * A very limited ncurses interface used for initial monitoring/testing of RF24Mesh
  * <img src="tmrh20/RF24Mesh_Ncurses.JPG">
  */
  
/**
 * @mainpage Mesh Networking Layer for RF24 Radios
 *
 * This class intends to provide a simple and seamless 'mesh' layer for sensor networks, allowing automatic and dynamic configuration
 * that can be customized to suit many scenarios. It is currently designed to interface directly with with the
 * <a href="http://tmrh20.github.com/RF24Network/">RF24Network Development library</a>, an <a href="http://en.wikipedia.org/wiki/Network_layer">OSI Network Layer</a> using nRF24L01(+) radios driven
 * by the newly optimized <a href="http://tmrh20.github.com/RF24/">RF24</a> library fork.
 *
 * @section Purpose Purpose/Goal
 *
 * Provide a simple user interface for creating dynamic sensor networks with the RF24 and RF24Network libraries.
 *
 *
 * <b>What works currently?</b>
 * @li Basic 'mesh' functionality using an <b>Arduino or RPi</b> as the master node which will generally receive the sensor data
 * @li Simple, dynamic addressing based on a pre-assigned unique identifier
 * @li Dynamic/On-the fly configuration of addresses and network topology
 *  
 * <b>What are the functional limitations?</b>
 * @li Address assignments are not saved, except on RPi/Linux devices. If the 'master' node goes down, all nodes need to reconnect to the mesh or restart to prevent addressing conflicts. 
 *
 *
 * @section Overview RF24Mesh Overview
 * The RF24Network library provides a system of addressing and routing for RF24 radio modules, that allows large wireless sensor networks to be constructed. RF24Mesh
 * provides extended features, including automatic addressing and dynamic configuration of wireless sensors. 
 * 
 * How does it work?
 * The 'master' node keeps track of the unique nodeIDs and the assigned RF24Network addresses. When a node is moved physically, or just loses its connection to the network,
 * it can be set to automatically re-join the mesh, and reconfigure itself within the network.
 *
 * Nodes are assigned a unique number ranging from 1 to 255, and just about everything else, addressing, routing, etc. is managed by the library.
 * 
 * In the 'mesh' configuration sensors/nodes can move around physically, far from the 'master node' using other nodes to route traffic over extended distances. Addressing and
 * topology can be reconfigured as connections are broken and re-established within different areas of the network.
 * 
 *
 * @section More How to learn more
 * 
 * @li Try it out!
 * @li <a href="Setup-Config.html">Setup and Configuration</a>
 * @li <a href="classRF24Mesh.html">RF24Mesh Class Documentation</a>
 * @li <a href="http://tmrh20.github.io/RF24Network_Dev/">RF24 Network -DEV- Class Documentation</a>
 * @li <a href="https://tmrh20.github.io">Documentation and Downloads</a>
 * @li <a href="https://github.com/TMRh20/RF24Mesh">Source Code</a>
 *
 * @page Setup-Config Setup And Config
 *
 * The initial testing version of RF24Mesh is built as a simple overlay for RF24Network. Users currently need to be familiar with the basics of sending and receiving data via
 * RF24Network, but do not need to understand the topology, routing or addressing systems. RF24Mesh will attempt to construct and maintain a mesh network, keeping all nodes
 * connected together.
 *
 * @section Requirements Requirements
 * <b>Hardware Requirements:</b>  <br>
 * @li 1 Raspberry Pi or Arduino to act as the Master Node <br>
 * @li 1 or more Arduino,RPi,etc. (Sensor Nodes)  <br>
 * @li 1 or more NRF24L01+ radio modules  <br>
 * @li 1 or more various sensors for your sensor nodes  <br>
 *  
 * <b>Software Requirements:</b>
 * @li <a href="https://github.com/TMRh20/RF24/archive/master.zip">RF24 Core Radio Library</a>
 * @li <a href="https://github.com/TMRh20/RF24Network/archive/Development.zip">RF24Network Development Library</a>
 * @li <a href="https://github.com/TMRh20/RF24Mesh/archive/master.zip">RF24Mesh - Dynamic Mesh Library</a>
 *  
 * @section Installation Installation
 * 1. If not installed, download and install the RF24, RF24Network DEV, and RF24Mesh libraries per the above links  <br>
 *  <br>
 * 2. Configure and test the hardware using examples from RF24 and RF24Network prior to attempting to use RF24Mesh  <br>
 *    a: <b>In Arduino IDE:</b> File > Examples > RF24 > GettingStarted  <br>
 *    b: <b>RPi:</b> Follow the Quick-Start instructions on <a href="https://github.com/TMRh20/RF24Network/tree/Development/RPi">GitHub</a>  <br>
 *  <br>
 * 3. Once testing is complete:  <br>
 *    a: <b>Arduino IDE:</b> File > Examples > RF24Mesh > RF24Mesh_Example  <br>
 *    b: <b>RPi:</b> Run 'make' from the examples directory. Then 'sudo ./RF24Mesh_Example_Master' to begin as the master node   <br>
 *    <br>
 * 4. Once configured and running, the Master Node will begin to assign addresses to the sensor nodes, which will find their way onto the network, and
 *    display incoming data from the sensor examples. Usage is very much the same as RF24Network, except for address assignment and network management.  
 *
 * @section Config Configuration
 *
 * As per the examples, nodes are configured with a unique value between 1 and 255. This allows them to change positions on the network, while still being identified. <br>
 *
 * For pre-configuration of the mesh, only a few options are available by editing RF24Mesh_config.h prior to compiling: <br>
 * 
 * @code  
 * #define MESH_MAX_CHILDREN 4  
 * @endcode
 * The max_children option restricts the maximum number of child nodes/node and limits the number of available addresses on the network. Max: 4 <br>
 *
 * @code  
 * #define MESH_NOMASTER  
 * @endcode
 * The nomaster option reduces program space and memory usage. Can be used on any node except for the master (nodeID 0) <br>
 *
 * 
 * @li See <a href="General-Usage.html">General Usage</a> for information on how to work with the mesh once connected.
 * 
 * @page General-Usage General Usage
 * 
 * One thing to keep in mind is the dynamic nature of RF24Mesh, and the need to verify connectivity to the network. For nodes that are constantly transmitting,
 * (every few seconds at most) it is suitable to check the connection, and/or renew the address when connectivity fails. Since data is not saved by the master 
 * node, if the master node goes down, all child nodes must renew their address. In this case, as long as the master node is down for a few seconds, the nodes
 * will all begin requesting an address.  <br>
 *  
 * Nodes that are not actively transmitting, should be configured to test their connection at predefined intervals, to allow them to reconnect as necessary.<br>
 *  
 * In the case of sleeping nodes, or nodes that will only be online temporarily, it is generally suitable to release the address prior to going offline, and 
 * requesting an address upon waking. Keep in mind, address requests can generally take anywhere from 10-15ms, up to few seconds in most cases. <br>
 *   
 * One of the recently introduced features is the ability to transmit payloads without the network returning a network-ack response. If solely using this method
 * of transmission, the node should also be configured to verify its connection via mesh.checkConnection(); periodically, to ensure connectivity.
 *
 * Beyond requesting and releasing addresses, usage is as outlined in the RF24Network Development documentation at http://tmrh20.github.io
 *
 *
 * 
 */
