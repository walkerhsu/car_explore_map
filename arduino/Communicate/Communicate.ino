#include <SoftwareSerial.h>
SoftwareSerial BT (13, 12);

#define RST_PIN 49
#define SS_PIN 53
char c2 = '\0';

void setup () {
  Serial.begin (9600);
  BT.begin(9600);
}
void loop () {
  while(BT.available()) {
    delay(5);
    c2=BT.read();
    Serial.print(c2);
  }
  while(Serial.available()) {
    c2=Serial.read();
    BT.write(c2);
  }
}
