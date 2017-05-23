#include <Servo.h>
#include <SoftwareSerial.h>

const int pot_pin = A0;
const int joystick_pin = A1;
const int button_pin = 4;

SoftwareSerial xbee(2,3);

void setup() {
  Serial.begin(9600); 
  xbee.begin(9600);
}

void loop() {
  if (xbee.available()) {
    String command_input = xbee.readStringUntil('\n');
    Serial.println(command_input);
    char id_char = command_input.charAt(0);
    if (id_char == 'p') {
      speed_source();
    }
    else if (id_char == 'j') {
      steering_source();
    }
    else if (id_char == 'b') {
      button_source();
    }
  }
}

void steering_source() {
  int joystick_value = analogRead(joystick_pin);  //read value from the pin attached to the joystick       
  xbee.println("j" + String(joystick_value));      //attach j to the beginning of this string as a means of differentiating potetiometer and joystick values
//  delay(100);
}

void speed_source() {
  int pot_value = analogRead(pot_pin);          
  xbee.println("p" + String(pot_value));
//  delay(100);
}

void button_source() {
  int button_state = digitalRead(button_pin);
  xbee.print("b" + String(button_state));
  xbee.println();
  delay(100);
}
