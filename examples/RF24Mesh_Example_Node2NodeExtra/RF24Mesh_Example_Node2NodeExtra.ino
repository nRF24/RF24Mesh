
/** RF24Mesh_Example_Node2Node.ino by TMRh20
 *
 * This example sketch shows how to communicate between two (non-master) nodes using
 * RF24Mesh & RF24Network
 **/


#include "RF24.h"
#include "RF24Network.h"
#include "RF24Mesh.h"
#include <SPI.h>
//#include <printf.h>


//########### USER CONFIG ###########

/**** Configure the nrf24l01 CE and CS pins ****/
RF24 radio(7, 8);
RF24Network network(radio);
RF24Mesh mesh(radio, network);

/**
 * User Configuration:
 * nodeID - A unique identifier for each radio. Allows addressing to change dynamically
 * with physical changes to the mesh. (numbers 1-255 allowed)
 *
 * otherNodeID - A unique identifier for the 'other' radio
 *
 **/
#define nodeID 3
#define otherNodeID 2

//#################################

uint32_t millisTimer = 0;
uint32_t stringTimer = 0;
char dataStr[] = {"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890"};
char tmpStr[sizeof(dataStr) + 1];
uint8_t strCtr = 1;

uint32_t delayTime = 120;

void setup() {

  Serial.begin(115200);
  //printf_begin();
  // Set the nodeID manually
  mesh.setNodeID(nodeID);
  // Connect to the mesh
  Serial.println(F("Connecting to the mesh..."));
  mesh.begin();
}

unsigned int sizeCtr = 2;

uint32_t errorCount = 0;
uint32_t duplicates = 0;
uint32_t totalData = 0;

void loop() {

  mesh.update();

  while (network.available()) {
    RF24NetworkHeader hdr;
    size_t dataSize = network.peek(hdr);
    totalData += dataSize;

    if (hdr.type == 'S') {
      if (dataSize != sizeCtr) {
        if (dataSize == sizeCtr + 1) {
          duplicates++;
        }
        sizeCtr = dataSize + 1;
        errorCount++;
      } else {
        sizeCtr++;
        if (sizeCtr > sizeof(dataStr)) {
          sizeCtr = 2;
        }
        //if(sizeCtr > 12){ sizeCtr = 2; }
      }
      network.read(hdr, &tmpStr, dataSize);
      //Serial.println(tmpStr);
    } else if (hdr.type == 'M') {
      uint32_t mills;
      network.read(hdr, &mills, sizeof(mills));
      Serial.print(F("Received "));
      Serial.print(mills);
      int _ID = 0;
      _ID = mesh.getNodeID(hdr.from_node);
      if ( _ID > 0) {
        if (_ID == nodeID) {
          Serial.println(F(" from master."));
        } else {
          Serial.print(F(" from node(ID) "));
          Serial.print(_ID);
          Serial.println('.');
        }
      } else {
        Serial.println(F("Mesh ID Lookup Failed"));
      }
      Serial.print(F("Total Data Received: "));
      Serial.print(totalData);
      Serial.println(" bytes");
      Serial.print(F("Detected Errors in data received (Including Duplicates): "));
      Serial.println(errorCount);
      Serial.print(F("Duplicates: "));
      Serial.println(duplicates);
      Serial.println(F("-------------------------------------"));
    }
  }


  // Send to the master node every second
  if (millis() - millisTimer >= 1000 ) {
    millisTimer = millis();

    // Send an 'M' type to other Node containing the current millis()
    if (!mesh.write(&millisTimer, 'M', sizeof(millisTimer), otherNodeID)) {
      Serial.println(F("Send fail"));
      if ( ! mesh.checkConnection() ) {
        Serial.println(F("Renewing Address"));
        mesh.renewAddress();
      } else {
        Serial.println(F("Send fail, Test OK"));
      }
    } else {
      Serial.print(F("Send OK: ")); Serial.println(millisTimer);
    }
  }

  if (millis() - stringTimer >= delayTime ) {
    stringTimer = millis();
    //Copy the current number of characters to the temporary array
    memcpy(tmpStr, dataStr, strCtr);
    //Set the last character to NULL
    tmpStr[strCtr] = '\0';

    // Send the temp string as an 'S' type message
    // Send it to otherNodeID (An RF24Mesh address lookup will be performed)
    //bool ok = mesh.write(tmpStr,'S',strCtr+1,otherNodeID);
    if (mesh.write(tmpStr, 'S', strCtr + 1, otherNodeID)) {
      strCtr++;
      delayTime = 333;
      //Set the sending length back to 1 once max size is reached
      if (strCtr == sizeof(dataStr)) {
        strCtr = 1;
      }
      //if(strCtr == 12){ strCtr=1; }
    }
  }
}
