
/**
 * @file RF24Mesh.h
 *
 * Class declaration for RF24Mesh
 */

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
// None defined

// No Network ACK types
#define MESH_ADDR_LOOKUP 196
#define MESH_ADDR_RELEASE 197
#define MESH_ID_LOOKUP 198

#define MESH_BLANK_ID 65535

#if defined(__AVR_ATtiny85__) || defined(__AVR_ATtiny84__)
    #define RF24_TINY
#endif

#include "RF24Mesh_config.h"

#if defined(__linux) && !defined(__ARDUINO_X86__)
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
    RF24Mesh(RF24 &_radio, RF24Network &_network);

    /**
     * Call this in setup() to configure the mesh and request an address.  <br>
     *
     * @code mesh.begin(); @endcode
     * This may take a few moments to complete.
     *
     * The following parameters are optional:
     * @param channel The radio channel (1-127) default:97
     * @param data_rate The data rate (RF24_250KBPS,RF24_1MBPS,RF24_2MBPS) default:RF24_1MBPS
     * @param timeout How long to attempt address renewal in milliseconds default:7500
     */
    bool begin(uint8_t channel = MESH_DEFAULT_CHANNEL, rf24_datarate_e data_rate = RF24_1MBPS, uint32_t timeout = MESH_RENEWAL_TIMEOUT);

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
    bool write(const void *data, uint8_t msg_type, size_t size, uint8_t nodeID = 0);

    /**
     * Set a unique nodeID for this node.
     *
     * This needs to be called before mesh.begin(), can be via serial connection, eeprom etc if configuring a large number of nodes...
     * @note If using RF24Gateway and/or RF24Ethernet, nodeIDs 0 & 1 are used by the master node.
     * @param nodeID Can be any unique value ranging from 1 to 253
     */
    void setNodeID(uint8_t nodeID);

    /**
     * Reconnect to the mesh and renew the current RF24Network address. Used to re-establish a connection to the mesh if physical location etc. has changed, or
     * a routing node goes down.
     * @note Currently times out after 7.5 seconds if address renewal fails.
     *
     * @note If all nodes are set to verify connectivity/reconnect at a specified period, restarting the master (and deleting dhcplist.txt on Linux) will result
     * in complete network/mesh reconvergence.
     * @param timeout How long to attempt address renewal in milliseconds default:7500
     *
     * @return Returns the newly assigned RF24Network address
     */
    uint16_t renewAddress(uint32_t timeout = MESH_RENEWAL_TIMEOUT);

    #if !defined(MESH_NOMASTER)
    /**
     * Only to be used on the master node. Provides automatic configuration for sensor nodes, similar to DHCP.
     * Call immediately after calling network.update() to ensure address requests are handled appropriately
     */
    void DHCP();

    #endif

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
    int16_t getNodeID(uint16_t address = MESH_BLANK_ID);

    /**
     * Tests connectivity of this node to the mesh.
     * @note If this function fails, address renewal should typically be done.
     * @return Return 1 if connected, 0 if mesh not responding
     */
    bool checkConnection();

    /**
     * Releases the currently assigned address lease. Useful for nodes that will be sleeping etc.
     * @note Nodes should ensure that addresses are released successfully prior to going offline.
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
     * @param nodeID - The unique identifier (1-253) of the node
     * @return Returns the RF24Network address of the node, -2 if successful but not in list, -1 if failed
     */
    int16_t getAddress(uint8_t nodeID);

    /**
     * Write to a specific node by RF24Network address.
     *
     */
    bool write(uint16_t to_node, const void *data, uint8_t msg_type, size_t size);

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
     * RF24Mesh ID and Address lookups as well as address renewal can take some time.
     * Set a callback function to enable additional processing while the mesh is working
     *
     * @code
     * void myCallbackFunction(){
     *   someValue = someOtherValue;
     * }
     * mesh.setCallback(myCallbackFunction);
     * @endcode
     *
     * @param meshCallback The name of a function to call
     */
    void setCallback(void (*meshCallback)(void));

    #define MESH_CALLBACK if (meshCallback) { meshCallback(); }

    #if !defined(MESH_NOMASTER)
    /**
     * Set/change a nodeID/RF24Network Address pair manually on the master node.
     *
     * @code
     * Set a static address for node 02, with nodeID 23, since it will just be a static routing node for example
     * running on an ATTiny chip.
     *
     * mesh.setAddress(23,02);
     * @endcode
     *
     * @code
     * Change/set the nodeID for an existing address
     *
     * uint16_t address = 012;
     * mesh.setAddress(3,address,true);
     * @endcode
     *
     * @param nodeID The nodeID to assign
     * @param address The octal RF24Network address to assign
     * @param searchBy Optional parameter. Default is search by nodeID and set the address. True allows searching by address and setting nodeID.
     * @return If the nodeID exists in the list,
     */
    void setAddress(uint8_t nodeID, uint16_t address, bool searchBy = false);

    void saveDHCP();
    void loadDHCP();

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

    #endif // !defined(MESH_NOMASTER)

    /**@}*/
    /**
     * @name Address list struct
     *
     *  See the list struct class reference
     */
    /**@{*/

    /**@}*/

    uint8_t _nodeID;

    #if !defined(MESH_NOMASTER)
    typedef struct {
        uint8_t nodeID;   /** NodeIDs and addresses are stored in the addrList array using this structure */
        uint16_t address; /** NodeIDs and addresses are stored in the addrList array using this structure */
    } addrListStruct;

    // Pointer used for dynamic memory allocation of address list
    addrListStruct *addrList; /** See the addrListStruct class reference */
    uint8_t addrListTop;      /** The number of entries in the assigned address list */
    #endif

private:
    RF24 &radio;
    RF24Network &network;

    void (*meshCallback)(void);
    bool requestAddress(uint8_t level);      /** Actual requesting of the address once a contact node is discovered or supplied **/
    bool waitForAvailable(uint32_t timeout); /** Waits for data to become available */

    #if !defined(MESH_NOMASTER)
    bool doDHCP;           /** Indicator that an address request is available */
    bool addrMemAllocated; /** Just ensures we don't re-allocate the memory buffer if restarting the mesh on master **/
    #endif

    void beginDefault(); /** Starts up the network layer with default address **/
    bool meshStarted;
    uint8_t getLevel(uint16_t address); /** Returns the number of digits in the specified address **/
};

#endif // define __RF24MESH_H__

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
 * @image html "images/RF24Mesh_Ncurses.JPG"
 */
