//Ultrasonic Sensor HC SR04 
// Functions defined include:
//    void ultrasonic_setup   (used to setup the ultrasonic echo and trig pins)
//    float get_raw_distance  (used to send ultrasonic sound wave)
//                            (provides an unfiltered value for distance in inches)
//    float get_distance      (used to get distance from the robot on wall or floor)
//                            (depends on whether !on_wall is true or false)
  
// define pin numbers for ultrasonic sensor
const int ULTRASONIC_FRONT_TRIG = A0;
const int ULTRASONIC_FRONT_ECHO = A1;
const int ULTRASONIC_BACK_TRIG = A2;
const int ULTRASONIC_BACK_ECHO = A3;

// Define constant values used throughout ultrasonic related operations
const float ULTRASONIC_DELAY_MS = 1000;   // Delay period between each iteration of get_distance function. MUST ACCOUNT FOR SKELETON LOOP TIME
const float MAX_SPEED = 4;                // Maximum operable speed of our robot in inches per sec
const int MAX_TRIES_ULTRASONIC = 10;      // number of times ultrasonic sensor will try to measure distance upon receiving invalid measure

// measure the maximum distance robot can travel using MAX_SPEED 
// MAX_SPEED will be measured by us (we can run robot at maximum operable speeds and measure speed) 
// MAX_SPEED must be in measured in in/s! 
const float MAX_DISTANCE_TRAVELED = MAX_SPEED * (ULTRASONIC_DELAY_MS / 1000);

// define variables for use throughout the program (that need to be stored with every iteration) (useful for calibrating)
float previous_distance;      //current distance to obstacle

void ultrasonic_setup(){
  pinMode (ULTRASONIC_FRONT_TRIG,OUTPUT);   //define trigPin as an output as it will send ultrasound waves
  pinMode(ULTRASONIC_BACK_TRIG,OUTPUT);
  pinMode (ULTRASONIC_FRONT_ECHO,INPUT);    //define echoPin as an input as it will receive ultrasound waves from trigPin
  pinMode(ULTRASONIC_BACK_ECHO,INPUT);
  previous_distance = get_raw_distance(ULTRASONIC_FRONT_TRIG, ULTRASONIC_FRONT_ECHO);
}

float get_raw_distance(int ultrasonic_pin_trig, int ultrasonic_pin_echo){
  float distance;
  // clear trigPin
  digitalWrite (ultrasonic_pin_trig,LOW);
  delayMicroseconds(2);

  //send ultrasound wave
  digitalWrite (ultrasonic_pin_trig,HIGH);
  delayMicroseconds(10); 
  digitalWrite(ultrasonic_pin_trig,LOW);
  
  //read the echopin. value returned measures time of TOTAL sound wave travel (from being transmitted to being received)
  float duration = pulseIn (ultrasonic_pin_echo,HIGH);

  //convert values to distance; converting seconds to cm using speed of sound (340 m/s)
  //divide by two to get the time traveled from hitting obstacle to being received
  float distance_cm = (duration*0.034)/2;

  //convert to inches
  distance = distance_cm * 0.3937; 
  return distance;
}

float get_distance(int ultrasonic_pin_trig, int ultrasonic_pin_echo) {
  float distance;
  
  // if distance traveled measured by Ultrasonic is greater than what our robot can physically achieve, value is invalid
  // for loop will return -1 and code will re-run until valid measure is given
  for (int attempts = 1; attempts <= MAX_TRIES_ULTRASONIC; attempts++) {
    distance = get_raw_distance(ultrasonic_pin_trig, ultrasonic_pin_echo);

    // check if distance traveled measured by ultrasonic sensor is achievable by our robot
    float distance_traveled = distance - previous_distance;
    
    //convert to positive number to account for robot traveling away or towards the wall 
    if (distance_traveled < 0){
      distance_traveled = distance_traveled * -1;
    }

    // if value measured by ultrasonic sensor is valid, the for loop will exit. 
    if (distance_traveled < MAX_DISTANCE_TRAVELED * attempts) {
      previous_distance = distance;
      break;
    }
    // if distance is not valid then the loop will run again. if attempts = MAX_TRIES_ULTRASONIC, then the loop will exit and print message indicating failure.
    else {
      distance = -1;
      delay(ULTRASONIC_DELAY_MS);
    }
  } 

  if (distance == -1){
    Serial.print("You screwed up");
    Serial.println();
    previous_distance = get_raw_distance(ultrasonic_pin_trig, ultrasonic_pin_echo);
  }
  
  Serial.print("Distance:");
  Serial.println(distance);
  return distance;  
}
