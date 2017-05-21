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
SoftwareSerial xbee(2,3);   // xbee communication is setup to be between 2 and 3. 2 and 3 are assigned to RX and TX   

void steering_setup(){
  Serial.begin(9600);
  xbee.begin(9600);        
  servo_left.attach(SERVO_LEFT_PIN);
  servo_right.attach(SERVO_RIGHT_PIN);
  brushless_front.attach(BRUSHLESS_FRONT_PIN);
  brushless_back.attach(BRUSHLESS_BACK_PIN);
}

float orient_steering_speed(){
  // if XBee is sending information, read from it
  // important background for this code : on the source side -> when we read a joystick value, we will store it as a string and add a 'j' in front to differentiate it from the potentiometer reading
  // example : joystick reads integer 526. we send string j526 to the receiver side 
  if (xbee.available()){
    String raw_input = xbee.readStringUntil('\n');      // store values sent from XBee until it hits a new line character
    char id_char = raw_input.charAt(0);                 // look at the first character in the string
    if (id_char == 'j') {                               // if the first character is a j, we know that it came from the joystick 
      int joystick_input = raw_input.substring(1).toInt();      // example: j526 -> raw_input.substring(1).toInt() converts string to int and discards j. end result is -> joystick_input = 526 
      Serial.print("The Joystick Input is");
      Serial.println(joystick_input);
      int servo_angle = map(joystick_input, 0,1023,1000,2000);  // the joystick takes analog values from 0 to 1023. writeMicroseconds command takes 1000 to 2000 as input. use the map function to assign our joystick input to our new range
      Serial.print("The Joystick Input is");
      Serial.println(servo_angle);
      servo_left.writeMicroseconds(servo_angle);       
      delay(100);
      servo_right.writeMicroseconds(servo_angle);    
      delay(100);
      
    } else if (id_char == 'p'){                            // if the first character is a p, we know that the value is from the potentiometer
      int pot_input = raw_input.substring(1).toInt();     
      Serial.print("The Potentiometer Input is ");
      Serial.println(pot_input);
      int brushless_speed = map(pot_input,0,1023,1000, 2000);
      Serial.print("The Output Brushless Motor Speed is ");
      Serial.println(brushless_speed);
      brushless_front.writeMicroseconds(brushless_speed);
      brushless_back.writeMicroseconds(brushless_speed);
      delay(100);
    }
  }
}


float orient_steering(){
    if (xbee.available()){
    String raw_input = xbee.readStringUntil('\n');      // store values sent from XBee until it hits a new line character
    char id_char = raw_input.charAt(0);                 // look at the first character in the string
    if (id_char == 'j') {                               // if the first character is a j, we know that it came from the joystick 
      int joystick_input = raw_input.substring(1).toInt();      // example: j526 -> raw_input.substring(1).toInt() converts string to int and discards j. end result is -> joystick_input = 526 
      Serial.print("The Joystick Input is");
      Serial.println(joystick_input);
      int servo_angle = map(joystick_input, 0,1023,1000,2000);  // the joystick takes analog values from 0 to 1023. writeMicroseconds command takes 1000 to 2000 as input. use the map function to assign our joystick input to our new range
      Serial.print("The Joystick Input is");
      Serial.println(servo_angle);
      servo_left.writeMicroseconds(servo_angle);          
      delay(100);
      servo_right.writeMicroseconds(servo_angle);    
      delay(100); 
    }
  }
}

