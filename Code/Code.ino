//including the libraries
#include <AFMotor.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <Arduino_FreeRTOS.h>

# define Start_Byte     0x7E
# define Version_Byte   0xFF
# define Command_Length 0x06
# define End_Byte       0xEF
# define Acknowledge    0x00
#define left A15
#define right A14

int xPosition = 0;
int yPosition = 0;
int SW_state = 0;
int mapX = 0;
int mapY = 0;
int VRx = A13;
int VRy = A12;
int SW = 53;



SoftwareSerial mySerial(35, 33);
int btnNext;
int btnPause;
int btnPrevious;
int volume = 15;
boolean Playing = false;
char ko;
int previousv=-1;

int lightSensorPin = A8;
int analogValue = 0;

const int a = 24;
const int b = 22;
const int c = 16;
const int d = 15;
const int e = 14;
const int f = 19;
const int g = 18;
const int h = 17;

int LedPin1 = 51;
int LedPin2 = 50;
int LedPin3 = 49;
int LedPin4 = 48;
int LedPin5 = 47;
int LedPin6 = 46;

int led1=37;
 int led2=31;
 int led3=41;

//defining motors
AF_DCMotor motor1(1, MOTOR12_1KHZ); 
AF_DCMotor motor2(2, MOTOR12_1KHZ);
AF_DCMotor motor3(3, MOTOR34_1KHZ);
AF_DCMotor motor4(4, MOTOR34_1KHZ);


void setup() {

  //declaring pin types
  pinMode(left,INPUT);
  pinMode(right,INPUT);
  pinMode(lightSensorPin, INPUT);
  pinMode(VRx, INPUT);
  pinMode(VRy, INPUT);
  pinMode(SW, INPUT_PULLUP); 

  pinMode(a, OUTPUT);  //A
  pinMode(b, OUTPUT);  //B
  pinMode(c, OUTPUT);  //C
  pinMode(d, OUTPUT);  //D
  pinMode(e, OUTPUT);  //E
  pinMode(f, OUTPUT);  //F
  pinMode(g, OUTPUT); //G
  
  pinMode(LedPin1, OUTPUT);
  pinMode(LedPin2,OUTPUT);
  pinMode(LedPin3,OUTPUT);
  pinMode(LedPin4, OUTPUT);
  pinMode(LedPin5,OUTPUT);
  pinMode(LedPin6,OUTPUT);

  pinMode(led1,OUTPUT);
  pinMode(led2,OUTPUT);
  pinMode(led3,OUTPUT);

 digitalWrite(led1, LOW);
 digitalWrite(led2, LOW);
 digitalWrite(led3, LOW);

  Wire.begin() ;
  btnNext=HIGH;
  btnPause=HIGH;
  btnPrevious=HIGH; 
  Serial.begin(9600);
  mySerial.begin(9600);
  //delay(1000);

  xTaskCreate(MyTask1, "motor", 1000, NULL, 2, NULL);
  xTaskCreate(MyTask2, "Light and Gear", 1000, NULL, 1, NULL);
  xTaskCreate(MyTask3, "MP3 and Screen", 1000, NULL, 0, NULL);

}

void loop() {

   //Serial.println("Task0");
   //delay(500);

  
}
////////////////////////////////Task 1//////////////////////////////////////////////////////////////////////////////////////////////////////

