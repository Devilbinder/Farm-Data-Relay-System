
#include "fdrs_sensor.h"


const uint8_t prefix[] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE}; // Should only be changed if implementing multiple FDRS systems.

uint8_t LoRaAddress[] = {0x42, 0x00};

uint32_t wait_time = 0;


FDRSBase::FDRSBase(): 
                _espnow_size(250 / sizeof(DataReading)),
                _data_count(0)
{
    fdrsData = new DataReading[_espnow_size];
}

FDRSBase::~FDRSBase(){
    delete[] fdrsData;
}

void FDRSBase::begin(uint8_t gtwy_mac,uint8_t reading_id) {

  _gtwy_mac = gtwy_mac;
  _reading_id = reading_id; 

#ifdef DEBUG
    Serial.begin(115200);
#endif
    DBG("FDRS Sensor ID " + String(_reading_id) + " initializing...");
    DBG(" Gateway: " + String (_gtwy_mac, HEX));
#ifdef POWER_CTRL
    DBG("Powering up the sensor array!");
    pinMode(POWER_CTRL, OUTPUT);
    digitalWrite(POWER_CTRL, 1);
#endif

    init();


}

void FDRSBase::send(void) {
  DBG("Sending FDRS Packet!");

  if(_data_count == 0){
        return;
  }
  transmit(fdrsData,_data_count);
  _data_count = 0;
}

void FDRSBase::load(float data, uint8_t type) {
  DBG("Data loaded. Type: " + String(t));
  if (_data_count > _espnow_size){
    send();
  } 
  DataReading dr;
  dr.id = _reading_id;
  dr.type = type;
  dr.data = data;
  fdrsData[_data_count] = dr;
  _data_count++;
}

void FDRSBase::sleep(int seconds){
  DBG("Sleepytime!");
#ifdef DEEP_SLEEP
  DBG(" Deep sleeping.");
#ifdef ESP32
  esp_sleep_enable_timer_wakeup(sleep_time * 1000000);
  esp_deep_sleep_start();
#endif
#ifdef ESP8266
  ESP.deepSleep(sleep_time * 1000000);
#endif
#endif
  DBG(" Delaying.");
  delay(seconds * 1000);
}


FDRS_EspNow::FDRS_EspNow(): 
                FDRSBase()
{

}


void FDRS_EspNow::init(void){
  
  // Init ESP-NOW for either ESP8266 or ESP32 and set MAC address
  
    memcpy(_gatewayAddress,MAC_PREFIX,5);
    _gatewayAddress[5] = _gtwy_mac;

    DBG("Initializing ESP-NOW!");
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
#if defined(ESP8266)
    if (esp_now_init() != 0) {
        return;
    }
    esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
    // Register peers
    esp_now_add_peer(_gatewayAddress, ESP_NOW_ROLE_COMBO, 0, NULL, 0);
#elif defined(ESP32)
    if(esp_now_init() != ESP_OK){
        DBG("Error initializing ESP-NOW");
        return;

    }
    esp_now_peer_info_t peerInfo;
    peerInfo.ifidx = WIFI_IF_STA;
    peerInfo.channel = 0;
    peerInfo.encrypt = false;
  // Register first peer
    memcpy(peerInfo.peer_addr, _gatewayAddress, 6);
    if(esp_now_add_peer(&peerInfo) != ESP_OK) {
        DBG("Failed to add peer");
        return;
    }
#endif
  DBG(" ESP-NOW Initialized.");

}

void FDRS_EspNow::transmit(DataReading *fdrsData, uint8_t _data_count){
    esp_now_send(_gatewayAddress, (uint8_t *) fdrsData, _data_count * sizeof(DataReading));
    delay(5);
    DBG(" ESP-NOW sent.");
}

#ifdef USE_LORA

FDRSLoRa::FDRSLoRa(uint8_t miso,uint8_t mosi,uint8_t sck,
                    uint8_t ss,uint8_t rst,uint8_t dio0,uint32_t band,uint8_t sf): 
                FDRSBase(),
                     _miso(miso),
                     _mosi(mosi),
                     _sck(sck),
                     _ss(ss),
                     _rst(rst),
                     _dio0(dio0),
                    _band(band),
                     _sf(sf)
{

}

void FDRSLoRa::init(void){
    _gatewayAddress[0] = prefix[3];
    _gatewayAddress[1] = prefix[4];
    _gatewayAddress[2] = _gtwy_mac;

    DBG("Initializing LoRa!");
    DBG(_band);
    DBG(_sf);
#ifndef __AVR__
  SPI.begin(_sck, _miso, _mosi, _ss);
#endif
    LoRa.setPins(_ss, _rst, _dio0);
    if (!LoRa.begin(_band)) {
        DBG("LoRa Initialize Failed.");
        while (1);
    }
    LoRa.setSpreadingFactor(_sf);
    DBG("LoRa Initialized.");
}

void FDRSLoRa::buildPacket(uint8_t* mac, DataReading * packet, uint8_t len) {
  uint8_t pkt[5 + (len * sizeof(DataReading))];
  memcpy(&pkt, mac, 3);  //
  memcpy(&pkt[3], &LoRaAddress, 2);
  memcpy(&pkt[5], packet, len * sizeof(DataReading));
  LoRa.beginPacket();
  LoRa.write((uint8_t*)&pkt, sizeof(pkt));
  LoRa.endPacket();
}

void FDRSLoRa::transmit(DataReading *fdrsData, uint8_t _data_count){
    buildPacket(_gatewayAddress, fdrsData, _data_count);
    DBG(" LoRa sent.");
}

#endif
