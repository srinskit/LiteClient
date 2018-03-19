#include <virtuabotixRTC.h>
#include <RHReliableDatagram.h>
#include <RH_NRF24.h>
#include <SPI.h>

#define nrf1 1
#define nrf2 2
#define nrf3 3

RH_NRF24 driver;

RHReliableDatagram manager(driver, nrf1);

// Creation of the Real Time Clock Object
//SCLK -> 6, I/O -> 7, CE -> 5
virtuabotixRTC myRTC(6, 7, 5);
int mnth, minute;
int hr=1;
int sc;
char msg = 'B';
int tflag = 0;
boolean toggle=0;

void send_message(char x)
  {
    //Serial.println("Hi");
    //char s[] = {x, '\0'};
    uint8_t s = x;
   if (manager.sendtoWait(&s, sizeof(s), nrf3))
    Serial.println("Sent to 3rd");
  else 
    Serial.println("3nd nrf not working");
  //delay(100);
  
  if (manager.sendtoWait(&s, sizeof(s), nrf2))
    Serial.println("Sent to 2nd");
  else 
    Serial.println("2nd nrf not working");
  
  switch(x)
    {
      case 'A': {
                  digitalWrite(3,HIGH);
                  break;
                }
      case 'B': {
                  digitalWrite(3,LOW);
                  break;
                }
      case 'C': {
                  digitalWrite(3,HIGH);
                  break;
                }
      case 'D': {
                  digitalWrite(3,LOW);
                  break;
                }
    }
    
  }
void setup() 
{
Serial.begin(38400);
  if (!manager.init())
    Serial.println("init failed");
pinMode(3,OUTPUT);

// Set the current date, and time in the following format:
// seconds, minutes, hours, day of the week, day of the month, month, year
myRTC.setDS1302Time(00, 00, hr, 6, 1, 4, 2017);
}

void loop() {
// This allows for the update of variables for time or accessing the individual elements.
//while(!(Serial.available()))

  myRTC.updateTime();

if(toggle == 1)
            {
              msg = 'C';
              //delay(200);
              //digitalWrite(3,HIGH);
            }
          if(toggle == 0)
            {
              msg = 'D';
              //delay(200);
              //digitalWrite(3,LOW);
            }
          
if((hr == 1) || (hr == 2))
         {
             msg = 'B';
             //delay(200);
             //digitalWrite(3,LOW);
         }
if((hr == 3) || (hr == 4))
         {
             msg = 'A';
             //delay(200);
             //digitalWrite(3,HIGH);
         }
         
// Start printing elements as individuals
//Serial.print("Current Time: ");
//Serial.print(myRTC.dayofmonth);
//Serial.print("/");
//mnth = myRTC.month;
//Serial.print(mnth);
//Serial.print("/");
//Serial.print(myRTC.year);
//Serial.print(" ");
//Serial.print(myRTC.hours);
//Serial.print(":");
//minute = myRTC.minutes;
//Serial.print(minute);
//Serial.print(":");

sc = myRTC.seconds;
//Serial.print(sc);
//Serial.print(":");
//Serial.print(toggle);
//Serial.print(":");
//Serial.println(msg);

//delay(1000);  

//if(Serial.read()=='A')
if(sc == 5)
  {
    hr++;
    if(hr == 5)
     {
       hr = 0;
       tflag = 0;
     } 
    myRTC.setDS1302Time(00, 00, hr, 6, 1, 4, 2017);
    send_message(msg);
  }
      if(hr==0)
          {
            if(tflag == 0)
              {
                toggle = !(toggle);
                tflag++;
              }
          }
          
        
         

// Delay so the program doesn't print non-stop
//delay(1000);

//myRTC.setDS1302Time(00, 17, 12, 6, 1, 4, 2017);
}
