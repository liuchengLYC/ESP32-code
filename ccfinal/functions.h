#include"gps.h"
#define RED_LED 3
#define GREEN_LED 4
#define noisyPin 5
#define buzzerPin 6

int melody[] = {
  330, 294, 262, 294,   // 3 2 1 2
  330, 349, 330, 294, 0,   // 3- 4 3 2
  330, 294, 262, 294,
  330, 349, 330, 294
};

int duration[] = {
  400, 200, 400, 200, 300, 100, 200, 200, 100,
  400, 200, 400, 200, 300, 100, 200, 200
};

void toggle(bool& lock, bool& stolen, int& count){
  if(lock){
    lock = false;
    stolen = false;
    count = 0;
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(noisyPin, LOW);
    sendlocation();
  }else{
    lock = true;
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, HIGH);
  }
  delay(1000); //necessary?
}

void playAlert() {
  tone(buzzerPin, 300);
  delay(200);
  noTone(buzzerPin);
  delay(200);
  tone(buzzerPin, 300);
  delay(200);
  noTone(buzzerPin);
  delay(200);

  for (int i = 0; i < sizeof(melody) / sizeof(int); i++) {
    tone(buzzerPin, melody[i]);
    delay(duration[i] * 2);
  }
  noTone(buzzerPin);
}