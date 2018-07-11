
// FINAL_MOWER_SKETCH
long usduration;                                  // Define ultra sonic duration
int usdistance;                                   // Define ultra sonic distance
int  maxusdistance = 8;                           // set max distance for utra sonic sensor
// Read sensors
const int trigPin = 12;                           // Define PIN 12 for trigger
const int echoPin = 13;                           // Define PIN 13 for echo 
int Sensepinleft = 4 ;                           // Define PIN A4 for left Sensor
int Sensepinright = 3 ;                          // Define PIN A3 for right Sensor
int Sensevalueleft;                              // Define variable for sensorvalue left
int Sensevalueright;                             // Define variable for sensorvalue right
int Sensemaxleft = 200;                            // Define variable for max sensorvalue left and set default
int Sensemaxright = 200;                           // Define variable for max sensorvalue right and set default

// Status LED
int Ledbat = 3;                                  // Define PIN 3 for digital output red LED
int Ledstart = 4;                                // Define PIN 4 for digital output green LED

// Read Battery voltage
int Voltpin = 0;                                 // Define PIN A0 for reading battery voltage
float Volt;                                     // Define variable for voltage
float Voltvalue;                                 // Define variable for avarage voltage calculation
float Voltlow = 10;                              // Define variable and setup for minimum operation voltage
int I;                                           // Define variable for IF-LOOP counter
int I_bat = 20;                                  // Define variable for battery status (low or high)

// Drive motors
int Driveleft = 9;                               // Define PIN 9 for left Motor PWM output               
int IN3 = 6;                                     // Define PIN 6 for left Motor IN3
int IN4 = 5;                                     // Define PIN 5 for left Motor IN4

int Driveright = 10;                             // Define PIN 10 for right Motor PWM output 
int IN1 = 8;                                     // Define PIN 8 for right Motor IN1
int IN2 = 7;                                     // Define PIN 7 for right Motor IN2

int Drivespeedleft = 150;                        // Define variable for left motor speed and set PWM value
int Drivespeedright = 150;                       // Define variable for right motor speed and set PWM value

int Turntime;                                    // Define variable for the time the mower has to turn
int I_Ramp;                                      // Define counter-variable for motor ramp

// Collision control
int Drivepinleft = 1;                            // Define PIN A1 for left motor current
int Drivepinright = 2;                           // Define PIN A2 for right motor current
int Drivesenseleft;                              // Define variable for left motor current
int Drivesenseright;                             // Define variable for right motor current

int Drivemaxleft = 250;                         // Define variable for max motor current left and set default
int Drivemaxright = 250;                        // Define variable for max motor current right and set default

// Cutter motor
int Cutterspeed = 0;                            // Define variable for Cutterspeed PWM value (must be "0" here!
int Cutter = 11;                                // Define PIN 11 for cutter motor PWM output 
int I_Start = 0;                                // Define variable for entering "Start()" function  

void setup() {                                   // Setup

pinMode(trigPin, OUTPUT);                       // Sets the trigPin as an Output
pinMode(echoPin, INPUT);                        // Sets the echoPin as an Input
Serial.begin(9600);                             // Starts the serial communication

  // Print
  Serial.begin(9600);                            // Start the serial communication

  // Status LEDs
  pinMode(Ledbat, OUTPUT);                       // Define Ledbat PIN as OUTPUT
  pinMode(Ledstart, OUTPUT);                     // Define Ledstart PIN as OUTPUT 

  // Drive motors
  pinMode(IN1, OUTPUT);                          // Define IN1 PIN as OUTPUT
  pinMode(IN2, OUTPUT);                          // Define IN2 PIN as OUTPUT
  pinMode(IN3, OUTPUT);                          // Define IN3 PIN as OUTPUT
  pinMode(IN4, OUTPUT);                          // Define IN4 PIN as OUTPUT
  
}
void loop() {
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);

// Reads the echoPin, returns the sound wave travel time in microseconds
usduration = pulseIn(echoPin, HIGH);

// Calculating the distance
usdistance= usduration*0.034/2;

