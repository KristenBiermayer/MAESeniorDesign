#include <SoftwareSerial.h>
#include <Servo.h>

const int servo_pin = 4;

Servo my_servo;
SoftwareSerial xbee(2,3);


void setup() {
  Serial.begin(9600);
  xbee.begin(9600);
  my_servo.attach(servo_pin);
}

void loop() {
  if (xbee.available()){
    int joystick_value = (xbee.readStringUntil('\n').toInt());
    Serial.write("Value from XBee ");
    Serial.println(joystick_value);
    int angle = map(joystick_value, 0, 1023, 1000, 2000);
    Serial.print("Value for Servo ");
    Serial.println(angle);
    my_servo.writeMicroseconds(angle);
  }
}
