/**
 * This example sketch shows how to manually configure a node via RF24Mesh as a master node, which
 * will receive all data from sensor nodes.
 *
 * The nodes can change physical or logical position in the network, and reconnect through different
 * routing nodes as required. The master node manages the address assignments for the individual nodes
 * in a manner similar to DHCP.
 */
#include "pico/stdlib.h" // printf(), sleep_ms(), to_ms_since_boot(), get_absolute_time()
#include <tusb.h>        // tud_cdc_connected()
#include <RF24.h>        // RF24 radio object
#include <RF24Network.h> // RF24Network network object
#include <RF24Mesh.h>    // RF24Mesh mesh object
#include "defaultPins.h" // board presumptive default pin numbers for CE_PIN and CSN_PIN

// instantiate an object for the nRF24L01 transceiver
RF24 radio(CE_PIN, CSN_PIN);

RF24Network network(radio);

RF24Mesh mesh(radio, network);

bool setup()
{
    // wait here until the CDC ACM (serial port emulation) is connected
    while (!tud_cdc_connected()) {
        sleep_ms(10);
    }

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
    return true;
}

void loop()
{
    // Call network.update as usual to keep the network updated
    mesh.update();

    // In addition, keep the 'DHCP service' running on the master node so addresses will
    // be assigned to the sensor nodes
    mesh.DHCP();

    // Check for incoming data from the sensors
    while (network.available()) {
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
}

int main()
{
    stdio_init_all(); // init necessary IO for the RP2040

    while (!setup()) { // if radio.begin() failed
        // hold program in infinite attempts to initialize radio
    }
    while (true) {
        loop();
    }
    return 0; // we will never reach this
}
