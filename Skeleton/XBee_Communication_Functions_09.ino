const int DELAY_XBEE = 50;

int get_joystick() {
  xbee.println('j');
  delay(DELAY_XBEE);
  while (true) {
    if (xbee.available()){
      String raw_input = xbee.readStringUntil('\n');
      char id_char = raw_input.charAt(0);                 // look at the first character in the string
      if (id_char == 'j') {                                // if the first character is a j, we know that it came from the joystick 
        int joystick_input = raw_input.substring(1).toInt();      // example: j526 -> raw_input.substring(1).toInt() converts string to int and discards j. end result is -> joystick_input = 526 
        return joystick_input;
      }
    }
  }
}

int get_potentiometer() {
  xbee.println('p');
  delay(DELAY_XBEE);
  while (true) {
    if (xbee.available()){
      String raw_input = xbee.readStringUntil('\n');
      char id_char = raw_input.charAt(0);                 // look at the first character in the string
      if (id_char == 'p') {                                // if the first character is a j, we know that it came from the joystick 
        int pot_input = raw_input.substring(1).toInt();      // example: j526 -> raw_input.substring(1).toInt() converts string to int and discards j. end result is -> joystick_input = 526 
        return pot_input;
      }
    }
  } 
}

int get_speed(char identifier) {
  xbee.println('V' + String(identifier));
  //Serial.println("get speed called xbee");
  delay(DELAY_XBEE);
  while (true) {
    if (xbee.available()) {
      String raw_input = xbee.readStringUntil('\n');
      char id_char = raw_input.charAt(0);
      char second_char = raw_input.charAt(1);
      if (id_char == 'V') {
        if (second_char == 'M') {
          int brushless_min_speed = raw_input.substring(2).toInt();
          return brushless_min_speed;
        }
        else if (second_char == 'F') {
          int brushless_speed = raw_input.substring(2).toInt();
          return brushless_speed;
        }
        else if (second_char == 'C') {
          int brushless_speed = raw_input.substring(2).toInt();
          return brushless_speed;
        }
        else if (second_char == 'D') {
          int brushless_speed = raw_input.substring(2).toInt();
          return brushless_speed;
        }
      }
    }
  }
}


int get_button(char setup_or_mount) {
  if (setup_or_mount == 's'){
    Serial.println("button check");
    xbee.println('s');
    delay(DELAY_XBEE);
    while (true) {
      if (xbee.available()) {
        String raw_input = xbee.readStringUntil('\n');
        Serial.println("raw_input");
        char id_char = raw_input.charAt(0);
        if (id_char == 's') {
          int mount_button_input = raw_input.substring(1).toInt();
          return mount_button_input;
        }
      }
    }
  }
  else if (setup_or_mount == 'm'){
    xbee.println('m');
    delay(DELAY_XBEE);
    while (true) {
      if (xbee.available()) {
        String raw_input = xbee.readStringUntil('\n');
        char id_char = raw_input.charAt(0);
        if (id_char == 'm') {
          int mount_button_input = raw_input.substring(1).toInt();
          return mount_button_input;
        }
      }
    }
  }
}

bool check_button_status(char setup_or_mount) {
  if (get_button(setup_or_mount) == HIGH) {
    return true;
  }
  else {
    return false;
  }
}

void lcd_message(String input_message) {
  xbee.println(input_message);
}
