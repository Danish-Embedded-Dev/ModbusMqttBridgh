#ifndef conf

//for debugging using USB serial 
#define DEBUG_BEGIN(...)    Serial.begin(__VA_ARGS__)
#define DEBUG_PRINT(...)    Serial.print(__VA_ARGS__)
#define DEBUG_PRINTLN(...)  Serial.println(__VA_ARGS__)

//for Modbus communication using Serial2
#define ModSerial Serial2  

#endif
