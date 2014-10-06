
/**
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
void pingNode(std::map<char,uint16_t>::iterator it);


int main()
{	
	initscr();			/* Start curses mode 		  */
	start_color();
	//keypad(stdscr, TRUE); //Enable user interaction
	init_pair(1, COLOR_GREEN, COLOR_BLACK);
	attron(COLOR_PAIR(1));
	printw("RF24Mesh Master Node Monitoring Interface by TMRh20 - 2014\n");
	attroff(COLOR_PAIR(1));
	// Set the nodeID to 0 for the master node
	mesh.setNodeID(0);
	// Connect to the mesh
	printw("Establishing mesh...");
	refresh();			/* Print it on to the real screen */
	mesh.begin();
	mvprintw(1,0,"                     ");
	
	uint32_t kbTimer = 0,kbCount = 0, pingTimer=millis();
	std::map<char,uint16_t>::iterator it = mesh.addrMap.begin();
	
while(1)
{

	  // Call network.update as usual to keep the network updated
    mesh.update();
    // In addition, keep the 'DHCP service' running on the master node so addresses will
    // be assigned to the sensor nodes
    mesh.DHCP();
  
    // Check for incoming data from the sensors
    while(network.available()){
 
    RF24NetworkHeader header;
    //network.peek(header);
    
    uint32_t dat=0; uint8_t boldID = 0;
	
    network.read(header,&dat,sizeof(dat));
	kbCount++;
	
	attron(A_BOLD | COLOR_PAIR(1));
    mvprintw(2,0,"[Last Payload Info]\n");
    attroff(A_BOLD | COLOR_PAIR(1));
	
    mvprintw(3,0," HeaderID: %u \n Type: %c\n From: 0%o\n",header.id,header.type,header.from_node);
	for (std::map<char,uint16_t>::iterator it=mesh.addrMap.begin(); it!=mesh.addrMap.end(); ++it){                  
		//mvprintw(xCoord++,34,"ID: %d  Network: 0%o\n",it->first,it->second);
		if(header.from_node == it->second){
			boldID = it->first;
		}
	}
	printNodes(boldID);		
	refresh();


  }
  delay(2);
  
  if(millis()-kbTimer > 1000){
	kbTimer = millis();
	attron(A_BOLD | COLOR_PAIR(1));
	mvprintw(8,0,"[Data Rate (In)]\n");
    attroff(A_BOLD | COLOR_PAIR(1));
	printw(" Kbps: %.2f",(kbCount * 32 * 8)/1000.00);
    kbCount = 0;	
  }
  
  if(millis()-pingTimer>1000){
    pingTimer=millis();
	pingNode(it);
	  it++;
    
	if(	it == mesh.addrMap.end()){ // if(mesh.addrMap.size() > 1){ it=mesh.addrMap.begin(); } continue;}
		it=mesh.addrMap.begin();
	}
	
  }
  
  
}
	
	endwin();			/* End curses mode		  */
	return 0;
}


void printNodes(uint8_t boldID){

   uint8_t xCoord = 2;
   attron(A_BOLD | COLOR_PAIR(1));
   mvprintw(xCoord++,33,"[Address Assignments]\n");
   attroff(A_BOLD | COLOR_PAIR(1));
  for (std::map<char,uint16_t>::iterator it=mesh.addrMap.begin(); it!=mesh.addrMap.end(); ++it){
    if( boldID == it->first ){
		attron(A_BOLD | COLOR_PAIR(1));
	}
	mvprintw(xCoord++,34,"ID: %d  Network: 0%o\n",it->first,it->second);
	attroff(A_BOLD | COLOR_PAIR(1));
  }

}

void pingNode(std::map<char,uint16_t>::iterator IT){

   attron(A_BOLD | COLOR_PAIR(1));
   mvprintw(11,0,"[Ping Test]\n");
   attroff(A_BOLD | COLOR_PAIR(1));
  //for (std::map<char,uint16_t>::iterator it=mesh.addrMap.begin(); it!=mesh.addrMap.end(); ++it){
    RF24NetworkHeader header(IT->second,NETWORK_ADDR_CONFIRM);
	uint32_t pingtime=millis();
	bool ok = network.write(header,0,0);
	pingtime = millis()-pingtime;
	mvprintw(12,0," ID:%d\n Net:0%o\n Time:%ums\n",IT->first,IT->second,pingtime);
	if(ok){	printw(" OK\n");
	} else{ attron(A_BOLD); printw(" FAIL\n"); attron(A_BOLD); }
  //}

}