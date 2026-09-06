#include <SoftwareSerial.h>

// ----------------------------------------------------
// BLUETOOTH
// HC-05 TX -> Arduino pin 10
// HC-05 RX -> Arduino pin 11
// ----------------------------------------------------
SoftwareSerial bluetooth(10, 11);   // RX, TX


// ----------------------------------------------------
// MOTOR DRIVER PINS
//
// LEFT SIDE  = 3 motors
// RIGHT SIDE = 3 motors
// ----------------------------------------------------

// Left motor group
const int LEFT_IN1 = 2;
const int LEFT_IN2 = 3;
const int LEFT_EN  = 5;    // PWM speed pin

// Right motor group
const int RIGHT_IN1 = 4;
const int RIGHT_IN2 = 7;
const int RIGHT_EN  = 6;   // PWM speed pin


// ----------------------------------------------------
// SPEED
// PWM range = 0 to 255
// ----------------------------------------------------
int motorSpeed = 200;


// ----------------------------------------------------
// SETUP
// ----------------------------------------------------
void setup()
{
  Serial.begin(9600);
  bluetooth.begin(9600);

  pinMode(LEFT_IN1, OUTPUT);
  pinMode(LEFT_IN2, OUTPUT);
  pinMode(LEFT_EN, OUTPUT);

  pinMode(RIGHT_IN1, OUTPUT);
  pinMode(RIGHT_IN2, OUTPUT);
  pinMode(RIGHT_EN, OUTPUT);

  stopRobot();

  Serial.println("Bluetooth 6-Wheel Robot Ready");
}


// ----------------------------------------------------
// MAIN LOOP
// ----------------------------------------------------
void loop()
{
  if (bluetooth.available())
  {
    char command = bluetooth.read();

    Serial.print("Received: ");
    Serial.println(command);

    switch (command)
    {
      case 'F':
      case 'f':
        moveForward();
        break;


      case 'B':
      case 'b':
        moveBackward();
        break;


      case 'L':
      case 'l':
        turnLeft();
        break;


      case 'R':
      case 'r':
        turnRight();
        break;


      case 'S':
      case 's':
        stopRobot();
        break;


      default:
        // Unknown command
        break;
    }
  }
}


// ====================================================
// FORWARD
// ====================================================
void moveForward()
{
  // Left 3 motors forward
  digitalWrite(LEFT_IN1, HIGH);
  digitalWrite(LEFT_IN2, LOW);

  // Right 3 motors forward
  digitalWrite(RIGHT_IN1, HIGH);
  digitalWrite(RIGHT_IN2, LOW);

  analogWrite(LEFT_EN, motorSpeed);
  analogWrite(RIGHT_EN, motorSpeed);

  Serial.println("FORWARD");
}


// ====================================================
// BACKWARD
// ====================================================
void moveBackward()
{
  // Left 3 motors backward
  digitalWrite(LEFT_IN1, LOW);
  digitalWrite(LEFT_IN2, HIGH);

  // Right 3 motors backward
  digitalWrite(RIGHT_IN1, LOW);
  digitalWrite(RIGHT_IN2, HIGH);

  analogWrite(LEFT_EN, motorSpeed);
  analogWrite(RIGHT_EN, motorSpeed);

  Serial.println("BACKWARD");
}


// ====================================================
// LEFT TURN
// ====================================================
void turnLeft()
{
  /*
     Left motors  -> backward
     Right motors -> forward

     This gives a sharp/pivot left turn.
  */

  digitalWrite(LEFT_IN1, LOW);
  digitalWrite(LEFT_IN2, HIGH);

  digitalWrite(RIGHT_IN1, HIGH);
  digitalWrite(RIGHT_IN2, LOW);

  analogWrite(LEFT_EN, motorSpeed);
  analogWrite(RIGHT_EN, motorSpeed);

  Serial.println("LEFT");
}


// ====================================================
// RIGHT TURN
// ====================================================
void turnRight()
{
  /*
     Left motors  -> forward
     Right motors -> backward

     This gives a sharp/pivot right turn.
  */

  digitalWrite(LEFT_IN1, HIGH);
  digitalWrite(LEFT_IN2, LOW);

  digitalWrite(RIGHT_IN1, LOW);
  digitalWrite(RIGHT_IN2, HIGH);

  analogWrite(LEFT_EN, motorSpeed);
  analogWrite(RIGHT_EN, motorSpeed);

  Serial.println("RIGHT");
}


// ====================================================
// STOP
// ====================================================
void stopRobot()
{
  digitalWrite(LEFT_IN1, LOW);
  digitalWrite(LEFT_IN2, LOW);

  digitalWrite(RIGHT_IN1, LOW);
  digitalWrite(RIGHT_IN2, LOW);

  analogWrite(LEFT_EN, 0);
  analogWrite(RIGHT_EN, 0);

  Serial.println("STOP");
}
