#define PWMA 11
#define PWMB 12
#define AIN1 2
#define AIN2 3
#define BIN1 5
#define BIN2 6

int extern velocity;
int extern kp;
int extern kd;
int extern time_go_straight;
int extern turn_speed_fast;
int extern turn_speed_slow;
int extern turn_right;
int extern turn_left;
int extern turn_u;

int extern val[5];
int extern error;
int extern prev_error;
int extern delta;
bool extern controled;
char extern command;

void MotorWriting(double vL=0, double vR=0){
  if(vL >= 0){
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, HIGH);
    analogWrite(PWMA,vL);
  } else if(vL < 0){
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);
    analogWrite(PWMA, -vL);
  }

  if(vR >= 0){
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, HIGH);
    analogWrite(PWMB, vR);
  } else if(vR < 0){
    digitalWrite(BIN1, HIGH);
    digitalWrite(BIN2, LOW);
    analogWrite(PWMB, -vR);
  }
}

inline void follow_track(){
  error = (-10) * val[0] + (-6) * val[1] + 6 * val[3] + 10 * val[4];
  delta = error * kp + (error - prev_error) * kd;
  prev_error = error;
  if(velocity + delta > 255 )
    MotorWriting(254 , velocity - delta); 
  else if(velocity - delta > 255)
    MotorWriting(velocity + delta, 254);
  else 
  // 10 8都是參數要調整
    MotorWriting(velocity + delta, velocity - delta);
}

inline bool meet_square(){
  return (val[0]&&val[1]&&val[2]&&val[3]&&val[4]);
}

void go_straight(){
  MotorWriting(velocity, velocity);
  delay(time_go_straight);
}

void turn(char type){
  #ifdef method1
  //用寫死的
  if(type == 'L'){
    MotorWriting(turn_speed_slow, turn_speed_fast);
    delay(turn_left);
  }else if(type == 'R'){
    MotorWriting(turn_speed_fast, turn_speed_slow);
    delay(turn_right);
  }else{
    MotorWriting(turn_speed_fast, -1 * turn_speed_fast);
    delay(turn_u);
  }
  if(digitalRead(32) == digitalRead(34) == digitalRead(36) == digitalRead(38) == digitalRead(40) == 0){
    if(type == 'L'){
      while(digitalRead(32)){}
    }
    else{
      while(digitalRead(40)){}
    }
  }
  #endif
  #ifdef method2
  //試看看用偵測的
  if(type == 'L'){
    MotorWriting(turn_speed_slow, turn_speed_fast);
    delay(turn_left);
    MotorWriting(turn_speed_slow / 2, turn_speed_fast / 2);
    while(digitalRead(34)){}
  }
  else if(type == 'R'){
    MotorWriting(turn_speed_fast, turn_speed_slow);
    delay(turn_right);
    MotorWriting(turn_speed_slow / 2, turn_speed_fast / 2);
    while(digitalRead(38)){}
  }
  else{
    MotorWriting(turn_speed_fast, -1 * turn_speed_fast);
    delay(turn_u);
    MotorWriting(turn_speed_fast / 2, -1 * turn_speed_fast / 2);
    while(digitalRead(38)){}
  }
  #endif
}
