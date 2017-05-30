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

void lcd_display(String input) {
  lcd.clear();
  char region_char = input.charAt(1);
  lcd.setCursor(0,0);
  if (region_char == '4') {
    lcd.print ("Region 4");
    lcd_display_button_r3(input);
  }
  else if (region_char == '3') {
    lcd.print ("Region 3");
    lcd_display_button_r3(input);
  }
  else if (region_char == '2') {
    lcd.print ("Region 2");
    lcd_display_button_r3(input);
  }
  else if (region_char == '1') {
    lcd.print ("Region 1");
    char button_char = input.charAt(2);
    if (button_char == 'M') {
      lcd.setCursor (0,1);
      lcd.print ("Mnt Wall?");
    }
    else if (button_char == 'W') {
      lcd.setCursor(0,1);
      lcd.print ("Rdy4Mnt");
    }
    else if (button_char == 'Y' || button_char == 'N') {
      lcd_display_button_r3(input);
    }
  }
}

void lcd_display_button_r3 (String input) {
  char button_char = input.charAt(2);
  char reg3_char = input.charAt(3);
  lcd.setCursor(0,1);
  if (button_char == 'Y') {
    lcd.print("BP");
    lcd.setCursor(3,1);
    if (reg3_char == 'Y') {
      lcd.print("R3P");
    }
    else if (reg3_char == 'N') {
      lcd.print("R3N");
    }
  }
  else if (button_char == 'N') {
    lcd.print("BNP");
    lcd.setCursor(4,1);
    if (reg3_char == 'Y') {
      lcd.print ("R3P");
    }
    else if (reg3_char == 'N') {
      lcd.print("R3N");
    }
  }
}
 
    
