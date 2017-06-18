// XBee and Steering Control
// Functions defined include:
// void steering_setup()      (setup the servomotors to the correct pins)
// float orient_steering()    (use the joystick commands sent from the XBee to orient direction of servomotors)
//                            (parallel steering - servomotors turn to result in same output angle)

// declare pins
const int SERVO_LEFT_PIN = 4;
const int SERVO_RIGHT_PIN = 5;


// declare classes
Servo servo_left;
Servo servo_right;
// declare variables

void detach_all_steering() {
  servo_left.detach();
  servo_right.detach();
  
}

void attach_all_steering() {
  servo_left.attach(SERVO_LEFT_PIN);
  servo_right.attach(SERVO_RIGHT_PIN);
}

void orient_steering(){
  detach_all();
  int joystick_input = get_joystick();
  int servo_angle_left = map(joystick_input, 0,1023,1000, 1333);  // the joystick takes analog values from 0 to 1023. writeMicroseconds command takes 1000 to 2000 as input. use the map function to assign our joystick input to our new range
  int servo_angle_right = map(joystick_input, 0, 1023, 1333, 1080);
  attach_all();
  servo_left.writeMicroseconds(servo_angle_left);
  servo_right.writeMicroseconds(servo_angle_right);
  delay(200); 
  detach_all();
}

// 75, 15 for wall
void region_four() {
  orient_steering();
  orient_speed('F');
}

void region_three() {
  set_constant_speed("Minimum");
  orient_propeller_angle ('P', 90, 0);
  orient_propeller_angle ('R', 170, 163);
  orient_steering();
}

void region_two() {
  set_constant_speed("Minimum");
  orient_propeller_angle ('P', 90, 0);
  orient_propeller_angle ('R', 170, 163);
}

void region_one(int iteration) {
  floor_to_wall_speed();
  orient_propeller_angle ('R', 170, 163); 
  floor_to_wall(iteration);         
}

void wall_climb() {
  orient_propeller_angle('P', 75, 15);
  orient_propeller_angle ('R', 170, 163);
  orient_speed('W');
}
