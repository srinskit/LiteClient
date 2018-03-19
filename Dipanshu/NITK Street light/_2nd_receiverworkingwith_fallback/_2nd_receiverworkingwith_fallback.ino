#include <RHReliableDatagram.h>
#include <RH_NRF24.h>
#include <SPI.h>
#include<Wire.h>
#include<RealTimeClockDS1307.h>

#define ter 3
#define nrfself 4
#define nrfnext 5
#define nrfnextnext 6
#define nrfprev 2
#define nrfprevprev 1
#define nrflastsec 0
#define nrflast 1
#define nrfsec 1


RH_NRF24 driver;

RHReliableDatagram manager(driver, nrfself);

char data[RH_NRF24_MAX_MESSAGE_LEN];
int o,s,b,stat,error,onoff,i,hour,minute,addr,high,optval=0;
int msg[29],kmsg[29];
int bright[2];
int flag=1;
int flagm=0;
int count,cntfb=0;
int fbflag1=0;
int fbflag2=0;
int cntfallback_0=0;
int hr[5]={0,0,0,0,0};
int mn[6]={0,0,0,0,0,0};
int inaddr[10]={0,0,0,0,0,0,0,0,0,0};
int msg1[29]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
char data1[5]="00000";
int *omsg;
int *omsg1;
char *inm;
char *inm1;

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



