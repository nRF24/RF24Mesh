 /** RF24Mesh_Example_Dyn_Master.cpp by spaniakos
  * 
  * Note: This sketch only functions on -Rasberry PI-
  *
  * This example sketch shows how to dynamically configure a node via RF24Mesh as a master node.
  *
  * The nodes can change physical or logical position in the network, and reconnect through different
  * routing nodes as required. The master node manages the address assignments for the individual nodes
  * in a manner similar to DHCP.
  * The NodeID 255 is considered as a masterID that will be hardcoded on each client node.
  * The header message 'a' is the code to that coresponds in NodeID request while the 'A' is the response frame code.
  * Note: later it may be coded as call from the library releasing the 'a' and 'A'
  *
  */
  
#include "RF24Mesh/RF24Mesh.h"  
#include <RF24/RF24.h>
#include <RF24Network/RF24Network.h>


RF24 radio(RPI_V2_GPIO_P1_15, BCM2835_SPI_CS0, BCM2835_SPI_SPEED_8MHZ);  
RF24Network network(radio);
RF24Mesh mesh(radio,network);
//dummy header initialization
RF24NetworkHeader header_auth(255,'A');
bool ok;

int main(int argc, char** argv) {
  
  // Set the nodeID to 0 for the master node
  mesh.setNodeID(0);
  // Connect to the mesh
  printf("start\n");
  mesh.begin();
  radio.printDetails();
  srand(millis());
while(1)
{
  
  // Call network.update as usual to keep the network updated
  mesh.update();

  // In addition, keep the 'DHCP service' running on the master node so addresses will
  // be assigned to the sensor nodes
  mesh.DHCP();
  
  
  // Check for incoming data from the sensors
  while(network.available()){
//    printf("rcv\n");
    RF24NetworkHeader header;
    network.peek(header);
    
    uint32_t dat=0;
    switch(header.type){
      // Display the incoming millis() values from the sensor nodes
      case 'a': network.read(header,&dat,sizeof(dat)); 
                printf("Rcv %u from 0%o\n",dat,header.from_node);
		printf("Generating NodeID from list  \n");
		uint8_t NewNodeID;
		uint16_t next_id;
		ok = false;
		while(!ok){
			NewNodeID = rand() % 254 + 1;
			ok =  true;
			for (int i=0;i<mesh.addrListTop;i++){
				if (NewNodeID == mesh.addrList[i].nodeID){
					ok = false;
				}
			}
		}
		printf("The NodeID will be : %i\n",NewNodeID);
		memcpy(&header_auth.to_node,&header.from_node,sizeof(uint16_t));
		memcpy(&header_auth.id,&header.next_id,sizeof(uint16_t));
		next_id = header.next_id + (uint16_t)1;
		memcpy(&header_auth.next_id,&next_id,sizeof(uint16_t));
		ok = false;
		while(!ok){
			printf("SENDING ");
			ok = network.write(header_auth,&NewNodeID,sizeof(NewNodeID));
			if (!ok){delay(50);printf("FAILED\n");}else{printf("OK\n");} //do not flood the antenna
		}
                 break;
      case 'M': network.read(header,&dat,sizeof(dat)); 
                printf("Rcv %u from 0%o\n",dat,header.from_node);
                 break;
      default:  network.read(header,0,0); 
                printf("Rcv bad type %d from 0%o\n",header.type,header.from_node); 
                break;
    }
  }
//Carefull not to flood the antenna, needs time to DHCP
delay(300);
  }
return 0;
}
