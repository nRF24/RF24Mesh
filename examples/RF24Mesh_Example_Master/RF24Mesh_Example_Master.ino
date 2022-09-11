

/** RF24Mesh_Example_Master.ino by TMRh20
 *
 *
 * This example sketch shows how to manually configure a node via RF24Mesh as a master node, which
 * will receive all data from sensor nodes.
 *
 * The nodes can change physical or logical position in the network, and reconnect through different
 * routing nodes as required. The master node manages the address assignments for the individual nodes
 * in a manner similar to DHCP.
 *
 */


#include "RF24Network.h"
#include "RF24.h"
#include "RF24Mesh.h"
#include <SPI.h>

/***** Configure the chosen CE,CS pins *****/
RF24 radio(7, 8);
RF24Network network(radio);
RF24Mesh mesh(radio, network);

uint32_t displayTimer = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    // some boards need this because of native USB capability
  }

  // Set the nodeID to 0 for the master node
  mesh.setNodeID(0);
  Serial.println(mesh.getNodeID());

  // Set the PA Level to MIN and disable LNA for testing & power supply related issues
  radio.begin();
  radio.setPALevel(RF24_PA_MIN, 0);

  // Connect to the mesh
  if (!mesh.begin()) {
    // if mesh.begin() returns false for a master node, then radio.begin() returned false.
    Serial.println(F("Radio hardware not responding."));
    while (1) {
      // hold in an infinite loop
    }
  }
}


void loop() {

  // Call mesh.update to keep the network updated
  mesh.update();

  // In addition, keep the 'DHCP service' running on the master node so addresses will
  // be assigned to the sensor nodes
  mesh.DHCP();


  // Check for incoming data from the sensors
  if (network.available()) {
    RF24NetworkHeader header;
    network.peek(header);

    uint32_t dat = 0;
    switch (header.type) {
      // Display the incoming millis() values from the sensor nodes
      case 'M':
        network.read(header, &dat, sizeof(dat));
        Serial.println(dat);
        break;
      default:
        network.read(header, 0, 0);
        Serial.println(header.type);
        break;
    }
  }

  if (millis() - displayTimer > 5000) {
    displayTimer = millis();
    Serial.println(" ");
    Serial.println(F("********Assigned Addresses********"));
    for (int i = 0; i < mesh.addrListTop; i++) {
      Serial.print("NodeID: ");
      Serial.print(mesh.addrList[i].nodeID);
      Serial.print(" RF24Network Address: 0");
      Serial.println(mesh.addrList[i].address, OCT);
    }
    Serial.println(F("**********************************"));
  }
}
