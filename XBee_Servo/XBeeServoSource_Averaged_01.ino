//Receiving Data
#include <Servo.h>
#include <SoftwareSerial.h>

const int joystick_pin = A1;

// for AveragedJoystickValue()
const int numReadings = 10;
int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average


SoftwareSerial xbee(2,3);

void setup() {
  Serial.begin(9600);
  xbee.begin(9600);
  for (int thisReading = 0; thisReading < numReadings; thisReading++)
    readings[thisReading] = 0;
}

void loop() {
  int joystick_value = AveragedJoystickValue(analogRead(joystick_pin));
  xbee.print(joystick_value);
  xbee.println();
  Serial.print(joystick_value);
  Serial.println();
  delay(1);
}

int AveragedJoystickValue(int inputValue)
{
  total = total - readings[readIndex];
  readings[readIndex] = inputValue;
  total = total + readings[readIndex];
  readIndex++;

  if (readIndex >= numReadings)
    readIndex = 0;

  Serial.print("The array: ");
  for (int i =0; i<=numReadings;i++)
  {
    Serial.print(readings[i]);
    Serial.print(" ");
  }
  Serial.print("\n");

  average = total / numReadings;
  //Serial.println(average);
  return average;
}
