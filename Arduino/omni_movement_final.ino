  int ena1 = 3;
  int motor1in1 = 2;
  int motor1in2 = 4;
  int motor2in1 = 6;
  int motor2in2 = 7;
  int enb1 = 5;


  int ena2 = 9;
  int motor3in1 = 8;
  int motor3in2 = 11;
  int motor4in1 = 12;
  int motor4in2 = 13;
  int enb2 = 10;

  String inputBuff;  // Buffer to hold serial input
  int i = 0;        // Index for processing input
  //String, int value. 
  void setup() {
    // Initialize motor pins as outputs
    pinMode(ena1, OUTPUT);
    pinMode(motor1in1, OUTPUT);
    pinMode(motor1in2, OUTPUT);
    pinMode(enb1, OUTPUT);
    pinMode(motor2in1, OUTPUT);
    pinMode(motor2in2, OUTPUT);

    pinMode(ena2, OUTPUT);
    pinMode(motor3in1, OUTPUT);
    pinMode(motor3in2, OUTPUT);
    pinMode(enb2, OUTPUT);
    pinMode(motor4in1, OUTPUT);
    pinMode(motor4in2, OUTPUT);
    
    Serial.begin(9600);  // Initialize serial communication
  }
  char direction;
  int speed;
  void loop() {
    // Check if data is available on serial
    inputBuff = "";
    if (Serial.available() > 0) {
      inputBuff = Serial.readStringUntil('\n');  // Read input until newline
      if (inputBuff.length() != 0) {
        direction = inputBuff.charAt(0);
        speed = inputBuff.substring(1).toInt();
        processInput(direction, speed);
        i += 1;
        if (i >= inputBuff.length()) {
          i = 0;  // Reset index after processing the buffer
        }
      }
    }
  }
  //Make a function that rotates right and left, 
  //Have the direction and speed be taken into account now, 
  // Function to process the input character
  void processInput(char input,int speed) {
    switch (input) {
      case 'f':  // Move forward
        moveForward(speed);
        break;
      case 'b':  // Move backward
        moveBackward(speed);
        break;
      case 'a':  // Turn left
        turnRight(speed);
        break;
      case 'd':  // Turn right
        turnLeft(speed);
        break;
      case 'r':
        rotateRight(speed);
        break;
      case 'l':
        rotateLeft(speed);
        break;
      default:
        stopAllMotors();  // Stop if input is not recognized
        break;
    }
  }

  // Function to move forward
  void moveForward(int speed) {
    analogWrite(ena1, speed);  // Set speed for motor 1
    digitalWrite(motor1in1, HIGH);
    digitalWrite(motor1in2, LOW);

    analogWrite(enb1, speed);  // Set speed for motor 2
    digitalWrite(motor2in1, HIGH);
    digitalWrite(motor2in2, LOW);

    analogWrite(ena2, speed);  // Set speed for motor 3
    digitalWrite(motor3in1, HIGH);
    digitalWrite(motor3in2, LOW);

    analogWrite(enb2, speed);  // Set speed for motor 4
    digitalWrite(motor4in1, HIGH);
    digitalWrite(motor4in2, LOW);
  }

  // Function to move backward
  void moveBackward(int speed) {
    analogWrite(ena1, speed);  // Set speed for motor 1
    digitalWrite(motor1in1, LOW);
    digitalWrite(motor1in2, HIGH);

    analogWrite(enb1,speed);  // Set speed for motor 2
    digitalWrite(motor2in1, LOW);
    digitalWrite(motor2in2, HIGH);

    analogWrite(ena2, speed);  // Set speed for motor 3
    digitalWrite(motor3in1, LOW);
    digitalWrite(motor3in2, HIGH);

    analogWrite(enb2, speed);  // Set speed for motor 4
    digitalWrite(motor4in1, LOW);
    digitalWrite(motor4in2, HIGH);
  }

  // Function to turn right
  void rotateRight(int speed) {
    // Motors 1 and 2 go backward
    analogWrite(ena1, speed);  // Set speed for motor 1
    digitalWrite(motor1in1, LOW);
    digitalWrite(motor1in2, HIGH);

    analogWrite(enb1, speed);  // Set speed for motor 2
    digitalWrite(motor2in1, LOW);
    digitalWrite(motor2in2, HIGH);

    // Motors 3 and 4 go forward
    analogWrite(ena2, speed);  // Set speed for motor 3
    digitalWrite(motor3in1, HIGH);
    digitalWrite(motor3in2, LOW);

    analogWrite(enb2, speed);  // Set speed for motor 4
    digitalWrite(motor4in1, HIGH);
    digitalWrite(motor4in2, LOW);
  }

  // Function to turn left
  void rotateLeft(int speed) {
    // Motors 1 and 2 go forward
    analogWrite(ena1, speed);  // Set speed for motor 1
    digitalWrite(motor1in1, HIGH);
    digitalWrite(motor1in2, LOW);

    analogWrite(enb1, speed);  // Set speed for motor 2
    digitalWrite(motor2in1, HIGH);
    digitalWrite(motor2in2, LOW);

    // Motors 3 and 4 go backward
    analogWrite(ena2, speed);  // Set speed for motor 3
    digitalWrite(motor3in1, LOW);
    digitalWrite(motor3in2, HIGH);

    analogWrite(enb2, speed);  // Set speed for motor 4
    digitalWrite(motor4in1, LOW);
    digitalWrite(motor4in2, HIGH);
  }
  // Function to turn right (pivot to the right)
  void turnRight(int speed) {
    // Motors on the left side go backward
    analogWrite(ena1, speed);  
    digitalWrite(motor1in1, HIGH);
    digitalWrite(motor1in2, LOW);

    analogWrite(enb1, speed);  
    digitalWrite(motor2in1, LOW);
    digitalWrite(motor2in2, HIGH);

    // Motors on the right side go forward
    analogWrite(ena2, speed);  
    digitalWrite(motor3in1, LOW);
    digitalWrite(motor3in2, HIGH);

    analogWrite(enb2, speed);  
    digitalWrite(motor4in1, HIGH);
    digitalWrite(motor4in2, LOW);
  }


  // Function to turn left (pivot to the left)
  void turnLeft(int speed) {
    // Motors on the left side go backward
    analogWrite(ena1, speed);  
    digitalWrite(motor1in1, LOW);
    digitalWrite(motor1in2, HIGH);

    analogWrite(enb1, speed);  
    digitalWrite(motor2in1, HIGH);
    digitalWrite(motor2in2, LOW);

    // Motors on the right side go forward
    analogWrite(ena2, speed);  
    digitalWrite(motor3in1, HIGH);
    digitalWrite(motor3in2, LOW);

    analogWrite(enb2, speed);  
    digitalWrite(motor4in1, LOW);
    digitalWrite(motor4in2, HIGH);
  }




  // Function to stop all motors
  void stopAllMotors() {
    digitalWrite(motor1in1, LOW);
    digitalWrite(motor1in2, LOW);
    analogWrite(ena1, 0);

    digitalWrite(motor2in1, LOW);
    digitalWrite(motor2in2, LOW);
    analogWrite(enb1, 0);

    digitalWrite(motor3in1, LOW);
    digitalWrite(motor3in2, LOW);
    analogWrite(ena2, 0);

    digitalWrite(motor4in1, LOW);
    digitalWrite(motor4in2, LOW);
    analogWrite(enb2, 0);
  }