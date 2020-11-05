#include <ESP32Servo.h>
#include <AceButton.h>

using namespace ace_button;

Servo myservo;

const int servoPin = 13;
const int potPin = 25;
const int ledPin = 2;
const int buttonPin = 0;

AceButton button(buttonPin);

void handleEvent(AceButton*, uint8_t, uint8_t);

int currentAngle = 0;
const int numOfSections = 8;
bool isIncreasing;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  ButtonConfig* buttonConfig = button.getButtonConfig();
  buttonConfig->setEventHandler(handleEvent);
  buttonConfig->setFeature(ButtonConfig::kFeatureClick);
  
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  myservo.setPeriodHertz(50);
  myservo.attach(servoPin, 100, 2500);
  myservo.write(currentAngle);
}

void loop() {
  button.check();
}

void advanceAngle() {
  // Advance
  if (isIncreasing) currentAngle += 180 / numOfSections;
  else currentAngle -= 180 / numOfSections;

  // Change direction if we're at either end
  if (currentAngle > 180) {
    isIncreasing = false;
    currentAngle -= 180 / numOfSections;
  } else if (currentAngle < 0) {
    isIncreasing = true;
    currentAngle += 180 / numOfSections;
  }

  // Write to servo.
  myservo.write(currentAngle);
  Serial.print("New angle: ");
  Serial.println(currentAngle);
}

void handleEvent(AceButton* /* button */, uint8_t eventType,
    uint8_t /* buttonState */) {
  switch (eventType) {
    case AceButton::kEventClicked:
      advanceAngle();
      break;
  }
}
