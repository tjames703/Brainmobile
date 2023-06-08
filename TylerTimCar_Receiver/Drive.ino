/**********
  Written by Tyler James
**********/

//define the two direction logic pins and the speed / PWM pin
const int DIR_A_L = 27; //Works: 27, 32
const int DIR_B_L = 26; //Works: 26, 33      Bad: 25
const int PWM_L = 14;   //Works: 14, 12
const int DIR_A_R = 32; //Works: 27, 32
const int DIR_B_R = 33; //Works: 26, 33      Bad: 25
const int PWM_R = 12;   //Works: 14, 12

void driveInit(){
  //set pins for output and input
  pinMode(DIR_A_L, OUTPUT);
  pinMode(DIR_B_L, OUTPUT);
  pinMode(PWM_L, OUTPUT);
  pinMode(DIR_A_R, OUTPUT);
  pinMode(DIR_B_R, OUTPUT);
  pinMode(PWM_R, OUTPUT);

  digitalWrite(DIR_A_L, LOW);
  digitalWrite(DIR_B_L, LOW);
  analogWrite(PWM_L, 0);
  digitalWrite(DIR_A_R, LOW);
  digitalWrite(DIR_B_R, LOW);
  analogWrite(PWM_R, 0);
}

void driveTest() {
  //drive forward at full speed by pulling DIR_A High
  //and DIR_B low, while writing a full 255 to PWM to 
  //control speed
  digitalWrite(DIR_A_L, HIGH);
  digitalWrite(DIR_B_L, LOW);
  analogWrite(PWM_L, 255);
  digitalWrite(DIR_A_R, HIGH);
  digitalWrite(DIR_B_R, LOW);
  analogWrite(PWM_R, 255);

  //wait 1 second
  delay(1000);

  //Brake the motor by pulling both direction pins to 
  //the same state (in this case LOW). PWM doesn't matter
  //in a brake situation, but set as 0.
  digitalWrite(DIR_A_L, LOW);
  digitalWrite(DIR_B_L, LOW);
  analogWrite(PWM_L, 0);
  digitalWrite(DIR_A_R, LOW);
  digitalWrite(DIR_B_R, LOW);
  analogWrite(PWM_R, 0);

  //wait 1 second
  delay(1000);

  //change direction to reverse by flipping the states
  //of the direction pins from their forward state
  digitalWrite(DIR_A_L, LOW);
  digitalWrite(DIR_B_L, HIGH);
  analogWrite(PWM_L, 150);
  digitalWrite(DIR_A_R, LOW);
  digitalWrite(DIR_B_R, HIGH);
  analogWrite(PWM_R, 150);

  //wait 1 second
  delay(1000);

  //Brake again
  digitalWrite(DIR_A_L, LOW);
  digitalWrite(DIR_B_L, LOW);
  analogWrite(PWM_L, 0);
  digitalWrite(DIR_A_R, LOW);
  digitalWrite(DIR_B_R, LOW);
  analogWrite(PWM_R, 0);

  //wait 1 second
  delay(1000);
}

void driveTest(int speed){
  drive(1,speed,1);
  drive(0,0,1);
  drive(2,speed/2,1);
  drive(0,0,1);
  drive(3,speed,1);
  drive(0,0,1);
  drive(4,speed,1);
  drive(0,0,1);

}


void drive(int dir){
  if (dir==0){
    drive(dir, 0, 1);
  }
  drive(dir, 7 ,1);
}

/* 
Dir: 0 = park, 1 = fwd, 2 = rev, 3 = left, 4 = right

Speed: 0-9 (default = 7)

Dur = Duration (Seconds) (default = 1)
*/
void drive(int dir, int speed, int dur){
  driveDir(dir);
  driveSpeed(speed);
  delay(dur*1000);
}

void driveDir(int dir){
  if (dir == 0){
    digitalWrite(DIR_A_L, LOW);
    digitalWrite(DIR_B_L, LOW);
    digitalWrite(DIR_A_R, LOW);
    digitalWrite(DIR_B_R, LOW);
  }
  else if (dir == 1){
    digitalWrite(DIR_A_L, HIGH);
    digitalWrite(DIR_B_L, LOW);
    digitalWrite(DIR_A_R, HIGH);
    digitalWrite(DIR_B_R, LOW);
  }
  else if (dir == 2){
    digitalWrite(DIR_A_L, LOW);
    digitalWrite(DIR_B_L, HIGH);
    digitalWrite(DIR_A_R, LOW);
    digitalWrite(DIR_B_R, HIGH);
  }
  else if (dir == 3){
    digitalWrite(DIR_A_L, LOW);
    digitalWrite(DIR_B_L, HIGH);
    digitalWrite(DIR_A_R, HIGH);
    digitalWrite(DIR_B_R, LOW);
  }
  else if (dir == 4){
    digitalWrite(DIR_A_L, HIGH);
    digitalWrite(DIR_B_L, LOW);
    digitalWrite(DIR_A_R, LOW);
    digitalWrite(DIR_B_R, HIGH);
  }
}

void potSpeed(){
  driveSpeed(readPot()/410);
}

void driveSpeed(int speed){
  analogWrite(PWM_L, 25*speed);
  analogWrite(PWM_R, 25*speed);
}



