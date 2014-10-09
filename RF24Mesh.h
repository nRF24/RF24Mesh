





#ifndef __RF24MESH_H__
#define __RF24MESH_H__

/**
 * @file RF24Mesh.h
 *
 * Class declaration for RF24Mesh
 */

#if defined (__AVR_ATtiny85__) || defined (__AVR_ATtiny84__)
	#define RF24_TINY
#endif



#if defined (__linux)
  #include <RF24/RF24.h>
  #include <RF24Network/RF24Network.h>
#else
  #include <RF24.h>
  #include <RF24Network.h>
  #include "RF24Mesh_config.h"
  #if defined (__AVR__) && !defined(RF24_TINY)
    #include <EEPROM.h>
  #endif
#endif

  #include <stddef.h>
  #include <stdint.h>
  
#if defined (__linux) || defined (ARDUINO_SAM_DUE)
  #include <map>
#endif



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
   */
  void begin();
  
  /**
   * Very similar to network.update(), it needs to be called regularly to keep the network
   * and the mesh going.
   */   
  void update();
  
  /**
   * Automatically construct a header and send a payload to the 'master' node.
   * Very similar to the standard network.write() function, which can be used directly.
   * @param data Send any type of data of any length (Very large payloads will be more error prone)
   * @param msg_type The user-defined (1-127) message header_type to send. Used to distinguish between different types of data being transmitted.
   * @return True if success, False if failed
   */
  bool write(const void* data, uint8_t msg_type, size_t size);
  
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
   * Get the nodeID of this node. The nodeID is linked to the assigned RF24Network address but is unique.
   * @return Returns the nodeID saved in memory OR 0 if not found.
   */
  uint8_t getNodeID();
  
  /**
   * Tests connectivity of this node to the mesh.
   * @return Return 1 if connected, 0 if mesh not responding after up to 1 second
   */
  
  bool checkConnection();
  
  /**
  * Reconnect to the mesh and renew the current RF24Network address. Used to re-establish a connection to the mesh if physical location etc. has changed, or
  * a routing node goes down.
  * @note Currently blocks until a connection is established and an address is received.  
  */
  void renewAddress();
  
  /**
   * Releases the currently assigned address lease. Useful for nodes that will be sleeping etc.
   */
  void releaseAddress();
  
  /**
   * The assigned RF24Network (Octal) address of this node 
   */
  uint16_t mesh_address; 
  
  /**
   * Convert a nodeID into an RF24Network address
   * @note If printing or displaying the address, it needs to be converted to octal format.
   *
   * When called on any node but the master node, this will result in a name lookup request being sent to the master node, and a response
   * returned containing the address corresponding to the included nodeID.
   * @return Returns the RF24Network address of the node or 0 if not found or lookup failed.
   */
  uint16_t getAddress(uint8_t nodeID);
  
  private:
  RF24& radio;
  RF24Network& network;  
  bool findNodes(RF24NetworkHeader& header, uint8_t level, uint16_t *address); /**< Broadcasts to all multicast levels to find available nodes **/
  bool requestAddress(uint8_t level); /**< Actual requesting of the address once a contact node is discovered or supplied **/
  bool waitForAvailable(uint32_t timeout); /**< Waits for data to become available */
  bool doDHCP; /**< Indicator that an address request is available */
  uint32_t lastSaveTime;
  
  public:
  #if defined (ARDUINO_SAM_DUE) || defined (__linux)
	std::map<char,uint16_t> addrMap;	
  #endif
  #if defined (ARDUINO_SAM_DUE) || defined (__linux) || defined(RF24_TINY) || defined (CORE_TEENSY)
	uint8_t _nodeID;
  #endif
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
 * What works currently?
 * @li Basic 'mesh' functionality using an RPi or Arduino Due as the master node which will receive the sensor data
 * @li Simple, dynamic addressing based on a pre-assigned unique identifier
 * @li Dynamic/On-the fly configuration of addresses and network topology
 *  
 * What are the functional limitations?
 * @li Address assignments are not saved. If the 'master' node goes down, all nodes need to reconnect to the mesh or restart to prevent addressing conflicts. 
 *
 * The layer does not (yet) provide:
 * @li A lot! This code is in the initial stages of development only, and may not function exactly as desired. 
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
 * @li 1 Raspberry Pi or Arduino Due to act as the Master Node  <br>
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
 *    a: <b>In Arduino IDE:</b> File > RF24 > GettingStarted  <br>
 *    b: <b>RPi:</b> Follow the Quick-Start instructions on <a href="https://github.com/TMRh20/RF24Network/tree/Development/RPi">GitHub</a>  <br>
 *  <br>
 * 3. Once testing is complete:  <br>
 *    a: <b>Arduino IDE:</b> File > RF24Mesh > RF24Mesh_Example  <br>
 *    b: <b>RPi:</b> Run 'make' from the examples directory. Then 'sudo ./RF24Mesh_Example_Master' to begin as the master node   <br>
 *    <br>
 * 4. Once configured and running, the Master Node will begin to assign addresses to the sensor nodes, which will find their way onto the network, and
 *    display incoming data from the sensor examples. Usage is very much the same as RF24Network, except for address assignment and network management.  
 *
 * @section Config Configuration
 *
 * As per the examples, nodes are configured with a unique value between 1 and 255. This allows them to change positions on the network, while still being identified. <br>
 *
 * For pre-configuration of the mesh, only one main option is available by editing RF24Mesh_config.h prior to compiling: <br>
 * 
 * @code #define MESH_MAX_CHILDREN 4 @endcode
 * This option restricts the maximum number of child nodes/node and limits the number of available addresses on the network. Max: 4
 *
 */
