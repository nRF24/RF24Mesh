
/** RF24Mesh_Example_Master.ino by TMRh20
 *
 *
 * This example sketch shows how to manually configure a node via RF24Mesh as a master node, which
 * will receive all data from sensor nodes. In addition, this sketch demonstrates how to reserve
 * addresses for static nodes, that will not be using RF24Mesh, or are not using the dynamic addressing
 * functionality of RF24Mesh.
 *
 * The nodes can change physical or logical position in the network, and reconnect through different
 * routing nodes as required. The master node manages the address assignments for the individual nodes
 * in a manner similar to DHCP.
 *
 **/


#include "RF24Network.h"
#include "RF24.h"
#include "RF24Mesh.h"
#include <SPI.h>
//Include eeprom.h for AVR (Uno, Nano) etc. except ATTiny
#include <EEPROM.h>

/***** Configure the chosen CE,CS pins *****/
RF24 radio(7, 8);
RF24Network network(radio);
RF24Mesh mesh(radio, network);


void setup() {
  Serial.begin(115200);

  // Set the nodeID to 0 for the master node
  mesh.setNodeID(0);
  Serial.println(mesh.getNodeID());
  // Connect to the mesh
  mesh.begin();

  // In this case, assign a static address to nodeIDs 23,24 at RF24Network address 02 && 03
  // This will prevent this master node from assigning the address to another node
  // This allows a set of static nodes to remain in place as routing nodes, while
  // other nodes can move around physically, using the static nodes to join or re-join
  // the network.
  // With this example, assign nodes 02 and 03 statically. This allows child nodes to join
  // the network as children of 00(master), node 02, or node 03, or as children of other
  // mesh nodes. If nodes 02 and 03 are placed in proximity to a group of mesh nodes, the
  // mesh nodes can attatch to the network via the static nodes, and route traffic through
  // either node, to the master node.
  mesh.setStaticAddress(23, 02);
  mesh.setStaticAddress(24, 03);

}

uint32_t displayTimer = 0;

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
    Serial.print("Got ");
    uint32_t dat = 0;
    switch (header.type) {
      // Display the incoming millis() values from the sensor nodes
      case 'M': network.read(header, &dat, sizeof(dat));
        Serial.print(dat);
        Serial.print(" from RF24Network address 0");
        Serial.println(header.from_node, OCT);
        break;
      default: network.read(header, 0, 0); Serial.println(header.type); break;
    }
  }

  // Display the currently assigned addresses and nodeIDs
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
