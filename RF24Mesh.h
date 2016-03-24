





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
   * 
   * The following parameters are optional:
   * @param channel The radio channel (1-127) default:97
   * @param data_rate The data rate (RF24_250KBPS,RF24_1MBPS,RF24_2MBPS) default:RF24_1MBPS
   * @param timeout How long to attempt address renewal in milliseconds default:60000
   */
  bool begin(uint8_t channel = MESH_DEFAULT_CHANNEL, rf24_datarate_e data_rate = RF24_1MBPS, uint32_t timeout=MESH_RENEWAL_TIMEOUT );
  
  /**
   * Very similar to network.update(), it needs to be called regularly to keep the network
   * and the mesh going.
   */   
  uint8_t update();
  
  /**
   * Automatically construct a header and send a payload
   * Very similar to the standard network.write() function, which can be used directly.
   *
   * @note Including the nodeID parameter will result in an automatic address lookup being performed.
   * @note Message types 1-64 (decimal) will NOT be acknowledged by the network, types 65-127 will be. Use as appropriate to manage traffic:
   * if expecting a response, no ack is needed.
   * 
   * @param data Send any type of data of any length (Max length determined by RF24Network layer)
   * @param msg_type The user-defined (1-127) message header_type to send. Used to distinguish between different types of data being transmitted.
   * @param size The size of the data being sent
   * @param nodeID **Optional**: The nodeID of the recipient if not sending to master
   * @return True if success, False if failed
   */
  bool write(const void* data, uint8_t msg_type, size_t size, uint8_t nodeID=0);
  
  /**
   * Set a unique nodeID for this node. This value is stored in program memory, so is saved after loss of power.  
   * 
   * This should be called before mesh.begin(), or set via serial connection or other methods if configuring a large number of nodes...  
   * @note If using RF24Gateway and/or RF24Ethernet, nodeIDs 0 & 1 are used by the master node.
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
   * @param address If no address is provided, returns the local nodeID, otherwise a lookup request is sent to the master node
   * @return Returns the unique identifier (1-255) or -1 if not found.
   */
  int16_t getNodeID(uint16_t address=MESH_BLANK_ID);
  
  /**
   * Tests connectivity of this node to the mesh.
   * @note If this function fails, the radio will be put into standby mode, and will not receive payloads until the address is renewed.
   * @return Return 1 if connected, 0 if mesh not responding after up to 1 second
   */
  
  bool checkConnection();
  
  /**
  * Reconnect to the mesh and renew the current RF24Network address. Used to re-establish a connection to the mesh if physical location etc. has changed, or
  * a routing node goes down.
  * @note Currently times out after 1 minute if address renewal fails. Network writes should not be attempted if address renewal fails.
  *
  * @note If all nodes are set to verify connectivity/reconnect at a specified period, leaving the master offline for this length of time should result
  * in complete network/mesh reconvergence.
  * @param timeout How long to attempt address renewal in milliseconds default:60000
  
  * @return Returns the newly assigned RF24Network address
  */
  uint16_t renewAddress(uint32_t timeout=MESH_RENEWAL_TIMEOUT);
  
  /**
   * Releases the currently assigned address lease. Useful for nodes that will be sleeping etc.
   * @note Nodes should ensure that addresses are releases successfully prior to renewal.
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
   * @note If printing or displaying the address, it needs to be converted to octal format: Serial.println(address,OCT);
   *
   * Results in a lookup request being sent to the master node.
   * @param nodeID - The unique identifier (1-255) of the node
   * @return Returns the RF24Network address of the node or -1 if not found or lookup failed.
   */
  int16_t getAddress(uint8_t nodeID);

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
  * Allow child nodes to discover and attach to this node.
  * @param allow True to allow children, False to prevent children from attaching automatically.
  */
  void setChild(bool allow);
  
  /**
  * Set/change a nodeID/RF24Network Address pair manually on the master node.
  *
  * @code
  * Set a static address for node 02, with nodeID 23, since it will just be a static routing node for example
  * running on an ATTiny chip.
  * 
  * mesh.setStaticAddress(23,02);
  * @endcode
  * @param nodeID The nodeID to assign
  * @param address The octal RF24Network address to assign
  * @return If the nodeID exists in the list, 
  */
  void setAddress(uint8_t nodeID, uint16_t address);
  
  void saveDHCP();
  void loadDHCP();
  
  /**@}*/
  /**
   * @name Address list struct
   *
   *  See the list struct class reference
   */
  /**@{*/

  /**@}*/

  uint8_t _nodeID;

  
#if !defined RF24TINY  
  typedef struct{
	uint8_t nodeID;       /**< NodeIDs and addresses are stored in the addrList array using this structure */
	uint16_t address;  /**< NodeIDs and addresses are stored in the addrList array using this structure */
  }addrListStruct;
  
  // Pointer used for dynamic memory allocation of address list
  addrListStruct *addrList;  /**< See the addrListStruct class reference */
  uint8_t addrListTop;       /**< The number of entries in the assigned address list */
