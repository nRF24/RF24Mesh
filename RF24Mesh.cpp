



#include "RF24Mesh.h"
#include "RF24Mesh_config.h"

RF24Mesh::RF24Mesh( RF24& _radio,RF24Network& _network ): radio(_radio),network(_network){}

void RF24Mesh::begin(){
  
  radio.begin();
  if(getNodeID()){ //Not master node
    mesh_address = MESH_DEFAULT_ADDRESS;
  }
  network.begin(MESH_DEFAULT_CHANNEL,mesh_address);
  
  if(getNodeID()){ //Not master node
    renewAddress();
  }
     
  
}

bool RF24Mesh::write(const void* data, uint8_t msg_type, size_t size ){
  RF24NetworkHeader header(00,msg_type);
  return network.write(header,data,size);  
}


void RF24Mesh::renewAddress(){
  static const uint16_t requestDelay = 500;
  uint8_t reqCounter = 0;

  while(!requestAddress(reqCounter)){
    delay(requestDelay);   
    (++reqCounter)%5;
  }
}


bool RF24Mesh::findNodes(RF24NetworkHeader& header,uint8_t level, uint16_t *address){

  // Multicast a NETWORK_POLL request to the specified level  
  
  network.multicast(header,0,0,level);
  
  // Wait for a response
  if( waitForAvailable(150UL) == 0 ){ 
    #if defined (MESH_DEBUG_SERIAL)
	Serial.print( millis() ); Serial.print(" MSH: No poll response from level ");Serial.println(level);
    #elif defined (MESH_DEBUG_PRINTF)
	printf( "%u MSH: No poll response from level %d\n", millis(), level);
	#endif
    return 0;
  }
  
    uint16_t tmp_address = 077;
    // Check to see if a valid response was received
    while( network.available()){
	  network.read(header,0,0);
	  // Grab the first response
	  if(header.type == NETWORK_POLL && tmp_address == 077){
		*address = header.from_node;
	  }
	}
	
	if(*address == 077){
      #ifdef MESH_DEBUG_SERIAL
		Serial.print( millis() );Serial.print(" MSH: Wrong type, expected poll response ");Serial.println(header.type);
	  #elif defined MESH_DEBUG_PRINTF
	    printf("%u MSH: Wrong type, expected poll response %d\n",millis(), header.type);
	  #endif
	  return 0;
    }
	
    #ifdef MESH_DEBUG_SERIAL
	Serial.print( millis() ); Serial.print(" MSH: Got poll from level "); Serial.println(level);
	#elif defined MESH_DEBUG_PRINTF
	printf("%u MSH: Got poll from level %d\n",millis(),level);
    #endif
	return 1;
}


bool RF24Mesh::requestAddress(uint8_t level){    
    
	RF24NetworkHeader header( 0100, NETWORK_POLL );
	
	struct addrResponseStruct{
	  uint16_t requester;
      uint16_t new_address;  
    }addrResponse;


     //Find another radio, starting with level 0 multicast  
    uint16_t contactNode;
	
    // Send the multicast broadcast to find an available contact node
    if( !findNodes(header,level,&contactNode) ) { 
		#ifdef MESH_DEBUG_SERIAL
		  Serial.print( millis() ); Serial.println(" FNds FAIL");
		#elif defined MESH_DEBUG_PRINTF
		  printf("%u FNds FAIL\n",millis() );
		#endif
		return 0;
    }
	
    // Request an address via the contact node
    header.type = NETWORK_REQ_ADDRESS;
    header.reserved = getNodeID();
    header.to_node = header.from_node;    
    
    // Do a direct write (no ack) to the contact node. Include the nodeId and address.
    network.write(header,&mesh_address,sizeof(addrResponse),contactNode);
    #ifdef MESH_DEBUG_SERIAL
	  Serial.print( millis() ); Serial.println(" MSH: Request address ");
	#elif defined MESH_DEBUG_PRINTF
	  printf("%u  MSH: Request address this node: 0%o\n",millis(),mesh_address);
	#endif
	
    if( !waitForAvailable(500UL) ){
		#ifdef MESH_DEBUG_SERIAL
          Serial.print( millis() ); Serial.print(" MSH: No address response from level "); Serial.println( level );
		#elif defined MESH_DEBUG_PRINTF
		  printf("%u MSH: No address response from level %d\n",millis(),level );
		#endif
	  return 0;
    }
    
    while(network.available() ){        
        network.peek(header);   
        uint8_t mask = 7;		
        switch(header.type){
          case NETWORK_ADDR_RESPONSE: 
		       
               network.read(header,&addrResponse,sizeof(addrResponse));
			   
			   if(!addrResponse.new_address || header.reserved != getNodeID() || !network.is_valid_address(addrResponse.new_address)){
                 #ifdef MESH_DEBUG_SERIAL
				   Serial.print(millis()); Serial.println(" MSH: Response discarded, wrong node");
				 #elif defined MESH_DEBUG_PRINTF
				   printf("%u Response discarded, wrong node 0%o from node 0%o sending node 0%o\n",millis(),addrResponse.new_address,header.from_node,addrResponse.requester);
                 #endif
				 return 0;
			   }
			   //mesh_address = 0;
			   mesh_address = addrResponse.new_address;
			   #ifdef MESH_DEBUG_SERIAL
				 Serial.print("Set address: 0");				    
					while(addrResponse.new_address){
						Serial.print((addrResponse.new_address & mask) );
						addrResponse.new_address >>= 3; //get the individual Octal numbers, specified in chunks of 3 bits
					}
					Serial.println("");
			   #elif defined (MESH_DEBUG_PRINTF)
				 printf("Set address 0%o rcvd 0%01o\n",mesh_address,addrResponse.new_address);
			   #endif
				 network.begin(90,mesh_address);
               return 1;
               break; 
          default: 
				   #ifdef MESH_DEBUG_SERIAL
					 Serial.print( millis() ); Serial.print(" MSH: Expect addr resp,got "); Serial.println(header.type);
                   #elif defined MESH_DEBUG_PRINTF
				     printf("%u MSH: Expect addr resp,got %d from 0%o\n", millis(), header.type, header.from_node);
				   #endif
		           network.read(header,0,0);
		           delay(5); 
		           break;
        }
		//network.update();
    }
    return 0;
  
}


