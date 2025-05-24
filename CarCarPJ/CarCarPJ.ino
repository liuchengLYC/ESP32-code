#include <TinyGPSPlus.h>
#include <SPI.h>
#include <MFRC522.h>
#include"RFID.h"

#define SS_PIN 53
#define RST_PIN 49
#define buzzerPin 6
#define RED_LED 3
#define GREEN_LED 4

MFRC522 mfrc522(SS_PIN, RST_PIN);
TinyGPSPlus gps;
String send_to_esp;

void setup() {
  //預留Serial1給hall sensor
  Serial.begin(9600);   // 輸出到電腦（Serial Monitor）
  Serial2.begin(9600);  // 與 ESP32 通訊
  Serial3.begin(9600);  // 與 GPS 模組通訊（RX3）

  SPI.begin();
  mfrc522.PCD_Init();

  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, LOW);

  initMelody(); 
  initHallSensor(); 
}

void loop() {
  updateHallSensor();

  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) return;

  if (isAllowedUID(mfrc522.uid.uidByte)) {
    toggleLED();
  } else {
    Serial.print("不允許的卡片 UID：");
    printUID(mfrc522.uid.uidByte);
    playAlert(); 
  }

  mfrc522.PICC_HaltA(); // 停止讀卡
  // 讀取 GPS 模組資料
  while (Serial3.available() > 0) {
    gps.encode(Serial3.read());
  }

  // 當資料更新時，顯示經緯度
  if (gps.location.isUpdated()) {
    Serial.print("Latitude: ");
    Serial.print(gps.location.lat(), 6);
    Serial.print(" | Longitude: ");
    Serial.println(gps.location.lng(), 6);
    send_to_esp = String(gps.location.lat(), 6) + "," + String(gps.location.lng(), 6);
    Serial.println(send_to_esp);
    Serial2.println(send_to_esp);
    delay(1000);
  }
}