// Prints the distance on the Serial Monitor
Serial.print("Distance: ");
Serial.println(usdistance);
// Read Voltpin for 10 times and calculate average voltvalue
Voltvalue = 0;
for (I = 0;I<10; I++){
 Volt = analogRead(Voltpin);
 Voltvalue = Voltvalue + Volt;
 delay(10);
}
  Voltvalue = Voltvalue / 10;
  Voltvalue = Voltvalue * 0.02765;               // 0.02765 is factor for voltagedevider 22K / 4,7K   
 
if (Voltvalue < Voltlow) {                      // Make dicission: Batteryvoltage O.K or low
  I_bat = 1;                                    // If battery voltage is low set status 1
  Stop();                                       // If battery is low go to “Stop()” function
  }
  

  if (I_Start < 10) {                           // Go to "Start()" function
  Start();
}


  //Collision control
  Drivesenseleft = analogRead(Drivepinleft);    // Read left motor current
  Drivesenseright = analogRead(Drivepinright);  // Read right motor current
  Serial.print("Motor current left = ");        // Print
  Serial.println(Drivesenseleft);
  Serial.print("Motor current right = ");
  Serial.println(Drivesenseright);
  if (Drivesenseleft > Drivemaxleft) {          // Compare left motor current 
    backward() ;
    Turnright();
  }
   if (Drivesenseright > Drivemaxright) {       // Compare right motor current 
    backward() ;
    Turnleft();
  }
  if (usdistance < maxusdistance){
    backward();
    Turnleft();
  }
// Read sensors
Sensevalueleft = analogRead(Sensepinleft);      // Read left sensor 
Sensevalueright = analogRead(Sensepinright);    // Read right sensor
Serial.print("Left sensor = ");
Serial.println(Sensevalueleft);
Serial.print("Right sensor = ");
Serial.println(Sensevalueright);
Serial.println(" ");
 

 if (Sensevalueleft > Sensemaxleft) {           // Compare left sensor
  analogWrite(Driveleft, 0);
  analogWrite(Driveright, 0);
  delay(250);
  backward();
  Turnright();
 }

 if (Sensevalueright > Sensemaxright) {         // Compare right sensor
  analogWrite(Driveleft, 0);
  analogWrite(Driveright, 0);
  delay(250);
  backward();
  Turnleft();
 }

 // Go forward
  forward();   
  delay(200);
 
}


/////////FUNCTIONS///////////

void Start() {

digitalWrite(Ledbat, LOW);                  // Switch red status-LED off
digitalWrite(Ledstart, HIGH);               // Switch green status-LED on
delay(500);

//Default Settings

Sensemaxleft = 200;                            // Set max sensorvalue left (should be >2)
Sensemaxright = 200;                           // Set max sensorvalue right (should be >2)

Drivemaxleft = 250;                         // Set max motor current left 
Drivemaxright = 250;                        // Set max motor current right 

Drivespeedleft = 200;                        // Set left motor speed (valid range 0....255) 
Drivespeedright = 200;                       // Set right motor speed (valid range 0....255)

Voltlow = 9;                                // Set minimum operation voltage

Cutterspeed = 200;                            // Set Cutter motor speed (50 should be maximum!)


 // Start cutter motor
                          
analogWrite(Cutter, Cutterspeed);          
delay(3000);

I_Start = 20;          // Set I_Start to a high value for not entering this function again
}


void Stop() {                                   // Stop the MOWER if battery is low

  Cutterspeed =0;                              // Switch off cutter motor
  analogWrite(Cutter, Cutterspeed);

  analogWrite(Driveleft, 0);                    // Switch off left drive motor
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,LOW); 
  
  analogWrite(Driveright, 0);                   // Switch off right drive motor
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,LOW); 
  

digitalWrite(Ledstart, LOW);                    // Switch green status LED OFF               

while (I_bat < 9)                              // As long as batterystatus is low, stay here
{
  // flash red status LED
digitalWrite(Ledbat, HIGH);
delay(100);
digitalWrite(Ledbat, LOW); 
delay(500);

Serial.println("Low Battery");                  // Print "Low battery" message        

}
}

