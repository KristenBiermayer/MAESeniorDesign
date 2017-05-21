// declare pin numbers for the propellers
const int PROPELLER_SPEED_FRONT_PIN = 123;   
const int PROPELLER_SPEED_BACK_PIN = 23241;    
const int PROPELLER_PITCH_FRONT_PIN = 123;
const int PROPELLER_PITCH_BACK_PIN = 123;
const int PROPELLER_ROLL_FRONT_PIN = 123;
const int PROPELLER_ROLL_BACK_PIN = 123;

// declare classes
Servo propeller_speed_front;
Servo propeller_speed_back;

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
