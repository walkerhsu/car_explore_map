#include <SoftwareSerial.h>
SoftwareSerial BT (13, 12);

#define RST_PIN 49
#define SS_PIN 53
char c2 = '\0';

int trigPinFr = 7;                  //Trig Pin
int echoPinFr = 6;                  //Echo Pin
int trigPinBk = 52;                  //Trig Pin
int echoPinBk = 53;                  //Echo Pin
int trigPinLf = 11;                  //Trig Pin
int echoPinLf = 10;                  //Echo Pin
int trigPinRt = 5;                  //Trig Pin
int echoPinRt = 4;                  //Echo Pin

float distanceFront = 0;
float distanceLeft = 0;
float distanceRight = 0;
float distanceBack = 0;
float distanceWall = 20;
float Temperature = 24;

double vL = 0;
double vR = 0;

int PWMA = 2;
int PWMB = 3;
int AIN1 = 15;
int AIN2 = 14;
int BIN1 = 16;
int BIN2 = 17;
int STBY = 18;

 
void setup() {
  Serial.begin (9600); 
  BT.begin(9600);
  pinMode(trigPinFr, OUTPUT);         
  pinMode(echoPinFr, INPUT);
  pinMode(trigPinLf, OUTPUT);        
  pinMode(echoPinLf, INPUT);
  pinMode(trigPinRt, OUTPUT);        
  pinMode(echoPinRt, INPUT);
  pinMode(trigPinBk, OUTPUT);        
  pinMode(echoPinBk, INPUT);
  
  pinMode(PWMA, OUTPUT);
  pinMode(STBY, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
}

float ranging(int trigPin ,int echoPin){
  float duration = 0;
  float distance = 0;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);     // 給 Trig 高電位，持續 10微秒
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  pinMode(echoPin, INPUT);             // 讀取 echo 的電位
  duration = pulseIn(echoPin, HIGH);   // 收到高電位時的時間
 
  distance = (duration/2) / (10000/(331.5+0.607*Temperature));         // 將時間換算成距離 cm 
  Serial.print("Distance=");
  Serial.print(distance);
  Serial.println("cm");
  return distance;
}

void parallelTracking(double distance , char dir){
  float Kp = 1;
  float error = 0;
  float errorRange = 10;
  int flag = 0;
  if(dir=='r'){
    error = distanceWall - distance;
    flag = 1;
  }
  else if(dir=='l'){
    error = distance - distanceWall;
    flag = -1;
  }
  if(error <= errorRange && error >= (-1)*errorRange){
    float correction = error * Kp;
    vL = vL - correction;
    vR = vR + correction;
  }
  else if(error < (-1)*errorRange){
    vL = vL + Kp*errorRange*flag;
    vR = vR - Kp*errorRange*flag;
  }
  else if(error > errorRange){
    vL = vL - Kp*errorRange*flag;
    vR = vR + Kp*errorRange*flag;
  }
}

void frontObstacle(float distanceFt) {
   float Vstraight = 150;
   vL = Vstraight;
   vR = Vstraight;
   float KST = 15;
   if(distanceFt<=distanceWall){
      vL = vL - (distanceWall - distanceFt)*KST;
      vR = vR - (distanceWall - distanceFt)*KST;
   }
}

void SpeedCheck() {
  if(vL >= 250){  
      vL = 250;
  }
  if(vL <= -250){  
      vL = -250;
  }
  if(vR >= 250){  
      vR = 250;
  }
  if(vR <= -250){  
      vR = -250;
  }
}

void MotorWriting(double vL , double vR) {
   digitalWrite(STBY, HIGH);
   if(vL<0) {
      digitalWrite(AIN1,HIGH);
      digitalWrite(AIN2,LOW);
      analogWrite(PWMA , (-1)*vL);
   }
   else if(vL>=0) {
      //向前轉
      digitalWrite(AIN1,LOW);
      digitalWrite(AIN2,HIGH);
      analogWrite(PWMA , vL);
   }
   if(vR<0){
      digitalWrite(BIN1,HIGH);
      digitalWrite(BIN2,LOW);
      analogWrite(PWMB , (-1)*vR);
   }
   else if(vR>=0){
      digitalWrite(BIN1,LOW);
      digitalWrite(BIN2,HIGH);
      analogWrite(PWMB , vR);
   }
}

void loop(){
  
  distanceFront = ranging(trigPinFr,echoPinFr);
  distanceLeft = ranging(trigPinLf,echoPinLf);
  distanceRight = ranging(trigPinRt,echoPinRt);
  distanceBack = ranging(trigPinBk,echoPinBk);

  
  if(distanceRight>=100) {
    MotorWriting(0 , 0);
    delay(200);
  }
  else {
    frontObstacle(distanceFront);
    parallelTracking(distanceRight , 'r');
    SpeedCheck();
    BT.print(distanceRight);
    BT.print(" /////// ");
    BT.print(vL);
    BT.print(" , ");
    BT.print(vR);
    BT.print("\n");
    MotorWriting(vL , vR);
    delay(20);
  }
}
  
