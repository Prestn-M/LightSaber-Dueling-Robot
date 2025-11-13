#include "arduino_secrets.h"

// Light saber BY: Preston Malana

// Enjoy 


#include <Servo.h>

Servo servoX, servoY;

const int servoPin1 = 10;
const int servoPin2 = 11;
const int ledPin    = 12;
const int buttonPin = 2;

int posX = 90;
int posY = 90;
int targetX = 90;
int targetY = 90;

void setup() {
  servoX.attach(servoPin1);
  servoY.attach(servoPin2);

  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);

  Serial.begin(9600);
}

// Limit servo range safely
int adjustPosition(int current, int delta) {
  current += delta;
  current = constrain(current, 0, 214);
  return current; 
}

// Smooth and fast servo movement with LED flicker
void smoothMove(Servo &servo, int &currentPos, int targetPos, int stepDelay = 2) {
  if (currentPos == targetPos) return;

  int step = (targetPos > currentPos) ? 2 : -2;  // move 2Â° per step (faster)

  while (currentPos != targetPos) {
    currentPos += step;

    // Stop exactly at the target
    if ((step > 0 && currentPos > targetPos) || (step < 0 && currentPos < targetPos)) {
      currentPos = targetPos;
    }

    servo.write(currentPos);

    // Flicker LED briefly while moving
    digitalWrite(ledPin, HIGH);
    delay(1);
    digitalWrite(ledPin, LOW);

    delay(stepDelay);  // small delay = faster movement
  }
}

void loop() {
  int xVal = analogRead(A0);
  int yVal = analogRead(A1);

  int deltaX = 0, deltaY = 0;

  // Joystick X
  if (xVal <= 450 || xVal >= 550) {
    deltaX = map(xVal, 0, 1023, -10, 10);
    targetX = adjustPosition(targetX, deltaX);
  }

  // Joystick Y
  if (yVal <= 450 || yVal >= 550) {
    deltaY = map(yVal, 0, 1023, 10, -10);
    targetY = adjustPosition(targetY, deltaY);
  }

  // Move both servos quickly and smoothly
  smoothMove(servoX, posX, targetX, 0.000011);
  smoothMove(servoY, posY, targetY, 0.000011);

  // LED ON when button is pressed
  if (digitalRead(buttonPin) == LOW)
    digitalWrite(ledPin, HIGH);
  else
    digitalWrite(ledPin, LOW);

  delay(0.001
  );
}