static void MyTask1(void* pvParameters){

   while(1){
     Serial.println("Task1");
     //line detected by both
     if(digitalRead(left)==0 && digitalRead(right)==0){
      //Forward
      digitalWrite(led1, LOW);
      digitalWrite(led2, LOW);
      digitalWrite(led3, LOW);
      motor1.run(FORWARD);
      motor1.setSpeed(150);
      motor2.run(FORWARD);
      motor2.setSpeed(150);
      motor3.run(FORWARD);
      motor3.setSpeed(150);
      motor4.run(FORWARD);
      motor4.setSpeed(150);
    }
    //line detected by left sensor
    else if(digitalRead(left)==0 && !analogRead(right)==0){
      //turn left
      digitalWrite(led1, HIGH);
      digitalWrite(led2, HIGH);
      digitalWrite(led3, HIGH);
      motor1.run(FORWARD);
      motor1.setSpeed(200);
      motor2.run(FORWARD);
      motor2.setSpeed(200);
      motor3.run(BACKWARD);
      motor3.setSpeed(200);
      motor4.run(BACKWARD);
      motor4.setSpeed(200);
  }
  //line detected by right sensor
  else if(!digitalRead(left)==0 && digitalRead(right)==0){
     //turn right
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
    motor1.run(BACKWARD);
    motor1.setSpeed(200);
    motor2.run(BACKWARD);
    motor2.setSpeed(200);
    motor3.run(FORWARD);
    motor3.setSpeed(200);
    motor4.run(FORWARD);
    motor4.setSpeed(200);
  }
  //line detected by none
  else if(!digitalRead(left)==0 && !digitalRead(right)==0){
    //stop
     digitalWrite(led1, HIGH);
     digitalWrite(led2, HIGH);
     digitalWrite(led3, HIGH);
     motor1.run(RELEASE);
     motor1.setSpeed(0);
     motor2.run(RELEASE);
     motor2.setSpeed(0);
     motor3.run(RELEASE);
     motor3.setSpeed(0);
     motor4.run(RELEASE);
     motor4.setSpeed(0);
  }
   vTaskDelay(20000/portTICK_PERIOD_MS);
}
} 
////////////////////////////////Task 2//////////////////////////////////////////////////////////////////////////////////////////////////////
static void MyTask2(void* pvParameters){

  while (1){
    Serial.println("Task2");
    xPosition = analogRead(VRx);
    yPosition = analogRead(VRy);
    SW_state = digitalRead(SW);
    mapX = map(xPosition, 0, 1023, -512, 512);
    mapY = map(yPosition, 0, 1023, -512, 512);
    analogValue = analogRead(lightSensorPin);
    
//  Serial.print("X: ");
//  Serial.print(mapX);
//  Serial.print(" | Y: ");
//  Serial.print(mapY);
//  Serial.print(" | Button: ");
//  Serial.println(SW_state);

  if(950<analogValue){            
    digitalWrite(LedPin1, HIGH);
    digitalWrite(LedPin2, HIGH);
    digitalWrite(LedPin3, HIGH);
    digitalWrite(LedPin4, HIGH);
    digitalWrite(LedPin5, HIGH);
    digitalWrite(LedPin6, HIGH);
  }
  else if(850<=analogValue  && analogValue <= 940){
    digitalWrite(LedPin1, HIGH);
    digitalWrite(LedPin2, HIGH);
    digitalWrite(LedPin3, LOW);
    digitalWrite(LedPin4, HIGH);
    digitalWrite(LedPin5, HIGH);
    digitalWrite(LedPin6, LOW);
  }
  else if(600<=analogValue  && analogValue < 750){
    digitalWrite(LedPin1, HIGH);
    digitalWrite(LedPin2, LOW);
    digitalWrite(LedPin3, LOW);
    digitalWrite(LedPin4, HIGH);
    digitalWrite(LedPin5, LOW);
    digitalWrite(LedPin6, LOW);
  }
  else {
    digitalWrite(LedPin1, LOW);
    digitalWrite(LedPin2, LOW);
    digitalWrite(LedPin3, LOW);
    digitalWrite(LedPin4, LOW);
    digitalWrite(LedPin5, LOW);
    digitalWrite(LedPin6, LOW);
  }
  if(mapX>=100&&mapX<=800&& SW_state==1){  // UP (d)
     digitalWrite(a,HIGH);
     digitalWrite(b,LOW);
     digitalWrite(c,LOW);
     digitalWrite(d,LOW);
     digitalWrite(e,LOW);
     digitalWrite(f,HIGH);
     digitalWrite(g,LOW);
}else if(mapX>=-800&&mapX<=-100&&SW_state==1){  // back (R)
     digitalWrite(a,HIGH);
     digitalWrite(b,HIGH);
     digitalWrite(c,HIGH);
     digitalWrite(d,HIGH);
     digitalWrite(e,LOW);
     digitalWrite(f,HIGH);
     digitalWrite(g,LOW);
}else if(SW_state==0){  // Normal (P)  
     digitalWrite(a,LOW);
     digitalWrite(b,LOW);
     digitalWrite(c,HIGH);
     digitalWrite(d,HIGH);
     digitalWrite(e,LOW);
     digitalWrite(f,LOW);
     digitalWrite(g,LOW);
}else{ // park n
     digitalWrite(a,HIGH);
     digitalWrite(b,HIGH);
     digitalWrite(c,LOW);
     digitalWrite(d,HIGH);
     digitalWrite(e,LOW);
     digitalWrite(f,HIGH);
     digitalWrite(g,LOW);
}
  vTaskDelay(20000/portTICK_PERIOD_MS);
}
}
////////////////////////////////Task 3//////////////////////////////////////////////////////////////////////////////////////////////////////
static void MyTask3(void* pvParameters){

  playFirst();
  Playing = true; 
   while(1){
          Serial.println("Task3");

 btnNext=HIGH;
   btnPause=HIGH;
   btnPrevious=HIGH; 
   
  Wire.requestFrom(5,1);
  while (Wire.available()) {
  if(Wire.available()!=0){
  ko = Wire.read();
  Serial.print(ko);
  }
  }
  
  if(ko==2&&ko!=previousv){
  Serial.print(ko);
  btnNext=HIGH;
  btnPause=LOW; 
  btnPrevious=HIGH; 
  delay(500);
  }
  if(ko==3&&ko!=previousv){
  Serial.println(ko);
  btnNext=LOW;
  btnPause=HIGH;
  btnPrevious=HIGH; 
  delay(500);
  }
  if(ko==4&&ko!=previousv){
  Serial.print(ko);
  btnNext=HIGH;
  btnPause=HIGH;
  btnPrevious=LOW;   
  delay(500);  
  }
 previousv=ko;
 
 if (btnPause == LOW)
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
 if (btnNext == LOW)
  {
    if(Playing)
    {
      next();
    }
  }
   if (btnPrevious == LOW)
  {
    if(Playing)
    {
      previous();
    }
  }
  Serial.println("Task3");
vTaskDelay(20000/portTICK_PERIOD_MS);
   
} }
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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


void exe_cmd(byte CMD, byte Par1, byte Par2)
{
  word checksum = -(Version_Byte + Command_Length + CMD + Acknowledge + Par1 + Par2);
  byte Command_line[10] = { Start_Byte, Version_Byte, Command_Length, CMD, Acknowledge, Par1, Par2, highByte(checksum), lowByte(checksum), End_Byte};
  
  for (byte x=0; x<10; x++)
  {
    mySerial.write(Command_line[x]);
  }
}
