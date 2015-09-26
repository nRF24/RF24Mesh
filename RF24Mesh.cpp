



#include "RF24Mesh.h"
#include "RF24Mesh_config.h"
#if defined (__linux) && !defined(__ARDUINO_X86__)
#include <fstream>
#endif

RF24Mesh::RF24Mesh( RF24& _radio,RF24Network& _network ): radio(_radio),network(_network){}


/*****************************************************/

bool RF24Mesh::begin(uint8_t channel, rf24_datarate_e data_rate){
  radio.begin();
  if(getNodeID()){ //Not master node
    mesh_address = MESH_DEFAULT_ADDRESS;
  }else{
    #if !defined (RF24_TINY) && !defined(MESH_NOMASTER)
	addrList = (addrListStruct*)malloc(2 * sizeof(addrListStruct));
	loadDHCP();
	#endif
    mesh_address = 0;
  }
  radio_channel = channel;
  radio.setChannel(radio_channel);
  radio.setDataRate(data_rate);  
  network.returnSysMsgs = 1;
  if(getNodeID()){ //Not master node
    if(!renewAddress()){
      return 0;
    }
  }else{
    network.begin(mesh_address);
  }
  return 1;
}

/*****************************************************/

uint8_t RF24Mesh::update(){

    
    
	uint8_t type = network.update();
    if(mesh_address == MESH_DEFAULT_ADDRESS){ return type; }
    
	#if !defined (RF24_TINY) && !defined(MESH_NOMASTER)
	if(type == NETWORK_REQ_ADDRESS){
	  doDHCP = 1;
	}

	if( (type == MESH_ADDR_LOOKUP || type == MESH_ID_LOOKUP) && !getNodeID()) {
	  RF24NetworkHeader& header = *(RF24NetworkHeader*)network.frame_buffer;
	  header.to_node = header.from_node;
	  
      if(type==MESH_ADDR_LOOKUP){
	    uint16_t returnAddr = getAddress(network.frame_buffer[sizeof(RF24NetworkHeader)]);
        network.write(header,&returnAddr,sizeof(returnAddr)); 
      }else{
        uint8_t returnAddr = getNodeID(network.frame_buffer[sizeof(RF24NetworkHeader)]);
        network.write(header,&returnAddr,sizeof(returnAddr));        
      }      
	  //printf("Returning lookup 0%o to 0%o   \n",returnAddr,header.to_node);
	  //network.write(header,&returnAddr,sizeof(returnAddr));	
	}else
	if(type == MESH_ADDR_RELEASE && !getNodeID() ){
		uint16_t *fromAddr = (uint16_t*)network.frame_buffer;
		for(uint8_t i=0; i<addrListTop; i++){
			if(addrList[i].address == *fromAddr){
				addrList[i].address = 0;
			}
		}		
	}
	#endif
	return type;
}

bool RF24Mesh::write(uint16_t to_node, const void* data, uint8_t msg_type, size_t size ){
    if(mesh_address == MESH_DEFAULT_ADDRESS){ return 0; }
	RF24NetworkHeader header(to_node,msg_type);	
	return network.write(header,data,size);	
}

/*****************************************************/

bool RF24Mesh::write(const void* data, uint8_t msg_type, size_t size, uint8_t nodeID){
  if(mesh_address == MESH_DEFAULT_ADDRESS){ return 0; }
  uint16_t toNode = 0;
  if(nodeID){
    toNode = getAddress(nodeID);
    if(!toNode){return 0;}
  }
  return write(toNode,data,msg_type,size);

}

/*****************************************************/

void RF24Mesh::setChannel(uint8_t _channel){

	radio_channel = _channel;
	radio.setChannel(radio_channel);
	radio.startListening();
}
/*****************************************************/

bool RF24Mesh::checkConnection(){

	RF24NetworkHeader header(00,NETWORK_PING);
	uint8_t count = 3;
	bool ok = 0;
	while(count-- && mesh_address != MESH_DEFAULT_ADDRESS){
        update();
        if(radio.rxFifoFull() || (network.networkFlags & 1)){
          return 1;
        }
		ok = network.write(header,0,0);
		if(ok){break;}
		delay(153);
	}
    if(!ok){ radio.stopListening(); }
	return ok;
	
}

/*****************************************************/

