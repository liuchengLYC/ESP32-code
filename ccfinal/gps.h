#ifndef GPS_H
#define GPS_H

#include <TinyGPSPlus.h>
extern TinyGPSPlus gps;

void sendlocation() {
  Serial.print("Latitude: ");
  Serial.print(gps.location.lat(), 6);
  Serial.print(" | Longitude: ");
  Serial.println(gps.location.lng(), 6);

  Serial2.println("start");
  Serial.println("start");

  // 等 ESP32 回 "connected"
  unsigned long start = millis();
  bool connected = false;
  while (millis() - start < 10000) {
    if (Serial2.available()) {
      String reply = Serial2.readStringUntil('\n');
      reply.trim();
      if (reply == "connected") {
        connected = true;
        break;
      } else {
        Serial.println(reply);
      }
    }

    while (Serial3.available()) {
      gps.encode(Serial3.read());
    }
  }
  if (!connected) {
    Serial.println("ESP32 no response.");
    return;
  }

  Serial.println("connected");

  String send_to_esp = String(gps.location.lat(), 6) + "," + String(gps.location.lng(), 6);
  Serial2.println(send_to_esp);
  Serial.println("sent");

  start = millis();
  bool done = false;
  while (millis() - start < 3000) {
    if (Serial2.available()) {
      String reply = Serial2.readStringUntil('\n');
      reply.trim();
      if (reply == "done") {
        Serial.println("ESP32 done, WiFi disconnected.");
        done = true;
        break;
      } else if (reply == "error") {
        Serial.println("ESP32 reported error.");
        break;
      }
    }

    while (Serial3.available()) {
      gps.encode(Serial3.read());
    }
  }

  if (!done) {
    Serial.println("ESP32 didn't answer done.");
  }

  Serial.println("done");
  delay(5000);
}
#endif