bool RF24Mesh::waitForAvailable(uint32_t timeout){
  
    unsigned long timer = millis();
    while(millis()-timer < timeout){
      network.update();
	}
    if(network.available()){ return 1; }
	else{  return 0; }
}




#if defined (__AVR__) && !defined (RF24_TINY)

uint8_t RF24Mesh::getNodeID(){

  if(EEPROM.read(509) == 'R' && EEPROM.read(510) == 'F'){
    return EEPROM.read(511);  
  }
  return 0;
}

void RF24Mesh::setNodeID(uint8_t nodeID){
  
  if(millis() - lastSaveTime < MESH_MIN_SAVE_TIME && millis() >= MESH_MIN_SAVE_TIME){	
	#ifdef MESH_DEBUG_SERIAL
	Serial.print( millis() ); Serial.println(" MSH: Can't change nodeID that fast"); 
	#endif	
	return;
  }
  lastSaveTime=millis();
  
  if(EEPROM.read(509) != 'R' || EEPROM.read(510) != 'F' || EEPROM.read(511) != nodeID){
    EEPROM.write(509,'R');  EEPROM.write(510,'F'); EEPROM.write(511, nodeID);  
    #ifdef MESH_DEBUG_SERIAL
	Serial.print( millis() ); Serial.println(" MSH: Wrote data to EEPROM");
	#endif  
  }else{    
	#ifdef MESH_DEBUG_SERIAL
	Serial.print( millis() ); Serial.println(" MSH: Data already stored in EEPROM");
	#endif  
  }
}

#endif


#if defined (ARDUINO_SAM_DUE) || defined(RF24_TINY) || defined (__linux)
uint8_t RF24Mesh::getNodeID(){
	return _nodeID;
}
void RF24Mesh::setNodeID(uint8_t nodeID){
	_nodeID = nodeID;
}
#endif


#if defined (ARDUINO_SAM_DUE) || defined (__linux)

void RF24Mesh::DHCP(){
  
  if(network.available()){
    RF24NetworkHeader header;
    network.peek(header);
    
    // Check for an address request payload
    if(header.type == NETWORK_REQ_ADDRESS){
      
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
        network.read(header,0,0);
	    return;
	  }
     
      network.read(header,&addrResponse,sizeof(addrResponse));    
     
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
	     printf("%u MSH: Rcv addr req from_id %d \n",millis(),from_id);
	   #endif
	   
       std::map<char,uint16_t>::iterator it;

       //Clear any values with this ID
       it=addrMap.find(from_id);
       if(it != addrMap.end()){  addrMap.erase(it); } //IF_MESH_DEBUG(printf("addr erased\n");); }
       
       for(uint16_t i=1; i<MESH_MAX_CHILDREN+1; i++){ // For each of the possible addresses (5 max)
         
        bool found = 0;
        addrResponse.new_address = fwd_by | (i << shiftVal);
		if(!network.is_valid_address(addrResponse.new_address)){ continue; }
        //Search through all assigned/stored addresses
        for (std::map<char,uint16_t>::iterator it=addrMap.begin(); it!=addrMap.end(); ++it){                  
          if( it->second == addrResponse.new_address ){ //address found in use
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
			network.write(header,&addrResponse,sizeof(addrResponse));
          }else{
		    delay(5);
            network.write(header,&addrResponse,sizeof(addrResponse),header.to_node);
          }
          
		  addrMap[from_id] = addrResponse.new_address;
          
		  #ifdef MESH_DEBUG_PRINTF
		    printf("Sent to 0%o phys: 0%o new: 0%o \n", header.to_node,addrResponse.requester,addrResponse.new_address);
          #endif
		  break;
        }else{
		  printf("not found\n");
		}
      }

   }
    
  }

}

#endif
