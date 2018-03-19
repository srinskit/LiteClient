/*0--[1,5,6,16,9,4,2,3,7,15,8,12,10,13,17,18,11,14,19,18]
 * 1--[16,11,17,4,8,7,1,10,12,6,13,2,9,3,5,14,15,19,18]
 * 2--[10,2,7,15,9,4,17,8,16,1,3,12,14,3,5,11,6,18,19]
 * 3--[7,9,1,5,17,4,15,11,6,2,16,13,14,3,8,10,12,18,19]
 * 
 */


#include <RHReliableDatagram.h>
#include <RH_NRF24.h>
#include <SPI.h>

#define nrf1 1
#define nrf2 2
#define nrf3 3

RH_NRF24 driver;

RHReliableDatagram manager(driver, nrf1);


int message[15];//{1,1,0,0,0,0,1,0,1,1,0,0,0,1,0};
int recvmsg1[16];
int key[4]={1,0,1,1};
int initialmsg[21];
int finalmsg[21];
int a=0;
int i=0;
char buf1[2]={0,0};
int finalmsg1[24];


int btod(int num[],int n)
{
  return(num[n+7]*1+num[n+6]*2+num[n+5]*4+num[n+4]*8+num[n+3]*16+num[n+2]*32+num[n+1]*64+num[n]*128);
}
void setup() {
Serial.begin(38400);
  if (!manager.init())
    Serial.println("init failed");// put your setup code here, to run once:

}

