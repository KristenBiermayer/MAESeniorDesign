#include <SoftwareSerial.h>
#include <Servo.h>

const int servo_right_pin = 4;
const int servo_left_pin = 5;

Servo servo_left;
Servo servo_right;
SoftwareSerial xbee(2,3);


void setup() {
  Serial.begin(9600);
  xbee.begin(9600);
  servo_left.attach(servo_left_pin);
  servo_right.attach(servo_right_pin);
}

void loop() {
  if (xbee.available()){
    int joystick_value = (xbee.readStringUntil('\n').toInt());
    Serial.write("Value from XBee ");
    Serial.println(joystick_value);
    int angle = map(joystick_value, 0, 1023, 1000, 2000);
    servo_left.writeMicroseconds(angle);
    servo_right.writeMicroseconds(angle);
    Serial.print("Value for Servo ");
    Serial.println(angle);
    }

}
