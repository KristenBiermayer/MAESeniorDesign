// declare pin numbers for the propellers  
const int BRUSHLESS_FRONT_PIN = 6;
const int BRUSHLESS_BACK_PIN = 7;  
const int PROPELLER_PITCH_FRONT_PIN = 10;
const int PROPELLER_PITCH_BACK_PIN = 13;
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
int angle_propeller_front = 180;
int angle_propeller_back = 0;

// brushless motor duty cycle constants
const int MIN_PROPELLER_SPEED = 1100;
const int FRONT_TRANSITION_SPEED = 1400;
const int BACK_TRANSITION_SPEED = 1400;
const int DOWN_WALL_SPEED = 1000;
const int UP_WALL_SPEED = 1000;
const int PROP_TRANSITION_DELAY = 300;
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
  int brushless_speed = map(pot_input,0,1023,1000,1600);
  attach_all_steering();
  attach_all_propeller();
  brushless_front.writeMicroseconds(brushless_speed);
  brushless_back.writeMicroseconds(brushless_speed);
  delay(300);
  detach_all_steering();
  detach_all_propeller();
}

void set_min_speed_propeller(){ 
//  Serial.println("set min speed function");
  detach_all_steering();
  detach_all_propeller();
  int brushless_min_speed = get_speed('M');
  attach_all_steering();
  attach_all_propeller();
  brushless_front.writeMicroseconds(brushless_min_speed);
  brushless_back.writeMicroseconds(brushless_min_speed);
  delay(300);
  detach_all_steering();
  detach_all_propeller();
  //Serial.println ("End of set min speed");
}

void vertical_propeller() {
  detach_all_steering();
  detach_all_propeller();
  attach_all_steering();
  attach_all_propeller();
  propeller_pitch_front.write(75);
  propeller_pitch_back.write(15);
//  propeller_pitch_front.write(90);
//  propeller_pitch_back.write(0);
  detach_all_steering();
  detach_all_propeller();
}

void floor_to_wall_speed() {
  detach_all_steering();
  detach_all_propeller();
  int brushless_speed = get_speed('F');
  attach_all_steering();
  attach_all_propeller();
  brushless_front.writeMicroseconds(brushless_speed);
  brushless_back.writeMicroseconds(brushless_speed);
  delay(300);
  detach_all_steering();
  detach_all_propeller();
}

void floor_to_wall(int iteration) {
// WITHOUT IMU 
  attach_all_propeller();
  attach_all_steering(); 
  if (iteration == 0) {
    propeller_pitch_front.write(180);
    propeller_pitch_back.write(0);
  }
  else if (iteration == 1) {
    propeller_pitch_front.write(45);
    propeller_pitch_back.write(45); 
  }
  delay(300);
//  Serial.print("angle_propeller_back = ");
//  Serial.println(angle_propeller_back);   
//  Serial.print("angle_propeller_front = ");
//  Serial.println(angle_propeller_front);  
//  angle_propeller_front -= ((180 - 45) / 2); 
//  angle_propeller_back += ((45 - 0) / 2);
  detach_all_propeller();
  detach_all_steering();
}

void roll_straight() {
  detach_all_steering();
  detach_all_propeller();
  attach_all_steering();
  attach_all_propeller();
  propeller_roll_front.write(167);
  propeller_roll_back.write(162);
  detach_all_steering();
  detach_all_propeller();
}

void move_down_wall_speed() {
  detach_all_steering();
  detach_all_propeller();
  int brushless_speed = get_speed('D');
  attach_all_steering();
  attach_all_propeller();
  brushless_front.writeMicroseconds(brushless_speed);
  brushless_back.writeMicroseconds(brushless_speed);
  delay(300);
  detach_all_steering();
  detach_all_propeller();
}

void move_down_wall() {
  attach_all_steering();
  attach_all_propeller();
  propeller_pitch_front.write(30);
  propeller_pitch_back.write(45);
  detach_all_steering();
  detach_all_propeller();
}

void wall_to_floor() {
  int variable;
  brushless_front.writeMicroseconds(variable);
  brushless_back.writeMicroseconds(variable);
  int angle_propeller_front = 0;      //initial position of front propeller on floor before proceeding to mount on wall
  propeller_pitch_front.write(angle_propeller_front);
  int angle_propeller_back = 45;           // initial position of back propeller on floor
  for(int i = 0; i < PROP_TRANSITION_INCREMENTS; i++) {
    propeller_pitch_back.write(angle_propeller_back);
    delay (PROP_TRANSITION_DELAY); 
    angle_propeller_back += ((180-45) / PROP_TRANSITION_INCREMENTS);
  }
}

void move_up_wall_speed() {
  detach_all_steering();
  detach_all_propeller();
  int brushless_speed = get_speed('C');
  attach_all_steering();
  attach_all_propeller();
  brushless_front.writeMicroseconds(brushless_speed);
  brushless_back.writeMicroseconds(brushless_speed);
  delay(300);
  detach_all_steering();
  detach_all_propeller();
}

void move_up_wall() {
  attach_all_steering();
  attach_all_propeller();
  propeller_pitch_front.write(45);
  propeller_pitch_back.write(45);
  detach_all_steering();
  detach_all_propeller();
}
