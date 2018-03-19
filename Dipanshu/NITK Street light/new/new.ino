#include <RHReliableDatagram.h>
#include <RH_NRF24.h>
#include <SPI.h>

#define ter 1
#define nrfnext 2
#define nrfself 1


RH_NRF24 driver;

RHReliableDatagram manager(driver, ter);



int o,s,b,stat,error,onoff,i,hour,minute,addr=0;
int msg[29]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int inmsg[29]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int bright[2];
int flag=1;
int hr[5]={0,0,0,0,0};
int mn[6]={0,0,0,0,0,0};
int inaddr[10]={0,0,0,0,0,0,0,0,0,0};
int msg1[29]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int *p,*q;
char data[30]="00000000000000000000000000000";
int integer[29]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int enc1[35]={0,3,7,15,13,27,32,30,6,4,2,18,11,26,20,10,12,19,24,22,31,14,8,21,9,17,28,16,25,29,33,23,5,1,34};
int enc2[35]={0,10,14,21,31,5,13,3,12,24,32,6,9,2,11,16,1,19,26,28,18,20,8,7,17,4,15,22,33,23,25,30,29,27,34};
int enc3[35]={0,5,31,25,14,28,33,7,18,11,6,2,32,20,30,10,9,13,26,22,19,4,21,1,17,24,8,3,29,27,15,23,12,16,34};
int enc4[35]={0,30,15,10,4,3,21,1,2,6,26,33,5,32,20,14,19,12,8,23,28,22,7,17,24,9,29,25,16,13,31,11,18,27,34};
int dec1[35]={0,33,10,1,9,32,8,2,22,24,15,12,16,4,21,3,27,25,11,17,14,23,19,31,18,28,13,5,26,29,7,20,6,30,34};
int dec2[35]={0,16,13,7,25,5,11,23,22,12,1,14,8,6,2,26,15,24,20,17,21,3,27,29,9,30,18,33,19,32,31,4,10,28,34};
int dec3[35]={0,23,11,27,21,1,10,7,26,16,15,9,32,17,4,30,33,24,8,20,13,22,19,31,25,3,18,29,5,28,14,2,12,6,34};
int dec4[35]={0,7,8,5,4,12,9,22,18,25,3,31,17,29,15,2,28,23,32,16,14,6,21,19,24,27,10,33,20,26,1,30,13,11,34};


uint8_t indata[RH_NRF24_MAX_MESSAGE_LEN];
void setup() {
  Serial.begin(38400);
  pinMode(3,OUTPUT);
  if (!manager.init())
    Serial.println("init failed");
    // put your setup code here, to run once:

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
  
  o=msg[0];
  s=msg[1];
  b=msg[2];
  stat=msg[3];
  
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
      Serial.print(hour);
      Serial.print(":");
      Serial.print(minute);
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
      for(i=0;i<=4;i++)
        msg1[i]=0;
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

      for(i=0;i<7;i++)
        {
          m1[i]=msg1[i];
          m2[i]=msg1[i+7];
          m3[i]=msg1[i+14];
          m4[i]=msg1[i+21];
          m5[i]=msg1[i+28];
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

      for(i=0;i<29;i++)
        Serial.print(msg1[i]);
    }
  }
}

