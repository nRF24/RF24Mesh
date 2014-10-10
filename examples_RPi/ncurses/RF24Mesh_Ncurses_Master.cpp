
/*
* RF24Mesh Master Node Monitoring Tool
* This is a generic tool for master nodes running RF24Mesh that will display address
* assignments, and information regarding incoming data, regardless of the specific
* configuration details.
*
* Requirements: NCurses 
* Install NCurses: apt-get install libncurses5-dev
* Setup:
* 1: make
* 2: sudo ./RF24Mesh_Ncurses_Master
* 
* NOTE: DEBUG MUST BE DISABLED IN RF24Mesh_config.h
*
* Once configured and running, the interface will display the header information, data rate, 
* and address assignments for all connected nodes.*
* The master node will also continuously ping each of the child nodes, one per second, while indicating
* the results.
*
*/

#include <ncurses.h>
#include "RF24Mesh/RF24Mesh.h"  
#include <RF24/RF24.h>
#include <RF24Network/RF24Network.h>

RF24 radio(RPI_V2_GPIO_P1_15, BCM2835_SPI_CS0, BCM2835_SPI_SPEED_8MHZ);  
RF24Network network(radio);
RF24Mesh mesh(radio,network);

void printNodes(uint8_t boldID);
void pingNode(uint8_t listNo);

uint8_t nodeCounter;

uint16_t failID = 0;

int main()
{	

    printf("Establishing mesh...\n");
	mesh.setNodeID(0);
    mesh.begin();
	radio.printDetails();

	initscr();			/* Start curses mode 		  */
	start_color();
	curs_set(0);
	//keypad(stdscr, TRUE); //Enable user interaction
	init_pair(1, COLOR_GREEN, COLOR_BLACK);
	init_pair(2, COLOR_RED, COLOR_BLACK);
	attron(COLOR_PAIR(1));
	printw("RF24Mesh Master Node Monitoring Interface by TMRh20 - 2014\n");
	attroff(COLOR_PAIR(1));
	refresh();			/* Print it on to the real screen */
	
	uint32_t kbTimer = 0,kbCount = 0, pingTimer=millis();	
	//std::map<char,uint16_t>::iterator it = mesh.addrMap.begin();
    unsigned long totalPayloads = 0;

while(1)
{

    // Call mesh.update to keep the network updated
    mesh.update();
    // In addition, keep the 'DHCP service' running on the master node so addresses will
    // be assigned to the sensor nodes
	mesh.DHCP();
    // Wait until a sensor node is connected
	if(sizeof(mesh.addrList) < 1){continue; }
	
	// Check for incoming data from the sensors
    while(network.available()){    
		RF24NetworkHeader header;
		network.peek(header);
	
		uint8_t boldID = 0;
		
		// Print the total number of received payloads
		mvprintw(9,0," Total: %lu\n",totalPayloads++);

		kbCount++;
	
		attron(A_BOLD | COLOR_PAIR(1));
		mvprintw(2,0,"[Last Payload Info]\n");
		attroff(A_BOLD | COLOR_PAIR(1));	
		
		// Read the network payload
		network.read(header,0,0);
		
		// Display the header info
		mvprintw(3,0," HeaderID: %u  \n Type: %d  \n From: 0%o  \n ",header.id,header.type,header.from_node);

		//refresh();
		//for (std::map<char,uint16_t>::iterator _it=mesh.addrMap.begin(); _it!=mesh.addrMap.end(); _it++){
		for(uint8_t i=0; i<mesh.addrListTop; i++){
			if(header.from_node == mesh.addrList[i].address){
				boldID = mesh.addrList[i].nodeID;
			}
		}
		printNodes(boldID);		

    }
	//refresh();
  
    if(millis()-kbTimer > 1000 && kbCount > 0){
	kbTimer = millis();
	attron(A_BOLD | COLOR_PAIR(1));
	mvprintw(7,0,"[Data Rate (In)]");
    attroff(A_BOLD | COLOR_PAIR(1));
	mvprintw(8,0," Kbps: %.2f",(kbCount * 32 * 8)/1000.00);
    kbCount = 0;
	
  }
  
  // Ping each connected node, one per second
  if(millis()-pingTimer>1003 && sizeof(mesh.addrList) > 0){
    pingTimer=millis();
	if(	nodeCounter == mesh.addrListTop){ // if(mesh.addrMap.size() > 1){ it=mesh.addrMap.begin(); } continue;}
		nodeCounter = 0;
	}
	pingNode(nodeCounter);
    nodeCounter++;	
  }
  
	/*uint32_t nOK,nFails;
	network.failures(&nFails,&nOK);
	attron(A_BOLD | COLOR_PAIR(1));
    mvprintw(2,24,"[Transmit Results] ");
    attroff(A_BOLD | COLOR_PAIR(1));
	mvprintw(3,25," #OK: %u   ",nOK);
	mvprintw(4,25," #Fail: %u   ",nFails);*/
  
  
  refresh();
  delay(2);
}//while 1
	
	endwin();			/* End curses mode		  */
	return 0;
}


void printNodes(uint8_t boldID){

   uint8_t xCoord = 2;
   attron(A_BOLD | COLOR_PAIR(1));
   mvprintw(xCoord++,27,"[Address Assignments]\n");
   attroff(A_BOLD | COLOR_PAIR(1));
  //for (std::map<char,uint16_t>::iterator it=mesh.addrMap.begin(); it!=mesh.addrMap.end(); ++it){
  for( uint8_t i=0; i<mesh.addrListTop; i++){
    //if( failID == it->first){
	if( failID == mesh.addrList[i].nodeID){
		attron(COLOR_PAIR(2));
	}else
	if( boldID == mesh.addrList[i].nodeID ){
		attron(A_BOLD | COLOR_PAIR(1));
	}
	mvprintw(xCoord++,28,"ID: %d  Network: 0%o   ",mesh.addrList[i].nodeID,mesh.addrList[i].address);
	attroff(A_BOLD | COLOR_PAIR(1));
	attroff(COLOR_PAIR(2));
  }
  mvprintw(xCoord++,28,"                   ");
  mvprintw(xCoord++,28,"                   ");
  mvprintw(xCoord++,28,"                   ");
}

void pingNode(uint8_t listNo){

   attron(A_BOLD | COLOR_PAIR(1));
   mvprintw(11,0,"[Ping Test]\n");
   attroff(A_BOLD | COLOR_PAIR(1));

    RF24NetworkHeader headers(mesh.addrList[listNo].address,NETWORK_PING);
	uint32_t pingtime=millis();
	bool ok;
	if(headers.to_node){
		ok = network.write(headers,0,0);
		if(ok && failID == mesh.addrList[listNo].nodeID){ failID = 0; }
		if(!ok){ failID = mesh.addrList[listNo].nodeID; }
	}
	pingtime = millis()-pingtime;
	mvprintw(12,0," ID:%d    ",mesh.addrList[listNo].nodeID);
	mvprintw(13,0," Net:0%o    ",mesh.addrList[listNo].address);
	mvprintw(14,0," Time:%ums       ",pingtime);
	
	if(ok || !headers.to_node){	mvprintw(15,0," OK  ");
	} else{ attron(A_BOLD); mvprintw(15,0," FAIL"); attron(A_BOLD); }
}
