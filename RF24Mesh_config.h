

#ifndef __RF24MESH_CONFIG_H__
#define __RF24MESH_CONFIG_H__

/*** User Configuration ***/
#define MESH_MAX_CHILDREN 4 /** Set 1 to 4 (Default: 4) Restricts the maximum children per node. The master node supports MESH_MAX_CHILDREN + 1 nodes **/
#ifdef RF24_TINY
  #define MESH_NOMASTER
#endif
//#define MESH_NOMASTER /** This can be set to 0 for all nodes except the master (nodeID 0) to save pgm space. Enabled automatically on ATTiny devices **/

/***Advanced User Config***/
#define MESH_DEFAULT_CHANNEL 97  /** Radio channel to operate on 1-127. This is normally modified by calling mesh.setChannel() */
#define MESH_RENEWAL_TIMEOUT 7500 /** How long to attempt address renewal ( renewAddress(); ) */
/**
* On the master node memory for the address list (nodeIDs + RF24Network Addresses) is allocated dynamically and re-allocated using this value
* Example: With a value of 10, space for 10 nodes is allocated on startup. Adding an 11th node will re-allocate the memory space to support 10 more nodes.
* On 8-bit AVRs (Arduino Uno, Nano etc) each entry uses 3-bytes of memory space. Minimize this value to save memory. Allocate enough to prevent memory fragmentation.
*/
#define MESH_MEM_ALLOC_SIZE 10 

/**************************/
/***       Debug        ***/
//#define MESH_DEBUG_MINIMAL /** Uncomment for the Master Node to print out address assignments as they are assigned */
//#define MESH_DEBUG         /** Uncomment to enable debug output to serial **/
/**************************/

/** Other Configuration */
//#define MESH_MIN_SAVE_TIME 30000 /** UNUSED Minimum time required before changing nodeID. Prevents excessive writing to EEPROM */

/** 
 * How long to wait in ms for a response during individual address lookups (getNodeID or getAddress).
 * If using distant nodes or a large number of nodes this value can be increased in general or for specific nodes. 
 * For Level 1 nodes a response typically takes 2-3ms in good conditions.
 * For Level 2 nodes: 4-5ms, Level 3: 6-15ms, Level 4: 10-25ms 
 **/
#define MESH_LOOKUP_TIMEOUT 135  

#define MESH_WRITE_TIMEOUT 115 /** How long mesh.write retries address lookups before timing out. Allows multiple attempts */
#define MESH_DEFAULT_ADDRESS NETWORK_DEFAULT_ADDRESS
//#define MESH_MAX_ADDRESSES 255 /** UNUSED Determines the max size of the array used for storing addresses on the Master Node */
//#define MESH_ADDRESS_HOLD_TIME 30000 /**UNUSED How long before a released address becomes available */ 

  #if defined (MESH_DEBUG)
    #if !defined (__linux) && !defined ARDUINO_SAM_DUE || defined TEENSY || defined(__ARDUINO_X86__)
		#define MESH_DEBUG_SERIAL		       
	#else
		#define MESH_DEBUG_PRINTF
	#endif
  #endif
  




#endif
