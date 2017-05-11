//Receiving Data
#include <Servo.h>
#include <SoftwareSerial.h>

const int pot_pin = A0;
const int joystick_pin = A1;

int prev_joystick_value;
int prev_pot_value;

SoftwareSerial xbee(2,3);

void setup() {
  Serial.begin(9600);
  xbee.begin(9600);
  prev_joystick_value = analogRead(joystick_pin);
  prev_pot_value = analogRead(pot_pin);
  
}

void loop() {
  int joystick_value = analogRead(joystick_pin);
  int pot_value = analogRead(pot_pin);
  if (joystick_value != prev_joystick_value){
    xbee.print("j" + String(joystick_value));
    xbee.println();
    prev_joystick_value = joystick_value;
  }
  if (pot_value != prev_pot_value){
    xbee.print("p" + String(pot_value));
    xbee.println();
    prev_pot_value = pot_value;
  }
  delay(200);
}
