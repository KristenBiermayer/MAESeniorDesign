// XBee and Steering Control
// Functions defined include:
// void steering_setup()      (setup the servomotors to the correct pins)
// float orient_steering()    (use the joystick commands sent from the XBee to orient direction of servomotors)
//                            (parallel steering - servomotors turn to result in same output angle)

// declare pins
const int SERVO_LEFT_PIN = 142424;
const int SERVO_RIGHT_PIN = 123124;
const int BRUSHLESS_FRONT_PIN = 12;
const int BRUSHLESS_BACK_PIN = 13;

// declare classes
Servo servo_left;
Servo servo_right;
Servo brushless_front;
Servo brushless_back;


void steering_setup(){
  Serial.begin(9600);
  xbee.begin(9600);        
  servo_left.attach(SERVO_LEFT_PIN);
  servo_right.attach(SERVO_RIGHT_PIN);
  brushless_front.attach(BRUSHLESS_FRONT_PIN);
  brushless_back.attach(BRUSHLESS_BACK_PIN);
}

bool enable_wall_mount_xbee() {
  get_button();
  if (xbee.available()) {
    String raw_input = xbee.readStringUntil('\n');
    char id_char = raw_input.charAt(0);
    if (id_char == 'b'){
      int button_input = raw_input.substring(1).toInt();
      if (button_input == 1) {
        return true;
      }
      if (button_input == 0) {
        return false;
      }
    }
  }
}

void orient_speed() {
  int pot_input = get_potentiometer();  
  int brushless_speed = map(pot_input,0,1023,1000, 2000);
  Serial.print("The Output Brushless Motor Speed is ");
  Serial.println(brushless_speed);
  brushless_front.writeMicroseconds(brushless_speed);
  brushless_back.writeMicroseconds(brushless_speed);
  delay(100);
}


void orient_steering(){
  int joystick_input = get_joystick();
  int servo_angle = map(joystick_input, 0,1023,1000,2000);  // the joystick takes analog values from 0 to 1023. writeMicroseconds command takes 1000 to 2000 as input. use the map function to assign our joystick input to our new range
  Serial.print("The Joystick Input is");
  Serial.println(servo_angle);
  servo_left.writeMicroseconds(servo_angle);          
  delay(100);
  servo_right.writeMicroseconds(servo_angle);    
  delay(100); 
}

void region_four() {
  on_floor();
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
  is_on_wall = true;          // after floor_to_wall function runs, robot should be on wall so change boolean logic to true
  region_3_stopcheck = false;
}


