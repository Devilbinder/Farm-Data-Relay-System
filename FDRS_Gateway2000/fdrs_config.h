//  FARM DATA RELAY SYSTEM
//
//  GATEWAY 2.000 Configuration

//#include <fdrs_globals.h> //Uncomment if you install the globals file
#define DEBUG     //Enable USB-Serial debugging

#define UNIT_MAC     0x01  // The address of this gateway

//Actions -- Define what happens when a packet arrives at each interface:
// Current function options are: sendESPNOW(MAC), sendSerial(), sendMQTT(),
//  bufferLoRa(interface), bufferESPNOW(interface), bufferSerial(), and bufferMQTT().

#define ESPNOWG_ACT    sendSerial();
#define SERIAL_ACT         
#define MQTT_ACT          
#define LORAG_ACT      sendSerial();

//#define USE_LORA
//#define USE_WIFI      //Used only for MQTT gateway

// Peer addresses
#define ESPNOW1_PEER  0x0E  // ESPNOW1 Address 
#define ESPNOW2_PEER  0x0F  // ESPNOW2 Address
#define LORA1_PEER    0x0E  // LoRa1 Address
#define LORA2_PEER    0x0F  // LoRa2 Address

// Peer Actions
#define ESPNOW1_ACT    
#define ESPNOW2_ACT                    
#define LORA1_ACT 
#define LORA2_ACT 

//Pins for UART data interface (ESP32 only)
#define RXD2 14
#define TXD2 15

//LoRa Configuration -- Needed only if using LoRa
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 14
#define DIO0 26
//433E6 for Asia
//866E6 for Europe
//915E6 for North America
#define BAND 915E6
#define SF 7

// Buffer Delays - in milliseconds - Uncomment to enable any buffer

//#define ESPNOW1_DELAY  0
//#define ESPNOW2_DELAY  0
//#define ESPNOWG_DELAY  0
//#define SERIAL_DELAY   0
//#define MQTT_DELAY     0
//#define LORAG_DELAY    1000
//#define LORA1_DELAY    1000
//#define LORA2_DELAY    1000

//#define USE_LED    //Not yet fully implemented
#define LED_PIN    32
#define NUM_LEDS    4

//WiFi and MQTT Credentials -- Needed only for MQTT gateway
#define WIFI_SSID   "Your SSID"  
#define WIFI_PASS   "Your Password"
#define MQTT_ADDR   "192.168.0.8"
// MQTT Topics
#define TOPIC_DATA "fdrs/data"
#define TOPIC_STATUS "fdrs/status"
#define TOPIC_COMMAND "fdrs/command" 
