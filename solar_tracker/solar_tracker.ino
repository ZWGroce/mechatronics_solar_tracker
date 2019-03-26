#include <Servo.h>
#include <Wire.h>
//Task: Make a solar tracker
Servo vert;
Servo horz;
 
//*****Define Variables*****
const int t_left = A0, t_right = A1, b_left = A2, b_right = A3;  //LDR Matrix pins

//LDR filtering stuff
float fc = 1, h = 0.02;  //fc = cutoff frequency [Hz], h = sampling time [s]
float wc = 2*PI*fc, a = 1-wc*h, b = wc*h;  //wc = fc in rad/s

float tleft, tright, bleft, bright;  //raw LDR values 
float tleftf, trightf, bleftf, brightf;  //filtered LDR values

float top, bottom, left, right;  
int twitchBuffer = 2;
int vertPosition = 100, horzPosition = 90;

//*****Program Initialization*****
void setup() {
  vert.attach(9);
  horz.attach(10);
  pinMode(t_left, INPUT);
  pinMode(t_right, INPUT);
  pinMode(b_left, INPUT);
  pinMode(b_right, INPUT);

  vert.write(vertPosition);
  horz.write(horzPosition);
  
  Serial.begin(9600);
}

//*****Main Program*****
void loop() {
  tleft = analogRead(t_left);
  tright = analogRead(t_right);
  bleft = analogRead(b_left);
  bright = analogRead(b_right);

  tleftf = a*tleftf + b*tleft;
  trightf = a*trightf + b*tright;
  bleftf = a*bleftf + b*bleft;
  brightf = a*brightf + b*bright;

  top = (tleftf + trightf)/2;
  bottom = (bleftf + brightf)/2;
  left = (tleftf + bleftf)/2;
  right = (trightf + brightf)/2;

  Serial.print(tleft); Serial.print("\t"); Serial.println(tright);
  Serial.print(bleft); Serial.print("\t"); Serial.println(bright); Serial.println();
  
  /*if(abs(top - bottom) > twitchBuffer){
    if(top > bottom){
      vertPosition++;
      vertPosition = constrain(vertPosition, 20, 180);
      vert.write(vertPosition);    
    }else{
      vertPosition--;
      vertPosition = constrain(vertPosition, 20, 180);
      vert.write(vertPosition);  
    }  
  }

  if(abs(left - right) > twitchBuffer){
    if(left > right){
      horzPosition--;
      horzPosition = constrain(horzPosition, 0, 180);
      horz.write(horzPosition); 
    }else{
      horzPosition++;
      horzPosition = constrain(horzPosition, 0, 180);
      horz.write(horzPosition);  
    }
  }

  delay(20);  //Might need to be lowered, needed to be about 30 to 40 for a large range sweep, but can probably be lower for small adjustments
  */
}
