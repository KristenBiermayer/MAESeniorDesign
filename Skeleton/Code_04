// declare global variables

// ADD A CONDITION WHERE THE DISTANCE TO THE WALL MUST BE LESS THAN THE THRESHOLD AT LEAST TWICE FOR SAFETY!

// include libraries
#include <Servo.h>
#include <SoftwareSerial.h>

// define pin numbers and variables for ultrasonic sensor
extern const int ULTRASONIC_FRONT_TRIG_L;
extern const int ULTRASONIC_FRONT_ECHO_L;
extern const int ULTRASONIC_FRONT_TRIG_R;
extern const int ULTRASONIC_FRONT_ECHO_R;
extern const int ULTRASONIC_BACK_TRIG;
extern const int ULTRASONIC_BACK_ECHO;
extern const float ULTRASONIC_DELAY_MS;
extern float previous_distance;

//define pin numbers and variables for XBee / Steering
extern const int SERVO_LEFT_PIN;
extern const int SERVO_RIGHT_PIN;

// define pin numbers and variables for the propellers
extern const int PROPELLER_FRONT_PIN;   //pins for the brushless motor in the front
extern const int PROPELLER_BACK_PIN;    // pins for the brushless motor in the back
extern const int PROPELLER_PITCH_FRONT_PIN;
extern const int PROPELLER_PITCH_BACK_PIN;
extern const int PROPELLER_ROLL_FRONT_PIN;
extern const int PROPELLER_ROLL_BACK_PIN;

extern const int FRONT_VERTICAL_PITCH = 1500;   // note that these are dummy variables
extern const int BACK_VERTICAL_PITCH = 1500;    // vertical = 90 / perpendicular to grond
extern const int FRONT_HORIZONTAL_PITCH = 1000;
extern const int BACK_HORIZONTAL_PITCH = 1000;

// declare variables that set up ranges for robot's distance to the wall
const float FAR_WALL_THRESHOLD = 2;
const float NEAR_WALL_THRESHOLD = 0;

// declare variables that set up ranges for robot's distance to floor
const float FAR_FLOOR_THRESHOLD = 2;
const float NEAR_FLOOR_THRESHOLD = 0;

bool is_on_wall = false;

// declare functions (to be removed later once file is added)

void on_floor() {
  // controlling orientation of propellers when robot is on the floor
}

void straighten_setup() {
  // orient the robot in a parallel position to the wall 
}

void floor_to_wall() {
  // controlling orientation of propellers when robot is transitioning from floor to wall
  // make sure the robot is a certain height above the threshold before allowing the ultrasonic sensor in the back to operate! 
}

void on_wall() {
  // controlling orientation of propellers when robot is on wall
}

void wall_to_floor() {
  // controlling orientation of propellers when robot is getting off wall
}

void setup() {
  ultrasonic_setup();
  steering_setup();
  Serial.begin(9600);
}

void loop() {
  float distance_to_obstacle;

  if (!is_on_wall) {
    distance_to_obstacle = get_distance(ULTRASONIC_FRONT_TRIG, ULTRASONIC_FRONT_ECHO);
  }
  else {
    distance_to_obstacle = get_distance(ULTRASONIC_BACK_TRIG, ULTRASONIC_BACK_ECHO);
  }

  // if value returned by ultrasonic sensor
  if (distance_to_obstacle == -1.0) {
    return;
  }

  // this is the MAIN IF/ELSE LOOP that considers two conditions - on the floor or not on the floor
  // when robot is on the floor condition is satisfied, this part of the loop will run
  if (!is_on_wall) {
    // if the robot is on the wall, there are three possibilities that the code will run through 
    // these possibilities can be thought of as "regions" 
    // regions will be defined by distance from the wall 

    // FAR TO WALL REGION
    // this region is defined as being far away from the wall! far away from any possibility of running into the wall
    // in this region, it is safe to steer the robot in any manner you please without worry of destruction (cue epic thunder)
    if (distance_to_obstacle > FAR_WALL_THRESHOLD) {
      on_floor();         // function to orient the propellers for flat ground movement. 
      orient_steering_speed();  // function will allow for XBee communication of joystick and potentiometer orientations.
                                // joystick = steering / potentiometer = speed of brushless motors on propellers
    }

    // MIDDLE REGION
    // this region is defined as getting close to the wall / not safe for user to have full control of robot 
    // we will take away user's ability to control the speed of the robot
    // only steering can be user controlled in this region
    else if (distance_to_obstacle <= FAR_WALL_THRESHOLD && distance_to_obstacle >= NEAR_WALL_THRESHOLD) {
      set_min_speed_propeller();  // set the minimum speed of propeller
      orient_steering();          // only allow input from joystick
    }

    // CLOSE TO WALL REGION
    else if (distance_to_obstacle <= NEAR_WALL_THRESHOLD){
      straighten_setup();         // for this part, we plan to just gently run the robot into the wall until it straightens
                                  // the vertigo robot does this to straighten itself as well!
      floor_to_wall();            // function that will make the robot transition from floor to wall
      is_on_wall = true;          // after floor_to_wall function runs, robot should be on wall

    }
  }

  // when robot is on the wall
  // this part will be undergoing some revisions soon! so don't take this part too seriously
  else {
    // when robot is far away from floor, robot movement will be controlled by joysticks/propellers
    if (distance_to_obstacle > FAR_FLOOR_THRESHOLD) {
      on_wall();
    }

    // when robot is transitioning from wall to floor
    else if (distance_to_obstacle <= FAR_FLOOR_THRESHOLD && distance_to_obstacle >= NEAR_FLOOR_THRESHOLD) {
      wall_to_floor();
      is_on_wall = false;
      previous_distance = get_raw_distance(ULTRASONIC_FRONT_TRIG, ULTRASONIC_FRONT_ECHO);
    }
  }
  delay(ULTRASONIC_DELAY_MS);
}
