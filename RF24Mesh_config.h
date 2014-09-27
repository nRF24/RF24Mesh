

#ifndef __RF24MESH_CONFIG_H__
#define __RF24MESH_CONFIG_H__

/*** User Configuration ***/
//#define MESH_DEBUG /** Uncomment to enable debug output to serial **/
#define MESH_MAX_CHILDREN 2 /** Set 1 to 4 (Default: 4) Restricts the maximum children per node. **/
/**************************/

/** Other Configuration */
#define MESH_MIN_SAVE_TIME 30000 /** Minimum time required before changing nodeID. Prevents excessive writing to EEPROM */

#define MESH_DEFAULT_ADDRESS 04444
#define MESH_DEFAULT_CHANNEL 90


  #if defined (MESH_DEBUG)
	#define IF_MESH_DEBUG(x) ({x;})
  #else
	#define IF_MESH_DEBUG(x)
  #endif






#endif