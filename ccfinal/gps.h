#ifndef GPS_H
#define GPS_H

#include <TinyGPSPlus.h>

TinyGPSPlus gps;
String inputBuffer = "";

void sendlocation() {
  // 持續處理 GPS 資料
  while (Serial3.available() > 0) {
    gps.encode(Serial3.read());
  }

  // 非阻塞讀取 Serial 指令
  while (Serial.available() > 0) {
    char c = Serial.read();
    if (c == '\n' || c == '\r') {
      inputBuffer.trim();
      if (inputBuffer == "s") {
        // 執行 start 流程
        if (gps.location.isUpdated()) {
          Serial2.println("start");
          Serial.println("start");

          // 等 ESP32 回 "connected"
          while (true) {
            if (Serial2.available()) {
              String reply = Serial2.readStringUntil('\n');
              reply.trim();
              if (reply == "connected") {
                break;  // 收到 connected，準備傳座標
              }
            }
          }

          Serial.println("connected");

          // 傳 GPS 座標
          String send_to_esp = String(gps.location.lat(), 6) + "," + String(gps.location.lng(), 6);
          Serial2.println(send_to_esp);
          Serial.println("sent");

          // 等 ESP32 回 "done"
          while (true) {
            if (Serial2.available()) {
              String reply = Serial2.readStringUntil('\n');
              reply.trim();
              if (reply == "done") {
                Serial.println("ESP32 done, WiFi disconnected.");
                break;
              } else if (reply == "error") {
                Serial.println("ESP32 reported error.");
                break;
              }
            }
          }

          Serial.println("done");
          delay(5000);
        } else {
          Serial.println("GPS not ready yet.");
        }
      }
      inputBuffer = "";  // 清空指令 buffer
    } else {
      inputBuffer += c;  // 累積字元到 inputBuffer
    }
  }
}
#endif