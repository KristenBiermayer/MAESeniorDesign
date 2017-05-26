void steering_source() {
  int joystick_value = analogRead(joystick_pin);  //read value from the pin attached to the joystick       
  xbee.println("j" + String(joystick_value));      //attach j to the beginning of this string as a means of differentiating potetiometer and joystick values

}

void speed_source() {
  int pot_value = analogRead(pot_pin);          
  xbee.println("p" + String(pot_value));
}

void button_source_setup() {
  int button_state = digitalRead(button_pin);
  Serial.println(button_state);
  xbee.println("s" + String(button_state));
}

void button_source_mount() {
  int button_state = digitalRead(button_pin);
  xbee.println("m" + String(button_state));
}
