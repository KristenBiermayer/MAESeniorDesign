void steering_source() {
  int joystick_value = analogRead(joystick_pin);  //read value from the pin attached to the joystick 
  Serial.println(joystick_value);      
  xbee.println("j" + String(joystick_value));      //attach j to the beginning of this string as a means of differentiating potetiometer and joystick values

}

void speed_source() {
  int pot_value = analogRead(pot_pin); 
  Serial.print("Potentiometer value: ");
  Serial.println(pot_value);         
  xbee.println("p" + String(pot_value));
}

void button_source_setup() {
  int button_state = digitalRead(button_pin);
  Serial.println(button_state);
  xbee.println("s" + String(button_state));
}

void button_source_mount() {
  int button_state = digitalRead(mount_pin);
  xbee.println("m" + String(button_state));
}

void get_brushless_speed (String input) {
  char second_char = input.charAt(1);
  if (second_char == 'M') {
    int brushless_min_speed = 1200;
    xbee.println("VM" + String(brushless_min_speed));
  }
  else if (second_char == 'F') {
    int brushless_speed = 1400;
    xbee.println("VF" + String(brushless_speed));
  }
  else if (second_char == 'C') {
    int brushless_speed = 1600;
    xbee.println("VC" + String(brushless_speed));
  }
  else if (second_char == 'D') {
    int brushless_speed = 1500;
    xbee.println("VD" + String(brushless_speed));
  }
}
void lcd_display(String input) {
  lcd.clear();
  char second_char = input.charAt(1);
  char third_char = input.charAt(2);
  lcd.setCursor(0,0);
  if (second_char == 'B') {
    lcd.print ("Button");
    lcd.setCursor(0,1);
    if (third_char == '4') {
      lcd.print ("Not Pressed");
    }
    else if (third_char == '2') {
      lcd.print("Pressed, in R2");
    }
    else if (third_char == '3') {
      lcd.print("Pressed, in R3");
    }
  }
  else if (second_char == 'M') {
    if (third_char == 'Q') {
      lcd.print("Mount Wall?");
      lcd.setCursor(0,1);
      lcd.print("Yes/No");
    }
    else if (third_char == 'S') {
      lcd.print ("Starting");
      lcd.setCursor(0,1);
      lcd.print ("To Mount");
    }
  }
  else if (second_char == 'W') {
    if (third_char == 'U') {
      lcd.print("Moving Up");
      lcd.setCursor(0,1);
      lcd.print("On Wall");
    }
    else if (third_char == 'D') {
      lcd.print("Moving Down");
      lcd.setCursor(0,1);
      lcd.print("Down Wall");
    }
  }
}
