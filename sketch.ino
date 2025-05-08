#include "arduino_secrets.h"
#include "thingProperties.h"

#define TRIG_PIN 14      // HC-SR04 Trig
#define ECHO_PIN 27      // HC-SR04 Echo
#define MQ2_PIN 26       // MQ2 Analog input

#define RELAY_PIN_1 33   // Relay 1
#define RELAY_PIN_2 32   // Relay 2

#define LED_PIN 2        // Onboard LED for status

void setup() {
  Serial.begin(9600);
  delay(1500);

  pinMode(LED_PIN, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(MQ2_PIN, INPUT);
  pinMode(RELAY_PIN_1, OUTPUT);
  pinMode(RELAY_PIN_2, OUTPUT);

  digitalWrite(LED_PIN, HIGH);

  initProperties();
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);

  while (ArduinoCloud.connected() == 0) {
    ArduinoCloud.update();
    delay(500);
  }

  digitalWrite(LED_PIN, LOW); // LED OFF after connected

  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
}

void loop() {
  ArduinoCloud.update();

  float distance = getDistanceCm();

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  Serial.print("Smoke Level: ");
  Serial.println(smokeLevel);

  if (distance < 20) {
    onMotionDetected();
  }

  int smokeDetected = digitalRead(MQ2_DIGITAL_PIN);
  if (smokeDetected == LOW) {
    Serial.println("Smoke Detected!");
  } else {
    Serial.println("No Smoke.");
  }

  delay(2000);
}

void onMotionDetected() {
  switch1 = true;
  switch2 = true;
  digitalWrite(RELAY_PIN_1, HIGH);
  digitalWrite(RELAY_PIN_2, HIGH);
}

void onSmokeDetected() {
  switch1 = false;
  switch2 = false;
  digitalWrite(RELAY_PIN_1, LOW);
  digitalWrite(RELAY_PIN_2, LOW);
}

float getDistanceCm() {
  long duration;
  float distance;

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration * 0.034 / 2;

  return distance;
}

void onSwitch1Change() {
  if(switch1) {
    digitalWrite(RELAY_PIN_1, HIGH);
  } else {
    digitalWrite(RELAY_PIN_1, HIGH);
  }
}

void onSwitch2Change() {
  if(switch2) {
    digitalWrite(RELAY_PIN_2, HIGH);
  } else {
    digitalWrite(RELAY_PIN_2, HIGH);
  }
}

void sendAlert(String message) {
  alert = message;
  ArduinoCloud.update();
}
