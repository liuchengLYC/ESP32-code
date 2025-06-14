#include <TinyGPSPlus.h>
#define test
TinyGPSPlus gps;

String inputBuffer = "";  // 新增，用來存 Serial 的指令

#ifdef test
void setup() {
  Serial.begin(9600);   // 輸出到電腦（Serial Monitor）
  Serial2.begin(9600);  // 與 ESP32 通訊
  Serial3.begin(9600);  // 與 GPS 模組通訊（RX3）
}

void loop() {
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

#ifndef test
void setup() {
  Serial.begin(9600);
  Serial2.begin(9600);
}
void loop() {
  // 先送 start 給 ESP32
  if (Serial.readStringUntil('\n') == "s") {
    while (Serial.available()) Serial.read();
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
    //String gpsData = String(gps.location.lat(), 6) + "," + String(gps.location.lng(), 6);
    String gpsData = "120.1314, 25.520";
    Serial2.println(gpsData);
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
    // 你可以選擇延遲一段時間再開始下一次
    delay(5000);
  }
}
#endif