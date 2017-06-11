#include <LiquidCrystal.h>
LiquidCrystal lcd(12,11,5,4,10,9);
int button_pin = 7;
int second_pin = 6;
int joystick_pin = A1;
int potentiometer = A0;

void setup() {
  lcd.begin (16,2);
  pinMode(button_pin, INPUT);
  pinMode(second_pin, INPUT);
  Serial.begin(9600);
}

void loop() {
  lcd.setCursor(0,0);
  lcd.print ("Hello");
  lcd.setCursor(0,1);
  lcd.print ("omg");
  lcd.setCursor (4,1);
  lcd.print ("Hi");
  Serial.print("The Button Value for 1 is ");
  Serial.println(digitalRead(button_pin));
  Serial.print("The Button value for 2 is ");
  Serial.println(digitalRead(second_pin));
  Serial.print("The potentiometer value is ");
  Serial.println(analogRead(potentiometer));
  int potentiometer_map = map(analogRead(potentiometer), 0, 1023, 1000, 1100);
  Serial.print("Mapped Value is ");
  Serial.println(potentiometer_map);
  Serial.print("The joystick value is ");
  Serial.println(analogRead(joystick_pin));
  delay(2000);
  
  
}
