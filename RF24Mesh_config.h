

#ifndef __RF24MESH_CONFIG_H__
#define __RF24MESH_CONFIG_H__

/*** User Configuration ***/
#define MESH_MAX_CHILDREN 4 /** Set 1 to 4 (Default: 4) Restricts the maximum children per node. **/
//#define MESH_NOMASTER /** This can be set to 0 for all nodes except the master (nodeID 0) to save pgm space **/

/***Advanced User Config***/
#define MESH_LOOKUP_TIMEOUT 3000  /** How long mesh write will retry address lookups before giving up. This is not used when sending to or from the master node. **/
#define MESH_WRITE_TIMEOUT 5550 /** UNUSED - How long mesh.write will retry failed payloads. */
#define MESH_DEFAULT_CHANNEL 97  /** Radio channel to operate on 1-127. This is normally modified by calling mesh.setChannel() */
#define MESH_RENEWAL_TIMEOUT 60000 /** How long to attempt address renewal */
/**************************/
/***       Debug        ***/
//#define MESH_DEBUG_MINIMAL /** Uncomment for the Master Node to print out address assignments as they are assigned */
//#define MESH_DEBUG         /** Uncomment to enable debug output to serial **/
/**************************/

/** Other Configuration */
#define MESH_MIN_SAVE_TIME 30000 /** Minimum time required before changing nodeID. Prevents excessive writing to EEPROM */
#define MESH_DEFAULT_ADDRESS 04444
#define MESH_MAX_ADDRESSES 255 /** Determines the max size of the array used for storing addresses on the Master Node */
//#define MESH_ADDRESS_HOLD_TIME 30000 /** How long before a released address becomes available */ 

  #if defined (MESH_DEBUG)
    #if !defined (__linux) && !defined ARDUINO_SAM_DUE || defined TEENSY || defined(__ARDUINO_X86__)
		#define MESH_DEBUG_SERIAL		       
	#else
		#define MESH_DEBUG_PRINTF
	#endif
  #endif

  




#endif
