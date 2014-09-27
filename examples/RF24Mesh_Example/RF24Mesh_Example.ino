
 /** RF24Mesh_Example.ino by TMRh20
  * 
  * This example sketch shows how to manually configure a node via RF24Mesh, and send data to the
  * master node.
  * The nodes will refresh their network address as soon as a single write fails. This allows the
  * nodes to change position in relation to each other and the master node.
  */

#include "RF24Network.h"
#include "RF24.h"
#include "RF24Mesh.h"
#include <SPI.h>
#include <EEPROM.h>
//#include <printf.h>

/** Configure the nrf24l01 CE and CS pins */
RF24 radio(7,8);
RF24Network network(radio);
RF24Mesh mesh(radio,network);

/** 
 * User Configuration: NodeID - A unique identifier for each radio. Allows addressing
 * to change dynamically with physical changes to the mesh.  
 *
 * In this example, configuration takes place below, prior to uploading the sketch to the device
 * A unique value from 1-255 must be configured for each node. 
 * This will be stored in EEPROM on AVR devices, so remains persistent between further uploads, loss of power, etc.
 *
 **/
    #define nodeID 1
    

uint32_t displayTimer=0;

void setup() {
  
  Serial.begin(115200);  
  // Set the nodeID manually
  mesh.setNodeID(nodeID);
  // Connect to the mesh
  mesh.begin();  
  
}



void loop() {
  
  network.update();
  
  // Send the current millis() to the master node every second
  if(millis() - displayTimer >= 1000){
    displayTimer = millis();
    
    if(!mesh.write(&displayTimer,'M')){
       
      // If a write fails, refresh the network address
      // The address could be refreshed per a specified timeframe or only when multiple sequential writes fail, etc.
       mesh.renewAddress(); 
    }
  }
}







