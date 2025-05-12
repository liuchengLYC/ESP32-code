#include<SoftwareSerial.h>
SoftwareSerial BT(18,19);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  BT.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available()) {
    while (Serial.available()) {
      BT.write(Serial.read());
      // delay(30);
    }
    BT.flush();
  }
  // Serial.println(BT.available());
  // if(BT.available()){
  //   while (BT.available()) {
  //     Serial.write(BT.read());
  //     delay(30);
  //   }
  //   Serial.flush();
  //   //delay(1000);
  // }
}
