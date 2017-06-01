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
  int button_state = digitalRead(mount_pin);
  xbee.println("m" + String(button_state));
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
      lcd.print ("Not Press");
    }
    else if (third_char == '2') {
      lcd.print( "Press R2");
    }
    else if (third_char == '3') {
      lcd.print("Press R3");
    }
  }
  else if (second_char == 'M') {
    lcd.print("Mount");
    lcd.setCursor(0,1);
    if (third_char == 'Q') {
      lcd.print("Wall? Y/N");
    }
    else if (third_char == 'S') {
      lcd.print ("Start");
      Serial.println ("HELLO");
    }
  }
}


    
