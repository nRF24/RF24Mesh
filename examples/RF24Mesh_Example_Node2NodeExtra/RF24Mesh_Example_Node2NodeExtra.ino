
 /** RF24Mesh_Example_Node2Node.ino by TMRh20
  * 
  * This example sketch shows how to communicate between two (non-master) nodes using
  * RF24Mesh & RF24Network
  */


#include "RF24.h"
#include "RF24Network.h"
#include "RF24Mesh.h"
#include <SPI.h>
//#include <printf.h>


/**** Configure the nrf24l01 CE and CS pins ****/
RF24 radio(7,8);
RF24Network network(radio);
RF24Mesh mesh(radio,network);

/** 
 * User Configuration: 
 * nodeID - A unique identifier for each radio. Allows addressing to change dynamically
 * with physical changes to the mesh. (numbers 1-255 allowed)
 * 
 * otherNodeID - A unique identifier for the 'other' radio
 * 
 **/
#define nodeID 1
#define otherNodeID 2    


uint32_t millisTimer=0;
uint32_t stringTimer=0;
char dataStr[] = {"abcdefghigklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890"};
char tmpStr[sizeof(dataStr)+1];
uint8_t strCtr=1;

void setup() {

  Serial.begin(115200);
  //printf_begin();
  // Set the nodeID manually
  mesh.setNodeID(nodeID);
  // Connect to the mesh
  Serial.println(F("Connecting to the mesh..."));
  mesh.begin(7);  
}


void loop() {
  
  mesh.update();

if(network.available()){
      RF24NetworkHeader hdr;
      size_t dataSize = network.peek(hdr);
      Serial.print("Size Rcv ");
      Serial.print(dataSize);
      Serial.print(": "); 
      
      if(hdr.type == 'S'){      
          char datt[dataSize];
          network.read(hdr,&datt,dataSize);
          Serial.println(datt);
      }else
      if(hdr.type == 'M'){
        uint32_t mills;
        network.read(hdr,&mills,sizeof(mills));
        Serial.print("Rcv ");
        Serial.print(mills);
        Serial.print(" from nodeID ");
        int _ID = mesh.getNodeID(hdr.from_node);
        if( _ID > 0){
           Serial.println(_ID);
        }else{
           Serial.println("Mesh ID Lookup Failed"); 
        }
      }
  }
  
  
  // Send to the master node every second
  if(millis() - millisTimer >= 1333){
    millisTimer = millis();
    
    // Send an 'M' type to other Node containing the current millis()
    if(!mesh.write(&millisTimer,'M',sizeof(millisTimer),otherNodeID)){
            Serial.println("Send fail");
            if( ! mesh.checkConnection() ){
              Serial.println("Renewing Address");
              mesh.renewAddress(); 
            }else{
              Serial.println("Send fail, Test OK"); 
            }
    }else{
            Serial.print("Send OK: "); Serial.println(millisTimer);
    }
  }
    if(millis() - stringTimer >= 1300){
      stringTimer=millis();
      //Copy the current number of characters to the temporary array
      memcpy(tmpStr,dataStr,strCtr);
      //Set the last character to NULL
      tmpStr[strCtr] = '\0';
    
      // Send the temp string as an 'S' type message
      // Send it to otherNodeID (An RF24Mesh address lookup will be performed)
      if(mesh.write(tmpStr,'S',strCtr+1,otherNodeID)){
        strCtr++;
        //Set the sending length back to 1 once max size is reached
        if(strCtr == sizeof(dataStr)+1){ strCtr=1; }
      }
    
    }

}






