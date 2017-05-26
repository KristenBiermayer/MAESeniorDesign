// declare pin numbers for the propellers
const int PROPELLER_SPEED_FRONT_PIN = 123;   
const int PROPELLER_SPEED_BACK_PIN = 23241;    
const int PROPELLER_PITCH_FRONT_PIN = 123;
const int PROPELLER_PITCH_BACK_PIN = 123;
const int PROPELLER_ROLL_FRONT_PIN = 123;
const int PROPELLER_ROLL_BACK_PIN = 123;

// declare variables
const int VERTICAL_PITCH_FRONT = 1500;   // note that these are dummy variables
const int VERTICAL_PITCH_BACK = 1500;    // vertical = 90 / perpendicular to grond
const int HORIZONTAL_PITCH_FRONT = 1000;
const int HORIZONTAL_PITCH_BACK = 1000;


const float FLOOR_MOUNT_DISTANCE = 1;

// declare classes
Servo propeller_speed_front;
Servo propeller_speed_back;
Servo pitch_motor_front;
Servo pitch_motor_back;

bool is_speed_zero(){
  return false;
}

void propeller_setup () {
  //to be done later
}

void on_floor() {
  
}

void set_min_speed_propeller(){
  int min_propeller_speed = 12;     
  propeller_speed_front.writeMicroseconds(min_propeller_speed);
  propeller_speed_back.writeMicroseconds(min_propeller_speed);
}

void straighten_setup() {
  
}

void floor_to_wall_setup() {
  float ultrasonic_tolerance = 0.5;     
  float ultrasonic_dist_front = get_diff_distance(ULTRASONIC_TRIG_FRONT_LEFT, ULTRASONIC_ECHO_FRONT_LEFT, ULTRASONIC_TRIG_FRONT_RIGHT, ULTRASONIC_ECHO_FRONT_RIGHT);
  if (ultrasonic_dist_front <= ultrasonic_tolerance) {
     pitch_motor_front.writeMicroseconds(HORIZONTAL_PITCH_FRONT);
     pitch_motor_back.writeMicroseconds(VERTICAL_PITCH_BACK);
    
  }
}

void floor_to_wall() {
  // controlling orientation of propellers when robot is transitioning from floor to wall
  // make sure the robot is a certain height above the threshold before allowing the ultrasonic sensor in the back to operate! 
}

void on_wall() {
  // controlling orientation of propellers when robot is on wall
}

void wall_to_floor() {
  // controlling orientation of propellers when robot is getting off wall
}

void move_up_wall() {
  // gets robot to region 4
}
