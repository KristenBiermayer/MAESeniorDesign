// declare pin numbers for the propellers  
const int BRUSHLESS_FRONT_PIN = 11;
const int BRUSHLESS_BACK_PIN = 12;  
const int PROPELLER_PITCH_FRONT_PIN = 123;
const int PROPELLER_PITCH_BACK_PIN = 123;
const int PROPELLER_ROLL_FRONT_PIN = 123;
const int PROPELLER_ROLL_BACK_PIN = 123;

// declare variables
const int VERTICAL_PITCH_FRONT = 1500;   // note that these are dummy variables
const int VERTICAL_PITCH_BACK = 1500;    // vertical = 90 / perpendicular to ground
const int HORIZONTAL_PITCH_FRONT = 1000;
const int HORIZONTAL_PITCH_BACK = 1000;
const int WALL_PITCH_DOWN_FRONT;
const int WALL_PITCH_DOWN_BACK;
const int WALL_PITCH_UP_FRONT;
const int WALL_PITCH_UP_BACK;
const int WALL_INITIAL_PITCH_FRONT;
const int WALL_INITIAL_PITCH_BACK;

// brushless motor duty cycle constants
const int MIN_PROPELLER_SPEED = 1040;
const int TRANSITION_SPEED = 1500;
const int PROP_TRANSITION_DELAY = 15;
const int PROP_TRANSITION_INCREMENTS = 5;

const float FLOOR_MOUNT_DISTANCE = 1;


// declare classes
Servo brushless_front;
Servo brushless_back;
Servo propeller_pitch_front;
Servo propeller_pitch_back;
Servo propeller_roll_front;
Servo propeller_roll_back;


void detach_all_propeller() {
  brushless_front.detach();
  brushless_back.detach();
  propeller_pitch_front.detach();
  propeller_pitch_back.detach();
  propeller_roll_front.detach();
  propeller_roll_back.detach();
}

void attach_all_propeller() {
  brushless_front.attach(BRUSHLESS_FRONT_PIN);
  brushless_back.attach(BRUSHLESS_BACK_PIN);
  propeller_pitch_front.attach(PROPELLER_PITCH_FRONT_PIN);
  propeller_pitch_back.attach(PROPELLER_PITCH_BACK_PIN);
  propeller_roll_front.attach(PROPELLER_ROLL_FRONT_PIN);
  propeller_roll_back.attach(PROPELLER_ROLL_BACK_PIN);
}

void orient_speed() {
  detach_all_steering();
  detach_all_propeller();
  int pot_input = get_potentiometer();  
  int brushless_speed = map(pot_input,0,1023,1000, 2000);
  attach_all_steering();
  attach_all_propeller();
  brushless_front.writeMicroseconds(brushless_speed);
  brushless_back.writeMicroseconds(brushless_speed);
  delay(300);
  detach_all_steering();
  detach_all_propeller();
}

void vertical_propeller() {
  propeller_pitch_front.writeMicroseconds(VERTICAL_PITCH_FRONT);
  propeller_pitch_back.writeMicroseconds(VERTICAL_PITCH_BACK);
}

void set_min_speed_propeller(){   
  brushless_front.writeMicroseconds(MIN_PROPELLER_SPEED);
  brushless_back.writeMicroseconds(MIN_PROPELLER_SPEED);
}

void straighten_setup() {
  set_min_speed_propeller();
  vertical_propeller();
}

void floor_to_wall() {
  // WITHOUT IMU  
  brushless_front.writeMicroseconds(TRANSITION_SPEED);
  brushless_back.writeMicroseconds(TRANSITION_SPEED);
  int angle_propeller_front = HORIZONTAL_PITCH_FRONT;       //initial position of back propeller on floor before proceeding to mount on wall
  int angle_propeller_back = VERTICAL_PITCH_BACK;           // initial position of back propeller on floor
  for(int i = 0; i < PROP_TRANSITION_INCREMENTS; i++) {
    propeller_pitch_front.writeMicroseconds(angle_propeller_front);
    propeller_pitch_back.writeMicroseconds(angle_propeller_back);
    delay (PROP_TRANSITION_DELAY);
    angle_propeller_front += ((WALL_INITIAL_PITCH_FRONT- HORIZONTAL_PITCH_FRONT) / PROP_TRANSITION_INCREMENTS);    
    angle_propeller_back += ((WALL_INITIAL_PITCH_BACK - VERTICAL_PITCH_BACK) / PROP_TRANSITION_INCREMENTS);
  } 
}

void move_down_wall() {
  propeller_pitch_front.writeMicroseconds(WALL_PITCH_DOWN_FRONT);
  propeller_pitch_back.writeMicroseconds(WALL_PITCH_DOWN_BACK);
}

void wall_to_floor() {
  propeller_pitch_back.writeMicroseconds(HORIZONTAL_PITCH_BACK);
  
}

void move_up_wall() {
  propeller_pitch_front.writeMicroseconds(WALL_PITCH_UP_FRONT);
  propeller_pitch_back.writeMicroseconds(WALL_PITCH_UP_BACK);
}
