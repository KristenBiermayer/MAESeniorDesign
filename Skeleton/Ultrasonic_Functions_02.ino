//Ultrasonic Sensor HC SR04 
// Functions defined include:
//    void ultrasonic_setup   (used to setup the ultrasonic echo and trig pins)
//    float get_raw_distance  (used to send ultrasonic sound wave)
//                            (provides an unfiltered value for distance in inches)
//    float get_distance      (used to get distance from the robot on wall or floor)
//                            (depends on whether !on_wall is true or false)
  
// define pin numbers for ultrasonic sensor
const int ULTRASONIC_TRIG_FRONT_LEFT = A0;
const int ULTRASONIC_ECHO_FRONT_LEFT = A1;
const int ULTRASONIC_TRIG_FRONT_RIGHT = A2;
const int ULTRASONIC_ECHO_FRONT_RIGHT = A3;
const int ULTRASONIC_TRIG_BACK = A4;
const int ULTRASONIC_ECHO_BACK = A5;

// Define constant values used throughout ultrasonic related operations
const float ULTRASONIC_DELAY_MS = 100;   // Delay period between each iteration of get_distance function. MUST ACCOUNT FOR SKELETON LOOP TIME
const float MAX_SPEED = 4;                // Maximum operable speed of our robot in inches per sec
const int MAX_TRIES_ULTRASONIC = 10;      // number of times ultrasonic sensor will try to measure distance upon receiving invalid measure
const float ULTRASONIC_DISTANCE_ERROR = 1;
const float WALL_MOUNT_DISTANCE = 0.5;
const float ULTRASONIC_DIFFERENCE_ERROR = 1;

// measure the maximum distance robot can travel using MAX_SPEED 
// MAX_SPEED will be measured by us (we can run robot at maximum operable speeds and measure speed) 
// MAX_SPEED must be in measured in in/s! 
const float MAX_DISTANCE_TRAVELED = MAX_SPEED * (ULTRASONIC_DELAY_MS / 1000);

// define variables for use throughout the program (that need to be stored with every iteration) (useful for calibrating)
float previous_distance;      //current distance to obstacle

void ultrasonic_setup(){
  pinMode (ULTRASONIC_TRIG_FRONT_LEFT,OUTPUT);   //define trigPin as an output as it will send ultrasound waves
  pinMode (ULTRASONIC_TRIG_FRONT_RIGHT, OUTPUT);
  pinMode(ULTRASONIC_TRIG_BACK,OUTPUT);
  pinMode (ULTRASONIC_ECHO_FRONT_LEFT,INPUT);    //define echoPin as an input as it will receive ultrasound waves from trigPin
  pinMode (ULTRASONIC_ECHO_FRONT_RIGHT,INPUT);
  pinMode(ULTRASONIC_ECHO_BACK,INPUT); 
}


// NOTE : We decided to have 2 ultrasonic sensors in the front of the robot. I have not had the time to add that to this code yet
// but I will get that done tonight after Mother's Day! 
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

float get_min_distance(int ultrasonic_pin_trig_left, int ultrasonic_pin_echo_left, int ultrasonic_pin_trig_right, int ultrasonic_pin_echo_right){
  float distance_to_obstacle_left = get_distance(ultrasonic_pin_trig_left, ultrasonic_pin_echo_left);
  float distance_to_obstacle_right = get_distance(ultrasonic_pin_trig_right, ultrasonic_pin_echo_right);
  float distance_to_obstacle = min(distance_to_obstacle_left, distance_to_obstacle_right);
  Serial.print( "Minimum Distance to Obstacle: ");
  Serial.println(distance_to_obstacle);
  return distance_to_obstacle; 
}

float get_raw_min_distance (int ultrasonic_pin_trig_left, int ultrasonic_pin_echo_left, int ultrasonic_pin_trig_right, int ultrasonic_pin_echo_right) {
  float distance_to_obstacle_left = get_raw_distance(ultrasonic_pin_trig_left, ultrasonic_pin_echo_left);
  float distance_to_obstacle_right = get_raw_distance(ultrasonic_pin_trig_right, ultrasonic_pin_echo_right);
  float distance_to_obstacle = min(distance_to_obstacle_left, distance_to_obstacle_right);
  Serial.print( "Minimum Distance to Obstacle: ");
  Serial.println(distance_to_obstacle);
  return distance_to_obstacle; 
}

float get_diff_distance (int ultrasonic_pin_trig_left, int ultrasonic_pin_echo_left, int ultrasonic_pin_trig_right, int ultrasonic_pin_echo_right){
  float distance_to_obstacle_left = get_distance(ultrasonic_pin_trig_left, ultrasonic_pin_echo_left);
  float distance_to_obstacle_right = get_distance(ultrasonic_pin_trig_right, ultrasonic_pin_echo_right);
  float dist_diff = distance_to_obstacle_left - distance_to_obstacle_right;

  if (dist_diff < 0) {
    dist_diff = dist_diff * -1;
  }  
}

bool is_speed_zero(){
  return false;
}
