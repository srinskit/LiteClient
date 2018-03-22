#include <RHReliableDatagram.h>
#include <RH_NRF24.h>
#include <SPI.h>
#include<Wire.h>
#include<RealTimeClockDS1307.h>

#define ter 1
#define nrfnext 3
#define nrfprev 1
#define nrfself 2

RH_NRF24 driver;

RHReliableDatagram manager(driver, nrfself);

char data[RH_NRF24_MAX_MESSAGE_LEN];
int o,s,b,stat,error,onoff,i,hour,minute,addr=0;
int msg[29];
int bright[2];
int flag=1;
int hr[5]={0,0,0,0,0};
int mn[6]={0,0,0,0,0,0};
int inaddr[10]={0,0,0,0,0,0,0,0,0,0};
int msg1[29]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int *p,*q;

void setup() 
{
  Serial.begin(38400);
  pinMode(3,OUTPUT);
  if (!manager.init())
    Serial.println("init failed");
  // Defaults after init are 2.402 GHz (channel 2), 2Mbps, 0dBm
}



int btod(int m[],int n)
{
  int sum=0,o=1;
  for(i=0;i<n;i++)
  {
    sum=sum+m[n-i-1]*o;
    o=o*2;
  }
  return sum;
}






void processdata(int msg[])
{
  char data1[5]="00000";
  int m1[7],m2[7],m3[7],m4[7],m5[7]={0,0,0,0,0,0,0};
  int finalmsg[35]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  int initmsg[35]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  int randkey=0;
  
  o=msg[0];
  s=msg[1];
  b=msg[2];
  stat=msg[3];
  error=msg[4];
  
  for(i=0;i<=9;i++)
    inaddr[i]=msg[i+5];
  
  onoff=msg[15];
  addr=btod(inaddr,10);
  bright[0]=msg[16];
  bright[1]=msg[17];
    
  if(o==1)
  {
     
     if(addr==nrfself)
      {
            if(bright[0]==0 && bright[1]==0)
              {analogWrite(3,0);flag=1;}
            if(bright[0]==0 && bright[1]==1)
              {analogWrite(3,15);flag=2;}
            if(bright[0]==1 && bright[1]==0)
              {analogWrite(3,80);flag=3;}
            if(bright[0]==1 && bright[1]==1)
              {analogWrite(3,255);flag=4;}
      }
  }

  if(s==1)
  {
    for(i=0;i<=4;i++)
      hr[i]=msg[i+18];
    for(i=0;i<=5;i++)
      mn[i]=msg[i+23];      
        
    hour=btod(hr,5);
    minute=btod(mn,6);
    if(hour>23 || minute>60)
      Serial.println("Time of invalid format");
    else
    {
      RTC.setHours(hour);
      RTC.setMinutes(minute);
      RTC.setClock();
      RTC.readClock();
      Serial.print(RTC.getHours());
      Serial.print(":");
      Serial.print(RTC.getMinutes());
      Serial.println(" ");
    }
  }

  if(b==1)
  {
    
    if(bright[0]==0 && bright[1]==0)
      {analogWrite(3,0);flag=1;}
    if(bright[0]==0 && bright[1]==1)
      {analogWrite(3,15);flag=2;}
    if(bright[0]==1 && bright[1]==0)
      {analogWrite(3,80);flag=3;}
    if(bright[0]==1 && bright[1]==1)
      {analogWrite(3,255);flag=4;}  
  }

  if(stat==1)
  {
    if(flag==1)
      analogWrite(3,0);
    if(flag==2)
      analogWrite(3,15);
    if(flag==3)
      analogWrite(3,80);
    if(flag==4)
      analogWrite(3,255);

    if(addr==nrfself)
    {
      msg1[0]=0;
      msg1[1]=0;
      msg1[2]=0;
      msg1[3]=1;
      msg1[4]=0;
      for(i=5;i<=14;i++)
        msg1[i]=msg[i];
      
      msg1[15]=0;
      
      if(flag==1)
        {
          msg1[16]=0;
          msg1[17]=0;
        }
      if(flag==2)
        {
          msg1[16]=0;
          msg1[17]=1;
        }
      if(flag==3)
        {
          msg1[16]=1;
          msg1[17]=0;
        }
      if(flag==4)
        {
          msg1[16]=1;
          msg1[17]=1;
        }
      for(i=18;i<=28;i++)
        msg1[i]=msg[i];

      
  
  
  for(i=3;i<32;i++)
    initmsg[i]=msg1[i-3];
  initmsg[1]=1;
  initmsg[2]=0;
  initmsg[32]=1;
  initmsg[33]=1;  
  
  randkey=random(0,4);
  switch(randkey)
  {
    case 0:
          initmsg[0]=initmsg[34]=0;
          for(int i =0;i<35;++i)
             finalmsg[enc1[i]] = initmsg[i];
          break;
          
    case 1:
          initmsg[0]=0;
          initmsg[34]=1;
          for(int i =0;i<35;++i)
             finalmsg[enc2[i]] = initmsg[i];
          break;

    case 2:
          initmsg[0]=1;
          initmsg[34]=0;
          for(int i =0;i<35;++i)
             finalmsg[enc3[i]] = initmsg[i];
          break;

    case 3:
          initmsg[0]=1;
          initmsg[34]=1;
          for(int i =0;i<35;++i)
             finalmsg[enc4[i]] = initmsg[i];
          break;
  }
  
      for(i=0;i<7;i++)
        {
          m1[i]=finalmsg[i];
          m2[i]=finalmsg[i+7];
          m3[i]=finalmsg[i+14];
          m4[i]=finalmsg[i+21];
          m5[i]=finalmsg[i+28];
        }
      int i1=btod(m1,7);
      int i2=btod(m2,7);
      int i3=btod(m3,7);
      int i4=btod(m4,7);
      int i5=btod(m5,7);

      data1[0]=(char)i1;
      data1[1]=(char)i2;
      data1[2]=(char)i3;
      data1[3]=(char)i4;
      data1[4]=(char)i5;
      
      if (manager.sendtoWait(data1, sizeof(data1), nrfprev))
        {}
    }
  }
}

