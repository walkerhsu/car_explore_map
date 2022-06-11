#include <SoftwareSerial.h>
SoftwareSerial BT (13, 12);

#define RST_PIN 49
#define SS_PIN 53

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
float distanceWall = 15;
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

int len = 0;
String dir;
char c1,c2;
char c;
String cango = "0000";
int curdir = 0;
bool start = true;
 
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
  //Serial.print("Distance=");
  Serial.print(distance);
  Serial.println("cm");
  return distance;
}

void parallelTracking(double distanceL , double distanceR){
  Serial.print("In parallelTracking!\n");
  vL = 80;
  vR = 80;
  float Kp = 5;
  float error = 0;
  int flag = 0;
  error = distanceL - distanceR;
  //Serial.print(error);
  //Serial.print("\n");
  float correction = error * Kp;
  vL = vL - correction;
  vR = vR + correction;
//  Serial.print(vL);
//  Serial.print("\n");
//  Serial.print(vR);
//  Serial.print("\n");
  MotorWriting(vL , vR);
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

void checkwhereCanGo(int curdir){
  int threshold = 40;
  distanceFront = ranging(trigPinFr,echoPinFr);
  distanceLeft = ranging(trigPinLf,echoPinLf);
  distanceRight = ranging(trigPinRt,echoPinRt);
  distanceBack = ranging(trigPinBk,echoPinBk);
  if(distanceFront >= threshold){
    cango[(0+curdir)%4] = '1';
  }
  else{
    cango[(0+curdir)%4] = '0';
  }
  if(distanceRight >= threshold){
    cango[(1+curdir)%4] = '1';
  }
  else{
    cango[(1+curdir)%4] = '0';
  }
  if(distanceBack >= threshold){
    cango[(2+curdir)%4] = '1';
  }
  else{
    cango[(2+curdir)%4] = '0';
  }
  if(distanceLeft >= threshold){
    cango[(3+curdir)%4] = '1';
  }
  else{
    cango[(3+curdir)%4] = '0';
  }
  Serial.print(cango);
  BT.print(cango+'\n');
}

void checkHowToGo(String dir , int curstep){
  int VSL = 80;
  int VSR = 80;
  int movement = dir[curstep] - curdir;
  curdir = dir[curstep];
  if((movement == 0)){
    // 直走
    c = 'S';
    Serial.print('S');
    Serial.print("\n");
    MotorWriting( 0 ,0 );
    delay(1000);
  }
  else if((movement == -1) || (movement == 3)) {
    // 左轉
    c = 'L';
    Serial.print('L');
    Serial.print("\n");
    MotorWriting( -80 ,80 );
    delay(860);
  }
  else if((movement == 1) || (movement == -3)) {
    // 右轉
    c = 'R';
    Serial.print('R');
    Serial.print("\n");
    MotorWriting( 80 ,-80 );
    delay(860);
  }
  else if((movement == 2) || (movement == -2)) {
    //迴轉
    MotorWriting( 0 , 0 );
    delay(160);
    MotorWriting( 80 ,-80 );
    delay(1800);
    MotorWriting( 0 , 0 );
    delay(160);
    c = 'B';
    Serial.print('B');
    Serial.print("\n");
  }
  else{
    //靜止
    c = 'N';
    MotorWriting(0,0);
  }
}
  
void BTtoSerial() {
  dir = "";
  while(BT.available()) {
    c2=BT.read();
    dir=dir+c2;
    Serial.print(c2);
   }
  //Serial.write(len);
  len = dir.length();
}

void SerialtoBT() {
  while(Serial.available()) {
    c1=Serial.read();
    BT.write(c1);
  }
}

void loop(){
  if (start == true){
    while(len==0) BTtoSerial(); 
    checkwhereCanGo(curdir);
    len = 0;
    while(len==0) BTtoSerial(); 
    start = false;
  }
  for(int i=0; i<len; i++){
    //Serial.print("im");
    float disAtFirst = 0 , disNow = 0;
    checkHowToGo(dir , i);
    Serial.print("distanceback : "); 
    disNow = ranging(trigPinBk,echoPinBk);
    Serial.print("distanceLeft : "); 
    distanceLeft = ranging(trigPinLf,echoPinLf);
    Serial.print("distanceRight : "); 
    distanceRight = ranging(trigPinRt,echoPinRt);
    distanceFront = ranging(trigPinFr,echoPinFr);
    disAtFirst = disNow;
    while(distanceFront>=20) { //disNow-disAtFirst<=40 && //distanceRight<=30 && distanceLeft<=30 &&
      //parallelTracking(distanceLeft , distanceRight);
      MotorWriting(80 , 80);
      SerialtoBT();
      delay(10);
      //Serial.print("distanceback : "); 
      disNow = ranging(trigPinBk,echoPinBk);
      Serial.print("distancefront : "); 
      distanceFront = ranging(trigPinFr,echoPinFr);
      //distanceLeft = ranging(trigPinLf,echoPinLf);
      //distanceRight = ranging(trigPinRt,echoPinRt);
//      parallelTracking(distanceRight , 'r')
    }
  }
  //Serial.print("out of loop");
  //MotorWriting(0 , 0);
  //delay(1000);
  checkwhereCanGo(curdir);
  len = 0;
  while(len==0) BTtoSerial();
  //MotorWriting(0 , 0);
  //delay(1000);
  
}
  

//  
//  if(distanceRight>=100) {
//    MotorWriting(0 , 0);
//    delay(200);
//  }
//  else if(){
//    
//  }
//  else {
//    frontObstacle(distanceFront);
//    parallelTracking(distanceRight , 'r');
//    SpeedCheck();
//    BT.print(distanceRight);
//    MotorWriting(vL , vR);
//    delay(20);
//  }
//}


  
