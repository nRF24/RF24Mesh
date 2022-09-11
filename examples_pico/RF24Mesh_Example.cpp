/** RF24Mesh_Example.cpp by TMRh20
 *
 * Note: This sketch only functions on RP2040 boards
 *
 * This example sketch shows how to manually configure a node via RF24Mesh, and send data to the
 * master node.
 * In this sketch, the nodes will refresh their network address as soon as a single write fails. This allows the
 * nodes to change position in relation to each other and the master node.
 *
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

uint32_t displayTimer = 0;

bool setup()
{
    // wait here until the CDC ACM (serial port emulation) is connected
    while (!tud_cdc_connected()) {
        sleep_ms(10);
    }

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
    return true;
}

void loop()
{
    // Call mesh.update to keep the network updated
    mesh.update();

    // Send the current millis() to the master node every second
    if (to_ms_since_boot(get_absolute_time()) - displayTimer >= 1000) {
        displayTimer = to_ms_since_boot(get_absolute_time());

        if (!mesh.write(&displayTimer, 'M', sizeof(displayTimer))) {
            // If a write fails, check connectivity to the mesh network
            if (!mesh.checkConnection()) {
                // The address could be refreshed per a specified timeframe or only when sequential writes fail, etc.
                do {
                    printf("Renewing Address...\n");
                } while (mesh.renewAddress() == MESH_DEFAULT_ADDRESS);
            }
            else {
                printf("Send fail, Test OK\n");
            }
        }
        else {
            printf("Send OK: %u\n", displayTimer);
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