uint16_t RF24Mesh::getAddress(uint8_t nodeID){

//#if defined (ARDUINO_SAM_DUE) || defined (__linux)
#if !defined RF24_TINY && !defined(MESH_NOMASTER)
	if(!getNodeID()){ //Master Node
		uint16_t address = 0;
		for(uint8_t i=0; i<addrListTop; i++){
			if(addrList[i].nodeID == nodeID){
				address = addrList[i].address;
				return address;
			}
		}
        return 0;
	}
#endif
    if(mesh_address == MESH_DEFAULT_ADDRESS){ return 0; }
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

int RF24Mesh::getNodeID(uint16_t address){

    if(address == MESH_BLANK_ID){
      return _nodeID;
    }else
    if(address == 0){
      return 0;
    }    
    
    if(!mesh_address){ //Master Node
        for(uint8_t i=0; i<addrListTop; i++){
            if(addrList[i].address == address){
                return addrList[i].nodeID;
            }
        }
    }else{
      if(mesh_address == MESH_DEFAULT_ADDRESS){ return -1; }
      RF24NetworkHeader header( 00, MESH_ID_LOOKUP );
      if(network.write(header,&address,sizeof(address)) ){
        uint32_t timer=millis(), timeout = 500;	
		while(network.update() != MESH_ID_LOOKUP){            
			if(millis()-timer > timeout){ return -1; }
		}
        return network.frame_buffer[sizeof(RF24NetworkHeader)];
      }
    }
    return -1;
}
/*****************************************************/

bool RF24Mesh::releaseAddress(){
    
    if(mesh_address == MESH_DEFAULT_ADDRESS){ return 0; }
    network.begin(MESH_DEFAULT_ADDRESS);
    mesh_address=MESH_DEFAULT_ADDRESS;
	RF24NetworkHeader header(00,MESH_ADDR_RELEASE);
	return network.write(header,0,0);
}

/*****************************************************/

uint16_t RF24Mesh::renewAddress(uint32_t timeout){

  if(radio.available()){ return 0; }
  uint8_t reqCounter = 0;
  uint8_t totalReqs = 0;
  radio.stopListening();

  network.networkFlags |= 2;
  delay(10);
  
  network.begin(MESH_DEFAULT_ADDRESS);
  mesh_address = MESH_DEFAULT_ADDRESS;
  
  uint32_t start = millis();
  while(!requestAddress(reqCounter)){
    if(millis()-start > timeout){ return 0; }
    delay(50 + ( (totalReqs+1)*(reqCounter+1)) * 2);
    (++reqCounter) = reqCounter%4;
    (++totalReqs) = totalReqs%10;
    
  }
  network.networkFlags &= ~2;
  return mesh_address;
}

/*****************************************************/

bool RF24Mesh::requestAddress(uint8_t level){    
    
	RF24NetworkHeader header( 0100, NETWORK_POLL );
     //Find another radio, starting with level 0 multicast	
	network.multicast(header,0,0,level);
	
	uint32_t timr = millis();
	uint16_t *contactNode = 0;
	
    struct addrResponseStruct{
	  uint16_t requester;
      uint16_t new_address;  
    }addrResponse;
	
	while(1){
		bool goodSignal = radio.testRPD();
		if(network.update() == NETWORK_POLL){
			contactNode = (uint16_t*)(&network.frame_buffer);
			if(goodSignal){
			    // This response was better than -64dBm
                #if defined (MESH_DEBUG_SERIAL)
	            Serial.print( millis() ); Serial.println(F(" MSH: Poll > -64dbm "));
                #elif defined (MESH_DEBUG_PRINTF)
	            printf( "%u MSH: Poll > -64dbm\n", millis() );
	            #endif
				break;
			}
		}
		if(millis() - timr > 25 ){
			if(!contactNode){
              #if defined (MESH_DEBUG_SERIAL)
	          Serial.print( millis() ); Serial.print(F(" MSH: No poll from level "));Serial.println(level);
              #elif defined (MESH_DEBUG_PRINTF)
	          printf( "%u MSH: No poll from level %d\n", millis(), level);
	          #endif		
		      return 0;
			}else{
              
              #if defined (MESH_DEBUG_SERIAL)
              Serial.print( millis() ); Serial.println(F(" MSH: Poll < -64dbm "));
              #elif defined (MESH_DEBUG_PRINTF)
	          printf( "%u MSH: Poll < -64dbm\n", millis() );
	          #endif	
			  break;
			}
		}
	}
	
   
    #ifdef MESH_DEBUG_SERIAL
	Serial.print( millis() ); Serial.print(F(" MSH: Got poll from level ")); Serial.println(level);
	#elif defined MESH_DEBUG_PRINTF
	printf("%u MSH: Got poll from level %d\n",millis(),level);
    #endif	

    // Request an address via the contact node
    header.type = NETWORK_REQ_ADDRESS;
    header.reserved = getNodeID();
    header.to_node = *contactNode;    
    
    uint16_t addr = MESH_DEFAULT_ADDRESS;
    // Do a direct write (no ack) to the contact node. Include the nodeId and address.	
    network.write(header,&addr,sizeof(addrResponse),*contactNode);
    #ifdef MESH_DEBUG_SERIAL
	  Serial.print( millis() ); Serial.println(F(" MSH: Request address "));
	#elif defined MESH_DEBUG_PRINTF
	  printf("%u  MSH: Request address this node: 0%o\n",millis(),mesh_address);
	#endif
	
	timr = millis();
	while(network.update() != NETWORK_ADDR_RESPONSE){
		if(millis() - timr > 25){
		#ifdef MESH_DEBUG_SERIAL
          Serial.print( millis() ); Serial.print(F(" MSH: No address response from level ")); Serial.println( level );
		#elif defined MESH_DEBUG_PRINTF
		  printf("%u MSH: No address response from level %d\n",millis(),level );
		#endif
	    return 0;
		}
	}
	//Serial.print("response took");
	//Serial.println(millis()-timr);
	#ifdef MESH_DEBUG_SERIAL
    uint8_t mask = 7;	char addrs[5] = "    ", count=3; uint16_t newAddr;	
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
	  Serial.print( millis() );Serial.print(F(" Set address: "));
	  newAddr = addrResponse.new_address;
	  while(newAddr){
		addrs[count] = (newAddr & mask)+48; //get the individual Octal numbers, specified in chunks of 3 bits, convert to ASCII by adding 48
		newAddr >>= 3;
		count--;
	  }
	  Serial.println(addrs);
	#elif defined (MESH_DEBUG_PRINTF)
	  printf("Set address 0%o rcvd 0%o\n",mesh_address,addrResponse.new_address);
	#endif
	mesh_address = addrResponse.new_address;
	//radio.begin();
    radio.stopListening();
    delay(10);
	network.begin(mesh_address);
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

void RF24Mesh::setNodeID(uint8_t nodeID){
	_nodeID = nodeID;
}

/*****************************************************/

void RF24Mesh::setStaticAddress(char nodeID, uint16_t address){
  
  addrList[addrListTop].nodeID = nodeID;
  addrList[addrListTop].address = address;
  ++addrListTop;
  addrList = (addrListStruct*)realloc(addrList,(addrListTop) * sizeof(addrListStruct));
}

/*****************************************************/

void RF24Mesh::loadDHCP(){
	
#if defined (__linux) && !defined(__ARDUINO_X86__)
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

/*****************************************************/

void RF24Mesh::saveDHCP(){
#if defined (__linux)  && !defined(__ARDUINO_X86__)
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

#if !defined (RF24_TINY) && !defined(MESH_NOMASTER)

void RF24Mesh::DHCP(){
  
  if(doDHCP){
		 doDHCP = 0;
  }else{ return; }
    RF24NetworkHeader header;
	memcpy(&header,network.frame_buffer,sizeof(RF24NetworkHeader));	
      
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
			#if !defined (__linux) && !defined ARDUINO_SAM_DUE || defined TEENSY || defined(__ARDUINO_X86__)
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
		  //printf("Send resp %d\n",sizeof(addrResponse));
          //This is a routed request to 00
          if(header.from_node != addrResponse.requester){ //Is NOT node 01 to 05
			delay(2);
			if( network.write(header,&addrResponse,sizeof(addrResponse)) ){
				//addrMap[from_id] = addrResponse.new_address;
			}else{
               network.write(header,&addrResponse,sizeof(addrResponse));
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
				#if defined (__linux) && !defined(__ARDUINO_X86__)
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
			#if defined (__linux)  && !defined(__ARDUINO_X86__)
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

}

/*****************************************************/

#endif
