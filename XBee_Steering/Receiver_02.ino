#include <SoftwareSerial.h>
#include <Servo.h>

const int servo_right_pin = 5;
const int servo_left_pin = 8;
const int fake_brushless_pin = 7;

Servo servo_left;
Servo servo_right;
Servo fake_brushless;
SoftwareSerial xbee(2,3);


void setup() {
  Serial.begin(9600);
  xbee.begin(9600);
  servo_left.attach(servo_left_pin);
  servo_right.attach(servo_right_pin);
  fake_brushless.attach(fake_brushless_pin);
  
}

void loop() {
  if (xbee.available()){
    String raw_input = xbee.readStringUntil('\n');
    char id_char = raw_input.charAt(0);
    if (id_char == 'j') {
      int joystick_input = raw_input.substring(1).toInt();
      Serial.print("The Joystick Input is");
      Serial.println(joystick_input);
      int servo_angle = map(joystick_input, 0,1023,1000,2000);
      Serial.print("The Joystick Input is");
      Serial.println(servo_angle);
      servo_left.writeMicroseconds(servo_angle);
      delay(100);
      servo_right.writeMicroseconds(servo_angle);    
      delay(100);
      
    } else {
      int pot_input = raw_input.substring(1).toInt();
      Serial.print("The Potentiometer Input is ");
      Serial.println(pot_input);
      int brushless_speed = map(pot_input,0,1023,1000, 2000);
      Serial.print("The Output Brushless Motor Speed is ");
      Serial.println(brushless_speed);
      fake_brushless.writeMicroseconds(brushless_speed);
      delay(100);
    }
  }
}
