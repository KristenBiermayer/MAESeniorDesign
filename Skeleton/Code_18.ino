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
extern const float ULTRASONIC_DISTANCE_ERROR_CLOSE;
extern const float ULTRASONIC_DIFFERENCE_ERROR;
extern const float WALL_MOUNT_DISTANCE;
extern float previous_distance;
extern int angle_propeller_back;
extern int angle_propeller_front;

extern const float FLOOR_MOUNT_DISTANCE;

// declare variables that set up ranges for robot's distance to the wall
const float FAR_WALL_THRESHOLD = 43;

// declare variables that  up ranges for robot's distance to floor
const float FAR_FLOOR_THRESHOLD = 3;
const float NEAR_FLOOR_THRESHOLD = 0;
int increment_tries = 0;

bool is_on_wall = false;
bool enable_wall_mount_setup = false; 
boolean old_enable_wall_mount_setup = false;
bool enable_wall_mount = false;
bool far_enough_from_floor = false;
bool undergoing_wall_mount = false;

void setup() {
  ultrasonic_setup();
  xbee.begin(9600);
  Serial.begin(9600);
}

void loop() {
  float distance_to_obstacle;
  
  // this is the MAIN IF/ELSE LOOP that considers two conditions - on the floor or not on the floor
  // when robot is on the floor condition is satisfied, this part of the loop will run
  // IF ROBOT IS NOT ON WALL
  if (!is_on_wall) {
    old_enable_wall_mount_setup = enable_wall_mount_setup;
    enable_wall_mount_setup = check_button_status('s');
    
    // this is the SECOND MAIN IF/ELSE LOOP that considers two conditions - whether the button is on or off (on - robot can begin mounting wall process / off - robot should not begin mounting wall process)
    // IF BUTTON IS NOT PRESSED
    if (!enable_wall_mount_setup) {     // if button is not on the wall and button has not been pressed, everywhere is region four
//      Serial.println("Hello");
      region_four();                    // region_four() = orient_steering + orient_speed + vertical_propeller  
      lcd_message ("LB4");              // region four is defined by - user control of steering and speed          
    } 
    // IF BUTTON IS PRESSED
    else {
//      if (old_enable_wall_mount_setup == false) {
//          Serial.println("Button pressed");
//          previous_distance = get_raw_min_distance(ULTRASONIC_TRIG_FRONT_LEFT, ULTRASONIC_ECHO_FRONT_LEFT, ULTRASONIC_TRIG_FRONT_RIGHT, ULTRASONIC_ECHO_FRONT_RIGHT);             
//      }
      distance_to_obstacle = get_raw_min_distance(ULTRASONIC_TRIG_FRONT_LEFT, ULTRASONIC_ECHO_FRONT_LEFT, ULTRASONIC_TRIG_FRONT_RIGHT, ULTRASONIC_ECHO_FRONT_RIGHT);
      // if value returned by ultrasonic sensor is invalid, then repeat
//      if (distance_to_obstacle == -1.0) {
//        return;
//      }   
      
      if (distance_to_obstacle > FAR_WALL_THRESHOLD) {        // if button is pressed, we allow only steering inputs from user when robot is beyond is region 2                               
        lcd_message("LB3");  
        //Serial.println("Button pressed, region 3");  
        region_three();                                       // set_min_speed_propeller and orient_steering
      }
      
      else if (distance_to_obstacle <= FAR_WALL_THRESHOLD ) {
        enable_wall_mount = check_button_status('m');
        if (enable_wall_mount == false) {
          region_two();       // set_min_speed_propeller 
          lcd_message("LB2");
          if (get_diff_distance(ULTRASONIC_TRIG_FRONT_LEFT, ULTRASONIC_ECHO_FRONT_LEFT, ULTRASONIC_TRIG_FRONT_RIGHT,ULTRASONIC_ECHO_FRONT_RIGHT) <= 1.5 &&
            distance_to_obstacle <= WALL_MOUNT_DISTANCE + ULTRASONIC_DISTANCE_ERROR_CLOSE && distance_to_obstacle >= WALL_MOUNT_DISTANCE - ULTRASONIC_DISTANCE_ERROR_CLOSE) {
            lcd_message("LMQ"); 
          }
        }
        else if (enable_wall_mount == true) {
          if (get_diff_distance(ULTRASONIC_TRIG_FRONT_LEFT, ULTRASONIC_ECHO_FRONT_LEFT, ULTRASONIC_TRIG_FRONT_RIGHT,ULTRASONIC_ECHO_FRONT_RIGHT) <= 1.5 &&
            distance_to_obstacle <= WALL_MOUNT_DISTANCE + ULTRASONIC_DISTANCE_ERROR_CLOSE && distance_to_obstacle >= WALL_MOUNT_DISTANCE - ULTRASONIC_DISTANCE_ERROR_CLOSE || undergoing_wall_mount == true){
            undergoing_wall_mount = true;
            lcd_message ("LMS");
            region_one(increment_tries);
            increment_tries += 1;
//            Serial.print("number of tries ");
//            Serial.println(increment_tries);
            if (increment_tries == 2) {
              is_on_wall = true; 
            }  
          }
          else {
            region_two();       // set_min_speed_propeller 
            lcd_message("LB2");
          }
        }
      }
    }
  }

  // when robot is on the wall      
  else if (is_on_wall == true) { 
    if (far_enough_from_floor == false) {
      roll_straight();
      move_up_wall();
      move_up_wall_speed();
      lcd_message("LWU");
      int distance_to_obstacle_front = get_raw_min_distance(ULTRASONIC_TRIG_FRONT_LEFT, ULTRASONIC_ECHO_FRONT_LEFT, ULTRASONIC_TRIG_FRONT_RIGHT, ULTRASONIC_ECHO_FRONT_RIGHT);
    }
    else if (far_enough_from_floor == true) {
      roll_straight();
      move_up_wall();
      move_down_wall_speed();
      lcd_message ("LWD");
    }
    
//    distance_to_obstacle = get_distance(ULTRASONIC_TRIG_BACK, ULTRASONIC_ECHO_BACK);
//    if (far_enough_from_floor == false){
//      lcd_message("LWU");
//      move_up_wall_speed();
//      roll_straight();
//      move_up_wall();
//      if (distance_to_obstacle < 2) {
//        far_enough_from_floor = true;
//      }
//    }
//    else {
//      move_down_wall_speed();
//      roll_straight();
//      move_down_wall();
//      if (distance_to_obstacle >= FLOOR_MOUNT_DISTANCE - ULTRASONIC_DISTANCE_ERROR_CLOSE && distance_to_obstacle <= FLOOR_MOUNT_DISTANCE + ULTRASONIC_DISTANCE_ERROR_CLOSE){
//        wall_to_floor();
//        is_on_wall = false;
//        far_enough_from_floor = false;
//      }
//    }
  }
}
