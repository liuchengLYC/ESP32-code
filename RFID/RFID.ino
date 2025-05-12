#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 9
#define SS_PIN 53
#define test

MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(9600);    
  Serial1.begin(9600);   
  SPI.begin();

  mfrc522.PCD_Init(); 
  Serial.println("RFID reader is ready to read.");
}

void loop() {
  // 1. 檢查是否有新卡
  if (mfrc522.PICC_IsNewCardPresent()) {
    // 2. 讀卡，成功後才做事
    if (mfrc522.PICC_ReadCardSerial()) {
      #ifdef test
      Serial.print("Card UID: ");
      for (byte i = 0; i < mfrc522.uid.size; i++) {
        // 若小於 0x10，前面補 0
        // 以 16 進位方式印出
        Serial.print(mfrc522.uid.uidByte[i], HEX);
        // 如果不是最後一個 Byte，就用空格或逗號分隔
        if (i < mfrc522.uid.size - 1) {
          Serial.print(" ");
        }
      }
      Serial.println();
      #endif
      #ifndef test
      Serial.println("**Card Detected**");

      // 讀出卡片 UID (以十六進位輸出)
      Serial.print("Card UID: ");
      for (byte i = 0; i < mfrc522.uid.size; i++) {
        // 若小於 0x10，前面補 0
        if (mfrc522.uid.uidByte[i] < 0x10) {
          Serial.print("0");
        }
        // 以 16 進位方式印出
        Serial.print(mfrc522.uid.uidByte[i], HEX);
        // 如果不是最後一個 Byte，就用空格或逗號分隔
        if (i < mfrc522.uid.size - 1) {
          Serial.print(" ");
        }
      }
      Serial.println();
      
      // 同步透過藍牙傳送 (如果需要)
      Serial1.print("Card UID: ");
      for (byte i = 0; i < mfrc522.uid.size; i++) {
        if (mfrc522.uid.uidByte[i] < 0x10) {
          Serial1.print("0");
        }
        Serial1.print(mfrc522.uid.uidByte[i], HEX);
        if (i < mfrc522.uid.size - 1) {
          Serial1.print(" ");
        }
      }
      Serial1.println();
      #endif
      // 讓卡片進入停止狀態並停止加密
      mfrc522.PICC_HaltA();
      mfrc522.PCD_StopCrypto1();
    }
  }
}
