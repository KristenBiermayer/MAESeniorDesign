//Receiving Data
#include <Servo.h>
#include <SoftwareSerial.h>

const int pot_pin = A0;
const int joystick_pin = A1;

SoftwareSerial xbee(2,3);

void setup() {
  Serial.begin(9600);
  xbee.begin(9600);
}

void loop() {
  int joystick_value = analogRead(joystick_pin);
  int pot_value = analogRead(pot_pin);
  xbee.print("j" + String(joystick_value));
  xbee.println();
  xbee.print("p" + String(pot_value));
  xbee.println();
  Serial.print(joystick_value);
  Serial.println();
  delay(1000);
}
