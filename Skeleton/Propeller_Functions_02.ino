// declare pin numbers for the propellers
const int PROPELLER_SPEED_FRONT_PIN = 123;   
const int PROPELLER_SPEED_BACK_PIN = 23241;    
const int PROPELLER_PITCH_FRONT_PIN = 123;
const int PROPELLER_PITCH_BACK_PIN = 123;
const int PROPELLER_ROLL_FRONT_PIN = 123;
const int PROPELLER_ROLL_BACK_PIN = 123;

// declare variables
const int VERTICAL_PITCH_FRONT = 1500;   // note that these are dummy variables
const int VERTICAL_PITCH_BACK = 1500;    // vertical = 90 / perpendicular to ground
const int HORIZONTAL_PITCH_FRONT = 1000;
const int HORIZONTAL_PITCH_BACK = 1000;
const int WALL_PITCH_FRONT;
const int WALL_PITCH_BACK;
const int WALL_INITIAL_PITCH_FRONT;
const int WALL_INITIAL_PITCH_BACK;

// brushless motor duty cycle constants
const int MIN_PROPELLER_SPEED = 1040;
const int PROP_TRANSITION_TIME;
const int PROP_TRANSITION_DELAY;
const int PROP_TRANSITION_INCREMENTS;

const float FLOOR_MOUNT_DISTANCE = 1;


// declare classes
Servo propeller_speed_front;
Servo propeller_speed_back;
Servo pitch_motor_front;
Servo pitch_motor_back;


void propeller_setup () {
  //to be done later
}

void vertical_propeller() {
  pitch_motor_front.writeMicroseconds(VERTICAL_PITCH_FRONT);
  pitch_motor_back.writeMicroseconds(VERTICAL_PITCH_BACK);
}

void set_min_speed_propeller(){   
  propeller_speed_front.writeMicroseconds(MIN_PROPELLER_SPEED);
  propeller_speed_back.writeMicroseconds(MIN_PROPELLER_SPEED);
}

void straighten_setup() {
  set_min_speed_propeller();
  vertical_propeller();
}

void floor_to_wall() {
  // WITHOUT IMU  
  int angle_propeller_front = HORIZONTAL_PITCH_FRONT;       //initial position of back propeller on floor before proceeding to mount on wall
  int angle_propeller_back = VERTICAL_PITCH_BACK;           // initial position of back propeller on floor
  for(int i = 0; i < PROP_TRANSITION_INCREMENTS; i++) {
    pitch_motor_front.writeMicroseconds(angle_propeller_front);
    pitch_motor_back.writeMicroseconds(angle_propeller_back);
    angle_propeller_front += ((WALL_INITIAL_PITCH_FRONT- HORIZONTAL_PITCH_FRONT) / PROP_TRANSITION_INCREMENTS);    
    // DO I NEED A DELAY HERE? 
    angle_propeller_back += ((WALL_INITIAL_PITCH_BACK - VERTICAL_PITCH_BACK) / PROP_TRANSITION_INCREMENTS);
    delay (PROP_TRANSITION_DELAY);
  } 
}

void move_down_wall() {
  pitch_motor_front.writeMicroseconds(WALL_PITCH_FRONT);
  pitch_motor_back.writeMicroseconds(WALL_PITCH_BACK);
}

void wall_to_floor() {
  
}

void move_up_wall() {
  on_wall();
}


//void floor_to_wall_setup() {
//  float ultrasonic_tolerance = 0.5;     
//  float ultrasonic_dist_front = get_diff_distance(ULTRASONIC_TRIG_FRONT_LEFT, ULTRASONIC_ECHO_FRONT_LEFT, ULTRASONIC_TRIG_FRONT_RIGHT, ULTRASONIC_ECHO_FRONT_RIGHT);
//  if (ultrasonic_dist_front <= ultrasonic_tolerance) {
//     pitch_motor_front.writeMicroseconds(HORIZONTAL_PITCH_FRONT);
//     pitch_motor_back.writeMicroseconds(VERTICAL_PITCH_BACK);   
//  }
//}