void forward()  {                               // Drive forward
    
  digitalWrite(IN3,LOW);                        // Switch PIN IN3 LOW
  digitalWrite(IN4,HIGH);                       // Switch PIN IN4 HIGH
 
  digitalWrite(IN1,LOW);                        // Switch PIN IN1 Low                  
  digitalWrite(IN2,HIGH);                       // Switch PIN IN2 HIGH

 analogWrite(Driveleft, Drivespeedleft);        // Set PWM-value for left motor
 analogWrite(Driveright, Drivespeedright);      // Set PWM-value for right motor
 
}

void backward() {                               // Drive backward

  // Switch IN-PINs for backward drive
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW); 
 
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW); 

 for (I_Ramp = 0; I_Ramp < 255; I_Ramp ++) {    // Counter-loop for motor ramp
 analogWrite(Driveleft, I_Ramp);
 analogWrite(Driveright, I_Ramp);
 delay(5);
  }

 analogWrite(Driveleft, Drivespeedleft);        // After motor ramp use default drivespeed-value
 analogWrite(Driveright, Drivespeedright);      // After motor ramp use default drivespeed-value

  delay(1000);                                  // The time the mower should go backwards
  analogWrite(Driveleft, 0);                    // Stop motor after going backwards
  analogWrite(Driveright, 0);                   // Stop motor after going backwards
  delay(250);                                   // Give the mower some time to stop

  }

  void Turnleft() {                             // Turn left                      
  // Switch IN-PINs for turn left
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW); 
  
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);   

   for (I_Ramp = 0; I_Ramp < 200; I_Ramp ++) {  // Counter-loop for motor ramp
 analogWrite(Driveleft, I_Ramp);
 analogWrite(Driveright, I_Ramp);
 delay(5);
 
  }

 analogWrite(Driveleft, Drivespeedleft);     // After motor ramp use default drivespeed-value
 analogWrite(Driveright, Drivespeedright);   // After motor ramp use default drivespeed-value

 Turntime = random(100, 1500);               // Random for turning time
 delay(Turntime); 

analogWrite(Driveleft, 0);                    // Stop motor after turning
analogWrite(Driveright, 0);                   // Stop motor after turning

Sensevalueleft = 0;                           // Set left sensor-value back to zero
Sensevalueright = 0;                          // Set right sensor-value back to zero
delay(250);

   // Switch IN-PINs for driving forward
  digitalWrite(IN3,LOW);      
  digitalWrite(IN4,HIGH);
 
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);

 for (I_Ramp = 0; I_Ramp < 200; I_Ramp ++) {  // Counter-loop for motor ramp
 analogWrite(Driveleft, I_Ramp);
 analogWrite(Driveright, I_Ramp);
 delay(5); 
  }

 analogWrite(Driveleft, Drivespeedleft);        // After motor ramp use default drivespeed-value
 analogWrite(Driveright, Drivespeedright);     // After motor ramp use default drivespeed-value

}


void Turnright() {                            // Turn right

  // Switch IN-PINs for turn right 
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);  
  
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);  

   for (I_Ramp = 0; I_Ramp < 200; I_Ramp ++) {    // Counter-loop for motor ramp
 analogWrite(Driveleft, I_Ramp);
 analogWrite(Driveright, I_Ramp);
 delay(5); 
  }
  
 analogWrite(Driveleft, Drivespeedleft);          // After motor ramp use default drivespeed-value
 analogWrite(Driveright, Drivespeedright);        // After motor ramp use default drivespeed-value

 Turntime = random(100, 1500);                    // Random for turning time
 delay(Turntime); 
 
analogWrite(Driveleft, 0);                        // Stop motor after turning
analogWrite(Driveright, 0);                       // Stop motor after turning

Sensevalueleft = 0;                           // Set left sensor-value back to zero
Sensevalueright = 0;                          // Set right sensor-value back to zero
delay(250);

// Switch IN-PINs for driving forward
  digitalWrite(IN3,LOW);  
  digitalWrite(IN4,HIGH);
 
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);

 for (I_Ramp = 0; I_Ramp < 200; I_Ramp ++) {      // Counter-loop for motor ramp
 analogWrite(Driveleft, I_Ramp);
 analogWrite(Driveright, I_Ramp);
 delay(5); 
  }

 analogWrite(Driveleft, Drivespeedleft);          // After motor ramp use default drivespeed-value
 analogWrite(Driveright, Drivespeedright);        // After motor ramp use default drivespeed-value
 
}


