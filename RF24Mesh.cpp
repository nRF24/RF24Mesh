



#include "RF24Mesh.h"
#include "RF24Mesh_config.h"
#if defined (__linux)
#include <fstream>
#endif

RF24Mesh::RF24Mesh( RF24& _radio,RF24Network& _network ): radio(_radio),network(_network),radio_channel(MESH_DEFAULT_CHANNEL){}

/*****************************************************/

void RF24Mesh::begin(){
  
  radio.begin();
  if(getNodeID()){ //Not master node
    mesh_address = MESH_DEFAULT_ADDRESS;
  }else{
    #if !defined (RF24_TINY)
	addrList = (addrListStruct*)malloc(2 * sizeof(addrListStruct));
	loadDHCP();
	#endif
    mesh_address = 0;
  }
  network.begin(radio_channel,mesh_address);
  
  if(getNodeID()){ //Not master node
    renewAddress();
  }  
}

/*****************************************************/

void RF24Mesh::update(){
	
	uint8_t type = network.update();
	
	#if !defined (RF24_TINY)
	if(type == NETWORK_REQ_ADDRESS){
	  doDHCP = 1;
	}
	
	if(type == MESH_ADDR_LOOKUP && !getNodeID()) {
	  uint8_t nodeID;
	  RF24NetworkHeader header;
	  memcpy(&header,network.frame_buffer,sizeof(RF24NetworkHeader));
	  memcpy(&nodeID,network.frame_buffer+sizeof(RF24NetworkHeader),sizeof(nodeID));  
	  header.to_node = header.from_node;
	  uint16_t returnAddr = getAddress(nodeID);
	  //printf("Returning lookup 0%o to 0%o   \n",returnAddr,header.to_node);
	  network.write(header,&returnAddr,sizeof(returnAddr));	
	}
	if(type == MESH_ADDR_RELEASE && !getNodeID() ){
		uint16_t fromAddr;
		memcpy(&fromAddr,network.frame_buffer,sizeof(fromAddr));
		for(uint8_t i=0; i<addrListTop; i++){
			if(addrList[i].address == fromAddr){
				addrList[i].address = 0;
			}
			
		}
		
		
	}
	#endif
	
}

bool RF24Mesh::write(uint16_t to_node, const void* data, uint8_t msg_type, size_t size ){
	RF24NetworkHeader header(to_node,msg_type);
	
	uint32_t writeTimer = millis();
    while ( !network.write(header,data,size) ){
	  
	  uint32_t delayTimer = millis();
	  while(millis() - delayTimer < 50){
		network.update();
	  }
	  if(millis()-writeTimer > MESH_WRITE_TIMEOUT){
		return 0;
	  }
    }
    return 1;
	
}

/*****************************************************/

bool RF24Mesh::write(const void* data, uint8_t msg_type, size_t size ){
  
  return write(00,data,msg_type,size);
    
}

/*****************************************************/

void RF24Mesh::setChannel(uint8_t _channel){

	radio_channel = _channel;
}
/*****************************************************/

bool RF24Mesh::checkConnection(){
	RF24NetworkHeader header(00,NETWORK_PING);
	uint8_t count = 3;
	bool ok;
	while(count--){
		ok = network.write(header,0,0);
		delay(153);
		if(ok){break;}
	}
	return ok;
	
}

/*****************************************************/

uint16_t RF24Mesh::getAddress(uint8_t nodeID){

//#if defined (ARDUINO_SAM_DUE) || defined (__linux)
#if !defined RF24_TINY
	if(!getNodeID()){ //Master Node
		uint16_t address = 0;
		for(uint8_t i=0; i<addrListTop; i++){
			if(addrList[i].nodeID == nodeID){
				address = addrList[i].address;
				return address;
			}
		}	
	}
	return 0;
#endif

	RF24NetworkHeader header( 00, MESH_ADDR_LOOKUP );
	
	if(network.write(header,&nodeID,sizeof(nodeID)+1) ){
		uint32_t timer=millis(), timeout = 500;		
		while(network.update() != MESH_ADDR_LOOKUP){
			if(millis()-timer > timeout){ return 0; }
		}		
	}
	uint16_t address;
	memcpy(&address,network.frame_buffer+sizeof(RF24NetworkHeader),sizeof(address));
	return address;	
}

/*****************************************************/

bool RF24Mesh::releaseAddress(){
	RF24NetworkHeader header(00,MESH_ADDR_RELEASE);
	return network.write(header,0,0);
}

/*****************************************************/

