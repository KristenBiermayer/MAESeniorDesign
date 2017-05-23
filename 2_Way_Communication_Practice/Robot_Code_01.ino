#include <SoftwareSerial.h>

SoftwareSerial xbee(2,3);

void setup() {
  Serial.begin(9600);
  xbee.begin(9600);
  // Empty out xbee channel to calibrate it
  while (xbee.available()){
    xbee.read();
  }
}

void loop() {
  get_joystick();
  get_potentiometer();
}

void get_joystick() {
  xbee.println('j');
  delay(100);
  if (xbee.available()) {
    Serial.print("joy");
    String joystic_val = xbee.readStringUntil('\n');
    Serial.println(joystic_val);
  }
}

void get_potentiometer(){
  xbee.println('p');
  delay(100);
  if (xbee.available()) {
    Serial.print("pot");
    String pot_val = xbee.readStringUntil('\n');
    Serial.println(pot_val);
  }
}
