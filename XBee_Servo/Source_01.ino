//Receiving Data
#include <Servo.h>
#include <SoftwareSerial.h>

const int joystick_pin = A1;

SoftwareSerial xbee(2,3);

void setup() {
  Serial.begin(9600);
  xbee.begin(9600);
}

void loop() {
  int joystick_value = analogRead(joystick_pin);
  xbee.print(joystick_value);
  xbee.println();
  Serial.print(joystick_value);
  Serial.println();
  delay(100);
}
