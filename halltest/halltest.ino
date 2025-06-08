#define HALL_PIN 7   // DO 腳位
unsigned long time = 0; 
void setup() {
  Serial.begin(9600);         
  pinMode(HALL_PIN, INPUT);   
}

void loop() {
  int state = digitalRead(HALL_PIN); // 讀取霍爾感測器狀態
  if(state == 1){
    while(digitalRead(HALL_PIN)){}
    Serial.println(millis() - time);
    time = millis();
  }
}