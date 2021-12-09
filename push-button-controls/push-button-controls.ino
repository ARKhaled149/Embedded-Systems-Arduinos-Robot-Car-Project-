#include <SoftwareSerial.h>

# define Start_Byte     0x7E
# define Version_Byte   0xFF
# define Command_Length 0x06
# define End_Byte       0xEF
# define Acknowledge    0x00

SoftwareSerial mySerial(10, 11);

const int btnNext     = A0;
const int btnPause    = A1;
const int btnPrevious = A2;
const int volumeUP    = A3;
const int volumeDOWN  = A4;

int volume = 15;
boolean Playing = false;

void setup () 
{
  pinMode(btnPause, INPUT);
  pinMode(btnNext, INPUT);
  pinMode(btnPrevious, INPUT);
  pinMode(volumeUP, INPUT);
  pinMode(volumeDOWN, INPUT);
  
  digitalWrite(btnPause, HIGH);
  digitalWrite(btnNext, HIGH);
  digitalWrite(btnPrevious, HIGH);
  digitalWrite(volumeUP, HIGH);
  digitalWrite(volumeDOWN, HIGH);
  
  mySerial.begin(9600);
  delay(1000);
  
  playFirst();
  Playing = true;
}

void loop () 
{  
 if (digitalRead(btnPause) == LOW)
  {
    if(Playing)
    {
      pause();
      Playing = false;
    }
    else
    {
      Playing = true;
      play();
    }
  }
  
 if (digitalRead(btnNext) == LOW)
  {
    if(Playing)
    {
      next();
    }
  }
   if (digitalRead(btnPrevious) == LOW)
  {
    if(Playing)
    {
      previous();
    }
  }
  
  if(digitalRead(volumeUP) == LOW)
  {
   volumeINC(); 
  }
  if(digitalRead(volumeDOWN) == LOW)
  {
    volumeDEC();
  }
}
  
void playFirst()
{
  exe_cmd(0x3F, 0, 0);
  delay(500);
  exe_cmd(0x06, 0, volume); 
  delay(500);
  exe_cmd(0x11,0,1); 
  delay(500);
}

void pause()
{
  exe_cmd(0x0E,0,0);
  delay(500);
}

void play()
{
  exe_cmd(0x0D,0,1); 
  delay(500);
}

void next()
{
  exe_cmd(0x01,0,1);
  delay(500);
}

void previous()
{
  exe_cmd(0x02,0,1);
  delay(500);
}

void volumeINC()
{
  volume = volume+1;
  if(volume==31)
  {
    volume=30;
  }
  exe_cmd(0x06, 0, volume); 
  delay(500);
}

void volumeDEC()
{
  volume = volume-1;
  if(volume==-1)
  {
    volume=0;
  }
  exe_cmd(0x06, 0, volume); 
  delay(500);
}

void exe_cmd(byte CMD, byte Par1, byte Par2)
{
  word checksum = -(Version_Byte + Command_Length + CMD + Acknowledge + Par1 + Par2);
  byte Command_line[10] = { Start_Byte, Version_Byte, Command_Length, CMD, Acknowledge, Par1, Par2, highByte(checksum), lowByte(checksum), End_Byte};
  
  for (byte x=0; x<10; x++)
  {
    mySerial.write(Command_line[x]);
  }
}
