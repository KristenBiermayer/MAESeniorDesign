// declare global variables

// ADD A CONDITION WHERE THE DISTANCE TO THE WALL MUST BE LESS THAN THE THRESHOLD AT LEAST TWICE FOR SAFETY!

// include libraries
#include <Servo.h>
#include <SoftwareSerial.h>

SoftwareSerial xbee(2,3);
// define pin numbers and variables for ultrasonic sensor
extern const int ULTRASONIC_TRIG_FRONT_LEFT;
extern const int ULTRASONIC_ECHO_FRONT_LEFT;
extern const int ULTRASONIC_TRIG_FRONT_RIGHT;
extern const int ULTRASONIC_ECHO_FRONT_RIGHT;
extern const int ULTRASONIC_TRIG_BACK;
extern const int ULTRASONIC_ECHO_BACK;
extern const float ULTRASONIC_DELAY_MS;
extern const float ULTRASONIC_DISTANCE_ERROR;
extern const float ULTRASONIC_DIFFERENCE_ERROR;
extern const float WALL_MOUNT_DISTANCE;
extern float previous_distance;


//define pin numbers and variables for XBee / Steering
extern const int SERVO_LEFT_PIN;
extern const int SERVO_RIGHT_PIN;

// define pin numbers and variables for the propellers
extern const int PROPELLER_SPEED_FRONT_PIN;   //pins for the brushless motor in the front
extern const int PROPELLER_SPEED_BACK_PIN;    // pins for the brushless motor in the back
extern const int PROPELLER_PITCH_FRONT_PIN;
extern const int PROPELLER_PITCH_BACK_PIN;
extern const int PROPELLER_ROLL_FRONT_PIN;
extern const int PROPELLER_ROLL_BACK_PIN;

extern const int VERTICAL_PITCH_FRONT;   // note that these are dummy variables
extern const int VERTICAL_PITCH_BACK;    // vertical = 90 / perpendicular to grond
extern const int HORIZONTAL_PITCH_FRONT;
extern const int HORIZONTAL_PITCH_BACK;

extern const float FLOOR_MOUNT_DISTANCE;

// declare variables that set up ranges for robot's distance to the wall
const float FAR_WALL_THRESHOLD = 3;
const float NEAR_WALL_THRESHOLD = 0;

// declare variables that set up ranges for robot's distance to floor
const float FAR_FLOOR_THRESHOLD = 3;
const float NEAR_FLOOR_THRESHOLD = 0;

bool is_on_wall = false;
bool enable_wall_mount_setup = false; 
bool enable_wall_mount = false;
bool far_enough_from_floor = false;

// declare functions (to be removed later once file is added)

void setup() {
  ultrasonic_setup();
  steering_setup();
  Serial.begin(9600);
}

void loop() {
  float distance_to_obstacle;
  distance_to_obstacle = get_min_distance(ULTRASONIC_TRIG_FRONT_LEFT, ULTRASONIC_ECHO_FRONT_LEFT, ULTRASONIC_TRIG_FRONT_RIGHT, ULTRASONIC_ECHO_FRONT_RIGHT);
  // if value returned by ultrasonic sensor is invalid, then repeat
  if (distance_to_obstacle == -1.0) {
    return;
  }
  
  // this is the MAIN IF/ELSE LOOP that considers two conditions - on the floor or not on the floor
  // when robot is on the floor condition is satisfied, this part of the loop will run
  // IF ROBOT IS NOT ON WALL
  if (!is_on_wall) {
    enable_wall_mount_setup = check_button_status('s');
    // this is the SECOND MAIN IF/ELSE LOOP that considers two conditions - whether the button is on or off (on - robot can begin mounting wall process / off - robot should not begin mounting wall process)
    // IF BUTTON IS NOT PRESSED
    if (!enable_wall_mount_setup) {     // if button is not on the wall and button has not been pressed, everywhere is region four
      lcd_message ("LB4");              // region four is defined by - user control of steering and speed
      region_four();                    // region_four() = orient_steering + orient_speed + vertical_propeller               
    } 
    // IF BUTTON IS PRESSED
    else {     
      if (distance_to_obstacle > FAR_WALL_THRESHOLD) {        // if button is pressed, we allow only steering inputs from user when robot is beyond is region 2
        lcd_message("LB3");                                   
        region_three();                                       // set_min_speed_propeller and orient_steering
      }
      else if (distance_to_obstacle <= FAR_WALL_THRESHOLD) {
        lcd_message("LB2");
        region_two();       // set_min_speed_propeller 
        if (get_diff_distance(ULTRASONIC_TRIG_FRONT_LEFT, ULTRASONIC_ECHO_FRONT_LEFT, ULTRASONIC_TRIG_FRONT_RIGHT,ULTRASONIC_ECHO_FRONT_RIGHT) <= ULTRASONIC_DIFFERENCE_ERROR &&
            distance_to_obstacle <= WALL_MOUNT_DISTANCE + ULTRASONIC_DISTANCE_ERROR && distance_to_obstacle >= WALL_MOUNT_DISTANCE - ULTRASONIC_DISTANCE_ERROR) {
            lcd_message ("LMQ");
          enable_wall_mount = check_button_status('m'); 
          if (enable_wall_mount == true) {
            lcd_message ("LMS");
            region_one();
            is_on_wall = true; 
          }
          else {
            return;
          }
        }
      }
    }
  }

  // when robot is on the wall      
  else {
    distance_to_obstacle = get_distance(ULTRASONIC_TRIG_BACK, ULTRASONIC_ECHO_BACK);
    // when robot is far away from floor, robot movement will be controlled by joysticks/propellers
    if (far_enough_from_floor == false){
      if (distance_to_obstacle < FAR_FLOOR_THRESHOLD) {
        move_up_wall();
      }
      else if (distance_to_obstacle >= FAR_FLOOR_THRESHOLD) {
        far_enough_from_floor = true;
      }
    }
    else {
      if (distance_to_obstacle > FAR_FLOOR_THRESHOLD) {
        move_down_wall();
      }
      else if (distance_to_obstacle <= FAR_FLOOR_THRESHOLD && distance_to_obstacle > NEAR_WALL_THRESHOLD) {
        move_down_wall();
      }
      else if (distance_to_obstacle < NEAR_WALL_THRESHOLD) {
        move_down_wall();
        if (distance_to_obstacle >= FLOOR_MOUNT_DISTANCE - ULTRASONIC_DISTANCE_ERROR && distance_to_obstacle <= FLOOR_MOUNT_DISTANCE + ULTRASONIC_DISTANCE_ERROR){
          wall_to_floor();
          is_on_wall = false;
          far_enough_from_floor = false;
        }
      }
    }
  }
  delay(ULTRASONIC_DELAY_MS);
}
