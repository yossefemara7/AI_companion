const int motor1pin1 = 4;
const int motor1pin2 = 5;
const int motor2pin1 = 2;
const int motor2pin2 = 3;
const int motor3pin1 = 8;
const int motor3pin2 = 9;
const int motor4pin1 = 11;
const int motor4pin2 = 12;
const int joyXPin = A1;    // Analog pin for X-axis
const int joyYPin = A0;    // Analog pin for Y-axis
const int swPin = 13;
// Variables to store joystick values
int rotation = 0;
int orientation = 0;
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for SSD1306 display connected using software SPI (default case):
#define OLED_MOSI   9
#define OLED_CLK   10
#define OLED_DC    11
#define OLED_CS    12
#define OLED_RESET 13
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);
#include <FluxGarage_RoboEyes.h>
roboEyes roboEyes;
unsigned long eventTimer;
bool event1wasPlayed = 0; // flag variables
bool event2wasPlayed = 0;
bool event3wasPlayed = 0;

const int forwardPins[4] = {motor1pin1,motor2pin1,motor3pin1,motor4pin1};
const int backwardPins[4] = {motor1pin2,motor2pin2,motor3pin2,motor4pin2};
void setup() {
  pinMode(swPin, INPUT_PULLUP);
  
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
  Serial.println("Control motors with joystick.");
for(int i = 0; i < 4;i++){
    pinMode(forwardPins[i],OUTPUT);
    pinMode(backwardPins[i],OUTPUT);
  }
  for(int i = 0; i < 4;i++){
    digitalWrite(forwardPins[i],HIGH);
    digitalWrite(backwardPins[i],LOW);
    digitalWrite(forwardPins[i],LOW);
    delay(500);
  }
}

#define XPOS   0 // Indexes into the 'icons' array in function below
#define YPOS   1
#define DELTAY 2

