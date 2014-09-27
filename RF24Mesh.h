





#ifndef __RF24MESH_H__
#define __RF24MESH_H__

/**
 * @file RF24Mesh.h
 *
 * Class declaration for RF24Mesh
 */

#include <RF24.h>
#include <RF24Network.h>
#include <RF24Mesh_config.h>
#if defined (__AVR__)
#include "EEPROM.h"
#endif
#include <stddef.h>
#include <stdint.h>
//#include "RF24Mesh_config.h"
#if defined (ARDUINO_SAM_DUE)
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
  * Reconnect to the mesh and renew the current RF24Network address. Used to re-establish a connection to the mesh if physical location etc. has changed, or
  * a routing node goes down.
  * @note Currently blocks until a connection is established and an address is received.  
  */
  void renewAddress();
  
  /**
  * Only to be used on the master node. Provides automatic configuration for sensor nodes, similar to DHCP.
  * Call immediately after calling network.update() to ensure address requests are handled appropriately
  */  
  void DHCP();
  
  /**< The assigned RF24Network (Octal) address of this node */
  uint16_t mesh_address; 
  
  private:
  RF24& radio;
  RF24Network& network;  
  bool findNodes(uint8_t level, uint16_t *address); /**< Broadcasts to all multicast levels to find available nodes **/
  bool requestAddress(uint8_t level); /**< Actual requesting of the address once a contact node is discovered **/
  bool waitForAvailable(uint32_t timeout); /**< Waits for data to become available */
  uint32_t lastSaveTime;
  
  #if defined (ARDUINO_SAM_DUE)
	std::map<char,uint16_t> addrMap;
	uint8_t _nodeID;
  #endif
 };
 
 #endif
 
 
 /**
  * @example RF24Mesh_Example.ino
  *
  * This example sketch shows how to manually configure a node via RF24Mesh, and send data to the
  * master node.
  * The nodes will refresh their network address as soon as a single write fails. This allows the
  * nodes to change position in relation to each other and the master node.
  */
  
 /**
  * @example RF24Mesh_Example_Master.ino
  *
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
 * @li <a href="classRF24Mesh.html">RF24Mesh Class Documentation</a>
 * @li <a href="https://tmrh20.github.io">Documentation and Downloads</a>
 * @li <a href="https://github.com/TMRh20/RF24Mesh">Source Code</a>
 *  
 */
