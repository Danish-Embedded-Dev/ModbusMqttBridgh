
#include <SPI.h>
SPIClass SPI(2);
#include <Ethernet_STM.h>
#include <PubSubClient.h>
#include "conf.h"


//Class instants
EthernetClient ethClient;
PubSubClient mqttclient(ethClient);

String Pub_Topic = "VECTRA_MODBUS";
String Sub_Topic = "VECTRA_MODBUS_DEV";
//String Sub_Topic = "modbus";

// Use assigned MAC address of WIZ550io
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};

int mqtt_port = 1883; //default port addres
String  mqtt_server = "192.168.120.164";

long lastReconnectAttempt = 0;

void setup() {
  //------------------------debug
  ModSerial.begin(9600); //using A9,A10 for Tx,Rx
  DEBUG_BEGIN(9600);  //Serial monitor
  while(!Serial){};

  //------------------------ethernet
  DEBUG_PRINTLN("Initialize Ethernet with DHCP:");
  //while (Ethernet.begin(mac) != 1);
  if (Ethernet.begin(mac) == 0) {
    DEBUG_PRINTLN("Failed to configuration");
  }

  // print your local IP address:
  DEBUG_PRINT("My IP address: ");
  DEBUG_PRINTLN(Ethernet.localIP());

  //------------------------mqtt
  mqttclient.setServer(mqtt_server.c_str(), mqtt_port);
  mqttclient.setCallback(callback);


}

void loop() {
  // reconnection
  if (!mqttclient.connected()) {
    long now = millis();
    if (now - lastReconnectAttempt > 5000) {
      lastReconnectAttempt = now;
      // Attempt to reconnect
      if (reconnect()) {
        lastReconnectAttempt = 0;
      }
    }
  } else {
    // mqttclient connected
    mqttclient.loop();
  }

  //recieving data from modbus device 
  if(ModSerial.available()>0){ 
    byte max_length = 200;
    byte Rx_data[max_length];
    serial_capture(Rx_data,&max_length); 
    mqttclient.publish(Pub_Topic.c_str(),Rx_data,max_length); 
 
  }
}

void serial_capture(byte *message,byte *max_lenght){ 
 byte message_pos = 0;
 while (ModSerial.available() > 0){ 
   //Read the next available byte in the serial receive buffer
   byte inByte = ModSerial.read();
   
   //Add the incoming byte to our message
   message[message_pos] = inByte;
   message_pos++; 
   delay(2);  //wait for UART to be filled 
 }
 *max_lenght = message_pos;
} 
