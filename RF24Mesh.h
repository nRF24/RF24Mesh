





#ifndef __RF24MESH_H__
#define __RF24MESH_H__

/**
 * @file RF24Mesh.h
 *
 * Class declaration for RF24Mesh
 */

#include <stddef.h>
#include <stdint.h>
//#include "RF24Mesh_config.h"


class RF24Network;


class RF24Mesh
{
public:
  /**
   * Construct the mesh
   *
   * @param _radio The underlying radio driver instance
   *
   */

  RF24Mesh( RF24Network& _network );
  
  
  void begin(uint8_t _channel);
  
  
 };
 
 #endif