uint16_t RF24Mesh::renewAddress(){
  static const uint16_t requestDelay = 250;
  uint8_t reqCounter = 0;
  network.begin(radio_channel,MESH_DEFAULT_ADDRESS);
  mesh_address = MESH_DEFAULT_ADDRESS;

  while(!requestAddress(reqCounter)){
    delay(requestDelay+(mesh_address%(7)*8));   
    (++reqCounter) = reqCounter%4;
  }
  return mesh_address;
}

/*****************************************************/

bool RF24Mesh::requestAddress(uint8_t level){    
    
	RF24NetworkHeader header( 0100, NETWORK_POLL );

	struct addrResponseStruct{
	  uint16_t requester;
      uint16_t new_address;  
    }addrResponse;

     //Find another radio, starting with level 0 multicast	
	network.multicast(header,0,0,level);

	// Wait for a response
	uint32_t timr = millis();
	while( network.update() != NETWORK_POLL){

	if(millis() - timr > 150UL){
	  #if defined (MESH_DEBUG_SERIAL)
	  Serial.print( millis() ); Serial.print(F(" MSH: No poll response from level "));Serial.println(level);
      #elif defined (MESH_DEBUG_PRINTF)
	  printf( "%u MSH: No poll response from level %d\n", millis(), level);
	  #endif

	  return 0;
	}
	}

  uint16_t *contactNode = (uint16_t*)(&network.frame_buffer);
  //memcpy(&contactNode,&network.frame_buffer,sizeof(contactNode));
    
    #ifdef MESH_DEBUG_SERIAL
	Serial.print( millis() ); Serial.print(F(" MSH: Got poll from level ")); Serial.println(level);
	#elif defined MESH_DEBUG_PRINTF
	printf("%u MSH: Got poll from level %d\n",millis(),level);
    #endif	

    // Request an address via the contact node
    header.type = NETWORK_REQ_ADDRESS;
    header.reserved = getNodeID();
    header.to_node = *contactNode;    
    
    // Do a direct write (no ack) to the contact node. Include the nodeId and address.
	
    network.write(header,&mesh_address,sizeof(addrResponse),*contactNode);
    #ifdef MESH_DEBUG_SERIAL
	  Serial.print( millis() ); Serial.println(F(" MSH: Request address "));
	#elif defined MESH_DEBUG_PRINTF
	  printf("%u  MSH: Request address this node: 0%o\n",millis(),mesh_address);
	#endif
	
	timr = millis();
	while(network.update() != NETWORK_ADDR_RESPONSE){
		if(millis() - timr > 500UL){
		#ifdef MESH_DEBUG_SERIAL
          Serial.print( millis() ); Serial.print(F(" MSH: No address response from level ")); Serial.println( level );
		#elif defined MESH_DEBUG_PRINTF
		  printf("%u MSH: No address response from level %d\n",millis(),level );
		#endif
	    return 0;
		}
	}
	
	#ifdef MESH_DEBUG_SERIAL
    uint8_t mask = 7;	char addr[5] = "    ", count=3; uint16_t newAddr;	
	#endif
	uint8_t registerAddrCount = 0;

	memcpy(&addrResponse,network.frame_buffer+sizeof(RF24NetworkHeader),sizeof(addrResponse));
			   
	if(!addrResponse.new_address || network.frame_buffer[7] != getNodeID() ){
		#ifdef MESH_DEBUG_SERIAL
		  Serial.print(millis()); Serial.println(F(" MSH: Response discarded, wrong node"));
		#elif defined MESH_DEBUG_PRINTF
		  printf("%u Response discarded, wrong node 0%o from node 0%o sending node 0%o\n",millis(),addrResponse.new_address,header.from_node,addrResponse.requester);
        #endif
		return 0;
	}
	#ifdef MESH_DEBUG_SERIAL
	  Serial.print(F("Set address: "));
	  newAddr = addrResponse.new_address;
	  while(newAddr){
		addr[count] = (newAddr & mask)+48; //get the individual Octal numbers, specified in chunks of 3 bits, convert to ASCII by adding 48
		newAddr >>= 3;
		count--;
	  }
	  Serial.println(addr);
	#elif defined (MESH_DEBUG_PRINTF)
	  printf("Set address 0%o rcvd 0%o\n",mesh_address,addrResponse.new_address);
	#endif
	mesh_address = addrResponse.new_address;
	//radio.begin();
	network.begin(radio_channel,mesh_address);
	header.to_node = 00;
	header.type = MESH_ADDR_CONFIRM;
	//network.write(header,0,0);
	//delay(55);
	while( !network.write(header,0,0) ){
		//delay(55);
		//printf("Retry register address...\n");
		if(registerAddrCount++ >= 6 ){ return 0; }
	}
    return 1;  
}

