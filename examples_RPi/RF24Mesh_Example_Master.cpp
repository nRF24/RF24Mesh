/** RF24Mesh_Example_Master.ino by TMRh20
 *
 * Note: This sketch only functions on -Arduino Due-
 *
 * This example sketch shows how to manually configure a node via RF24Mesh as a master node, which
 * will receive all data from sensor nodes.
 *
 * The nodes can change physical or logical position in the network, and reconnect through different
 * routing nodes as required. The master node manages the address assignments for the individual nodes
 * in a manner similar to DHCP.
 *
 */
#include "RF24Mesh/RF24Mesh.h"
#include <RF24/RF24.h>
#include <RF24Network/RF24Network.h>

RF24 radio(22, 0);
RF24Network network(radio);
RF24Mesh mesh(radio, network);

int main(int argc, char** argv)
{
    // Set the nodeID to 0 for the master node
    mesh.setNodeID(0);

    // Set the PA Level to MIN and disable LNA for testing & power supply related issues
    radio.begin();
    radio.setPALevel(RF24_PA_MIN, 0);

    // Connect to the mesh
    printf("start\n");
    if (!mesh.begin()) {
        // if mesh.begin() returns false for a master node, then radio.begin() returned false.
        printf("Radio hardware not responding.\n");
        return 0;
    }
    radio.printDetails();

    while (1)
    {
        // Call network.update as usual to keep the network updated
        mesh.update();

        // In addition, keep the 'DHCP service' running on the master node so addresses will
        // be assigned to the sensor nodes
        mesh.DHCP();

        // Check for incoming data from the sensors
        while (network.available())
        {
            // printf("rcv\n");
            RF24NetworkHeader header;
            network.peek(header);

            uint32_t dat = 0;
            switch (header.type)
            { // Display the incoming millis() values from the sensor nodes
                case 'M':
                    network.read(header, &dat, sizeof(dat));
                    printf("Rcv %u from 0%o\n", dat, header.from_node);
                    break;
                default:
                    network.read(header, 0, 0);
                    printf("Rcv bad type %d from 0%o\n", header.type, header.from_node);
                    break;
            }
        }
        delay(2);
    }
    return 0;
}
