#include<SPI.h>
#include<MFRC522.h>
#include<TinyGPSPlus.h>
#include"gps.h"
#include"rfid.h"
#include"functions.h"

#define RED_LED 3
#define GREEN_LED 4
#define noisyPin 5 //high dB
#define buzzerPin 6
#define hallPin 7
#define RST_PIN 49
#define SS_PIN 53

MFRC522 mfrc522(SS_PIN, RST_PIN);
TinyGPSPlus gps;

bool lock = true;
bool stolen = false;
bool pending = false;
unsigned long time = 0;
unsigned long threshold = 1000;
int count = 0;

void setup() {
  Serial.begin(9600);   // 輸出到電腦（Serial Monitor）
  Serial2.begin(9600);  // 與 ESP32 通訊
  Serial3.begin(9600);  // 與 GPS 模組通訊（RX3）

  SPI.begin();
  mfrc522.PCD_Init();

  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(noisyPin, OUTPUT);
  pinMode(hallPin, INPUT);
  digitalWrite(RED_LED, HIGH);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(noisyPin, LOW);
}

void loop() {
  while (Serial3.available() > 0) {
    gps.encode(Serial3.read());
  }
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()){
    if(isAllowedUID(mfrc522.uid.uidByte)){
      toggle(lock, stolen, count);
      if(lock){
        pending = true;
      }
      printUID(mfrc522.uid.uidByte);
    }else{
      playAlert();
      printUID(mfrc522.uid.uidByte);
    }
    delay(1000);
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
  }
  if(pending && gps.location.isValid() && gps.location.age() < 2000){
    sendlocation();
    pending = false;
  }
  if(lock){
    if(!digitalRead(hallPin)){
      while(!digitalRead(hallPin)){}
      unsigned long interval = millis() - time;
      if(interval < threshold){
        if(++count > 10)
          stolen = true;
        Serial.println(count);
      }else{
        stolen = false;
      }
      time = millis();
    }
    if(stolen){
      digitalWrite(noisyPin, HIGH);
      //tone(buzzerPin, 300);
    }else{
      digitalWrite(noisyPin, LOW);
    }
  }else{
    //
  }
}