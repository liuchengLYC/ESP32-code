#include<MFRC522.h>
#include<SPI.h>
#include"RFID.h"
#include"move.h"
#include"bluetooth.h"

#define test1
#define test_var
#define method2

#define RST_PIN 9
#define SS_PIN 53
#define PWMA 11
#define PWMB 12
#define AIN1 2
#define AIN2 3
#define BIN1 5
#define BIN2 6

int val[5];

int velocity = 100;
int kp = 5;
int kd = 2;
int time_go_straight = 600;
int turn_speed_fast = 100;
int turn_speed_slow = 20;
int turn_right = 400;
int turn_left = 400;
int turn_u = 800;


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
  delay(10);
  Serial1.println(velocity);
  kp = Serial1.readStringUntil('\n').toInt();
  delay(10);
  Serial1.println(kp);
  kd = Serial1.readStringUntil('\n').toInt();
  delay(10);
  Serial1.println(kd);
  time_go_straight = Serial1.readStringUntil('\n').toInt();
  delay(10);
  Serial1.println(time_go_straight);
  turn_speed_fast = Serial1.readStringUntil('\n').toInt();
  delay(10);
  Serial1.println(turn_speed_fast);
  turn_speed_slow = Serial1.readStringUntil('\n').toInt();
  delay(10);
  Serial1.println(turn_speed_slow);
  turn_right = Serial1.readStringUntil('\n').toInt();
  delay(10);
  Serial1.println(turn_right);
  turn_left = Serial1.readStringUntil('\n').toInt();
  delay(10);
  Serial1.println(turn_left);
  turn_u = Serial1.readStringUntil('\n').toInt();
  delay(10);
  Serial1.println(turn_u);
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
  delay(1);
}
