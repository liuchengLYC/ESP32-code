#include<MFRC522.h>
#include<SPI.h>
#include"RFID.h"
#include"move.h"
#include"bluetooth.h"

#define test1
#define test_var

#define RST_PIN 9
#define SS_PIN 53
#define PWMA 11
#define PWMB 12
#define AIN1 2
#define AIN2 3
#define BIN1 5
#define BIN2 6

int val[5];

int velocity = 150;
int kp = 4;
int kd = 5;
int gostraight = 600;  
int rspeed_fast = 170;
int rspeed_slow = 30;
int right_time = 390;
int lspeed_fast = 170;
int lspeed_slow = 30;
int left_time = 400;
int uspeed_r = 170;
int uspeed_l = 170;
int u_time = 450;
double porpotion = 2;

int error = 0; 
int prev_error = 0;
int delta = 0;
bool controled = false; 
char command;
MFRC522 mfrc522(SS_PIN, RST_PIN);
byte* UID = NULL;
byte ID_size;

void setup() {
  pinMode(32 , INPUT); // 目前預設該接腳作為輸入
  pinMode(34 , INPUT);
  pinMode(36 , INPUT);
  pinMode(38 , INPUT);
  pinMode(40 , INPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(AIN2, OUTPUT);
  //  pinMode(STBY, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  Serial.begin(9600); // 表示開始傳遞與接收序列埠資料
  Serial1.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();

  controled = false;
  command = '\0';
  #ifdef test_var
  while(Serial1.available() == 0){
    //沒事做
  }
  velocity = Serial1.readStringUntil('\n').toInt();
  delay(20);
  Serial1.println(velocity);
  kp = Serial1.readStringUntil('\n').toInt();
  delay(20);
  Serial1.println(kp);
  kd = Serial1.readStringUntil('\n').toInt();
  delay(20);
  Serial1.println(kd);
  gostraight = Serial1.readStringUntil('\n').toInt();
  delay(20);
  Serial1.println(gostraight);
  rspeed_fast = Serial1.readStringUntil('\n').toInt();
  delay(20);
  Serial1.println(rspeed_fast);
  rspeed_slow = Serial1.readStringUntil('\n').toInt();
  delay(20);
  Serial1.println(rspeed_slow);
  right_time = Serial1.readStringUntil('\n').toInt();
  delay(20);
  Serial1.println(right_time);
  lspeed_fast = Serial1.readStringUntil('\n').toInt();
  delay(20);
  Serial1.println(lspeed_fast);
  lspeed_slow = Serial1.readStringUntil('\n').toInt();
  delay(20);
  Serial1.println(lspeed_slow);
  left_time = Serial1.readStringUntil('\n').toInt();
  delay(20);
  Serial1.println(left_time);
  uspeed_r = Serial1.readStringUntil('\n').toInt();
  delay(20);
  Serial1.println(uspeed_r);
  uspeed_l = Serial1.readStringUntil('\n').toInt();
  delay(20);
  Serial1.println(uspeed_l);
  u_time = Serial1.readStringUntil('\n').toInt();
  delay(20);
  Serial1.println(u_time);
  porpotion = Serial1.readStringUntil('\n').toDouble();
  delay(20);
  Serial1.println(porpotion);
  #endif
}
//傳入格式 123\n456\n789\n

void follow_command(){
  if(command == 's'){
    controled = true;
    Serial1.println(command);
    command = '\0';
  }
  if(command == 'e'){
    controled = false;
    MotorWriting();
  }
  if(controled){
    for(int i=0; i<5; i++){
      val[i]=digitalRead(32+2*i);
    }
    UID = NULL;
    UID = RFID(ID_size);
    if(UID){
      send_byte(UID, ID_size);
    }
    if(meet_square()){
      Serial.println(command);
      switch(command){
        case 'f':
          go_straight();
          break;
        case 'r':
          turn('R');
          break;
        case 'l':
          turn('L');
          break;
        case 'u':
          turn('U');
          break;
        default:
          MotorWriting();
      }
      Serial1.println(command);
      command = '\0';
    }
    else{
      follow_track();
    }
  }
}

void loop(){
  receiver();
  follow_command();
}