#include <WiFi.h>

const char* ssid = "TestWiFi";       // ← 請改成你的 Wi-Fi 名稱（英文數字）
const char* password = "12345678";   // ← 請改成你的 Wi-Fi 密碼

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);         // 設定為「工作站模式」
  WiFi.begin(ssid, password);  // 嘗試連接

  int retry = 0;
  while (WiFi.status() != WL_CONNECTED && retry < 20) {
    delay(500);
    Serial.print(".");
    retry++;
  }

  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("WiFi connected!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.print("RSSI: ");
    Serial.print(WiFi.RSSI());
    Serial.println(" dBm");
  } else {
    Serial.println("WiFi connection failed!");
  }
}

void loop() {
  // 可選：每 5 秒顯示一次 RSSI
  delay(5000);
  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("RSSI now: ");
    Serial.print(WiFi.RSSI());
    Serial.println(" dBm");
  }
}