void loop() {
  char data1[5]="00000";
  if(manager.available())
  {
    uint8_t len = sizeof(data);
    uint8_t from;
    if (manager.recvfromAck(data, &len,&from))
    {
      
      int i1=(int)data[0];
  int i2=(int)data[1];
  int i3=(int)data[2];
  int i4=(int)data[3];
  int i5=(int)data[4];
  int im[35]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  int fm[35]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  int binaryNumber1[7],binaryNumber2[7],binaryNumber3[7],binaryNumber4[7],binaryNumber5[7]={0,0,0,0,0,0,0};
  
  int remainder=0,quotient=0;
  i=0;
  quotient = i1;
  while(quotient!=0){
     binaryNumber1[i++]= quotient % 2;
     quotient = quotient / 2;
    }
  if(i<=6)
  {
    while(i<=6)
    {
      binaryNumber1[i]=0;
      i++;
  }}

  
  remainder=0,quotient=0;
  i=0;
  quotient = i2;
  while(quotient!=0){
     binaryNumber2[i++]= quotient % 2;
     quotient = quotient / 2;
     
    }
  if(i<=6)
  {
    while(i<=6)
    {
      binaryNumber2[i]=0;
      i++;
  }}

  
  remainder=0,quotient=0;
  i=0;
  quotient = i3;
  while(quotient!=0){
     binaryNumber3[i++]= quotient % 2;
     quotient = quotient / 2;
     
    }
  if(i<=6)
  {
    while(i<=6)
    {
      binaryNumber3[i]=0;
      i++;
  }}

  
  remainder=0,quotient=0;
  i=0;
  quotient = i4;
  while(quotient!=0){
     binaryNumber4[i++]= quotient % 2;
     quotient = quotient / 2;
     
    }
  if(i<=6)
  {
    while(i<=6)
    {
      binaryNumber4[i]=0;
      i++;
  }}

  
  remainder=0,quotient=0;
  i=0;
  quotient = i5;
  while(quotient!=0){
     binaryNumber5[i++]= quotient % 2;
     quotient = quotient / 2;
     
    }
  if(i<=6)
  {
    while(i<=6)
    {
      binaryNumber5[i]=0;
      i++;
  }}
  
  
  for(i=0;i<7;i++)
  {
    im[i]=binaryNumber1[6-i];
    im[i+7]=binaryNumber2[6-i];
    im[i+14]=binaryNumber3[6-i];
    im[i+21]=binaryNumber4[6-i];
    im[i+28]=binaryNumber5[6-i];
  }
 
  if(im[0]==0 && im[34]==0)
  {
    for(int i =0;i<35;++i)
        fm[dec1[i]] = im[i];
  }

  if(im[0]==0 && im[34]==1)
  {
    for(int i =0;i<35;++i)
        fm[dec2[i]] = im[i];
  }

  if(im[0]==1 && im[34]==0)
  {
    for(int i =0;i<35;++i)
        fm[dec3[i]] = im[i];
  }

  if(im[0]==1 && im[34]==1)
  {
    for(int i =0;i<35;++i)
        fm[dec4[i]] = im[i];
  }
  if(fm[1]!=1 || fm[2]!=0 || fm[32]!=1 || fm[33]!=1)
  {
    for(i=0;i<29;i++)
      msg[i]=0;
    Serial.println("Unknown sender");
  }
  else
  {
    for(i=0;i<29;i++)
      msg[i]=fm[i+3];
  }

      for(i=0;i<29;i++)
      Serial.print(msg[i]); 
      Serial.println(); 
      o=msg[0];
      s=msg[1];
      b=msg[2];
      stat=msg[3];
      error=msg[4];
      if(o==1 || s==1 || b==1 || stat==1 && from==nrfprev)
      {
        processdata(msg);
        if (manager.sendtoWait(data, sizeof(data), nrfnext))
          {}        
      }

      else
      {
        if (manager.sendtoWait(data, sizeof(data), nrfprev))
          {}
      }
    }
  }
}
