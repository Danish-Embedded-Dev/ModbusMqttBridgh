

boolean reconnect() {
  if (mqttclient.connect("Extensity_Module")) {
     DEBUG_PRINTLN("connected");
    // Once connected, publish an announcement...
    mqttclient.publish(Pub_Topic.c_str(), "MODBUS_BRIDGE_CONNECTED");
    // ... and resubscribe
    mqttclient.subscribe(Sub_Topic.c_str());;
  }
  return mqttclient.connected();
}

const char* get_cmd = "?";
const char* set_cmd = "set";

void callback(char* topic, byte* payload, unsigned int length) {
   DEBUG_PRINT("Message arrived [");
   DEBUG_PRINT(topic);
   DEBUG_PRINTLN("] ");

   
  byte message[length]; //addition 2 byte is for CRC checksum


//capuring payload data  byte capture and transfer 
  for (int i = 0; i < length; i++) {
    message[i] = (byte)payload[i];
     DEBUG_PRINT(message[i], HEX);  DEBUG_PRINT(" ");
  } 
  DEBUG_PRINTLN();
  
  ModSerial.write(message, length); //sending bytes to modbus serially
  delay(1);                       // 

}
