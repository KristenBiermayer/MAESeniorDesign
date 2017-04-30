//Ultrasonic Sensor HC SR04 

// define pin numbers 
const int trigPin = A0;
const int echoPin = A1; 

// Define constant values used throughout program
const float DELAY_MS = 1000;  // Delay period between each iteration of get_distance function. MUST ACCOUNT FOR SKELETON LOOP TIME
const float MAX_SPEED = 4;   // Maximum operable speed of our robot in inches per sec
const int MAX_TRIES_ULTRASONIC = 10;  // number of times ultrasonic sensor will try to measure distance upon receiving invalid measure

// measure the maximum distance robot can travel using MAX_SPEED 
// MAX_SPEED will be measured by us (we can run robot at maximum operable speeds and measure speed) 
// MAX_SPEED must be in measured in in/ms! 
const float MAX_DISTANCE_TRAVELED = MAX_SPEED * (DELAY_MS / 1000);

// define variables for use throughout the program (that need to be stored with every iteration) (useful for calibrating)
float previous_distance;      //current distance to obstacle

void setup () {
  pinMode (trigPin,OUTPUT);   //define trigPin as an output as it will send ultrasound waves
  pinMode (echoPin,INPUT);    //define echoPin as an input as it will receive ultrasound waves from trigPin
  Serial.begin(9600);        // begin serial communication
  previous_distance = get_raw_distance();
}

float get_raw_distance(){
  float distance;
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
  distance = distance_cm * 0.3937; 
  return distance;
}

float get_distance() {
  float distance;
  // if distance traveled measured by Ultrasonic is greater than what our robot can physically achieve, value is invalid
  // for loop will return -1 and code will re-run until valid measure is given
  for (int attempts = 1; attempts <= MAX_TRIES_ULTRASONIC; attempts++) {
    distance = get_raw_distance();
  
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
      delay(DELAY_MS);
    }
  }

  if (distance == -1){
    Serial.print("You screwed up");
  }
  
  Serial.print("Distance:");
  Serial.println(distance);
  return distance;  
}

void loop() {
  float distance = get_distance();
  delay(DELAY_MS);
}
