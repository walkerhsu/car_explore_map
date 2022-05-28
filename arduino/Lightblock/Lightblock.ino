int photoint = 45; // 光遮斷器訊號輸出
int sensorRead = 0;
int lastSensorRead = 0;
int loops = 0;
int PWMA = 2;
int PWMB = 3;
int AIN1 = 15;
int AIN2 = 14;
int BIN1 = 16;
int BIN2 = 17;
int STBY = 18;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);          //Arduino 和電腦資料傳輸速率為 每秒 9600 bits
  pinMode(photoint, INPUT);    //Arduino 從 pin 2 讀入光遮斷器的訊號
  pinMode(PWMA, OUTPUT);
  pinMode(STBY, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  MotorWriting(50 , 50);
  sensorRead = digitalRead(photoint);       //讀取光遮斷器訊號，存入變數
  if (sensorRead == 1 && lastSensorRead == 0) {
    loops = loops + 1;
    Serial.print("turning loops : ");
    Serial.print(loops);
    Serial.print("\n");
  }
  lastSensorRead = sensorRead;
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
