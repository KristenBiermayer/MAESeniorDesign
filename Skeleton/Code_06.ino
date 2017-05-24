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

// declare variables that set up ranges for robot's distance to the wall
const float FAR_WALL_THRESHOLD = 2;
const float NEAR_WALL_THRESHOLD = 0;

// declare variables that set up ranges for robot's distance to floor
const float FAR_FLOOR_THRESHOLD = 2;
const float NEAR_FLOOR_THRESHOLD = 0;

bool is_on_wall = false;
bool enable_wall_mount = false; 
bool region_3_stopcheck = false;     // checks if the robot has stopped completely in region 3. robot will only launch into wall mount mode if button is pressed AND robot has stopped completely in region 3

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
    enable_wall_mount = enable_wall_mount_xbee();
    // if button is not pressed
    if (!enable_wall_mount) {
      region_four();                             
    }
    // if button is pressed
    else {
      if (region_3_stopcheck == true) {
        // REGION 4
        if (distance_to_obstacle > FAR_WALL_THRESHOLD) {
          region_four;
          region_3_stopcheck == false;
        }
        // REGION 3
        else if (distance_to_obstacle > NEAR_WALL_THRESHOLD) {
          region_three();
        }
        // REGION 2
        else if (distance_to_obstacle <= NEAR_WALL_THRESHOLD) {
          region_two();        // for this part, we plan to just gently run the robot into the wall until it straightens
          // REGION 1
          if (distance_to_obstacle <= WALL_MOUNT_DISTANCE + ULTRASONIC_DISTANCE_ERROR &&
              distance_to_obstacle >= WALL_MOUNT_DISTANCE - ULTRASONIC_DISTANCE_ERROR) {
            region_one();
          }
        }
      }
      // region_3_stopcheck is false
      else {
        if (distance_to_obstacle > NEAR_WALL_THRESHOLD && distance_to_obstacle <= FAR_WALL_THRESHOLD && is_speed_zero()){
          region_3_stopcheck = true;
        }
        else {
          region_four();
        }  
      }
    }
  }

  // when robot is on the wall
  // this part will be undergoing some revisions soon! so don't take this part too seriously       
  else {
    distance_to_obstacle = get_distance(ULTRASONIC_TRIG_BACK, ULTRASONIC_ECHO_BACK);
    // when robot is far away from floor, robot movement will be controlled by joysticks/propellers
    if (distance_to_obstacle > FAR_FLOOR_THRESHOLD) {
      on_wall();
    }

    // when robot is transitioning from wall to floor
    else if (distance_to_obstacle <= FAR_FLOOR_THRESHOLD && distance_to_obstacle >= NEAR_FLOOR_THRESHOLD) {
      wall_to_floor();
      is_on_wall = false;
      previous_distance = get_min_distance(ULTRASONIC_TRIG_FRONT_LEFT, ULTRASONIC_ECHO_FRONT_LEFT, ULTRASONIC_TRIG_FRONT_RIGHT, ULTRASONIC_ECHO_FRONT_RIGHT);
    }
  }
  delay(ULTRASONIC_DELAY_MS);
}