char *encrypt(int *msg1)
{
  
  int m1[7],m2[7],m3[7],m4[7],m5[7]={0,0,0,0,0,0,0};
  int finalmsg[35]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  int initmsg[35]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  int randkey=0;

  
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
          initmsg[0]=0;
          initmsg[34]=0;
          finalmsg[0]=initmsg[0];
          finalmsg[1]=initmsg[3];
          finalmsg[2]=initmsg[7];
          finalmsg[3]=initmsg[15];
          finalmsg[4]=initmsg[13];
          finalmsg[5]=initmsg[27];
          finalmsg[6]=initmsg[32];
          finalmsg[7]=initmsg[30];
          finalmsg[8]=initmsg[6];
          finalmsg[9]=initmsg[4];
          finalmsg[10]=initmsg[2];
          finalmsg[11]=initmsg[18];
          finalmsg[12]=initmsg[11];
          finalmsg[13]=initmsg[26];
          finalmsg[14]=initmsg[20];
          finalmsg[15]=initmsg[10];
          finalmsg[16]=initmsg[12];
          finalmsg[17]=initmsg[19];
          finalmsg[18]=initmsg[24];
          finalmsg[19]=initmsg[22];
          finalmsg[20]=initmsg[31];
          finalmsg[21]=initmsg[14];
          finalmsg[22]=initmsg[8];
          finalmsg[23]=initmsg[21];
          finalmsg[24]=initmsg[9];
          finalmsg[25]=initmsg[17];
          finalmsg[26]=initmsg[28];
          finalmsg[27]=initmsg[16];
          finalmsg[28]=initmsg[25];
          finalmsg[29]=initmsg[29];
          finalmsg[30]=initmsg[33];
          finalmsg[31]=initmsg[23];
          finalmsg[32]=initmsg[5];
          finalmsg[33]=initmsg[1];
          finalmsg[34]=initmsg[34];
          break;
          
    case 1:
          initmsg[0]=0;
          initmsg[34]=1;
          finalmsg[0]=initmsg[0];
          finalmsg[1]=initmsg[10];
          finalmsg[2]=initmsg[14];
          finalmsg[3]=initmsg[21];
          finalmsg[4]=initmsg[31];
          finalmsg[5]=initmsg[5];
          finalmsg[6]=initmsg[13];
          finalmsg[7]=initmsg[3];
          finalmsg[8]=initmsg[12];
          finalmsg[9]=initmsg[24];
          finalmsg[10]=initmsg[32];
          finalmsg[11]=initmsg[6];
          finalmsg[12]=initmsg[9];
          finalmsg[13]=initmsg[2];
          finalmsg[14]=initmsg[11];
          finalmsg[15]=initmsg[16];
          finalmsg[16]=initmsg[1];
          finalmsg[17]=initmsg[19];
          finalmsg[18]=initmsg[26];
          finalmsg[19]=initmsg[28];
          finalmsg[20]=initmsg[18];
          finalmsg[21]=initmsg[20];
          finalmsg[22]=initmsg[8];
          finalmsg[23]=initmsg[7];
          finalmsg[24]=initmsg[17];
          finalmsg[25]=initmsg[4];
          finalmsg[26]=initmsg[15];
          finalmsg[27]=initmsg[22];
          finalmsg[28]=initmsg[33];
          finalmsg[29]=initmsg[23];
          finalmsg[30]=initmsg[25];
          finalmsg[31]=initmsg[30];
          finalmsg[32]=initmsg[29];
          finalmsg[33]=initmsg[27];
          finalmsg[34]=initmsg[34];
          break;

    case 2:
          initmsg[0]=1;
          initmsg[34]=0;
          finalmsg[0]=initmsg[0];
          finalmsg[1]=initmsg[5];
          finalmsg[2]=initmsg[31];
          finalmsg[3]=initmsg[25];
          finalmsg[4]=initmsg[14];
          finalmsg[5]=initmsg[28];
          finalmsg[6]=initmsg[33];
          finalmsg[7]=initmsg[7];
          finalmsg[8]=initmsg[18];
          finalmsg[9]=initmsg[11];
          finalmsg[10]=initmsg[6];
          finalmsg[11]=initmsg[2];
          finalmsg[12]=initmsg[32];
          finalmsg[13]=initmsg[20];
          finalmsg[14]=initmsg[30];
          finalmsg[15]=initmsg[10];
          finalmsg[16]=initmsg[9];
          finalmsg[17]=initmsg[13];
          finalmsg[18]=initmsg[26];
          finalmsg[19]=initmsg[22];
          finalmsg[20]=initmsg[19];
          finalmsg[21]=initmsg[4];
          finalmsg[22]=initmsg[21];
          finalmsg[23]=initmsg[1];
          finalmsg[24]=initmsg[17];
          finalmsg[25]=initmsg[24];
          finalmsg[26]=initmsg[8];
          finalmsg[27]=initmsg[3];
          finalmsg[28]=initmsg[29];
          finalmsg[29]=initmsg[27];
          finalmsg[30]=initmsg[15];
          finalmsg[31]=initmsg[23];
          finalmsg[32]=initmsg[12];
          finalmsg[33]=initmsg[16];
          finalmsg[34]=initmsg[34];
          break;

    case 3:
          initmsg[0]=1;
          initmsg[34]=1;
          finalmsg[0]=initmsg[0];
          finalmsg[1]=initmsg[30];
          finalmsg[2]=initmsg[15];
          finalmsg[3]=initmsg[10];
          finalmsg[4]=initmsg[4];
          finalmsg[5]=initmsg[3];
          finalmsg[6]=initmsg[21];
          finalmsg[7]=initmsg[1];
          finalmsg[8]=initmsg[2];
          finalmsg[9]=initmsg[6];
          finalmsg[10]=initmsg[26];
          finalmsg[11]=initmsg[33];
          finalmsg[12]=initmsg[5];
          finalmsg[13]=initmsg[32];
          finalmsg[14]=initmsg[20];
          finalmsg[15]=initmsg[14];
          finalmsg[16]=initmsg[19];
          finalmsg[17]=initmsg[12];
          finalmsg[18]=initmsg[8];
          finalmsg[19]=initmsg[23];
          finalmsg[20]=initmsg[28];
          finalmsg[21]=initmsg[22];
          finalmsg[22]=initmsg[7];
          finalmsg[23]=initmsg[17];
          finalmsg[24]=initmsg[24];
          finalmsg[25]=initmsg[9];
          finalmsg[26]=initmsg[29];
          finalmsg[27]=initmsg[25];
          finalmsg[28]=initmsg[16];
          finalmsg[29]=initmsg[13];
          finalmsg[30]=initmsg[31];
          finalmsg[31]=initmsg[11];
          finalmsg[32]=initmsg[18];
          finalmsg[33]=initmsg[27];
          finalmsg[34]=initmsg[34];
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
//      for(i=0;i<5;i++)
//Serial.print(data1[i]);
Serial.print("");
//delay(10);
      return data1;
      
}