#endif

  /**
   * @name Deprecated
   *
   *  Methods provided for backwards compabibility with old/testing code.
   */
  /**@{*/
  
  /**
   * Calls setAddress()
   */
  void setStaticAddress(uint8_t nodeID, uint16_t address);
  
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
  uint16_t lastID,lastAddress;

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
 * @section Overview RF24Mesh Overview
 * The RF24Network library provides a system of addressing and routing for RF24 radio modules, that allows large wireless sensor networks to be constructed. <br> RF24Mesh
 * provides extended features, including automatic addressing and dynamic configuration of wireless sensors. 
 * 
 * **How does it work?**<br>
 * The 'master' node keeps track of the unique nodeIDs and the assigned RF24Network addresses. When a node is moved physically, or just loses its connection to the network,
 * it can be set to automatically re-join the mesh, and reconfigure itself within the network.
 *
 * Nodes are assigned a unique number ranging from 1 to 255, and just about everything else, addressing, routing, etc. is managed by the library.
 * 
 * In the 'mesh' configuration sensors/nodes can move around physically, far from the 'master node' using other nodes to route traffic over extended distances. Addressing and
 * topology can be reconfigured as connections are broken and re-established within different areas of the network.
 *
 * <b>What works currently? </b>
 * @li Basic 'mesh' functionality using an <b>Arduino or RPi</b> as the master node which will generally send or received data to/from nodes.
 * @li Simple, dynamic addressing based on a pre-assigned unique identifier
 * @li Dynamic/On-the fly configuration of addresses and network topology
 * @li Automatic routing and handling of data - Nodes will join together to extend the range of radio links by routing data as required.
 *  
 * <b>Status (Dec 2015)</b>
 * @li RF24Mesh is mostly complete and stable, but is being actively utilized & tested with development ongoing.
 * @li Address assignments are not saved, except on RPi/Linux devices. If the 'master' node goes down, all nodes need to reconnect to the mesh or restart to prevent addressing conflicts. 
 *
 *
 * @section More How to learn more
 * 
 * @li Try it out!
 * @li <a href="Setup-Config.html">Setup and Configuration</a>
 * @li <a href="General-Usage.html">Usage & Overview </a> 
 * @li <a href="classRF24Mesh.html">RF24Mesh Class Documentation</a>
 * @li <a href="http://tmrh20.github.io/RF24Network/">RF24 Network Class Documentation</a>
 * @li <a href="http://tmrh20.github.io/RF24Ethernet/">RF24Ethernet: TCP/IP based Mesh over RF24</a>
 * @li <a href="https://tmrh20.github.io">All Documentation and Downloads</a>
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
 * @li <a href="https://github.com/TMRh20/RF24Network/archive/master.zip">RF24Network Library</a>
 * @li <a href="https://github.com/TMRh20/RF24Mesh/archive/master.zip">RF24Mesh - Dynamic Mesh Library</a>
 *  
 * @section Installation Installation
 * 1. If not installed, use the Arduino Library Manager, or download and install the RF24, RF24Network, and RF24Mesh libraries per the above links  <br>
 *  <br>
 * 2. Configure and test the hardware using examples from RF24 and RF24Network prior to attempting to use RF24Mesh  <br>
 *    a: <b>In Arduino IDE:</b> File > Examples > RF24 > GettingStarted  <br>
 *    b: <b>RPi:</b> Follow the RF24 instructions on <a href="http://tmrh20.github.io/RF24/RPi.html">GitHub</a>  <br>
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
 * @section Net_Design Network Design Options
 * 
 * 1. **Static Network** (No Mesh): 
 * RF24Network can be configured manually, with a static design. RF24Mesh is not used at all. See http://tmrh20.github.io/RF24Network/Addressing.html <br><br>
 *
 * 2. **Static Network w/Dynamic Assignment:**
 * RF24Mesh is only used to acquire an address on startup. Nodes are generally expected to remain stationary. Changes to 
 * the network would be addressed manually, by adding, removing, or resetting nodes. Users can choose to use RF24Network functions directly, or use RF24Mesh. <br><br>
 *
 * 3. **Dynamic Network & Assignment:** 
 * Nodes join the mesh automatically and re-attach as required. <br><br>
 *
 * 4. **Hybrid Network**:
 * Utilizes a combination of static & dynamic nodes. Requires initial planning and deployment, but can result in a more stable network, easing
 * the use of sleeping nodes. <br><br>
 *
 * @section NetMgmt Network Management
 *
 * RF24Network addresses can be viewed as MAC addresses, and RF24Mesh nodeIDs viewed as static IP addresses. When joining or re-attaching to the network, nodes
 * will request a RF24Network address, and are identified via nodeID. 
 *
 * **Raspberry Pi/Linux:** On Linux devices, the RF24Gateway will save address assignments to file (dhcplist.txt) so they will be restored, even if the
 * gateway is restarted. To force network re-convergence, delete the dhcplist.txt file and restart the gateway. If nodes are configured to verify their connection at a set
 * interval, the gateway should be left offline for at least that period of time. 
 *
 * **Arduino/AVR:** On all other devices, the address list is not saved. To force network re-convergence, restart the gateway. If nodes are configured to verify their
 * connection at a set interval, the gateway should be left offline for at least that period of time.
 *
 * If a node/nodeID is removed from the network permanently, the address should be released prior to removal. If it is not, the assigned RF24Network address should be 
 * written to 0 in the RF24Mesh address list.
 * 
 * @section MeshComm Mesh Communication
 *
 * RF24Mesh nodeIDs are unique identifiers, while RF24Network addresses change dynamically within a statically defined structure. Due to this structure, it is simple
 * for any node to communicate with the master node, since the RF24Network address is always known (00). Conversely, the master node maintains a list of every node on the 
 * network, so address 'lookups' return immediately. 
 *
 * Communication from node-to-node requires address queries to be sent to the master node, since individual nodes may change RF24Network & radio address at any time. 
 * Due to the extra data transmissions, node-to-node communication is less efficient.
 *
 * @section General General Usage
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
 * @section RF24Network RF24Network
 * 
 * Beyond requesting and releasing addresses, usage is outlined in the class documentation, and further information regarding RF24Network is available at
 * http://tmrh20.github.io/RF24Network
 *
 *
 * 
 */