/*****************************************************/
/*
bool RF24Mesh::waitForAvailable(uint32_t timeout){
  
    unsigned long timer = millis();
    while(millis()-timer < timeout){
      network.update();
	  if(network.available()){ return 1; }
	}
    if(network.available()){ return 1; }
	else{  return 0; }
}
*/
/*****************************************************/

#if defined (__AVR__) && !defined (RF24_TINY)

uint8_t RF24Mesh::getNodeID(){

  if(EEPROM.read(509) == 'R' && EEPROM.read(510) == 'F'){
    return EEPROM.read(511);  
  }
  return 0;
}

/*****************************************************/

void RF24Mesh::setNodeID(uint8_t nodeID){
  
  if(millis() - lastSaveTime < MESH_MIN_SAVE_TIME && millis() >= MESH_MIN_SAVE_TIME){	
	#ifdef MESH_DEBUG_SERIAL
	Serial.print( millis() ); Serial.println(F(" MSH: Can't change nodeID that fast")); 
	#endif	
	return;
  }
  lastSaveTime=millis();
  
  if(EEPROM.read(509) != 'R' || EEPROM.read(510) != 'F' || EEPROM.read(511) != nodeID){
    EEPROM.write(509,'R');  EEPROM.write(510,'F'); EEPROM.write(511, nodeID);  
    #ifdef MESH_DEBUG_SERIAL
	Serial.print( millis() ); Serial.println(F(" MSH: Wrote data to EEPROM"));
	#endif  
  }else{    
	#ifdef MESH_DEBUG_SERIAL
	Serial.print( millis() ); Serial.println(F(" MSH: Data already stored in EEPROM"));
	#endif  
  }
}

/*****************************************************/

#else

/*****************************************************/

uint8_t RF24Mesh::getNodeID(){
	return _nodeID;
}
void RF24Mesh::setNodeID(uint8_t nodeID){
	_nodeID = nodeID;
}
#endif

/*****************************************************/
void RF24Mesh::loadDHCP(){
	
#if defined (__linux)
	std::ifstream infile ("dhcplist.txt",std::ifstream::binary);
	if(!infile){ return; }
	
    addrList[addrListTop].nodeID = 255;
	addrList[addrListTop].address = 01114;	
	
	infile.seekg(0,infile.end);
	int length = infile.tellg();
	infile.seekg(0,infile.beg);

	addrList = (addrListStruct*)realloc(addrList,length+sizeof(addrListStruct));

	addrListTop = length/sizeof(addrListStruct);
	for(int i=0; i<addrListTop; i++){
		infile.read( (char*)&addrList[i],sizeof(addrListStruct));
	
	}
	infile.close();
#endif	
}


void RF24Mesh::saveDHCP(){
#if defined (__linux)
	std::ofstream outfile ("dhcplist.txt",std::ofstream::binary | std::ofstream::trunc);

	//printf("writingToFile %d  0%o size %d\n",addrList[0].nodeID,addrList[0].address,sizeof(addrListStruct));
	
	for(int i=0; i< addrListTop; i++){
		outfile.write( (char*)&addrList[i],sizeof(addrListStruct));
    }
	outfile.close();
	
	/*addrListStruct aList;
	std::ifstream infile ("dhcplist.txt",std::ifstream::binary);
	infile.seekg(0,infile.end);
	int length = infile.tellg();
	infile.seekg(0,infile.beg);
	//addrList = (addrListStruct*)malloc(length);
	
	//infile.read( (char*)&addrList,length);
	infile.read( (char*)&aList,sizeof(addrListStruct));
	 //addrListTop = length/sizeof(addrListStruct);
	//for(int i=0; i< addrListTop; i++){
	printf("ID: %d  ADDR: 0%o  \n",aList.nodeID,aList.address);
	//}
	infile.close();*/
#endif
}

/*****************************************************/

#if !defined (RF24_TINY)