int *decrypt(char *data)
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
    fm[0]=im[0];
    fm[3]=im[1];
    fm[7]=im[2];
    fm[15]=im[3];
    fm[13]=im[4];
    fm[27]=im[5];
    fm[32]=im[6];
    fm[30]=im[7];
    fm[6]=im[8];
    fm[4]=im[9];
    fm[2]=im[10];
    fm[18]=im[11];
    fm[11]=im[12];
    fm[26]=im[13];
    fm[20]=im[14];
    fm[10]=im[15];
    fm[12]=im[16];
    fm[19]=im[17];
    fm[24]=im[18];
    fm[22]=im[19];
    fm[31]=im[20];
    fm[14]=im[21];
    fm[8]=im[22];
    fm[21]=im[23];
    fm[9]=im[24];
    fm[17]=im[25];
    fm[28]=im[26];
    fm[16]=im[27];
    fm[25]=im[28];
    fm[29]=im[29];
    fm[33]=im[30];
    fm[23]=im[31];
    fm[5]=im[32];
    fm[1]=im[33];
    fm[34]=im[34];
  }

  if(im[0]==0 && im[34]==1)
  {
    fm[0]=im[0];
    fm[10]=im[1];
    fm[14]=im[2];
    fm[21]=im[3];
    fm[31]=im[4];
    fm[5]=im[5];
    fm[13]=im[6];
    fm[3]=im[7];
    fm[12]=im[8];
    fm[24]=im[9];
    fm[32]=im[10];
    fm[6]=im[11];
    fm[9]=im[12];
    fm[2]=im[13];
    fm[11]=im[14];
    fm[16]=im[15];
    fm[1]=im[16];
    fm[19]=im[17];
    fm[26]=im[18];
    fm[28]=im[19];
    fm[18]=im[20];
    fm[20]=im[21];
    fm[8]=im[22];
    fm[7]=im[23];
    fm[17]=im[24];
    fm[4]=im[25];
    fm[15]=im[26];
    fm[22]=im[27];
    fm[33]=im[28];
    fm[23]=im[29];
    fm[25]=im[30];
    fm[30]=im[31];
    fm[29]=im[32];
    fm[27]=im[33];
    fm[34]=im[34];
  }

  if(im[0]==1 && im[34]==0)
  {
    fm[0]=im[0];
    fm[5]=im[1];
    fm[31]=im[2];
    fm[25]=im[3];
    fm[14]=im[4];
    fm[28]=im[5];
    fm[33]=im[6];
    fm[7]=im[7];
    fm[18]=im[8];
    fm[11]=im[9];
    fm[6]=im[10];
    fm[2]=im[11];
    fm[32]=im[12];
    fm[20]=im[13];
    fm[30]=im[14];
    fm[10]=im[15];
    fm[9]=im[16];
    fm[13]=im[17];
    fm[26]=im[18];
    fm[22]=im[19];
    fm[19]=im[20];
    fm[4]=im[21];
    fm[21]=im[22];
    fm[1]=im[23];
    fm[17]=im[24];
    fm[24]=im[25];
    fm[8]=im[26];
    fm[3]=im[27];
    fm[29]=im[28];
    fm[27]=im[29];
    fm[15]=im[30];
    fm[23]=im[31];
    fm[12]=im[32];
    fm[16]=im[33];
    fm[34]=im[34];
  }

  if(im[0]==1 && im[34]==1)
  {
    fm[0]=im[0];
    fm[30]=im[1];
    fm[15]=im[2];
    fm[10]=im[3];
    fm[4]=im[4];
    fm[3]=im[5];
    fm[21]=im[6];
    fm[1]=im[7];
    fm[2]=im[8];
    fm[6]=im[9];
    fm[26]=im[10];
    fm[33]=im[11];
    fm[5]=im[12];
    fm[32]=im[13];
    fm[20]=im[14];
    fm[14]=im[15];
    fm[19]=im[16];
    fm[12]=im[17];
    fm[8]=im[18];
    fm[23]=im[19];
    fm[28]=im[20];
    fm[22]=im[21];
    fm[7]=im[22];
    fm[17]=im[23];
    fm[24]=im[24];
    fm[9]=im[25];
    fm[29]=im[26];
    fm[25]=im[27];
    fm[16]=im[28];
    fm[13]=im[29];
    fm[31]=im[30];
    fm[11]=im[31];
    fm[18]=im[32];
    fm[27]=im[33];
    fm[34]=im[34];
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
  return msg;
}



