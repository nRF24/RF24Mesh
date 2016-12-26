from RF24 import *
from RF24Network import *
from RF24Mesh import *


class Mesh(object):
    def __init__(self):
        # radio setup for RPi B Rev2: CS0=Pin 24
        self.radio = RF24(RPI_V2_GPIO_P1_15, RPI_V2_GPIO_P1_24, BCM2835_SPI_SPEED_8MHZ)
        self.network = RF24Network(self.radio)
        self.mesh = RF24Mesh(self.radio, self.network)
        
        self.mesh.setNodeID(0)
        self.mesh.begin(108, RF24_250KBPS)
        self.radio.setPALevel(RF24_PA_MAX) # Power Amplifier
        self.radio.printDetails()

        while 1:
            self.mesh.update()
            self.mesh.DHCP()

            while self.network.available():
                print("Received message")
                header, payload = self.network.read(10)
