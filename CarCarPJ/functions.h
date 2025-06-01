//Buzzer的東西
#define HALL_PIN 7 

extern bool isHallAlertActive;
extern bool isRed;
bool magnetNear = false;
unsigned long awayStart = 0;
const unsigned long HALL_THRESHOLD = 1000;

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

void initMelody() {
  
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

void playHallAlert() {          
  if (isHallAlertActive) return; 
  isHallAlertActive = true;
  tone(buzzerPin, 1000);        
}

void stopHallAlert() {
  if (!isHallAlertActive) return;
  isHallAlertActive = false;
  noTone(buzzerPin);
}
//LED的東西

void toggleLED() {
  if (!isRed) {                             
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);
    Serial.println("紅燈亮起");
  } else {                                   
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, HIGH);
    Serial.println("綠燈亮起");
    stopHallAlert();                          //刷到有效卡解解除警報
  }
  isRed = !isRed;
  delay(1000);
}
//Hall的東西

void initHallSensor() {
  pinMode(HALL_PIN, INPUT_PULLUP);   
}

void updateHallSensor() {
  if (!isRed) return;       // 解鎖時不偵測

  bool nowNear = (digitalRead(HALL_PIN) == LOW);

  if (magnetNear && !nowNear) {
    awayStart = millis();
  }
  if (!magnetNear && nowNear) {
    unsigned long T = millis() - awayStart;
    if (T < HALL_THRESHOLD) {
      playHallAlert();               
    }
  }
  magnetNear = nowNear;
}