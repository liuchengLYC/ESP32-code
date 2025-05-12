#include <MFRC522.h>  // 引用程式庫
#include <SPI.h>
MFRC522 extern mfrc522;
//原版
byte* RFID(byte& idSize) {
    // 確認是否有新卡片
    if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
        byte* id = mfrc522.uid.uidByte;  // 取得卡片的UID
        idSize = mfrc522.uid.size;       // 取得UID的長度
#ifdef DEBUG
        Serial.print("UID Size: ");  // 顯示卡片的UID長度值
        Serial.println(idSize);
        for (byte i = 0; i < idSize; i++) {  // 逐一顯示UID碼
            Serial.print("id[");
            Serial.print(i);
            Serial.print("]: ");
            Serial.println(id[i], HEX);  // 以16進位顯示UID值
        }
        Serial.println();
#endif
        mfrc522.PICC_HaltA();  // 讓卡片進入停止模式
        return id;
    }
    return NULL;
}
