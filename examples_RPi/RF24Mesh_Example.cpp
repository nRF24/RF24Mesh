/** RF24Mesh_Example.cpp by TMRh20
 *
 * Note: This sketch only functions on -RPi-
 *
 * This example sketch shows how to manually configure a node via RF24Mesh, and send data to the
 * master node.
 * In this sketch, the nodes will refresh their network address as soon as a single write fails. This allows the
 * nodes to change position in relation to each other and the master node.
 *
 */
#include "RF24Mesh/RF24Mesh.h"
#include <RF24/RF24.h>
#include <RF24Network/RF24Network.h>

RF24 radio(22, 0);
RF24Network network(radio);
RF24Mesh mesh(radio, network);

uint32_t displayTimer = 0;

int main(int argc, char** argv)
{

    // Set the nodeID to 0 for the master node
    mesh.setNodeID(4);

    // Set the PA Level to MIN and disable LNA for testing & power supply related issues
    radio.begin();
    radio.setPALevel(RF24_PA_MIN, 0);

    // Connect to the mesh
    printf("start nodeID %d\n", mesh.getNodeID());
    if (!mesh.begin()) {
        if (radio.isChipConnected()) {
            do {
                // mesh.renewAddress() will return MESH_DEFAULT_ADDRESS on failure to connect
                printf("Could not connect to network.\nConnecting to the mesh...\n");
            } while (mesh.renewAddress() == MESH_DEFAULT_ADDRESS);
        }
        else {
            printf("Radio hardware not responding.\n");
            return 0;
        }
    }
    radio.printDetails();

    while (1)
    {
        // Call mesh.update to keep the network updated
        mesh.update();

        // Send the current millis() to the master node every second
        if (millis() - displayTimer >= 1000)
        {
            displayTimer = millis();

            if (!mesh.write(&displayTimer, 'M', sizeof(displayTimer)))
            {
                // If a write fails, check connectivity to the mesh network
                if (!mesh.checkConnection())
                {
                    // The address could be refreshed per a specified timeframe or only when sequential writes fail, etc.
                    printf("Renewing Address\n");
                    mesh.renewAddress();
                }
                else
                {
                    printf("Send fail, Test OK\n");
                }
            }
            else
            {
                printf("Send OK: %u\n", displayTimer);
            }
        }
        delay(1);
    }
    return 0;
}
