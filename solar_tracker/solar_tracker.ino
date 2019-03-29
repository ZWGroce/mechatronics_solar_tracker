#include <Servo.h>  //Includes a library for servo motor functionality
#include <Wire.h>  //Includes a library to utilize I2C
//Task: Make a solar tracker
Servo vert;  //Creates an object for the servo controlling the pitch
Servo horz;  //Creates an object for the servo controlling the yaw
 
//*****Define Variables*****
const int t_left = A0, t_right = A1, b_left = A2, b_right = A3;  //LDR Matrix pins

float tleft, tright, bleft, bright;  //raw LDR values
float tleftMax, trightMax, bleftMax, brightMax;

float top, bottom, left, right;  
int twitchBuffer = 3;
int vertPosition = 30, horzPosition = 105;

bool pitch = LOW, yaw = LOW;  //True means increment, false means deccrement

const int ledPin = 7;

int startTime;

//*****Program Initialization*****
void setup() {
  vert.attach(9);
  horz.attach(10);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  vert.write(vertPosition);
  horz.write(horzPosition);
  
  Serial.begin(9600);
  delay(1000);

  startTime = millis();  //Updates the start time
  while((millis()-startTime) < 4000){  //Runs while the time since starting calibration is less than 7 seconds
    tleft = analogRead(t_left);  //Reads the value of the left LDR
    tright = analogRead(t_right);  //Reads the value of the center LDR
    bleft = analogRead(b_left);  //Reads the value of the right LDR
    bright = analogRead(b_right);

    if(tleft > tleftMax){  //Triggers if the left LDR is greater than the maximum value (starts at 0)
      tleftMax = tleft;  //Sets the maximum value equal to the current (larger) LDR reading
    }

    if(tright > trightMax){  //Triggers if the right LDR is greater than the maximum value (starts at 0)
      trightMax = tright;  //Sets the maximum value equal to the current (larger) LDR reading  
    }

    if(bleft > bleftMax){  //Triggers if the left LDR is greater than the maximum value (starts at 0)
      bleftMax = bleft;  //Sets the maximum value equal to the current (larger) LDR reading
    }

    if(bright > brightMax){  //Triggers if the right LDR is greater than the maximum value (starts at 0)
      brightMax = bright;  //Sets the maximum value equal to the current (larger) LDR reading  
    }
    
    sweep();
    
    delay(20);  //Halts the program for 20 milliseconds
  }

  tleftMax += 5;
  trightMax += 5;
  bleftMax += 5;
  brightMax += 5;
  
  delay(1000);
}

//*****Main Program*****
void loop() {
  tleft = analogRead(t_left);
  tright = analogRead(t_right);
  bleft = analogRead(b_left);
  bright = analogRead(b_right);

  if(tleft <= tleftMax && tright <= trightMax && bleft <= bleftMax && bright <= brightMax){
    digitalWrite(ledPin, LOW);
    sweep();
    digitalWrite(ledPin, HIGH);
    delay(5);
    digitalWrite(ledPin, LOW);
    delay(10);
  }else{
    track();
    digitalWrite(ledPin, HIGH);
    delay(10);
  }
}

void sweep(){  
  if(pitch == HIGH){
    vertPosition += 1;
    if(vertPosition >= 70){
      vertPosition = constrain(vertPosition, 0, 180);
      pitch = LOW;
    }
  }else{
    vertPosition -= 1;
    if(vertPosition <= 5){
      vertPosition = constrain(vertPosition, 0, 180);
      pitch = HIGH;
    }  
  }

  if(yaw == HIGH){
    horzPosition += 2;
    if(horzPosition >= 150){
      horzPosition = constrain(horzPosition, 0, 180);
      yaw = LOW;
    }
  }else{
    horzPosition -= 2;
    if(horzPosition <= 60){
      horzPosition = constrain(horzPosition, 0, 180);
      yaw = HIGH;
    }  
  }

  vert.write(vertPosition);
  horz.write(horzPosition);
}

void track(){
  top = (tleft + tright)/2;
  bottom = (bleft + bright)/2;
  left = (tleft + bleft)/2;
  right = (tright + bright)/2;
   
  if(abs(top - bottom) > twitchBuffer){
    if(top > bottom){
      vertPosition--;
      vertPosition = constrain(vertPosition, 5, 100);
      vert.write(vertPosition);    
    }else{
      vertPosition++;
      vertPosition = constrain(vertPosition, 5, 100);
      vert.write(vertPosition);  
    }  
  }

  if(abs(left - right) > twitchBuffer){
    if(left > right){
      horzPosition++;
      horzPosition = constrain(horzPosition, 0, 180);
      horz.write(horzPosition); 
    }else{
      horzPosition--;
      horzPosition = constrain(horzPosition, 0, 180);
      horz.write(horzPosition);  
    }
  }  
}
