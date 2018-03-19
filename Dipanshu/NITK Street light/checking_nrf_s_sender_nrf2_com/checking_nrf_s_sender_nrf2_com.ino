//NRF 2 COM-4


#include <RHReliableDatagram.h>
#include <RH_NRF24.h>
#include <SPI.h>

#define nrf1 0000000001
#define nrf2 0000000010
#define nrf3 0000000011

// Singleton instance of the radio driver
RH_NRF24 driver;
 //RH_NRF24 driver(8, 7);   // For RFM73 on Anarduino Mini

// Class to manage message delivery and receipt, using the driver declared above
RHReliableDatagram manager(driver, nrf2);
int i=0;
void setup() 
{
  Serial.begin(38400);
  if (!manager.init())
    Serial.println("init failed");
//  if (!driver.setRF(RH_NRF24::DataRate250kbps, RH_NRF24::TransmitPower0dBm))
//    Serial.println("setRF failed"); 
  // Defaults after init are 2.402 GHz (channel 2), 2Mbps, 0dBm
}

char data[5] = "12345";


void loop()
{

    Serial.println(driver.statusRead());
    if (manager.sendtoWait(data, sizeof(data), nrf1))
        Serial.println("Sent to 1st");    
  
  
  
//    delay(1000);
//    for(i=0;i<5;i++)
//    data[i]="0";


}

