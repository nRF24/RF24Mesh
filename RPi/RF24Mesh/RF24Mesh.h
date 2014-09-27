





#ifndef __RF24MESH_H__
#define __RF24MESH_H__

/**
 * @file RF24Mesh.h
 *
 * Class declaration for RF24Mesh
 */

#include <RF24/RF24.h>
#include <RF24Network/RF24Network.h>
//#include <RF24Mesh/RF24Mesh_config.h>
#if defined (__AVR__)
#include "EEPROM.h"
#endif
#include <stddef.h>
#include <stdint.h>
//#include "RF24Mesh_config.h"
//#if defined (ARDUINO_SAM_DUE)
  #include <map>
//#endif

class RF24;
class RF24Network;


class RF24Mesh
{
public:
  /**
   * Construct the mesh
   *
   * @param _radio The underlying radio driver instance
   * @param _network The underlying network instance
   */

  RF24Mesh( RF24& _radio,RF24Network& _network );
  
  /**
   * Call this in setup() to configure the mesh and request an address.  <br>
   * This may take a few moments.  
   */
  void begin();
  
  /**
   * Automatically construct a header and send a payload to the master node.<br>
   * It is almost as easy just to use the standard network.write() functions.
   * @param data Send any type of data of any length (Very large payloads will be more error prone)
   * @param msg_type The user-defined (1-127) message header_type to send. Used to distinguish between different types of data being transmitted.
   * @return True if success, False if failed
   */
  bool write(const void* data, uint8_t msg_type, size_t size);
  
  /**
   * Set a unique nodeID for this node. This value, if changed, will be written to EEPROM on AVR so it will remain set, even after loss of power.  
   * This can be called before mesh.begin(), or set via serial connection or other if configuring a large number of nodes...  
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
  * Reconnect to the mesh and renew the current RF24Network address. Can be used to re-establish communication if location etc. has changed.
  * @note Currently blocks until a connection is established and an address is received.  
  */
  void renewAddress();
  
  /**
  * Only to be used on the master node. Provides automatic configuration for sensor nodes.
  * Call immediately after calling network.update() to ensure address requests are handled appropriately
  */  
  void DHCP();
  
  uint16_t mesh_address; /**< The RF24Network address of this node */
private:
  RF24& radio;
  RF24Network& network;
  
  bool findNodes(RF24NetworkHeader& header,uint8_t level); /**< Broadcasts to all multicast levels to find available nodes **/
  bool requestAddress(uint8_t level); /**< Actual requesting of the address once a contact node is discovered **/
  bool waitForAvailable(uint32_t timeout); /**< Waits for data to become available */
  uint32_t lastSaveTime;
  uint32_t lastFileWrite;
  
  //#if defined (ARDUINO_SAM_DUE)
	std::map<char,uint16_t> addrMap;
  //#endif
 };
 
 #endif