void fallback()
{
  if ((RTC.getHours()>17) && (RTC.getHours()<24))
  {
      digitalWrite(3 , HIGH);
      flag=4;
    if ((flagm == 1) && (count == 0))
    {
      flagm = 0;
      count = 1;
    }
    else if ((flagm == 0) && (count == 0))
    {
      flagm = 1;          
      count = 1;
    
    }
  } 
  if ((RTC.getHours()>=0) && (RTC.getHours()<7))                                  //fallback
  {
    count = 0;
    if (flagm == 1)
      {digitalWrite(3 , HIGH);flag=4;}
    else
      {digitalWrite(3 , LOW);flag=1;}   
  }
  
  if ((RTC.getHours()>=7) && (RTC.getHours()<=17)) 
  {
    if ((flagm == 1) && (count == 0))
    {
      flagm = 0;
      count = 1;
    }
    else if ((flagm == 0) && (count == 0))
    {
      flagm = 1;          
      count = 1;
    }
    digitalWrite(3 , LOW);
    flag=1;
  }
}




void processdata(int msg[])
{
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
        
    if(addr==nrfself)
    {
      msg1[0]=0;
      msg1[1]=0;
      msg1[2]=0;
      msg1[3]=1;
      for(i=4;i<=14;i++)
        msg1[i]=msg[i];
      
      msg1[15]=0;

      high=0;
      for(i=0;i<40;i++)
      {
        optval=analogRead(A0);
//        Serial.println(optval);
        if (optval >= 700)
            high++;
      }
      if(high == 0)
        {
          msg1[16]=0;
          msg1[17]=0;
        }
      if (high >= 1 && high <= 4)
        {
          msg1[16]=0;
          msg1[17]=1;
        }
      if (high >=5 && high <=19)
        {
          msg1[16]=1;
          msg1[17]=0;
        }
      if (high ==40)
        {
          msg1[16]=1;
          msg1[17]=1;
        }
      
      for(i=18;i<=28;i++)
        msg1[i]=msg[i];

      inm=encrypt(msg1);
      for(i=0;i<5;i++)
        data[i]=inm[i];
      if (manager.sendtoWait(data, sizeof(data), nrfprev))
        {}
      else if(nrfsec == 0)
      if (manager.sendtoWait(data, sizeof(data), nrfprevprev))
        {}
    }
  }
  if ((RTC.getHours() >= 0) && (RTC.getHours() <= 1))
  {
    if ((flag == 2) || (flag == 3) || (flag == 4))
      flagm = 1;
    else if (flag == 1)
      flagm = 0;
  }
}

