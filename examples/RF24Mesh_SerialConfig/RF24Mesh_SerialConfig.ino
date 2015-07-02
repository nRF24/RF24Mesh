
/** RF24Mesh_SerialConfig.ino by TMRh20
 *
 * This example sketch shows how the same sketch can be written to a large number of devices, which are
 * configured later via Serial input.
 *
 **/

#include "RF24Network.h"
#include "RF24.h"
#include "RF24Mesh.h"
#include <SPI.h>
#include <EEPROM.h>
#include <printf.h>

/** Configure the nrf24l01 CE and CS pins */
RF24 radio(7, 8);
RF24Network network(radio);
RF24Mesh mesh(radio, network);

/**
 * User Configuration: NodeID - A unique identifier for each radio. Allows addressing
 * to change dynamically with physical changes to the mesh.
 *
 * In this example, user configuration of the node takes place via Serial input.
 * Input a unique decimal value from 1-255 OR simply use characters 0-9, a-z, A-Z etc.
 * The nodeID will be set/changed and saved in EEPROM upon user input via Serial, and stored
 * between power loss, etc. Configuration only needs to be done once.
 *
 **/


void setup() {

  Serial.begin(115200);
  printf_begin();

  // If this is a new node, the nodeID will return 0. Once the node is configured with an ID other than 0, this
  // bit will no longer run.
  while (!mesh.getNodeID()) {
    // Wait for the nodeID to be set via Serial
    if (Serial.available()) {
      mesh.setNodeID(Serial.read());
      Serial.print("Set NodeID: ");
      Serial.println(mesh.getNodeID());
    }
  }

  // Now that this node has a unique ID, connect to the mesh
  mesh.begin();

}

unsigned long displayTimer = 0;

void loop() {

  mesh.update();

  // Send an update in every second
  if (millis() - displayTimer >= 1000) {
    displayTimer = millis();
    // Send the current millis() value to the master node as an 'M' type message
    mesh.write(&displayTimer, 'M', sizeof(displayTimer));
  }

}
