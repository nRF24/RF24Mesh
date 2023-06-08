
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
#define MESH_ADDR_LOOKUP  196
#define MESH_ADDR_RELEASE 197
#define MESH_ID_LOOKUP    198

#define MESH_BLANK_ID 65535

#if defined(__AVR_ATtiny85__) || defined(__AVR_ATtiny84__)
    #define RF24_TINY
#endif

#include "RF24Mesh_config.h"

#if defined(__linux) && !defined(__ARDUINO_X86__) && !defined(USE_RF24_LIB_SRC)
    #include <RF24/RF24.h>
    #include <RF24Network/RF24Network.h>
    #define RF24_LINUX
#else
    #include <RF24.h>
    #include <RF24Network.h>
    #if defined(ARDUINO_ARCH_NRF52) || defined(ARDUINO_ARCH_NRF52840)
        #include <nrf_to_nrf.h>
    #endif
#endif

#include <stddef.h>
#include <stdint.h>

class RF24;
#if defined(ARDUINO_ARCH_NRF52) || defined(ARDUINO_ARCH_NRF52840) || defined(ARDUINO_ARCH_NRF52833)
class nrf_to_nrf;
#endif

template<class radio_t>
class ESBNetwork;

/**
 * @tparam network_t The `network` object's type. Defaults to `RF24Network` for legacy behavior.
 * This new abstraction is really meant for using the nRF52840 SoC as a drop-in replacement
 * for the nRF24L01 radio. For more detail, see the
 * [nrf_to_nrf Arduino library](https://github.com/TMRh20/nrf_to_nrf).
 * @tparam radio_t The `radio` object's type. Defaults to `RF24` for legacy behavior.
 * This new abstraction is really meant for using the nRF52840 SoC as a drop-in replacement
 * for the nRF24L01 radio. For more detail, see the
 * [nrf_to_nrf Arduino library](https://github.com/TMRh20/nrf_to_nrf).
 */
template<class network_t = ESBNetwork<RF24>, class radio_t = RF24>
class ESBMesh
{
    /**
     * @name RF24Mesh
     *
     * The mesh library and class documentation is currently in active development and usage may change.
     */
    /**@{*/

public:
    /**
     * Construct the mesh.
     * 
     * v2.0 supports a backward compatible constructor:
     * @code
     * RF24 radio(7, 8);
     * RF24Network network(radio);
     * RF24Mesh mesh(radio, network); // for nRF24L01
     *
     * nrf_to_nrf radio1;
     * RF52Network network1(radio1);
     * RF52Mesh mesh1(network1, radio1); // for nRF52xxx family
     * @endcode
     * 
     * @see v2.0 supports [nrf_to_nrf Arduino library](https://github.com/TMRh20/nrf_to_nrf)
     * for nrf52 chips' internal radio.
     * @param _radio The underlying radio driver instance
     * @param _network The underlying network instance
     */
    ESBMesh(radio_t& _radio, network_t& _network);

    /**
     * Call this in setup() to configure the mesh and request an address.  <br>
     *
     * @code mesh.begin(); @endcode
     * This may take a few moments to complete.
     *
     * The following parameters are optional:
     * @param channel The radio channel (0 - 125). Default is 97.
     * @param data_rate The data rate (RF24_250KBPS, RF24_1MBPS, RF24_2MBPS). Default is RF24_1MBPS.
     * @param timeout How long to attempt address renewal in milliseconds. Default is 7500.
     */
    bool begin(uint8_t channel = MESH_DEFAULT_CHANNEL, rf24_datarate_e data_rate = RF24_1MBPS, uint32_t timeout = MESH_RENEWAL_TIMEOUT);

    /**
     * Very similar to network.update(), it needs to be called regularly to keep the network
     * and the mesh going.
     */
    uint8_t update();

    /**
     * Automatically construct a header and send a payload.
     * Very similar to the standard network.write() function, which can be used directly.
     *
     * @note Including the @ref _nodeID "nodeID" parameter will result in an automatic address lookup being performed.
     * @note Message types 1 - 64 (decimal) will NOT be acknowledged by the network, types 65 - 127 will be. Use as appropriate to manage traffic:
     * if expecting a response, no ack is needed.
     *
     * @param data Send any type of data of any length (maximum length determined by RF24Network layer).
     * @param msg_type The user-defined (1 - 127) message header_type to send. Used to distinguish between different types of data being transmitted.
     * @param size The size of the data being sent
     * @param nodeID **Optional**: The @ref _nodeID "nodeID" of the recipient if not sending to master.
     * @return True if success; false if failed
     */
    bool write(const void* data, uint8_t msg_type, size_t size, uint8_t nodeID = 0);

