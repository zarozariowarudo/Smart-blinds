const short dest = 7780; // Steps needed to fully close blind
const short speed = 1600; // You shouldn't change it but you can change and test new speed(+speed=slow down, -speed=faster)

const byte pin_motor_step = 10; // step pin
const byte pin_motor_dir = 8; // dir pin
const byte close_level = 100; // value to close blinds
const byte open_level = 200; // value to open blinds

bool direction = false;
bool closed = false;
bool motor_running = false;
short light = 0;
int steps = 0;

void setup() {
  // Setting up our stepper
  pinMode(pin_motor_step, OUTPUT);
  pinMode(pin_motor_dir, OUTPUT);
  
}


void motor_run_step() {
  digitalWrite(pin_motor_dir, direction ? HIGH : LOW); // Setting direction

  if (motor_running) { // If it needs to close / open 
    // Rotating our stepper
    digitalWrite(pin_motor_step, HIGH);
    //delayMicroseconds(speed); // Optionaly     
    digitalWrite(pin_motor_step, LOW);
    delayMicroseconds(speed);  

    if (direction) steps++; // choosing to add or minus steps depended on our direction
    else steps--;

    if (steps >= dest && closed) { // if it closed so stop
      motor_running = false;
      digitalWrite(pin_motor_step, LOW);
    }
    else if (steps <= 0 && !closed) { // if it opened so stop
      motor_running = false;
      digitalWrite(pin_motor_step, LOW);
    }

  }
}



void loop() {
  // Reading our LDR and converting it value from 1023 0 to 255 0
  light = analogRead(A1);
  light = map(light, 0, 1023, 0, 255);
  light = constrain(light, 0, 255);

  if (light < close_level && !closed) { // If light level is lover than close_level and not closed then close
    direction = true;
    motor_running = true;
    closed = true;
  } else if (light > open_level && closed) { //
    direction = false;
    motor_running = true; 
    closed = false;
  }

  motor_run_step(); // running our procedure 

} 

// program to test how much that "steps" you need to close blind. I used AI because I lost my own programm

/*
  MANUAL CALIBRATION PROGRAM
  Hold the button to spin the motor. 
  Check Serial Monitor for the "steps" value.
*/
/*
const byte pin_motor_step = 10;
const byte pin_motor_dir = 8;
const byte pin_button = 2; // Connect your button here (Pin 2 to Button to GND)

const short speed = 1600; 
int steps = 0;
bool direction = true; // Change to false if motor spins wrong way

void setup() {
  Serial.begin(9600);
  
  pinMode(pin_motor_step, OUTPUT);
  pinMode(pin_motor_dir, OUTPUT);
  
  // Using INPUT_PULLUP: button connects pin to GND when pressed
  pinMode(pin_button, INPUT_PULLUP); 
  
  Serial.println("Calibration started...");
  Serial.println("Hold the button to move the motor. Read 'steps' value below:");
}

void loop() {
  // If button is pressed (LOW because of INPUT_PULLUP)
  if (digitalRead(pin_button) == LOW) {
    
    digitalWrite(pin_motor_dir, direction ? HIGH : LOW);
    
    // Making one step
    digitalWrite(pin_motor_step, HIGH);
    // delayMicroseconds(speed); // Optional
    digitalWrite(pin_motor_step, LOW);
    delayMicroseconds(speed);
    
    steps++; // Counting steps
    
    // Printing current step to Serial Monitor
    Serial.print("Current steps: ");
    Serial.println(steps);
  }
}*/