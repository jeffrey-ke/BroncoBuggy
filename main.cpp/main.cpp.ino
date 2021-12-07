#include <Servo.h>


/*Headlights*/
#define H_LITE 4
/*Blinkers*/
#define R_BLK 2
#define L_BLK 8
/*Backlights*/
#define B_LITE 5
/*Button*/
#define BUTTON_POWER 6
#define BUTTON_INPUT 7
/*Photoresistor*/
#define IN_PHORES A0
/*Piezo*/
#define OUT_PIZ 3
/*Servos*/
Servo leftServo, rightServo;
#define LSSp 10
#define RSSp 9
/*directions*/
#define left 1
#define right 0

void setup() {
  //here I have to set up all my pins:
  /* Headlights */
  pinMode(H_LITE, OUTPUT);

  /* Blinkers */
  pinMode(R_BLK, OUTPUT);
  pinMode(L_BLK, OUTPUT);

  /* Backlights */
  pinMode(B_LITE, OUTPUT);

  /* Button */
  pinMode(BUTTON_POWER, OUTPUT); 
  digitalWrite(BUTTON_POWER, HIGH); //power for button should always be on.

  pinMode(BUTTON_INPUT, INPUT);

  /* Photoresistor */
  pinMode(IN_PHORES, INPUT);

  /* Piezo */
  pinMode(OUT_PIZ, OUTPUT);
  
  /* Left Servo */
  leftServo.attach(LSSp);
  
  /* Right Servo */
  rightServo.attach(RSSp);

  Serial.begin(9600);

  
}

//Note: forward is R0 and L180
void loop() {
  if(digitalRead(BUTTON_INPUT) == HIGH){
    startTest();
  }
}

void startTest(){
  forward(1000);
}

void forward(int duration){
  rightServo.write(0);
  leftServo.write(180);
  for(int i = 0; i < duration%1000; i++){
    delay(1000);
    if(isDark()){
      digitalWrite(H_LITE, HIGH);
    }else {
      delay(1000);
      duration -= 1000;
      digitalWrite(H_LITE, LOW);
    }
  }
  rightServo.write(90);
  leftServo.write(90);
}

void backUp(int duration){
  rightServo.write(0);
  leftServo.write(180);
  for(int i = 0; i < duration/400; i++){
    tone(OUT_PIZ, 200);
    delay(200);
    noTone(OUT_PIZ);
    delay(200);
  }
  delay(duration%400);
  rightServo.write(90);
  leftServo.write(90);
}

void turn(int turnDir, int duration){
  digitalWrite(L_BLK, LOW);
  digitalWrite(R_BLK, LOW);
  if(turnDir == left){
    rightServo.write(0);
    leftServo.write(90); //stops left wheel
  } else {
    leftServo.write(180);
    rightServo.write(90); //stops right wheel
  }
  for(int i = 0; i < duration/400; i++){
    digitalWrite((turnDir == left)? L_BLK : R_BLK, HIGH);
    delay(200);
    digitalWrite((turnDir == left)? L_BLK : R_BLK, LOW);
    delay(200);
  }
  delay(duration%400); //delay for the remainder of the time
  leftServo.write(90); //stops both motors
  rightServo.write(90);
}

int isDark(){
  float lightLevel = analogRead(IN_PHORES);
  Serial.println(lightLevel);
  if(lightLevel < 200.0){
    return 1;
  } else {
    return 0;
  }
}
