#define PWMA 11
#define PWMB 12
#define AIN1 2
#define AIN2 3
#define BIN1 5
#define BIN2 6
#define method2

int extern velocity;
int extern kp;
int extern kd;
int extern gostraight;
int extern rspeed_fast;
int extern rspeed_slow;
int extern right_time;
int extern lspeed_fast;
int extern lspeed_slow;
int extern left_time;
int extern uspeed_r;
int extern uspeed_l;
int extern u_time;
double extern porpotion;

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
    MotorWriting(velocity + delta, velocity - delta);
}

inline bool meet_square(){
  return ((val[1]&&val[2]&&val[3] && (val[0] || val[4])));
}

void go_straight(){
  MotorWriting(velocity, velocity - 20);
  delay(gostraight);
  MotorWriting();
  delay(50);
}

void turn(char type){
  if(type == 'L'){
    MotorWriting(lspeed_slow, lspeed_fast);
    delay(left_time);
    MotorWriting(lspeed_slow / porpotion, lspeed_fast / porpotion);
    while(!digitalRead(36)){
      //Serial1.println("Ldone");
    }
  }
  else if(type == 'R'){
    MotorWriting(rspeed_fast, rspeed_slow);
    delay(right_time);
    MotorWriting(rspeed_fast / porpotion, rspeed_slow / porpotion);
    while(!digitalRead(36)){
      //Serial1.println("Rdone");
    }
  }
  else{
    MotorWriting(uspeed_l, -1 * uspeed_r);
    delay(u_time);
    MotorWriting(uspeed_l / (porpotion + 0.5), -1 * uspeed_r / (porpotion + 0.5));
    while(!digitalRead(36)){
      //Serial1.println("Udone");
    }
  }
  MotorWriting();
  delay(100);
}
