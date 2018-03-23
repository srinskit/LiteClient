//NRF 3 COM-5

#include <RHReliableDatagram.h>
#include <RH_NRF24.h>
#include <SPI.h>

#define nrf1 0000000001
#define nrf2 0000000010
#define nrf3 0000000011

RH_NRF24 driver;

RHReliableDatagram manager(driver, nrf3);

void setup() 
{
  Serial.begin(38400);
  if (!manager.init())
    Serial.println("init failed");
  // Defaults after init are 2.402 GHz (channel 2), 2Mbps, 0dBm
}


uint8_t buf[RH_NRF24_MAX_MESSAGE_LEN];

void loop()
{
  Serial.println(driver.statusRead());
  if (manager.available())
  {
    
    uint8_t len = sizeof(buf);
    uint8_t from;
    if (manager.recvfromAck(buf, &len, &from))
    {
      Serial.println((char*)buf);
    }
    if (manager.sendtoWait(buf, sizeof(buf), nrf1))
        Serial.println("Sent to 2nd");
  }
}

