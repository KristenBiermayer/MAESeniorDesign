//Ultrasonic Sensor HC SR04 

// define pin numbers 
const int trigPin = A0;
const int echoPin = A1; 

// Define constant values used throughout program
const float DELAY_MS = 1000;  // Delay period between ultrasonic pulses
const float MAX_SPEED = 10;   // Maximum operable speed of our robot

// define variables for use throughout the program (that need to be stored with every iteration) (useful for calibrating)
float previous_distance;      //current distance to obstacle

void setup () {
  pinMode (trigPin,OUTPUT);   //define trigPin as an output as it will send ultrasound waves
  pinMode (echoPin,INPUT);    //define echoPin as an input as it will receive ultrasound waves from trigPin
  Serial.begin(9600);        // begin serial communication
}

float get_distance() {
  // clear trigPin
  digitalWrite (trigPin,LOW);
  delayMicroseconds(2);

  //send ultrasound wave
  digitalWrite (trigPin,HIGH);
  delayMicroseconds(10); 
  digitalWrite(trigPin,LOW);
  
  //read the echopin. value returned measures time of TOTAL sound wave travel (from being transmitted to being received)
  float duration = pulseIn (echoPin,HIGH);

  //convert values to distance; converting seconds to cm using speed of sound (340 m/s)
  //divide by two to get the time traveled from hitting obstacle to being received
  float distance_cm = (duration*0.034)/2;

  //convert to inches
  float distance = distance_cm * 0.3937; 

  float distance_traveled = distance - previous_distance;
 
  //convert to positive number to account for robot traveling away or towards the wall 
  if (distance_traveled < 0){
    distance_traveled = distance_traveled * -1;
  }

  // measure the maximum distance robot can travel using MAX_SPEED 
  // MAX_SPEED will be measured by us (we can run robot at maximum operable speeds and measure speed) 
  // MAX_SPEED must be in measured in in/ms! 
  float max_distance_traveled = MAX_SPEED * DELAY_MS;

  // if distance traveled measured by Ultrasonic is greater than what our robot can physically achieve, value is invalid
  if (distance_traveled > max_distance_traveled) {
    return -1.0;
  } else {
    previous_distance = distance;
  }
  
  Serial.print("Distance:");
  Serial.println(distance);
  
  return distance;  
}

void loop() {
  float distance = get_distance();
}
