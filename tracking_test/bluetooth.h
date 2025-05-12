char extern command;

inline void receiver(){
  if(Serial1.available()){
    command = Serial1.read();
  }
}

// send UID back through Serial1(bluetooth serial)
void send_byte(byte* id, byte& idSize) {
    //Serial1.print("ID[");
    for (byte i = 0; i < idSize; i++) {
          if (mfrc522.uid.uidByte[i] < 0x10) {
            Serial1.print("0");
          }
          Serial1.print(mfrc522.uid.uidByte[i], HEX);
    }
    Serial1.println();
    //Serial1.println("]");
}  // send_byte