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
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
  OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

/* Comment out above, uncomment this block to use hardware SPI
#define OLED_DC     6
#define OLED_CS     7
#define OLED_RESET  8
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
  &SPI, OLED_DC, OLED_RESET, OLED_CS);
*/

#define NUMFLAKES     10 // Number of snowflakes in the animation example

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16
int forwardPins[] = {motor1pin1,motor2pin1,motor3pin1,motor4pin1};
int backwardPins[] = {motor1pin2,motor2pin2,motor3pin2,motor4pin2};
void setup() {
  Serial.println("started");
  pinMode(swPin, INPUT_PULLUP);
  for(int i = 0; i < sizeof(forwardPins);i++){
    pinMode(forwardPins[i],OUTPUT);
    pinMode(backwardPins[i],OUTPUT);
  }
  Serial.begin(9600);  // Initialize serial communication
  Serial.println("Control motors with joystick.");

  for(int i = 0; i < sizeof(forwardPins);i++){
    digitalWrite(forwardPins[i],HIGH);
    digitalWrite(backwardPins[i],LOW);
    digitalWrite(forwardPins[i],LOW);
    delay(500);
  }
  // testanimate();

  // rotate();
}

#define XPOS   0 // Indexes into the 'icons' array in function below
#define YPOS   1
#define DELTAY 2


void loop() {  
  joyStickControl();
}
void joyStickControl(){
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
    backward(velocity);
  }
  else if(angleDegrees >= 157.5 &&  angleDegrees < 202.5 &&  velocity > 0.1){
    right(velocity);
  }
  else if(angleDegrees >= 247.5 &&  angleDegrees < 292.5 &&  velocity > 0.1){
    forward(velocity);
  }
  else if(angleDegrees >= 337.5 &&  angleDegrees < 360 || angleDegrees >= 0 && angleDegrees < 22.5 &&  velocity > 0.1){
    left(velocity);
  }
 
  else if(!switchState){
    if(orientation % 2 == 0){
    rotate_right();
    }
    else{
    rotate_left();
    }
    rotation = rotation + 1;
  }
  if(velocity < 0.05){
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

// Function to map velocity to PWM value
int mapVelocityToPWM(float velocity) {
  //use the map function that is, maybe if we know the minimum velocity
  int pwmValue = int(velocity * 255); // Map to range 0-255
  if (pwmValue > 255) pwmValue = 255; // Ensure PWM does not exceed 255
  if (pwmValue < -255) pwmValue = -255; // Ensure PWM does not go below -255
  return pwmValue;
}


// Function to control motor using PWM
void stopAllMotors(){
  for(int i = 0; i < sizeof(forwardPins);i++){
    analogWrite(forwardPins[i],0);
    analogWrite(backwardPins[i],0);
  }
}


void forward(float velocity){
  stopAllMotors();
  for(int i = 0; i < sizeof(forwardPins);i++){
    analogWrite(forwardPins[i],velocity*250);
  }
}
void backward(float velocity){
  stopAllMotors();
  for(int i = 0; i < sizeof(backwardPins);i++){
    analogWrite(backwardPins[i],velocity*250);
  }
}
void right(float velocity){
  stopAllMotors();
  // analogWrite(motor1pin1, velocity*250);
  // analogWrite(motor1pin2, 0);
  //May or may not work
  for(int i = 0; i < sizeof(forwardPins)-1;i++){
    analogWrite(forwardPins[i],velocity*250);
    analogWrite(backwardPins[i+=1],velocity*250);
  }
  
  // analogWrite(motor2pin1, 0);
  // analogWrite(motor2pin2, 250*velocity);


  // analogWrite(motor3pin1, velocity*250);
  // analogWrite(motor3pin2, 0);


  // analogWrite(motor4pin1, 0);
  // analogWrite(motor4pin2, velocity*250);
}
void left(float velocity){
  stopAllMotors();
  analogWrite(motor1pin2, velocity*250);
  analogWrite(motor2pin1, velocity*250);
  analogWrite(motor3pin2, velocity*250);
  analogWrite(motor4pin1, velocity*250);
}
void FR(float velocity){
  stopAllMotors();
  for(int i = 0; i < sizeof(forwardPins)-1;i+=2){
    analogWrite(backwardPins[i],velocity*750);
  }
  // analogWrite(motor1pin2, velocity*750);
  // analogWrite(motor3pin2, velocity*750);
}
void FL(float velocity){
  stopAllMotors();
  for(int i = 1; i < sizeof(forwardPins);i+=2){
    analogWrite(backwardPins[i],velocity*750);
  }
  // analogWrite(motor4pin2, velocity*750);
  // analogWrite(motor2pin2, velocity*750);
}
void rotate_right(){
  stopAllMotors();
  analogWrite(motor2pin1, 250);
  analogWrite(motor3pin1, 250);
  analogWrite(motor1pin2, 250);
  analogWrite(motor4pin2, 250);
  delay(100);
}
void rotate_left(){
  stopAllMotors();
  analogWrite(motor2pin2, 250);
  analogWrite(motor3pin2, 250);
  analogWrite(motor1pin1, 250);
  analogWrite(motor4pin1, 250);
  delay(100);
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

