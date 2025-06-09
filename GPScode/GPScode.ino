#include<TinyGPSPlus.h>
String send_to_esp;
TinyGPSPlus gps;
void setup() {
  Serial.begin(9600);   // 輸出到電腦（Serial Monitor）
  Serial2.begin(9600);  // 與 ESP32 通訊
  Serial3.begin(9600);  // 與 GPS 模組通訊（RX3）
}

void loop() {
  Serial.print("&");
  while (Serial3.available() > 0) {
    gps.encode(Serial3.read());
  }
  if (gps.location.isUpdated()) {
    send_to_esp = String(gps.location.lat(), 6) + "," + String(gps.location.lng(), 6);
    Serial2.println("start");
  }
  Serial.print("$");
  // 等 ESP32 回 "connected"
  while (true) {
    if (Serial2.available()) {
      String reply = Serial2.readStringUntil('\n');
      reply.trim();
      if (reply == "connected") {
        break; // 收到 connected，準備傳座標        
      }
    }
  }
  Serial.print("*");
  Serial2.println(send_to_esp);
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
  Serial.print("#");
  delay(1000);
}
