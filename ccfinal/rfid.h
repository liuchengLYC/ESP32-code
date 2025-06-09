const byte allowedUIDs[][4] = {
  {0xE5, 0x56, 0x37, 0x52},
  {0xA5, 0x5E, 0x25, 0x52},
  {0x35, 0x87, 0x22, 0x52}
};
const int numAllowed = sizeof(allowedUIDs) / sizeof(allowedUIDs[0]);

bool isAllowedUID(byte *uid) {
  for (int i = 0; i < numAllowed; i++) {
    bool match = true;
    for (int j = 0; j < 4; j++) {
      if (uid[j] != allowedUIDs[i][j]) {
        match = false;
        break;
      }
    }
    if (match) return true;
  }
  return false;
}
void printUID(byte *uid) {
  for (byte i = 0; i < 4; i++) {
    Serial.print("0x");
    if (uid[i] < 0x10) Serial.print("0");
    Serial.print(uid[i], HEX);
    if (i < 3) Serial.print(" ");
  }
  Serial.println();
}