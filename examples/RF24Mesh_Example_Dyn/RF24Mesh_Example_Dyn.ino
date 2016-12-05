 #include "RF24.h"
#include "RF24Network.h"
#include "RF24Mesh.h"
#include <SPI.h>
#include <EEPROM.h>

/**** Configure the nrf24l01 CE and CS pins ****/
RF24 radio(9,10);
RF24Network network(radio);
RF24Mesh mesh(radio,network);
#define Default_nodeID 255
#define debug 0
uint32_t displayTimer=0;

void setup() {
  
  Serial.begin(115200);
  if (debug || mesh.getNodeID() == (uint8_t)Default_nodeID || mesh.getNodeID() == 0){ 
    clean_EEPROM();
    mesh.setNodeID(Default_nodeID);
    authenticate();
  }else{
    Serial.print(F("NodeID "));
    Serial.println(mesh.getNodeID());
    Serial.println(F("Connecting to the mesh..."));
    mesh.setNodeID(mesh.getNodeID());
    mesh.begin();  
  }  
}

void loop() {
  
  mesh.update();

  // Send to the master node every second
  if(millis() - displayTimer >= 1000){
    displayTimer = millis();    
    
    // Send an 'M' type message containing the current millis()
    if(!mesh.write(&displayTimer,'M',sizeof(displayTimer))){
       
      // If a write fails, check connectivity to the mesh network
      if( ! mesh.checkConnection() ){
        //refresh the network address
        Serial.println("Renewing Address");
        mesh.renewAddress(); 
      }else{
        Serial.println("Send fail, Test OK"); 
      }
    }else{
      Serial.print("Send OK: "); Serial.println(displayTimer);
    }
  }

}

void clean_EEPROM(){
  EEPROM.write(509, 0);
  EEPROM.write(510, 0);
  EEPROM.write(511, 0);
  Serial.println("CLEARED");
}

void authenticate(){
  Serial.println("connecting...");
  mesh.begin();
  uint8_t d = 1;
  bool er = true;
  while(er){
    mesh.update();
    Serial.print("Sendin ");
    if(mesh.write(&d,'a',sizeof(d))){
       Serial.println("OK");
       Serial.println("adress request send");
       er = false;
    }else{
      Serial.println("FAILED");
      delay(1000);
    }
  }   
  Serial.println("Waiting new address");
  er = true;
  
  while(er){
  mesh.update();
  if( network.available() ){
    RF24NetworkHeader header;
    network.peek(header);
    
    uint8_t dat=0;
    Serial.print("header type: ");
    Serial.println(header.type);
    switch(header.type){
      // Display the incoming millis() values from the sensor nodes
      case 'A': network.read(header,&dat,sizeof(uint8_t));
        Serial.println(dat); 
        mesh.setNodeID(dat);
        break;
      default: Serial.println("NO ADDRESS");
                break;
    }
    er = false;
  }
  }
  Serial.println("address aquired");
  Serial.println("Restart the Machine");
  //set pin that will go high
  exit(0);
}


