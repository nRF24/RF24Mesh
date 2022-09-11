"""
Simplest RF24Mesh example that transmits a time stamp (in milliseconds) 1 per second.
"""
import sys
import time
import struct
from RF24 import RF24, RF24_PA_MIN
from RF24Network import RF24Network
from RF24Mesh import RF24Mesh

start = time.monotonic()


def millis():
    """:Returns: Delta time since started example in milliseconds. Wraps value around
    the width of a ``long`` integer."""
    return int((time.monotonic() - start) * 1000) % (2**32)


# radio setup for RPi B Rev2: CS0=Pin 24
radio = RF24(22, 0)
network = RF24Network(radio)
mesh = RF24Mesh(radio, network)

mesh.setNodeID(4)

# Set the PA Level to MIN and disable LNA for testing & power supply related issues
radio.begin()
radio.setPALevel(RF24_PA_MIN, 0)

print("starting nodeID", mesh.getNodeID())
if not mesh.begin():
    if radio.isChipConnected():
        try:
            print("Could not connect to network.\nConnecting to mesh...")
            while mesh.renewAddress() == 0o4444:
                print("Could not connect to network.\nConnecting to mesh...")
        except KeyboardInterrupt:
            radio.powerDown()
            sys.exit()
    else:
        raise OSError("Radio hardware not responding or could not connect to mesh.")
radio.printDetails()

TIMER = 0

try:
    while True:
        # Call mesh.update to keep the network updated
        mesh.update()

        if (millis() - TIMER) >= 1000:
            TIMER = millis()

            if not mesh.write(struct.pack("L", TIMER), ord("M")):
                # If a write fails, check connectivity to the mesh network
                if not mesh.checkConnection():
                    # The address could be refreshed per a specified time frame
                    # or only when sequential writes fail, etc.
                    print("Send fail. Renewing Address...")
                    while mesh.renewAddress() == 0o4444:
                        print("Renewing Address...")
                else:
                    print("Send fail, Test OK")
            else:
                print("Send OK:", TIMER)
        time.sleep(0.001)  # delay 1 ms
except KeyboardInterrupt:
    print("powering down radio and exiting.")
    radio.powerDown()