void RF24Mesh::DHCP(){
  
  if(doDHCP){
		 doDHCP = 0;
  }else{ return; }
    RF24NetworkHeader header;
	memcpy(&header,network.frame_buffer,sizeof(RF24NetworkHeader));	
    //network.peek(header);
    
    // Check for an address request payload
    //if(header.type == NETWORK_REQ_ADDRESS){
      
      struct addrResponseStruct{
        uint16_t requester;
        uint16_t new_address;  
      } addrResponse;
      
           
      // Get the unique id of the requester
      uint8_t from_id = header.reserved;
      if(!from_id){
	  #ifdef MESH_DEBUG_PRINTF
		 printf("MSH: Invalid id 0 rcvd\n");
      #endif
        //network.read(header,0,0);
	    return;
	  }
     
      //network.read(header,&addrResponse,sizeof(addrResponse));
	  memcpy(&addrResponse,network.frame_buffer+sizeof(RF24NetworkHeader),sizeof(addrResponse));
     
     // Get the address of the sender (initial, or intermediary)
      uint16_t fwd_by = header.from_node;      
     uint8_t shiftVal = 0;

     if(header.from_node == addrResponse.requester || header.from_node == MESH_DEFAULT_ADDRESS){ //Addresses 01-05     
        fwd_by = 0; // No forwarding address
     }else{ //Addresses 01111-05555             
       uint16_t m = fwd_by;
       uint8_t count = 0;
       
       while(m){  //Octal addresses convert nicely to binary in threes. Address 03 = B011  Address 033 = B011011
         m >>= 3; //Find out how many digits are in the octal address
         count++; 
       }       
       shiftVal = count*3; //Now we know how many bits to shift when adding a child node 1-5 (B001 to B101) to any address
     }

       #ifdef MESH_DEBUG_PRINTF
	   //  printf("%u MSH: Rcv addr req from_id %d \n",millis(),from_id);
	   #endif
       
	   for(int i=MESH_MAX_CHILDREN; i> 0; i--){ // For each of the possible addresses (5 max)
         
        bool found = 0;
        addrResponse.new_address = fwd_by | (i << shiftVal);
		if(!addrResponse.new_address ){ /*printf("dumped 0%o\n",addrResponse.new_address);*/ continue; }

		for(uint8_t i=0; i < addrListTop; i++){
			#if defined (MESH_DEBUG_MINIMAL)
			#if !defined (__linux) && !defined ARDUINO_SAM_DUE || defined TEENSY
			Serial.print("ID: ");Serial.print(addrList[i].nodeID,DEC);Serial.print(" ADDR: ");			
			uint16_t newAddr = addrList[i].address;
			char addr[5] = "    ", count=3, mask=7;
			while(newAddr){
				addr[count] = (newAddr & mask)+48; //get the individual Octal numbers, specified in chunks of 3 bits, convert to ASCII by adding 48
				newAddr >>= 3;
				count--;
			}
			Serial.println(addr);
			#else
			printf("ID: %d ADDR: 0%o\n", addrList[i].nodeID,addrList[i].address);
			#endif
			#endif
			if(  (addrList[i].address == addrResponse.new_address && addrList[i].nodeID != from_id ) || addrResponse.new_address == MESH_DEFAULT_ADDRESS){
				found = 1;
				break;
			}
		}		
		
        if(!found){

          header.type = NETWORK_ADDR_RESPONSE;
          header.to_node = header.from_node;
		  //This is a routed request to 00
          if(header.from_node != addrResponse.requester){ //Is NOT node 01 to 05
			delay(2);
			if( network.write(header,&addrResponse,sizeof(addrResponse)) ){
				//addrMap[from_id] = addrResponse.new_address;
			}
          }else{
		    delay(2);
		    network.write(header,&addrResponse,sizeof(addrResponse),header.to_node);
			//addrMap[from_id] = addrResponse.new_address;
          }
       		uint32_t timer=millis();
			while(network.update() != MESH_ADDR_CONFIRM){
				if(millis()-timer>900){
				    //printf("No addr confirmation from 0%o\n",header.to_node);
					return;
				}
				
			}
		  //printf("Got addr confirmation from 0%o\n",header.to_node);
          found = 0;
		  for(uint8_t i=0; i < addrListTop; i++){
			if(  addrList[i].nodeID == from_id  ){
				addrList[i].address = addrResponse.new_address;
				found = 1;
				#if defined (__linux)
				if(millis()-lastFileSave > 300){
					lastFileSave = millis();
					saveDHCP();
				}
				#endif
				break;
			}
		  }		  
		  if(!found){
		    addrList[addrListTop].nodeID = from_id;
			addrList[addrListTop].address = addrResponse.new_address;
			#if defined (__linux)
			if(millis()-lastFileSave > 300){
				lastFileSave = millis();
				saveDHCP();
			}
			#endif			
			++addrListTop;
			addrList = (addrListStruct*)realloc(addrList,(addrListTop+1) * sizeof(addrListStruct));
			
		  }
		  
		  
		  #ifdef MESH_DEBUG_PRINTF
		    printf("Sent to 0%o phys: 0%o new: 0%o id: %d\n", header.to_node,addrResponse.requester,addrResponse.new_address,header.reserved);
          #endif
		  
		  break;
        }else{
		#if defined (MESH_DEBUG_PRINTF)
		  printf("not allocated\n");
		#endif
		}
      }

   //}else{
	//break;
   //}
    
  //}

}

/*****************************************************/

#endif
