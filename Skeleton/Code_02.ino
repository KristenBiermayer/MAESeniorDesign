// declare global variables

// declare pin numbers
const int ULTRASONIC_FRONT = 3;
const int ULTRASONIC_BACK = 4;

// declare variables that set up ranges for robot's distance to the wall
const float FAR_WALL_THRESHOLD = 16;
const float NEAR_WALL_THRESHOLD = 0;

// declare variables that set up ranges for robot's distance to floor 
const float FAR_FLOOR_THRESHOLD = 16;
const float NEAR_FLOOR_THRESHOLD = 0;

bool is_on_wall = false;

// declare functions
float get_distance(int ultrasonic_pin) {
  // read from ultrasonic sensor and return distance from wall or floor depending on input     
}

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
  // put your setup code here, to run once:

}

void loop() {
  float distance_to_obstacle;
  if (!is_on_wall){
    distance_to_obstacle = get_distance(ULTRASONIC_FRONT);
    
  }
  else {
    distance_to_obstacle = get_distance(ULTRASONIC_BACK);
  }

  // if value returned by ultrasonic sensor 
  if (distance == -1.0){
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
    }
  }
}
