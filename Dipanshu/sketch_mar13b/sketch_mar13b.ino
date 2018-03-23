#include <SPI.h>
#include<Wire.h>
#include<RealTimeClockDS1307.h>
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
RTC.setHours(23);
      RTC.setMinutes(8);
      RTC.setClock();
}

void loop() {
  // put your main code here, to run repeatedly:

      RTC.readClock();
      Serial.print(RTC.getHours());
      Serial.print(":");
      Serial.print(RTC.getMinutes());
      Serial.print(":");
      Serial.print(RTC.getSeconds());
      Serial.println(" ");
      delay(1000);
}
