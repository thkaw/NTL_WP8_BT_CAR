/*
  - based on the http://developer.nokia.com/Community/Wiki/Windows_Phone_8_communicating_with_Arduino_using_Bluetooth
 
  NTL_WP8_BT_CAR
  
  Extend Moto Function By Nathaniel - 2013/11/22
  Author E-Mail: s99115247@stu.edu.tw 
  Author Blog:http://www.dotblogs.com.tw/thkaw/
*/

#include <SoftwareSerial.h>

//moto A
int dir1PinA = 13;
int dir2PinA = 12;
int speedPinA = 10;

//moto B
int dir1PinB = 11;
int dir2PinB = 8;
int speedPinB = 9;

int Power_Val = 100; //用來儲存馬達出力程度;

const int TX_BT = 6;
const int RX_BT = 7;

SoftwareSerial btSerial(TX_BT, RX_BT);

//Frequency to send periodic messages to Windows Phone, in milliseconds.
const unsigned long periodicMessageFrequency = 5000;
unsigned long time = 0;

//Process the incoming command from Windows Phone.
void processCommand(char* command) {
  Serial.print("RecvCommand:");
  Serial.println(command);
  String sCmd = command;
  if(strcmp(command,"Forward") == 0) {
    ForwardDrive();
    sendMessage("Forward");
  } 
  else if(strcmp(command,"Back") == 0) {
    BackDrive();
    sendMessage("Back");
  } 
  else if(strcmp(command,"Left") == 0) {
    LeftDrive();
    sendMessage("Left");
  } 
  else if(strcmp(command,"Right") == 0) {
    RightDrive();
    sendMessage("Right");
  } 
  else if(strcmp(command,"Stop") == 0) {
    StopDrive();
    sendMessage("Stop");
  } 
  else if(sCmd.indexOf("PowerVal") >= 0) {

    int spliterPosition;

    spliterPosition = sCmd.indexOf(';');
    Power_Val = sCmd.substring(spliterPosition+1, sCmd.length()).toInt();
    Serial.print("Power_Val:");
    Serial.println(Power_Val);
    sendMessage(command);    
  }
}
  
//Send a message back to the Windows Phone.
void sendMessage(char* message) {
  int messageLen = strlen(message);
  if(messageLen < 256) {  
    btSerial.write(messageLen);
    btSerial.print(message);
  }
}

//Send a set of periodic messages to the Windows Phone.
//This message could be a sensor data, like a thermometer data.
void sendPeriodicMessages() {

}

//Setup Arduino function
void setup() {
  Serial.begin(9600);
  Serial.println("USB Connected");
  btSerial.begin(9600); 

  //set Motor PinMode
  pinMode(dir1PinA, OUTPUT);
  pinMode(dir2PinA, OUTPUT);
  pinMode(speedPinA, OUTPUT);

  pinMode(dir1PinB, OUTPUT);
  pinMode(dir2PinB, OUTPUT);
  pinMode(speedPinB, OUTPUT);
}

//Loop Arduino function
void loop() {
  if(btSerial.available()) {
    int commandSize = (int)btSerial.read();
    char command[commandSize];
    int commandPos = 0;
    while(commandPos < commandSize) {
      if(btSerial.available()) {
        command[commandPos] = (char)btSerial.read();
        commandPos++;
      }
    }
    command[commandPos] = 0;
    processCommand(command);
  }
  //  unsigned long currentTime = millis();
  //  if((currentTime - time) > periodicMessageFrequency) {
  //    sendPeriodicMessages();
  //    time = currentTime;
  //  }
}
 

//以下是馬達部分
void MOTODRIVE(char motor,int speed,int dir) //控制馬達用副程式
{
  if(motor == 'R')
  {
    analogWrite(speedPinA, speed);
    if(dir == 1) 
    {
      digitalWrite(dir1PinA, LOW);
      digitalWrite(dir2PinA, HIGH);
    }
    else 
    {
      digitalWrite(dir1PinA, HIGH);
      digitalWrite(dir2PinA, LOW); 
    }     
  }

  if(motor == 'L')
  {
    analogWrite(speedPinB, speed);
    if(dir == 1) 
    {
      digitalWrite(dir1PinB, LOW);
      digitalWrite(dir2PinB, HIGH);
    }
    else 
    {
      digitalWrite(dir1PinB, HIGH);
      digitalWrite(dir2PinB, LOW); 
    }     
  }
}
 
void TESTDrive()//測試用的馬達動作
{
  MOTODRIVE('R',0,0);
  MOTODRIVE('L',0,0);
  delay(50);
  MOTODRIVE('L',(Power_Val),1);
  delay(2000);
  MOTODRIVE('L',(Power_Val),0);
  delay(200);
}

void TESTDrive2()//測試用的馬達動作
{
  MOTODRIVE('R',0,0);
  MOTODRIVE('L',0,0);
  delay(10);
  MOTODRIVE('R',(Power_Val),1);
  delay(5000);
  MOTODRIVE('R',(Power_Val),0);
  delay(200);
}

void ForwardDrive()
{
  MOTODRIVE('R',(Power_Val),1);
  MOTODRIVE('L',(Power_Val),1);
}

void BackDrive()
{
  MOTODRIVE('R',(Power_Val),0);
  MOTODRIVE('L',(Power_Val),0);
}

void LeftDrive()//新版左轉
{
  MOTODRIVE('R',0,0);
  MOTODRIVE('L',0,0);
  delay(50);
  MOTODRIVE('L',(Power_Val),1);
}

void RightDrive()//新版右轉
{
  MOTODRIVE('R',0,0);
  MOTODRIVE('L',0,0);
  delay(50);
  MOTODRIVE('R',(Power_Val),1);
}

void StopDrive()
{
  MOTODRIVE('R',0,0);
  MOTODRIVE('L',0,0);
  delay(50);
}






