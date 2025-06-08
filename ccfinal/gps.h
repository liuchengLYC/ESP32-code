#ifndef GPS_H
#define GPS_H
#include<TinyGPSPlus.h>
TinyGPSPlus gps;

void sendlocation(){
  while (Serial3.available() > 0) {
    gps.encode(Serial3.read());
  }
  if (gps.location.isUpdated()) {
    String send_to_esp = String(gps.location.lat(), 6) + "," + String(gps.location.lng(), 6);
    Serial2.println("start");

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
    delay(1000);
  }
}
#endif