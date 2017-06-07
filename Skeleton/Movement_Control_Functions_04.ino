// XBee and Steering Control
// Functions defined include:
// void steering_setup()      (setup the servomotors to the correct pins)
// float orient_steering()    (use the joystick commands sent from the XBee to orient direction of servomotors)
//                            (parallel steering - servomotors turn to result in same output angle)

// declare pins
const int SERVO_LEFT_PIN = 5;
const int SERVO_RIGHT_PIN = 6;
const int BRUSHLESS_FRONT_PIN = 12;
const int BRUSHLESS_BACK_PIN = 11;

// declare classes
Servo servo_left;
Servo servo_right;
Servo brushless_front;
Servo brushless_back;

// declare variables


void steering_setup(){       
  //servo_left.attach(SERVO_LEFT_PIN);
  //servo_right.attach(SERVO_RIGHT_PIN);
  brushless_front.attach(BRUSHLESS_FRONT_PIN);
  brushless_back.attach(BRUSHLESS_BACK_PIN);
}

bool check_button_status(char setup_or_mount) {
  if (get_button(setup_or_mount) == HIGH) {
    return true;
  }
  else {
    return false;
  }
}

void orient_speed() {
  brushless_front.detach();
  brushless_back.detach();
  int pot_input = get_potentiometer();  
  int brushless_speed = map(pot_input,0,1023,1000, 1100);
//  Serial.print("The Output Brushless Motor Speed is ");
//  Serial.println(brushless_speed);
  brushless_front.attach(BRUSHLESS_FRONT_PIN);
  brushless_back.attach(BRUSHLESS_BACK_PIN);
  brushless_front.writeMicroseconds(brushless_speed);
  brushless_back.writeMicroseconds(brushless_speed);
  delay(200);
  brushless_front.detach();
  brushless_back.detach();
}


void orient_steering(){
  servo_left.detach();
  servo_right.detach();
  int joystick_input = get_joystick();
  int servo_angle = map(joystick_input, 0,1023,1000,2000);  // the joystick takes analog values from 0 to 1023. writeMicroseconds command takes 1000 to 2000 as input. use the map function to assign our joystick input to our new range
  servo_left.attach(SERVO_LEFT_PIN);
  servo_right.attach(SERVO_RIGHT_PIN);
  servo_right.writeMicroseconds(servo_angle);
  servo_left.writeMicroseconds(servo_angle);
  delay(200);
  servo_left.detach();
  servo_right.detach();  
}

void region_four() {
  vertical_propeller();
  orient_steering();
  orient_speed();
}

void region_three() {
  set_min_speed_propeller();
  orient_steering();
}

void region_two() {
  straighten_setup(); 
}

void region_one() {
  floor_to_wall();            // function that will make the robot transition from floor to wall
                              // after floor_to_wall function runs, robot should be on wall so change boolean logic to true
}
