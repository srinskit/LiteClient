
#include <Wire.h>
#include <RealTimeClockDS1307.h>
char formatted[] = "00-00-00 00:00:00x";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
      RTC.sqwEnable(RTC.SQW_4kHz);

      RTC.start();

    //RTC.readClock();

    RTC.setHours(5);
     RTC.setClock();
    RTC.setMinutes(35);
     RTC.setClock();
RTC.setSeconds(14);
 RTC.setClock();
RTC.setDate(9);
 RTC.setClock();
RTC.setMonth(6);
 RTC.setClock();
RTC.setYear(14);
 RTC.setClock();
RTC.set24h();
        RTC.setClock();


}
      int x,y,z;

void loop() {
  // put your main code here, to run repeatedly:
    RTC.readClock();
      x = RTC.getHours();
      y= RTC.getMinutes();
      z = RTC.getSeconds();
    //RTC.getFormatted(formatted);
    Serial.println(x);
    Serial.println(y);
    Serial.println(z);
    Serial.println();
    x=y=z=0;
          //  RTC.setClock();

    delay(500);
}
