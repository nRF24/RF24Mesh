/**
 * @file RF24Mesh_config.h
 *
 * macro definitions for configuring RF24Mesh
 */

#ifndef __RF24MESH_CONFIG_H__
#define __RF24MESH_CONFIG_H__

/*** User Configuration ***/
/**
 * @brief Set 1 to 4 (Default: 4) Restricts the maximum children per node.
 *
 * The master node supports MESH_MAX_CHILDREN + 1 nodes
 */
#ifndef MESH_MAX_CHILDREN
    #define MESH_MAX_CHILDREN 4
#endif // MESH_MAX_CHILDREN
#if defined(RF24_TINY)
    #define MESH_NOMASTER
#endif

// un-comment for non-master nodes not running on ATTiny MCUs
//#define MESH_NOMASTER

/***Advanced User Config***/
/**
 * @brief Radio channel to operate on 1-127.
 *
 * This is normally modified by calling RF24Mesh::setChannel()
 */
#ifndef MESH_DEFAULT_CHANNEL
    #define MESH_DEFAULT_CHANNEL 97
#endif // MESH_DEFAULT_CHANNEL

/**
 * @brief How long to attempt address renewal in milliseconds
 * @see RF24Mesh::renewAddress()
 */
#ifndef MESH_RENEWAL_TIMEOUT
    #define MESH_RENEWAL_TIMEOUT 7500
#endif // MESH_RENEWAL_TIMEOUT

/**
 * @brief master node memory allocation
 *
 * On the master node memory for the address list (nodeIDs + RF24Network Addresses) is allocated dynamically and re-allocated using this value
 *
 * Example: With a value of 10, space for 10 nodes is allocated on startup. Adding an 11th node will re-allocate the memory space to support 10 more nodes.
 *
 * On 8-bit AVRs (Arduino Uno, Nano etc) each entry uses 3-bytes of memory space. Minimize this value to save memory. Allocate enough to prevent memory fragmentation.
 */
#ifndef MESH_MEM_ALLOC_SIZE
    #define MESH_MEM_ALLOC_SIZE 10
#endif // MESH_MEM_ALLOC_SIZE

/**************************/
/***       Debug        ***/
//#define MESH_DEBUG_MINIMAL /** Uncomment for the Master Node to print out address assignments as they are assigned */
//#define MESH_DEBUG         /** Uncomment to enable debug output to serial **/
/**************************/

/*** Other Configuration ***/
//#define MESH_MIN_SAVE_TIME 30000 /** UNUSED Minimum time required before changing nodeID. Prevents excessive writing to EEPROM */

/**
 * @brief How long to wait in ms for a response during individual address lookups
 * @see RF24Mesh::getNodeID and RF24Mesh::getAddress
 * If using distant nodes or a large number of nodes this value can be increased in general or for specific nodes.
 * For Level 1 nodes a response typically takes 2-3ms in good conditions.
 * For Level 2 nodes: 4-5ms, Level 3: 6-15ms, Level 4: 10-25ms
 **/
#ifndef MESH_LOOKUP_TIMEOUT
    #define MESH_LOOKUP_TIMEOUT 135
#endif // MESH_LOOKUP_TIMEOUT

/** @brief How long RF24Mesh::write() retries address lookups before timing out. Allows multiple attempts */
#ifndef MESH_WRITE_TIMEOUT
    #define MESH_WRITE_TIMEOUT 115
#endif // MESH_WRITE_TIMEOUT

#ifndef MESH_DEFAULT_ADDRESS
    #define MESH_DEFAULT_ADDRESS NETWORK_DEFAULT_ADDRESS
#endif // MESH_DEFAULT_ADDRESS

#define MESH_MULTICAST_ADDRESS NETWORK_MULTICAST_ADDRESS

//#define MESH_MAX_ADDRESSES 255 /* UNUSED Determines the max size of the array used for storing addresses on the Master Node */
//#define MESH_ADDRESS_HOLD_TIME 30000 /* UNUSED How long before a released address becomes available */

#if (defined(__linux) || defined(linux)) && !defined(__ARDUINO_X86__) && !defined(USE_RF24_LIB_SRC)
    #include <RF24/RF24_config.h>

//ATXMega
#elif defined(XMEGA)
    #include "../../rf24lib/rf24lib/RF24_config.h"
#else
    #include <RF24_config.h>
#endif

#if defined(MESH_DEBUG_MINIMAL)
    #define IF_MESH_DEBUG_MINIMAL(x) ({ x; })
#else
    #define IF_MESH_DEBUG_MINIMAL(x)
#endif

#if defined(MESH_DEBUG)
    #define IF_MESH_DEBUG(x) ({ x; })
#else
    #define IF_MESH_DEBUG(x)
#endif

#endif // __RF24MESH_CONFIG_H__
