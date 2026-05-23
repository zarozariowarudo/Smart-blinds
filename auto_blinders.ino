const byte pin_motor_step = 10; // step pin
const byte pin_motor_dir = 8; // dir pin
const byte close_level = 100; // value to close blinds
const byte open_level = 200; // value to open blinds
const short start = 0;
const short dest = 7780; // Steps needed to fully close blind
const short speed = 1600; // You shouldn't change it but you can change and test new speed(+speed=slow down, -speed=faster)

class Stepper
{
  private:
    byte pin_motor_step;
    byte pin_motor_dir;

    short dest;
    short start;
    short speed;

    bool dir = false;
    bool running = false;
    int steps = 0;
    bool closed = false;

  public:
    void init(int npin_motor_step, int npin_motor_dir)
    {
      pin_motor_step  = npin_motor_step;
      pin_motor_dir   = npin_motor_dir;

      pinMode(pin_motor_step, OUTPUT);
      pinMode(pin_motor_dir, OUTPUT);
    }
    void process()
    {
      digitalWrite(pin_motor_dir, dir ? HIGH : LOW);

      if (running)
      {
        if (dir)  steps++;
        else      steps--;

        digitalWrite(pin_motor_step, HIGH);
        //delayMicroseconds(speed); // Optionaly     
        digitalWrite(pin_motor_step, LOW);
        delayMicroseconds(speed); 

        if (steps >= dest)  { running = false; closed = true; }
        if (steps <= start) { running = false; closed = false; }
      }

    }
    void close()
    {
      if (isClosed()) return;
      running = true;
      dir = true;
      closed = true;
    }
    void open()
    {
      if (!isClosed()) return;
      running = true;
      dir = false;
      closed = false;
    }
    bool isClosed()
    {
      return closed;
    }
    void setPath(int nstart, int ndest, int nspeed)
    {
      start = nstart;
      dest  = ndest;
      speed = nspeed;
    }
};

Stepper myStepper;

void setup() {
  // Setting up our stepper
  myStepper.init(pin_motor_step, pin_motor_dir);
  myStepper.setPath(start, dest, speed);
}

void loop() {
  // Reading our LDR and converting it value from 1023 0 to 255 0
  int light = analogRead(A1);
  light = map(light, 0, 1023, 0, 255);
  light = constrain(light, 0, 255);

  if (light < close_level && !myStepper.isClosed()) { // If light level is lover than close_level and not closed then close
    myStepper.close();
  } else if (light > open_level && myStepper.isClosed()) { //
    myStepper.open();
  }

  myStepper.process(); // running our procedure 
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