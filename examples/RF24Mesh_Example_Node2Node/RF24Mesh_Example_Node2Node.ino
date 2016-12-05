
/** RF24Mesh_Example_Node2NodeExtra.ino by TMRh20
 *
 * This example sketch shows how to communicate between two (non-master) nodes using
 * RF24Mesh & RF24Network
 **/


#include "RF24.h"
#include "RF24Network.h"
#include "RF24Mesh.h"
#include <SPI.h>
//#include <printf.h>


/**** Configure the nrf24l01 CE and CS pins ****/
RF24 radio(7, 8);
RF24Network network(radio);
RF24Mesh mesh(radio, network);

/**
 * User Configuration:
 * nodeID - A unique identifier for each radio. Allows addressing to change dynamically
 * with physical changes to the mesh. (numbers 1-255 allowed)
 *
 * otherNodeID - A unique identifier for the other radio
 *
 **/
#define nodeID 1
#define otherNodeID 2


uint32_t millisTimer = 0;

void setup() {

  Serial.begin(115200);
  // Set the nodeID
  mesh.setNodeID(nodeID);
  // Connect to the mesh
  Serial.println(F("Connecting to the mesh..."));
  mesh.begin();
  Serial.println(F("Connected."));
}


void loop() {

  mesh.update();

  if (network.available()) {
    RF24NetworkHeader header;
    uint32_t mills;
    network.read(header, &mills, sizeof(mills));
    Serial.print(F("Received ")); Serial.print(mills);
    int _ID = mesh.getNodeID(header.from_node);
    if ( _ID > 0) {
      if (_ID == nodeID) {
        Serial.println(F(" from master."));
      } else {
        Serial.print(F(" from nodeID "));
        Serial.print(_ID);
        Serial.println('.');
      }
    } else {
      Serial.println(F("Mesh ID Lookup Failed"));
    }
  }


  // Send to the other node every second
  if (millis() - millisTimer >= 1000) {
    millisTimer = millis();

    // Send an 'M' type to other Node containing the current millis()
    if (!mesh.write(&millisTimer, 'M', sizeof(millisTimer), otherNodeID)) {
      if ( ! mesh.checkConnection() ) {
        Serial.println(F("Renewing Address"));
        mesh.renewAddress();
      } else {
        Serial.println(F("Send fail, Test OK"));
      }
    }
  }
}