void loop() {
if(Serial.available()>0)
  buf1[0]=Serial.read();
if(Serial.available()>0)
  buf1[1]=Serial.read();

int d=(int)buf1[0];
int e=(int)buf1[1];
buf1[0]=0;
buf1[1]=0;

if((d!=0)||(e!=0))
{
  int remainder=0,quotient=0;
  int binaryNumber1[8]={0,0,0,0,0,0,0,0};
  i=0;
  quotient = d;
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
  quotient = e;
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


  for(i=0;i<=7;i++)
 {   recvmsg1[i]=binaryNumber1[7-i];
    recvmsg1[i+8]=binaryNumber2[7-i];
 }

 for(i=1;i<=15;i++)
  message[i-1]=recvmsg1[i];

 /*for(i=0;i<=7;i++)
  Serial.print(binaryNumber2[i]);*/
  //delay(100000);*/
a=random(0,4);

switch(a)
{
  case 0:
          initialmsg[0]=0;
          initialmsg[20]=0;
          initialmsg[1]=1;
          initialmsg[2]=0;
          initialmsg[19]=1;
          initialmsg[18]=1;
          for(i=3;i<=17;i++)
            initialmsg[i]=message[i-3];

          finalmsg[0]=0;
          finalmsg[20]=0;
          finalmsg[1]=initialmsg[1];
          finalmsg[2]=initialmsg[5];
          finalmsg[3]=initialmsg[6];
          finalmsg[4]=initialmsg[16];
          finalmsg[5]=initialmsg[9];
          finalmsg[6]=initialmsg[4];
          finalmsg[7]=initialmsg[2];
          finalmsg[8]=initialmsg[3];
          finalmsg[9]=initialmsg[7];
          finalmsg[10]=initialmsg[15];
          finalmsg[11]=initialmsg[8];
          finalmsg[12]=initialmsg[12];
          finalmsg[13]=initialmsg[10];
          finalmsg[14]=initialmsg[13];
          finalmsg[15]=initialmsg[17];
          finalmsg[16]=initialmsg[11];
          finalmsg[17]=initialmsg[14];
          finalmsg[18]=initialmsg[19];
          finalmsg[19]=initialmsg[18];
          break;
          
        
  case 1:
          initialmsg[0]=0;
          initialmsg[20]=1;
          initialmsg[1]=1;
          initialmsg[2]=0;
          initialmsg[18]=1;
          initialmsg[19]=1;
          for(i=3;i<=17;i++)
            initialmsg[i]=message[i-3];

          finalmsg[0]=0;
          finalmsg[20]=1;
          finalmsg[1]=initialmsg[16];
          finalmsg[2]=initialmsg[11];
          finalmsg[3]=initialmsg[17];
          finalmsg[4]=initialmsg[4];
          finalmsg[5]=initialmsg[8];
          finalmsg[6]=initialmsg[7];
          finalmsg[7]=initialmsg[1];
          finalmsg[8]=initialmsg[10];
          finalmsg[9]=initialmsg[12];
          finalmsg[10]=initialmsg[6];
          finalmsg[11]=initialmsg[13];
          finalmsg[12]=initialmsg[2];
          finalmsg[13]=initialmsg[9];
          finalmsg[14]=initialmsg[3];
          finalmsg[15]=initialmsg[5];
          finalmsg[16]=initialmsg[14];
          finalmsg[17]=initialmsg[15];
          finalmsg[18]=initialmsg[19];
          finalmsg[19]=initialmsg[18];
          
          break;
     
  case 2:
          initialmsg[0]=1;
          initialmsg[20]=0;
          initialmsg[1]=1;
          initialmsg[2]=0;
          initialmsg[19]=1;
          initialmsg[18]=1;
          for(i=3;i<=17;i++)
            initialmsg[i]=message[i-3];

          finalmsg[0]=1;
          finalmsg[20]=0;
          finalmsg[1]=initialmsg[10];
          finalmsg[2]=initialmsg[2];
          finalmsg[3]=initialmsg[7];
          finalmsg[4]=initialmsg[15];
          finalmsg[5]=initialmsg[9];
          finalmsg[6]=initialmsg[4];
          finalmsg[7]=initialmsg[17];
          finalmsg[8]=initialmsg[8];
          finalmsg[9]=initialmsg[16];
          finalmsg[10]=initialmsg[1];
          finalmsg[11]=initialmsg[3];
          finalmsg[12]=initialmsg[12];
          finalmsg[13]=initialmsg[14];
          finalmsg[14]=initialmsg[3];
          finalmsg[15]=initialmsg[5];
          finalmsg[16]=initialmsg[11];
          finalmsg[17]=initialmsg[6];
          finalmsg[18]=initialmsg[18];
          finalmsg[19]=initialmsg[19];
          break;
      
  case 3:
          initialmsg[0]=1;
          initialmsg[20]=1;
          initialmsg[1]=1;
          initialmsg[2]=0;
          initialmsg[18]=1;
          initialmsg[19]=1;
          for(i=3;i<=17;i++)
            initialmsg[i]=message[i-3];

          finalmsg[0]=1;
          finalmsg[20]=1;
          finalmsg[1]=initialmsg[7];
          finalmsg[2]=initialmsg[9];
          finalmsg[3]=initialmsg[1];
          finalmsg[4]=initialmsg[5];
          finalmsg[5]=initialmsg[17];
          finalmsg[6]=initialmsg[4];
          finalmsg[7]=initialmsg[15];
          finalmsg[8]=initialmsg[11];
          finalmsg[9]=initialmsg[6];
          finalmsg[10]=initialmsg[2];
          finalmsg[11]=initialmsg[16];
          finalmsg[12]=initialmsg[13];
          finalmsg[13]=initialmsg[14];
          finalmsg[14]=initialmsg[3];
          finalmsg[15]=initialmsg[8];
          finalmsg[16]=initialmsg[10];
          finalmsg[17]=initialmsg[12];
          finalmsg[18]=initialmsg[18];
          finalmsg[19]=initialmsg[19];
          break;
}

 
Serial.println("jumbled signal=");
for(i=0;i<=20;i++)
{
  
  Serial.print(finalmsg[i]);
}

for(i=3;i<=23;i++)
finalmsg1[i]=finalmsg[i-3];
finalmsg1[0]=0;
finalmsg1[1]=0;
finalmsg1[2]=0;

char l=btod(finalmsg1,0);
char m=btod(finalmsg1,8);
char n=btod(finalmsg1,16);
char final1[4];
final1[0]=l;
final1[1]=m;
final1[2]=n;
final1[3]='\0';
Serial.println(" ");
Serial.println(final1);
/*Serial.println(btod(finalmsg1,0));
Serial.println(btod(finalmsg1,8));
Serial.println(btod(finalmsg1,16));
*/

if (manager.sendtoWait(final1, sizeof(final1), nrf3))
    Serial.println("Sent to 3nd");
else 
    Serial.println("3nd nrf not working");
if (manager.sendtoWait(final1, sizeof(final1), nrf1))
    Serial.println("Sent to 1st");
else 
    Serial.println("1st nrf not working");
/*Serial.println(" ");
delay(10000);*/
}
}
