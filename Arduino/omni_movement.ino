const int motor1pin1 = 4;
const int motor1pin2 = 5;
const int motor2pin1 = 2;
const int motor2pin2 = 3;
const int motor3pin1 = 8;
const int motor3pin2 = 9;
const int motor4pin1 = 11;
const int motor4pin2 = 12;
// Variables to store joystick values
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for SSD1306 display connected using software SPI (default case):
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#include <FluxGarage_RoboEyes.h>

roboEyes roboEyes;
String inputBuff;
int i = 0;
unsigned long PosTimer;

const int forwardPins[4] = {motor1pin1,motor2pin1,motor3pin1,motor4pin1};
const int backwardPins[4] = {motor1pin2,motor2pin2,motor3pin2,motor4pin2};
void setup() {
  PosTimer = millis() + 250;
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C or 0x3D
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  // Define eye shapes, all values in pixels
  roboEyes.begin(SCREEN_WIDTH,SCREEN_HEIGHT,100);
  //This changes what the eyes look like 
  roboEyes.setWidth(36, 36); // byte leftEye, byte rightEye
  roboEyes.setHeight(36, 36); // byte leftEye, byte rightEye
  roboEyes.setBorderradius(8, 8); // byte leftEye, byte rightEye
  roboEyes.setSpacebetween(10); // int space -> can also be negative

  roboEyes.setAutoblinker(ON,3,3);//Animation that changes whether it's blinking or not, THIS MAY ALSO NEED TO TURNED OFF BUT UNKNOWN FOR SURE.
  roboEyes.setIdleMode(ON,2,2); // On by default just make sure this is turned off when moving the eyes to somewhere else on the screen.
  Serial.begin(9600);  // Initialize serial communication
for(int i = 0; i < 4;i++){
    pinMode(forwardPins[i],OUTPUT);
    pinMode(backwardPins[i],OUTPUT);
  }
}
void loop() { 
  //Use non blocking code when dealing with the face animations so no while loops
  roboEyes.update();
  if(Serial.available() > 0){
    inputBuff = Serial.readStringUntil('\n');
  }
  if (inputBuff.length() !=0){
      input(inputBuff.charAt(i));
  }else{
    i = 0;
    inputBuff = "";
  }
}
void input(char input){
    switch(input){
      //Happy
      case 'h': roboEmotions(0);break;
      //Tired
      case 't': roboEmotions(2);break;
      //Angry
      case 'a': roboEmotions(1);break;
      //Default
      case 'd': roboEmotions(6);break;
      //Drive
      default: driveDirection(250,input);break;
    }
}
void idleAnimation(bool state){
  if(state){
    roboEyes.setIdleMode(ON,2,2);
  }else{
    roboEyes.setIdleMode(OFF,2,2);
  }
}
void moveEyesRandomDirection(int direction){
  idleAnimation(false);
  roboEyes.setPosition(direction);
}
void roboEmotions(int emotions){
  //A function that changes 
    switch (emotions){
      case 0:
      //Happy
      roboEyes.setMood(HAPPY);
      break;
      case 1:
      //Angry
      roboEyes.setMood(ANGRY);
      break;
      case 2:
      //Tired
      roboEyes.setMood(TIRED);
      break;
      default:
      //Default
      roboEyes.setMood(DEFAULT);
      break;
    }
    inputBuff.remove(inputBuff.charAt(i));
    roboEyes.update();
}

// Function to control motor using PWM
void stopAllMotors(){
  for(int i = 0; i < 4;i++){
    analogWrite(forwardPins[i],0);
    analogWrite(backwardPins[i],0);
  }
}
void driveDirection(float velocity, char direction){
 if(millis() > PosTimer){
    stopAllMotors();
  switch(direction){
    case 'f':
    //Forward
    Serial.println(direction);
    for(int i = 0; i < 4;i++){
      analogWrite(forwardPins[i],velocity*250);
    }
    break;
    case 'b':
    //Backward
    for(int i = 0; i < 4;i++){
      analogWrite(backwardPins[i],velocity*250);
    }
    break;
    case 'r':
    //Right
    analogWrite(motor1pin1, velocity*250);
    analogWrite(motor2pin2, velocity*250);
    analogWrite(motor3pin2, velocity*250);
    analogWrite(motor4pin1, velocity*250);
    break;
    case 'l': 
    //Left
    analogWrite(motor1pin2, velocity*250);
    analogWrite(motor2pin1, velocity*250);
    analogWrite(motor3pin1, velocity*250);
    analogWrite(motor4pin2, velocity*250);
    break;
    case 'rr':
    //Rotate Right
    analogWrite(motor2pin1, 250);
    analogWrite(motor3pin1, 250);
    analogWrite(motor1pin2, 250);
    analogWrite(motor4pin2, 250);
    break;
    case 'rl':
    //Rotate left
    analogWrite(motor2pin2, 250);
    analogWrite(motor3pin2, 250);
    analogWrite(motor1pin1, 250);
    analogWrite(motor4pin1, 250);
    break;
  }
  inputBuff.remove(inputBuff.charAt(i));
  i+=1;
  PosTimer = millis()+250;
}
}

