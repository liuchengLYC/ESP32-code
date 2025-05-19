#include <WiFi.h>
#include <HTTPClient.h>
#include "time.h"

const char* ssid = "liucheng";
const char* password = "20051102";
const char* serverURL = "https://gps-tracker-6ps0.onrender.com/api/gps";

const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 8 * 3600;
const int daylightOffset_sec = 0;

#define RXD2 16  // ESP32 的 RX2
#define TXD2 17  // 可不接，但若需要也可配置 TX2

void setup() {
  Serial.begin(115200);       // 給開發者監看訊息
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2); // 與 UNO 通訊

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  delay(1000);  // 等待時間同步完成
}

void loop() {
  if (Serial2.available()) {
    String gpsData = Serial2.readStringUntil('\n');
    gpsData.trim(); // 去除換行與空白

    double latitude, longitude;
    if (parseGPS(gpsData, latitude, longitude)) {
      sendToServer(latitude, longitude);
    } else {
      Serial.println("Invalid GPS data: " + gpsData);
    }
  }
}

bool parseGPS(String data, double& lat, double& lon) {
  int commaIndex = data.indexOf(',');
  if (commaIndex == -1) return false;

  String latStr = data.substring(0, commaIndex);
  String lonStr = data.substring(commaIndex + 1);

  lat = latStr.toDouble();
  lon = lonStr.toDouble();
  return true;
}

void sendToServer(double latitude, double longitude) {
  struct tm timeinfo;
  char isoTime[30] = "unknown";
  if (getLocalTime(&timeinfo)) {
    strftime(isoTime, sizeof(isoTime), "%Y-%m-%dT%H:%M:%S", &timeinfo);
  }

  HTTPClient http;
  http.begin(serverURL);
  http.addHeader("Content-Type", "application/json");

  String json = "{";
  json += "\"device_id\":\"bike_001\",";
  json += "\"latitude\":" + String(latitude, 6) + ",";
  json += "\"longitude\":" + String(longitude, 6) + ",";
  json += "\"timestamp\":\"" + String(isoTime) + "\"";
  json += "}";

  Serial.println("Sending JSON:");
  Serial.println(json);

  int httpCode = http.POST(json);
  if (httpCode > 0) {
    Serial.printf("Sent (%d): %s\n", httpCode, http.getString().c_str());
  } else {
    Serial.printf("POST failed: %d\n", httpCode);
  }

  http.end();
}
