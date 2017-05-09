// declare global variables

// declare pin numbers
// define pin numbers for ultrasonic sensor
extern const int ULTRASONIC_FRONT_TRIG;
extern const int ULTRASONIC_FRONT_ECHO;
extern const int ULTRASONIC_BACK_TRIG;
extern const int ULTRASONIC_BACK_ECHO;
extern const float ULTRASONIC_DELAY_MS;
extern float previous_distance;

// declare variables that set up ranges for robot's distance to the wall
const float FAR_WALL_THRESHOLD = 2;
const float NEAR_WALL_THRESHOLD = 0;

// declare variables that set up ranges for robot's distance to floor 
const float FAR_FLOOR_THRESHOLD = 2;
const float NEAR_FLOOR_THRESHOLD = 0;

bool is_on_wall = false;

// declare functions (to be removed later once file is added) 
float orient_steering(){
  // read from joysticks and orient servomotor position
}

void on_floor(){
  // controlling orientation of propellers when robot is on the floor 
}

void floor_to_wall(){
  // controlling orientation of propellers when robot is transitioning from floor to wall
}

void on_wall(){
  // controlling orientation of propellers when robot is on wall
}

void wall_to_floor(){
  // controlling orientation of propellers when robot is getting off wall 
}

void setup() { 
  ultrasonic_setup();
  Serial.begin(9600);  
}

void loop() {
  float distance_to_obstacle;
  
  if (!is_on_wall){
    distance_to_obstacle = get_distance(ULTRASONIC_FRONT_TRIG, ULTRASONIC_FRONT_ECHO);
  }
  else {
    distance_to_obstacle = get_distance(ULTRASONIC_BACK_TRIG, ULTRASONIC_BACK_ECHO);
  }

  // if value returned by ultrasonic sensor 
  if (distance_to_obstacle == -1.0){
    return;
  }
  
  // when robot is on the floor
  if (!is_on_wall){
    // when robot is far away from wall, robot movement will be controlled by joysticks/propellers
    if (distance_to_obstacle > FAR_WALL_THRESHOLD) {
      on_floor();
      orient_steering();
    }
  
    // when robot is transitioning from floor to wall
    else if (distance_to_obstacle <= FAR_WALL_THRESHOLD && distance_to_obstacle >= NEAR_WALL_THRESHOLD) {
      floor_to_wall();
      is_on_wall = true; 
      previous_distance = get_raw_distance(ULTRASONIC_BACK_TRIG, ULTRASONIC_BACK_ECHO);
    }
  }

  // when robot is on the wall
  else {
    // when robot is far away from floor, robot movement will be controlled by joysticks/propellers
    if (distance_to_obstacle > FAR_FLOOR_THRESHOLD){
      on_wall();
    }
    
    // when robot is transitioning from wall to floor
    else if (distance_to_obstacle <= FAR_FLOOR_THRESHOLD && distance_to_obstacle >= NEAR_FLOOR_THRESHOLD){
      wall_to_floor();
      is_on_wall = false;
      previous_distance = get_raw_distance(ULTRASONIC_FRONT_TRIG, ULTRASONIC_FRONT_ECHO);
    }
  }

}
