#include <RHReliableDatagram.h>
#include <RH_NRF24.h>
#include <SPI.h>

#define nrf1 1
#define nrf2 2
#define nrf3 3


RH_NRF24 driver;

RHReliableDatagram manager(driver, nrf3);

void setup() {
  // put your setup code here, to run once:
Serial.begin(38400);
  if (!manager.init())
    Serial.println("init failed");
}

int l,m,o;
int a=0;
int finalmsg[21];
int i=0;
int recvmsg1[24];
int recvmsg[21];
uint8_t buf1[RH_NRF24_MAX_MESSAGE_LEN];

void loop() {
  // put your main code here, to run repeatedly:

if (manager.available())
  {
    
    uint8_t len1 = sizeof(buf1);
    uint8_t from1;
    if (manager.recvfromAck(buf1, &len1,&from1))
    {
      Serial.println(" ");
  
l=(int)buf1[0];
m=(int)buf1[1];
o=(int)buf1[2];

 /*Serial.println(l);
 Serial.println(m);
 Serial.println(n);
 delay(1000);
 */
  
  int remainder=0,quotient=0;
  int binaryNumber1[8]={0,0,0,0,0,0,0,0};
  i=0;
  quotient = l;
  while(quotient!=0){
     binaryNumber1[i++]= quotient % 2;
     quotient = quotient / 2;
     
    }
  if(i<7)
  {
    while(i<=7)
    {
      binaryNumber1[i]=0;
      i++;
  }}

  
  remainder=0;
  quotient=0;
  int binaryNumber2[8]={0,0,0,0,0,0,0,0};
  i=0;
  quotient = m;
  while(quotient!=0){
     binaryNumber2[i++]= quotient % 2;
     quotient = quotient / 2;
    }
      if(i<7)
  {
    while(i<=7)
    {
      binaryNumber2[i]=0;
      i++;
  }}

  
  remainder=0;
  quotient=0;
  int binaryNumber3[8]={0,0,0,0,0,0,0,0};
  i=0;
  quotient = o;
  while(quotient!=0){
     binaryNumber3[i++]= quotient % 2;
     quotient = quotient / 2;
    }
    if(i<7)
  {
    while(i<=7)
    {
      binaryNumber3[i]=0;
      i++;
  }}

  
  for(i=0;i<=7;i++)
 {   recvmsg1[i]=binaryNumber1[7-i];
    recvmsg1[i+8]=binaryNumber2[7-i];
    recvmsg1[i+16]=binaryNumber3[7-i];
 }
 for(i=3;i<=23;i++)
  recvmsg[i-3]=recvmsg1[i];
  


if((recvmsg[0]==1)&& (recvmsg[20]==1))
  a=3;
if((recvmsg[0]==0)&& (recvmsg[20]==1))
  a=1;
if((recvmsg[0]==1)&& (recvmsg[20]==0))
  a=2;
if((recvmsg[0]==0)&& (recvmsg[20]==0))
  a=0;


switch(a)
{
  case 0:
          finalmsg[0]=0;
          finalmsg[20]=0;
          finalmsg[1]=recvmsg[1];
          finalmsg[2]=recvmsg[7];
          finalmsg[18]=recvmsg[19];
          finalmsg[19]=recvmsg[18];
          if((finalmsg[1]!=1)||(finalmsg[2]!=0)||(finalmsg[18]!=1)||(finalmsg[19]!=1))
            {
              Serial.println("Unknown sender");
              break;
            }
          finalmsg[5]=recvmsg[2];
          finalmsg[6]=recvmsg[3];
          finalmsg[9]=recvmsg[5];
          finalmsg[4]=recvmsg[6];
          finalmsg[3]=recvmsg[8];
          finalmsg[7]=recvmsg[9];
          finalmsg[15]=recvmsg[10];
          finalmsg[8]=recvmsg[11];
          finalmsg[12]=recvmsg[12];
          finalmsg[10]=recvmsg[13];
          finalmsg[13]=recvmsg[14];
          finalmsg[11]=recvmsg[16];
          finalmsg[14]=recvmsg[17];
          finalmsg[16]=recvmsg[4];
          finalmsg[17]=recvmsg[15];
          break;


    case 1:
          finalmsg[0]=0;
          finalmsg[20]=1;

          finalmsg[1]=recvmsg[7];
          finalmsg[2]=recvmsg[12];
          finalmsg[19]=recvmsg[18];
          finalmsg[18]=recvmsg[19];
          
          if((finalmsg[1]!=1)||(finalmsg[2]!=0)||(finalmsg[18]!=1)||(finalmsg[19]!=1))
            {
              Serial.println("Unknown sender");
              break;
            }
           finalmsg[16]=recvmsg[1];
          finalmsg[17]=recvmsg[3];
          finalmsg[11]=recvmsg[2];
          finalmsg[4]=recvmsg[4];
          finalmsg[8]=recvmsg[5];
          finalmsg[7]=recvmsg[6];
          finalmsg[15]=recvmsg[17];
          finalmsg[10]=recvmsg[8];
          finalmsg[12]=recvmsg[9];
          finalmsg[6]=recvmsg[10];
          finalmsg[13]=recvmsg[11];
          finalmsg[9]=recvmsg[13];
          finalmsg[3]=recvmsg[14];
          finalmsg[5]=recvmsg[15];
          finalmsg[14]=recvmsg[16];
          
          break;

    case 2:
          finalmsg[0]=1;
          finalmsg[20]=0;

          finalmsg[1]=recvmsg[10];
          finalmsg[2]=recvmsg[2];
          finalmsg[18]=recvmsg[18];
          finalmsg[19]=recvmsg[19];
          if((finalmsg[1]!=1)||(finalmsg[2]!=0)||(finalmsg[18]!=1)||(finalmsg[19]!=1))
            {
              Serial.println("Unknown sender");
              break;
            }
          
          finalmsg[10]=recvmsg[1];
          finalmsg[7]=recvmsg[3];
          finalmsg[15]=recvmsg[4];
          finalmsg[9]=recvmsg[5];
          finalmsg[4]=recvmsg[6];
          finalmsg[8]=recvmsg[8];       
          finalmsg[3]=recvmsg[11];
          finalmsg[12]=recvmsg[12];
          finalmsg[14]=recvmsg[13];
          finalmsg[3]=recvmsg[14];
          finalmsg[5]=recvmsg[15];
          finalmsg[11]=recvmsg[16];
          finalmsg[6]=recvmsg[17];
          finalmsg[16]=recvmsg[9];
          finalmsg[17]=recvmsg[7];
          break;



    case 3:
          finalmsg[0]=1;
          finalmsg[20]=1;
          finalmsg[1]=recvmsg[3];
          finalmsg[2]=recvmsg[10];
          finalmsg[18]=recvmsg[18];
          finalmsg[19]=recvmsg[19];
          if((finalmsg[1]!=1)||(finalmsg[2]!=0)||(finalmsg[18]!=1)||(finalmsg[19]!=1))
            {
              Serial.println("Unknown sender");
              break;
            }
          finalmsg[7]=recvmsg[1];
          finalmsg[9]=recvmsg[2];
          finalmsg[5]=recvmsg[4];     
          finalmsg[4]=recvmsg[6];
          finalmsg[15]=recvmsg[7];
          finalmsg[11]=recvmsg[8];
          finalmsg[6]=recvmsg[9];
          
          
          finalmsg[13]=recvmsg[12];
          finalmsg[14]=recvmsg[13];
          finalmsg[3]=recvmsg[14];
          finalmsg[8]=recvmsg[15];
          finalmsg[10]=recvmsg[16];
          finalmsg[12]=recvmsg[17];
          finalmsg[16]=recvmsg[11];
          finalmsg[17]=recvmsg[5];
          break;
}

if((finalmsg[1]==1)&&(finalmsg[2]==0)&&(finalmsg[18]==1)&&(finalmsg[19]==1))
{
  Serial.print("actual signal=");
  for(i=0;i<=20;i++)
    Serial.print(finalmsg[i]);
}


}}

}
