#include <Servo.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd (12, 11, 5, 4, 10, 9);

const int pot_pin = A0;
const int joystick_pin = A1;
const int button_pin = 7;


SoftwareSerial xbee(2,3);

void setup() {
  Serial.begin(9600); 
  xbee.begin(9600);
  pinMode(button_pin,INPUT);
  lcd.begin (16,2);
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
    else if (id_char == 's') {
      button_source_setup();
    }
    else if (id_char == 'm') {
      button_source_mount ();
    } 
    else if (id_char == 'L'){
      lcd_display(command_input);
    }
  }
}
