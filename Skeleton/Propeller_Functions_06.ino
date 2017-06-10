// declare pin numbers for the propellers  
const int BRUSHLESS_FRONT_PIN = 6;
const int BRUSHLESS_BACK_PIN = 7;  
const int PROPELLER_PITCH_FRONT_PIN = 11;
const int PROPELLER_PITCH_BACK_PIN = 12;
const int PROPELLER_ROLL_FRONT_PIN = 8;
const int PROPELLER_ROLL_BACK_PIN = 9;

// declare variables
const int VERTICAL_PITCH_FRONT = 90;   // note that these are dummy variables
const int VERTICAL_PITCH_BACK = 0;    // vertical = 90 / perpendicular to ground
const int HORIZONTAL_PITCH_FRONT = 180;
const int HORIZONTAL_PITCH_BACK = 1000;
const int WALL_PITCH_DOWN_FRONT;
const int WALL_PITCH_DOWN_BACK;
const int WALL_PITCH_UP_FRONT;
const int WALL_PITCH_UP_BACK;
const int WALL_INITIAL_PITCH_FRONT;
const int WALL_INITIAL_PITCH_BACK;

// brushless motor duty cycle constants
const int MIN_PROPELLER_SPEED = 1040;
const int FRONT_TRANSITION_SPEED = 1500;
const int BACK_TRANSITION_SPEED = 1500;
const int DOWN_WALL_SPEED = 1000;
const int UP_WALL_SPEED = 1000;
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
  int brushless_speed = map(pot_input,0,1023,1000,1200);
  attach_all_steering();
  attach_all_propeller();
  brushless_front.writeMicroseconds(brushless_speed);
  brushless_back.writeMicroseconds(brushless_speed);
  delay(300);
  detach_all_steering();
  detach_all_propeller();
}

void vertical_propeller() {
  attach_all_steering();
  attach_all_propeller();
  propeller_pitch_front.write(90);
  propeller_pitch_back.write(0);
  detach_all_steering();
  detach_all_propeller();
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
  brushless_front.writeMicroseconds(FRONT_TRANSITION_SPEED);
  brushless_back.writeMicroseconds(BACK_TRANSITION_SPEED);
  int angle_propeller_front = 180;      //initial position of front propeller on floor before proceeding to mount on wall
  int angle_propeller_back = 0;           // initial position of back propeller on floor
  for(int i = 0; i < PROP_TRANSITION_INCREMENTS; i++) {
    propeller_pitch_front.write(angle_propeller_front);
    propeller_pitch_back.write(angle_propeller_back);
    delay (PROP_TRANSITION_DELAY);
    angle_propeller_front += ((180 - 45) / PROP_TRANSITION_INCREMENTS);    
    angle_propeller_back += ((45 - 0) / PROP_TRANSITION_INCREMENTS);
  } 
}

void move_down_wall() {
  brushless_front.writeMicroseconds(DOWN_WALL_SPEED);
  brushless_back.writeMicroseconds(DOWN_WALL_SPEED);
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
