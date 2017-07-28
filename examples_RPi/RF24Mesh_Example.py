#!/usr/bin/env python3

from RF24 import *
from RF24Network import *
from RF24Mesh import *

from time import sleep, time
from struct import pack

start = time()

def millis():

    return int((time()-start)*1000) % (2 ** 32)

def delay(ms):
    ms = ms % (2**32)
    sleep(ms/1000.0)

# radio setup for RPi B Rev2: CS0=Pin 24
radio = RF24(RPI_V2_GPIO_P1_15, RPI_V2_GPIO_P1_24, BCM2835_SPI_SPEED_8MHZ)
network = RF24Network(radio)
mesh = RF24Mesh(radio, network)

mesh.setNodeID(4)
print("start nodeID", mesh.getNodeID());
mesh.begin()
radio.setPALevel(RF24_PA_MAX) # Power Amplifier
radio.printDetails()

displayTimer = 0

while 1:
    # Call mesh.update to keep the network updated
    mesh.update()

    if (millis() - displayTimer) >= 1000:
        displayTimer = millis()

        if not mesh.write(pack("L", displayTimer), ord('M')):
            # If a write fails, check connectivity to the mesh network
            if not mesh.checkConnection():
                # The address could be refreshed per a specified timeframe or only when sequential writes fail, etc.
                print("Renewing Address")
                mesh.renewAddress()
            else:
                print("Send fail, Test OK")
        else:
            print("Send OK:", displayTimer)
    delay(1);
    