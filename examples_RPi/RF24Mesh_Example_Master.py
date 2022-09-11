"""
Example of using the rf24_mesh module to operate the nRF24L01 transceiver as
a Mesh network master node.
"""
import struct
from RF24 import RF24, RF24_PA_MIN
from RF24Network import RF24Network
from RF24Mesh import RF24Mesh


# radio setup for RPi B Rev2: CS0=Pin 24
radio = RF24(22, 0)
network = RF24Network(radio)
mesh = RF24Mesh(radio, network)
mesh.setNodeID(0)

# Set the PA Level to MIN and disable LNA for testing & power supply related issues
radio.begin()
radio.setPALevel(RF24_PA_MIN, 0)

if not mesh.begin():
    # if mesh.begin() returns false for a master node,
    # then radio.begin() returned false.
    raise OSError("Radio hardware not responding.")
radio.printDetails()

try:
    while True:
        mesh.update()
        mesh.DHCP()

        while network.available():
            header, payload = network.read(struct.calcsize("L"))
            print(f"Received message {header.toString()}")
except KeyboardInterrupt:
    print("powering down radio and exiting.")
    radio.powerDown()