    /**
     * Set a unique @ref _nodeID "nodeID" for this node.
     *
     * This needs to be called before ESBMesh::begin(). The parameter value passed can be fetched
     * via serial connection, EEPROM, etc when configuring a large number of nodes.
     * @note If using RF24Gateway and/or RF24Ethernet, nodeIDs 0 & 1 are used by the master node.
     * @param nodeID Can be any unique value ranging from 1 to 255 (reserving 0 for the master node).
     */
    void setNodeID(uint8_t nodeID);

    /**
     * @brief Reconnect to the mesh and renew the current RF24Network address.
     *
     * This is used to re-establish a connection to the mesh network if physical location of a node
     * or surrounding nodes has changed (or a routing node becomes unavailable).
     *
     * @note If all nodes are set to verify connectivity and reconnect at a specified period, then
     * restarting the master (and deleting dhcplist.txt on Linux) will result in complete
     * network/mesh re-convergence.
     * @param timeout How long to attempt address renewal in milliseconds. Default is 7500
     * @return The newly assigned RF24Network address. If the connecting process fails, then
     * MESH_DEFAULT_ADDRESS is returned because all consciously unconnected nodes use that address.
     */
    uint16_t renewAddress(uint32_t timeout = MESH_RENEWAL_TIMEOUT);

#if !defined(MESH_NOMASTER)
    /**
     * This is only to be used on the master node because it manages allocation of network addresses
     * for any requesting (non-master) node's ID, similar to DHCP.
     *
     * @warning On master nodes, It is required to call this function immediately after calling
     * ESBMesh::update() to ensure address requests are handled appropriately.
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
     * @param address If no address is provided, returns the local @ref _nodeID "nodeID",
     * otherwise a lookup request is sent to the master node
     * @return The unique identifier of the node in the range [1, 255] or -1 if node was not found.
     */
    int16_t getNodeID(uint16_t address = MESH_BLANK_ID);

    /**
     * Tests connectivity of this node to the mesh.
     * @note If this function fails, address renewal should typically be done.
     * @return 1 if connected, 0 if mesh not responding
     */
    bool checkConnection();

    /**
     * Releases the currently assigned address lease. Useful for nodes that will be sleeping etc.
     * @note Nodes should ensure that addresses are released successfully prior to going offline.
     * @return True if successfully released, otherwise false.
     */
    bool releaseAddress();

    /**
     * The assigned RF24Network (Octal) address of this node
     * @return An unsigned 16-bit integer containing the RF24Network address in octal format.
     */
    uint16_t mesh_address;

    /**
     * @brief Convert a @ref _nodeID "nodeID" into an RF24Network address
     * @note If printing or displaying the address, it needs to be converted to octal format:
     * @code{.ino} Serial.println(address, OCT); @endcode
     *
     * Results in a lookup request being sent to the master node.
     * @param nodeID The unique identifier of the node in the range [1, 255].
     * @return The RF24Network address of the node, -2 if successful but not in list, -1 if failed.
     */
    int16_t getAddress(uint8_t nodeID);

    /** @brief Write to a specific node by RF24Network address. */
    bool write(uint16_t to_node, const void* data, uint8_t msg_type, size_t size);

    /**
     * Change the active radio channel after the mesh has been started.
     * @param _channel The value passed to `RF24::setChannel()`
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
     * @param meshCallback The name of a function to call. This function should consume no required input parameters.
     */
    void setCallback(void (*meshCallback)(void));

#define MESH_CALLBACK \
    if (meshCallback) meshCallback();

