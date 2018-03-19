//NRF 1 COM-3

#include <RHReliableDatagram.h>
#include <RH_NRF24.h>
#include <SPI.h>

#define nrf1 0000000001
#define nrf2 0000000010
#define nrf3 0000000011

RH_NRF24 driver;

RHReliableDatagram manager(driver, nrf1);

void setup() 
{
  Serial.begin(38400);
  if (!manager.init())
    Serial.println("init failed");
//  if (!driver.setRF(RH_NRF24::DataRate250kbps, RH_NRF24::TransmitPowerm18dBm))
//    Serial.println("setRF failed"); 
  // Defaults after init are 2.402 GHz (channel 2), 2Mbps, 0dBm
}


uint8_t buf[RH_NRF24_MAX_MESSAGE_LEN];

void loop()
{
  
  
//    else
//    {
  uint8_t stat=driver .statusRead();
  Serial.println(stat);
  if (manager.available())
  {
    
//    Serial.println("here");
    uint8_t len = sizeof(buf);
    uint8_t from;
    if (manager.recvfromAckTimeout(buf, &len, 2000,&from))
    {
      Serial.println((char*)buf);
    }
  } 
  
//  else if (!manager.init())
//    Serial.println("init failed");
//
//  Serial.println("here1");
}
//}

