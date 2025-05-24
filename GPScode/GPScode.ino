#include<TinyGPSPlus.h>

TinyGPSPlus gps;
void setup() {
  Serial.begin(9600);   // 輸出到電腦（Serial Monitor）
  Serial2.begin(9600);  // 與 ESP32 通訊
  Serial3.begin(9600);  // 與 GPS 模組通訊（RX3）
}

void loop() {
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