    /**
     * Set or change a @ref _nodeID "nodeID" : node address (key : value) pair manually.
     * This function is for use on the master node only.
     *
     * @code
     * // Set a static address for node 02, with nodeID 23, since it will just be
     * // a static routing node for example running on an ATTiny chip.
     * mesh.setAddress(23, 02);
     * @endcode
     * @code
     * // Change or set the nodeID for an existing address
     * uint16_t address = 012;
     * mesh.setAddress(3, address, true);
     * @endcode
     *
     * @param nodeID The @ref _nodeID "nodeID" to assign
     * @param address The octal RF24Network address to assign
     * @param searchBy Optional parameter. Default is search by @ref _nodeID "nodeID" and
     * set the address. True allows searching by address and setting @ref _nodeID "nodeID".
     */
    void setAddress(uint8_t nodeID, uint16_t address, bool searchBy = false);

#if !defined(MESH_NOMASTER)
    /**
     * Save the @ref addrList to a binary file named "dhcplist.txt".
     * @note This function is for use on the master node only and only on Linux or x86 platforms.
     */
    void saveDHCP();

    /**
     * Load the @ref addrList from a binary file named "dhcplist.txt".
     * @note This function is for use on the master node only and only on Linux or x86 platforms.
     */
    void loadDHCP();

    /**
     * @name Deprecated
     *
     *  Methods provided for backwards compatibility with old/testing code.
     */
    /**@{*/

    /** @deprecated For backward compatibility with older code. Use the synonymous setAddress() instead. */
    void setStaticAddress(uint8_t nodeID, uint16_t address);

#endif // !defined(MESH_NOMASTER)
    /**@}*/

    /**
     * The unique identifying number used to differentiate mesh nodes' from their assigned network
     * address. Ideally, this is set before calling begin() or renewAddress(). It is up to the
     * network administrator to make sure that this number is unique to each mesh/network node.
     *
     * This nodeID number is typically in the range [0, 255], but remember that `0` is reserved for
     * the master node. Other external systems may reserve other node ID numbers, for instance
     * RF24Gateway/RF24Ethernet reserves the node ID number `1` in addition to the master node ID
     * `0`.
     */
    uint8_t _nodeID;

#if !defined(MESH_NOMASTER)
    /**
     * @brief A struct for storing a  @ref _nodeID "nodeID" and an address in a single element of
     * the ESBMesh::addrList array.
     *
     * @note This array only exists on the mesh network's master node.
     */
    typedef struct
    {
        /** @brief The @ref _nodeID "nodeID" of an network node (child) */
        uint8_t nodeID;
        /** @brief The logical address of an network node (child) */
        uint16_t address;
    } addrListStruct;

    /**
     * @name Address list struct
     * @brief helping members for managing the list of assigned addresses
     * @see the addrListStruct struct reference
     */
    /**@{*/

    // Pointer used for dynamic memory allocation of address list
    /**
     * @brief A array of addrListStruct elements for assigned addresses.
     * @see addrListStruct class reference
     */
    addrListStruct* addrList;
    /** @brief The number of entries in the addrListStruct of assigned addresses. */
    uint8_t addrListTop;
#endif
    /**@}*/

private:
    radio_t& radio;
    network_t& network;

    /** Function pointer for customized callback usage in long running algorithms. */
    void (*meshCallback)(void);

    /** Actual requesting of the address once a contact node is discovered or supplied **/
    bool requestAddress(uint8_t level);

#if !defined(MESH_NOMASTER)
    /** Indicator that an address request is available. */
    bool doDHCP;
    /** Just ensures we don't re-allocate the memory buffer if restarting the mesh on master. **/
    bool addrMemAllocated;
#endif

    /** Starts up the network layer with default address. */
    void beginDefault();
    /** A flag asserted in begin() after putting the radio in TX mode. */
    bool meshStarted;
    /** Returns the number of octal digits in the specified address. */
    uint8_t getLevel(uint16_t address);
};

/**
 * A type definition of the template class `ESBMesh` to maintain backward compatibility.
 * 
 * ```.cpp
 * RF24 radio(7, 8);
 * RF24Network network(radio);
 * 
 * RF24Mesh mesh(radio, network);
 * // is equivalent to
 * ESBMesh<ESBNetwork<RF24>, RF24> mesh(radio, network);
 * ```
 */
typedef ESBMesh<ESBNetwork<RF24>, RF24> RF24Mesh;
#if defined(ARDUINO_ARCH_NRF52) || defined(ARDUINO_ARCH_NRF52840) || defined(ARDUINO_ARCH_NRF52833)
typedef ESBMesh<ESBNetwork<nrf_to_nrf>, nrf_to_nrf> RF52Mesh;
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
 * @image html "images/RF24Mesh_Ncurses.JPG"
 */

#endif // define __RF24MESH_H__
