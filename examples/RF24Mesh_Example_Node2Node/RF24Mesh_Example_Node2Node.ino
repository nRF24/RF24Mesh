
/** RF24Mesh_Example_Node2NodeExtra.ino by TMRh20
 *
 * This example sketch shows how to communicate between two (non-master) nodes using
 * RF24Mesh & RF24Network
 */


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
 * otherNodeID - A unique identifier for the 'other' radio
 *
 **/
#define nodeID 1
#define otherNodeID 2


uint32_t millisTimer = 0;

void setup() {

  Serial.begin(115200);
  while (!Serial) {
    // some boards need this because of native USB capability
  }

  // Set the nodeID
  mesh.setNodeID(nodeID);

  // Connect to the mesh
  Serial.println(F("Connecting to the mesh..."));
  if (!mesh.begin()) {
    if (radio.isChipConnected()) {
      do {
        // mesh.renewAddress() will return MESH_DEFAULT_ADDRESS on failure to connect
        Serial.println(F("Could not connect to network.\nConnecting to the mesh..."));
      } while (mesh.renewAddress() == MESH_DEFAULT_ADDRESS);
    } else {
      Serial.println(F("Radio hardware not responding."));
      while (1) {
        // hold in an infinite loop
      }
    }
  }
}


void loop() {

  mesh.update();

  if (network.available()) {
    RF24NetworkHeader header;
    uint32_t mills;
    network.read(header, &mills, sizeof(mills));
    Serial.print(F("Rcv "));
    Serial.print(mills);
    Serial.print(F(" from nodeID "));
    int _ID = mesh.getNodeID(header.from_node);
    if (_ID > 0) {
      Serial.println(_ID);
    } else {
      Serial.println(F("Mesh ID Lookup Failed"));
    }
  }


  // Send to the other node every second
  if (millis() - millisTimer >= 1000) {
    millisTimer = millis();

    // Send an 'M' type to other Node containing the current millis()
    if (!mesh.write(&millisTimer, 'M', sizeof(millisTimer), otherNodeID)) {
      if (!mesh.checkConnection()) {
        do {
          Serial.println(F("Reconnecting to mesh network..."));
        } while (mesh.renewAddress() == MESH_DEFAULT_ADDRESS);
      } else {
        Serial.println(F("Send fail, Test OK"));
      }
    }
  }
}