void loop() { 
  //Use non blocking code when dealing with the face animations so no while loops 
  roboEmotions(random(0,3)); // Emotion state for happy, tired, angry, switches randomly between them. 
  moveEyesRandomDirection(random(1,9));//Moving the eyes in a specific direction. Random for right now(To change this remove random (1-9)) N, NE, E, SE, S, SW, W, NW, DEFAULT
  roboEyes.update();
  //Place the input function from the python script so that everytime it check's whether if data's available
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
void eyeAnimations(int animation){
  //This method can be used to create custom animations if wanted, make sure to use timer with non-blocking code so no while loops and flags for it,
  idleAnimation(false);
  switch(animation){
    case 0:
    //Example
  if(millis() >= eventTimer+2000 && event1wasPlayed == 0){
    event1wasPlayed = 1; // flag variable to make sure the event will only be handled once
    roboEyes.open(); // open eyes 
  }
  // Do once after defined number of milliseconds
  if(millis() >= eventTimer+4000 && event2wasPlayed == 0){
    event2wasPlayed = 1; // flag variable to make sure the event will only be handled once
    roboEyes.setMood(HAPPY);
    roboEyes.anim_laugh();
    //roboEyes.anim_confused();
  }
  // Do once after defined number of milliseconds
  if(millis() >= eventTimer+6000 && event3wasPlayed == 0){
    event3wasPlayed = 1; // flag variable to make sure the event will only be handled once
    roboEyes.setMood(TIRED);
    //roboEyes.blink();
  }
  // Do once after defined number of milliseconds, then reset timer and flags to restart the whole animation sequence
  if(millis() >= eventTimer+8000){
    roboEyes.close(); // close eyes again
    roboEyes.setMood(DEFAULT);
    // Reset the timer and the event flags to restart the whole "complex animation loop"
    eventTimer = millis(); // reset timer
    event1wasPlayed = 0; // reset flags
    event2wasPlayed = 0;
    event3wasPlayed = 0;
  }
  break;
}
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
    roboEyes.update();
}

// Function to map velocity to PWM value
int mapVelocityToPWM(float velocity) {
  //use the map function that is, maybe if we know the minimum velocity
  int pwmValue = int(velocity * 255); // Map to range 0-255
  if (pwmValue > 255) pwmValue = 255; // Ensure PWM does not exceed 255
  if (pwmValue < -255) pwmValue = -255; // Ensure PWM does not go below -255
  return pwmValue;
}
//Make the eyes constantly move however change the emotion based on the paramete

// Function to control motor using PWM
void stopAllMotors(){
  for(int i = 0; i < 4;i++){
    analogWrite(forwardPins[i],0);
    analogWrite(backwardPins[i],0);
  }
}
void joyStick(){
  int direction =0;
  int joyXValue = analogRead(joyXPin);
  int joyYValue = analogRead(joyYPin);
  bool switchState = digitalRead(swPin);
  if(!switchState){
    Serial.println(switchState);
  }
  float X = (joyXValue - 512.0) / 512.0;
  float Y = (joyYValue - 512.0) / 512.0;


  // Calculate velocity and angle
  float velocity = sqrt(X * X + Y * Y);
  float angle = atan2(Y, X); // Calculate angle in radians
  // Convert angle to degrees
  float angleDegrees = angle * (180.0 / PI);


  // Adjust angle so that zero degrees is to the right and starts from above the horizontal
  angleDegrees -= 90.0; // Rotate by 90 degrees to set zero to the right


  // Flip vertically
  angleDegrees -= 180.0; // Flip the angle vertically
  // Normalize angle to be in the range [0, 360)
  if (angleDegrees < 0) {
    angleDegrees += 360.0;
  } else if (angleDegrees >= 360.0) {
    angleDegrees -= 360.0;
  }
  Serial.println(angleDegrees);

  // if(angleDegrees >= 22.5 &&  angleDegrees < 67.5 &&  velocity > 0.1){
  //   FR(velocity);
  // }
  // if(angleDegrees >= 112.5 &&  angleDegrees < 157.5 &&  velocity > 0.1){
  //   FL(velocity);
  // }
  if(angleDegrees >= 67.5 &&  angleDegrees < 112.5 &&  velocity > 0.1){
    direction =1;
  }
  else if(angleDegrees >= 157.5 &&  angleDegrees < 202.5 &&  velocity > 0.1){
    direction =2;
  }
  else if(angleDegrees >= 247.5 &&  angleDegrees < 292.5 &&  velocity > 0.1){
    direction =0;
  }
  else if(angleDegrees >= 337.5 &&  angleDegrees < 360 || angleDegrees >= 0 && angleDegrees < 22.5 &&  velocity > 0.1){
    direction =3;
  }
 
  else if(!switchState){
    if(orientation % 2 == 0){
    direction = 6;
    }
    else{
    direction =7;
    }
    rotation = rotation + 1;
  }
  if(velocity >0.05){
    driveDirection(velocity,direction);
  }else{
    stopAllMotors();
  }
  if(rotation > 5){
    Serial.println("Hey");
    rotation = 0;
    orientation = orientation + 1;
  }
  angleDegrees = 0;
  velocity = 0;
  delay(10);  // Slight delay for stability
}
void driveDirection(float velocity, int direction){
  stopAllMotors();
  switch(direction){
    case 0:
    //Forward
    for(int i = 0; i < 4;i++){
      analogWrite(forwardPins[i],velocity*250);
    }
    break;
    case 1:
    //Backward
    for(int i = 0; i < 4;i++){
      analogWrite(backwardPins[i],velocity*250);
    }
    break;
    case 2:
    //Right
    for(int i = 0; i < 4;i+=2){
      analogWrite(forwardPins[i],velocity*250);
      analogWrite(backwardPins[i+1],velocity*250);
    }
    break;
    case 3: 
    //Left
    for(int i = 0; i < 4;i+=2){
      analogWrite(backwardPins[i],velocity*250);
      analogWrite(forwardPins[i+1],velocity*250);
    }
    break;
    case 6:
    //Rotate Right
    analogWrite(motor2pin1, 250);
    analogWrite(motor3pin1, 250);
    analogWrite(motor1pin2, 250);
    analogWrite(motor4pin2, 250);
    delay(100);
    break;
    case 7:
    //Rotate left
    analogWrite(motor2pin2, 250);
    analogWrite(motor3pin2, 250);
    analogWrite(motor1pin1, 250);
    analogWrite(motor4pin1, 250);
    delay(100);
    break;
  }
}
void controlMotor(int pin1, int pin2, int speed) {
  if (speed > 0) {
    analogWrite(pin1, speed); // Positive speed
    analogWrite(pin2, 0);
  } else if (speed < 0) {
    //Why -speed is here
    analogWrite(pin1, 0);
    analogWrite(pin2, -speed); // Negative speed
  } else {
    analogWrite(pin1, 0); // Stop the motor
    analogWrite(pin2, 0);
  }
}