void loop() {
  if(Serial.available()>0)
    data[0]=Serial.read();
  if(Serial.available()>0)
    data[1]=Serial.read();
  if(Serial.available()>0)
    data[2]=Serial.read();
  if(Serial.available()>0)
    data[3]=Serial.read();
  if(Serial.available()>0)
    data[4]=Serial.read();
  if(Serial.available()>0)
    data[5]=Serial.read();
  if(Serial.available()>0)
    data[6]=Serial.read();
  if(Serial.available()>0)
    data[7]=Serial.read();
  if(Serial.available()>0)
    data[8]=Serial.read();
  if(Serial.available()>0)
    data[9]=Serial.read();
  if(Serial.available()>0)
    data[10]=Serial.read();
  if(Serial.available()>0)
    data[11]=Serial.read();
  if(Serial.available()>0)
    data[12]=Serial.read();
  if(Serial.available()>0)
    data[13]=Serial.read();
  if(Serial.available()>0)
    data[14]=Serial.read();
  if(Serial.available()>0)
    data[15]=Serial.read();
  if(Serial.available()>0)
    data[16]=Serial.read();
  if(Serial.available()>0)
    data[17]=Serial.read();
  if(Serial.available()>0)
    data[18]=Serial.read();
  if(Serial.available()>0)
    data[19]=Serial.read();
  if(Serial.available()>0)
    data[20]=Serial.read(); 
  if(Serial.available()>0)
    data[21]=Serial.read();
  if(Serial.available()>0)
    data[22]=Serial.read();
  if(Serial.available()>0)
    data[23]=Serial.read();
  if(Serial.available()>0)
    data[24]=Serial.read();
  if(Serial.available()>0)
    data[25]=Serial.read();
  if(Serial.available()>0)
    data[26]=Serial.read();
  if(Serial.available()>0)
    data[27]=Serial.read();
  if(Serial.available()>0)
    data[28]=Serial.read();
                                     
  
  if(data[0]!=0 || data[1]!=0 || data[2]!=0 || data[3]!=0 || data[4]!=0 || data[5]!=0 || data[6]!=0 || data[7]!=0 || data[8]!=0 || data[9]!=0 || data[10]!=0 || data[11]!=0 || data[12]!=0 || data[13]!=0 || data[14]!=0 || data[15]!=0 || data[16]!=0 || data[17]!=0 || data[18]!=0 || data[19]!=0 || data[20]!=0 || data[21]!=0 || data[22]!=0 || data[23]!=0 || data[24]!=0 || data[25]!=0 || data[26]!=0 || data[27]!=0 || data[28]!=0)
  {
    for(i=0;i<29;i++)
      {
        integer[i]=(int)data[i];
        msg[i]=integer[i]-48;
      }
      
      o=msg[0];
      s=msg[1];
      b=msg[2];
      stat=msg[3];
      error=msg[4];
      if(o==1 || s==1 || b==1 || stat==1)
      {
        processdata(msg);
        
        
        char dataen[5]="00000";
        int m1[7],m2[7],m3[7],m4[7],m5[7]={0,0,0,0,0,0,0};
        int finalmsg[35]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
        int initmsg[35]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
        int randkey=0;
  
         
          for(i=3;i<32;i++)
            initmsg[i]=msg[i-3];
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

      dataen[0]=(char)i1;
      dataen[1]=(char)i2;
      dataen[2]=(char)i3;
      dataen[3]=(char)i4;
      dataen[4]=(char)i5;  
      
      if (manager.sendtoWait(dataen, sizeof(dataen), nrfnext))
          {} 
                 
      }

    
    uint8_t len = sizeof(indata);
    uint8_t from;
    if (manager.recvfromAckTimeout(indata, &len, 400, &from))
    {
          int i1=(int)indata[0];
  int i2=(int)indata[1];
  int i3=(int)indata[2];
  int i4=(int)indata[3];
  int i5=(int)indata[4];
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
      inmsg[i]=0;
    Serial.println("Unknown sender");
  }
  else
  {
    for(i=0;i<29;i++)
      inmsg[i]=fm[i+3];
  }
  for(i=0;i<29;i++)
      Serial.print(inmsg[i]);
    Serial.println(" ");
    }
  } 
  data[0]=0; 
  data[1]=0;
  data[2]=0;
  data[3]=0;
  data[4]=0;
  data[5]=0;
  data[6]=0;
  data[7]=0;
  data[8]=0;
  data[9]=0;
  data[10]=0;
  data[11]=0;
  data[12]=0;
  data[13]=0;
  data[14]=0;
  data[15]=0;
  data[16]=0;
  data[17]=0;
  data[18]=0;
  data[19]=0;
  data[20]=0;
  data[21]=0;
  data[22]=0;
  data[23]=0;
  data[24]=0;
  data[25]=0;
  data[26]=0;
  data[27]=0;
  data[28]=0;
    
  //delay(1000);
}
