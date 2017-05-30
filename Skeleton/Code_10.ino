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
const float FAR_WALL_THRESHOLD = 2;
const float NEAR_WALL_THRESHOLD = 0;

// declare variables that set up ranges for robot's distance to floor
const float FAR_FLOOR_THRESHOLD = 2;
const float NEAR_FLOOR_THRESHOLD = 0;

bool is_on_wall = false;
bool enable_wall_mount_setup = false; 
bool region_3_stopcheck = false;     // checks if the robot has stopped completely in region 3. robot will only launch into wall mount mode if button is pressed AND robot has stopped completely in region 3
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
  if (!is_on_wall) {
    enable_wall_mount_setup = check_button_status('s');
    // this is the SECOND MAIN IF/ELSE LOOP that considers two conditions - whether the button is on or off (on - robot can begin mounting wall process / off - robot should not begin mounting wall process)
    // if button is not pressed
    if (!enable_wall_mount_setup) {     // if button is not on the wall and button has not been pressed, everywhere is region four
      lcd_message ("L4NN");             // sends the signal to the xbee that will change the message on lcd screen to "Region 4 BNP R3N" (Button Not Pressed, Region 3 No)
      region_four();                             
    } 
    // if button is pressed
    else {
      // this is the THIRD MAIN IF/ELSE LOOP that considers two conditions - whether or not the region 3 stopcheck has been passed
      // if region_3_stopcheck = false, then robot has never completely stopped in region 3
      if (region_3_stopcheck == false) { // if button has been pressed but we have not satisfied region 3 stopcheck in the past iterations of code
        
        // look for region 3 
        if (distance_to_obstacle > NEAR_WALL_THRESHOLD && distance_to_obstacle <= FAR_WALL_THRESHOLD){    // want to check for whether region_3_stopcheck can be set to true -> check if robot is in region three and whether the speed is equal to zero
          if (is_speed_zero()) {                    // if speed is equal to zero, then we can set region_3_stopcheck = true;
            region_3_stopcheck = is_speed_zero();                       
            lcd_message ("L3YY");
          }
          else {                    // speed is not equal to zero so we will slow down the robot by stopping all inputs, etc.
            stop_robot ();          // stop taking user inputs, etc. to stop the robot
            lcd_message ("L3YN");
            
          }
        }
        else { // note: we are stil in the region_3_stopcheck == false if/loop of the code
          // region 4
          region_four();      // everything else that is not in region 3 will behave as region 4
          if (distance_to_obstacle > FAR_WALL_THRESHOLD) {
            lcd_message ("L4YN");   // led messages will signal user of their location so it will be easier to navigate to region 3
          }
          // region 2   (note: we don't have a section for region 1 since region 1 is a "sub-section of region 2)
          else if (distance_to_obstacle <= NEAR_WALL_THRESHOLD) {
            lcd_message ("L2YN");
          }
        }   
      }
      // if region_3_stopcheck = true, then the robot has completely stopped in region 3 at some point in the past 
      else {
        // REGION 4
        if (distance_to_obstacle >= FAR_WALL_THRESHOLD) {   // if user manuevers robot back to region 3, then we want to negate the region_3_stopcheck 
          lcd_message ("L4YY");
          region_four();
          region_3_stopcheck == false;
        }
        // REGION 3
        else if (distance_to_obstacle > NEAR_WALL_THRESHOLD) {  // if user has satisfied region 3 stopcheck and still is in region 3
          lcd_message ("L3YY");
          region_three();       // allows user to control steering and not speed. We will set the minimum speed of propellers 
        }
        // REGION 2
        else if (distance_to_obstacle <= NEAR_WALL_THRESHOLD) {   // if user has satisfied region 3 stopcheck in the past and is in region 2
          lcd_message ("L2YY");
          region_two();        // for this part, we plan to just gently run the robot into the wall until it straightens
          // REGION 1
          if (distance_to_obstacle <= WALL_MOUNT_DISTANCE + ULTRASONIC_DISTANCE_ERROR &&      // when the robot reads a certain distance from the wall (a safe distance from which it can start climbing)
              distance_to_obstacle >= WALL_MOUNT_DISTANCE - ULTRASONIC_DISTANCE_ERROR) {      // then robot can begin to mount the wall if a secondary button is pressed
            lcd_message("L1M");   
            enable_wall_mount = check_button_status('m');                                     // check if button has been pressed
            if (enable_wall_mount) {                                                         
              lcd_message ("L1W");
              region_one();      // transition from floor to wall
            }
            else {
              return;
            }
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
        // keep moving up 
      }
      else if (distance_to_obstacle >= FAR_FLOOR_THRESHOLD) {
        far_enough_from_floor = true;
      }
    }
    else {
      if (distance_to_obstacle > FAR_FLOOR_THRESHOLD) {
        // region 4 wall 
      }
      else if (distance_to_obstacle <= FAR_FLOOR_THRESHOLD && distance_to_obstacle > NEAR_WALL_THRESHOLD) {
        // region 3 wall
      }
      else if (distance_to_obstacle < NEAR_WALL_THRESHOLD) {
        // region 2 wall
        if (distance_to_obstacle >= FLOOR_MOUNT_DISTANCE - ULTRASONIC_DISTANCE_ERROR && distance_to_obstacle <= FLOOR_MOUNT_DISTANCE + ULTRASONIC_DISTANCE_ERROR){
          // region 1 wall
          is_on_wall = false;
          far_enough_from_floor = false;
          region_3_stopcheck = false;
        }
      }
    }
  }
  delay(ULTRASONIC_DELAY_MS);
}
