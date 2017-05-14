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
  int joystick_value = analogRead(joystick_pin);  //read value from the pin attached to the joystick
  int pot_value = analogRead(pot_pin);            
  if (joystick_value != prev_joystick_value){     // if the joystick value is different from the previous joystick value, we want to send it over to the user
                                                  // if the current value is equal to the previous value, then the same state will be held and the information will be useless to send over
    xbee.print("j" + String(pot_value));      //attach j to the beginning of this string as a means of differentiating potetiometer and joystick values
    xbee.println();   // send the information to the receiver through xbee
    prev_joystick_value = joystick_value;   // since our current value is different from our previous value, we want to equate our current value to our previous value for our next iteration of this loop
  }
  if (pot_value != prev_pot_value){         
    xbee.print("p" + String(pot_value));
    xbee.println();
    prev_pot_value = pot_value;
  }
  delay(200);
}