void loop() {
  if(fbflag2 == 1)
  {
    fallback();
    Serial.println("Go to fallback");
  }
  
  if (driver.statusRead()==0 && fbflag1 == 0)
    cntfallback_0+=1;
  if ((driver.statusRead()!=14 && driver.statusRead()!=64 && driver.statusRead()!=0 && driver.statusRead()!=78&&driver.statusRead()!=110&& driver.statusRead()!=96&& driver.statusRead()!=32&& driver.statusRead()!=46) || (cntfallback_0>=5 && driver.statusRead()!=14 && driver.statusRead()!=64 && driver.statusRead()!=78&& driver.statusRead()!=96&& driver.statusRead()!=32 && driver.statusRead()!=46&&driver.statusRead()!=110))
  {
    if (fbflag1==0)
      cntfb++;
    if (cntfb>=10)
    {
      Serial.print("Go to fallback : ");
      Serial.println(driver.statusRead());
      fbflag1 = 1;
      fallback();
    }
  }
  else
  {
    if (fbflag1 == 1)
    if (manager.init())
      {}
    fbflag1 = 0;
    cntfb = 0;
    if (driver.statusRead()!=0)
    cntfallback_0=0;
    
    char data1[5]="00000";
    if(manager.available())
    {
      uint8_t len = sizeof(data);
      uint8_t from;
      if (manager.recvfromAck(data, &len,&from))
      {
            
            omsg=decrypt(data);
            for(i=0;i<29;i++)
            Serial.print(omsg[i]); 
            Serial.println(); 
            o=omsg[0];
            s=omsg[1];
            b=omsg[2];
            stat=omsg[3];
            error=omsg[4];
            for (i = 0; i <= 9; i++)
              inaddr[i] = omsg[i + 5];
            addr = btod(inaddr, 10);            
            
            if((o==1 || s==1 || b==1 || stat==1 || error==1) && (from==nrfprev || from==nrfprevprev))
            {
                  if (o ==1 && s == 1 && b == 1 && stat == 1 && error == 1)
                  {
                    fallback();
                    fbflag2 = 1;
                    Serial.println("Go to Fallback");
                  }
                  else
                  {
                    processdata(omsg);
                    fbflag2 = 0;
                  }
                  
                   if (addr!=nrfself)
                   {
                     if (nrflast == 0)
                     {
                         if (manager.sendtoWait(data, sizeof(data), nrfnext))
                          {}
                         else              
                         if (addr!=nrfnext) 
                         {
                            if(nrflastsec == 0)   
                              if (manager.sendtoWait(data, sizeof(data), nrfnextnext))
                                {}
                            if ((s==1 || b==1) && nrflastsec == 1)
                            {
                              if (manager.sendtoWait(data, sizeof(data), nrfprev))
                                {}
                              else if(nrfsec == 0)
                              if (manager.sendtoWait(data, sizeof(data), nrfprevprev))
                                {}                              
                            }
                         }
                      }
                      else 
                      {
                        if ((s==1 || b==1) && error==0)
                        {
                          if (manager.sendtoWait(data, sizeof(data), nrfprev))
                            {}
                          else if(nrfsec == 0)
                          if (manager.sendtoWait(data, sizeof(data), nrfprevprev))
                            {}                              
                        }
                        if (error == 1)
                        {
                          for(i=0;i<29;i++)
                            kmsg[i]=msg[i];
                          kmsg[4]=0;  
                          inm1=encrypt(kmsg);
                          for(i=0;i<5;i++)
                            data[i]=inm1[i];
                          if (manager.sendtoWait(data, sizeof(data), nrfnext))
                            {} 
                          else if (manager.sendtoWait(data, sizeof(data), nrfnextnext))
                                {}
                        }
                      }    
                   }
                   else if(stat!=1)
                   {
                      if (manager.sendtoWait(data, sizeof(data), nrfprev))
                        {}
                      else if(nrfsec == 0)
                      if (manager.sendtoWait(data, sizeof(data), nrfprevprev))
                        {}
                   }     
       
            }
      
            else
            {
              if (manager.sendtoWait(data, sizeof(data), nrfprev))
                {}
              else if(nrfsec == 0)
              if (manager.sendtoWait(data, sizeof(data), nrfprevprev))
                {}
            }
      }
    }
  }
}
