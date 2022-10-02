/* ****************
UGA Makeathon 2022
Ocean Cleaner Robot
**************** */
#include <SoftwareSerial.h>

#define MOTOR1 2 // Movement Motor
#define MOTOR2 3 // Collector Motor
#define LED 13
#define ECHO 4
#define TRIG 5
SoftwareSerial HM10(0, 1); // RX = 0, TX = 1
char appData;  
String inData = "";

long duration;
int distance;

void setup() {
  // Initialize serial communication to recieve voltage output
  Serial.begin(9600);
  HM10.begin(9600); // set HM10 serial at 9600 baud rate
  // Declare inputs and outputs
  pinMode(MOTOR1, OUTPUT);
  pinMode(MOTOR2, OUTPUT);
  pinMode(LED, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(TRIG, OUTPUT);
}

void loop() {

  HM10.listen();  // listen the HM10 port
  while (HM10.available() > 0) { // if HM10 sends something then read
    appData = HM10.read();
    inData = String(appData); // save the data in string format
    Serial.write(appData);
  }

  if (Serial.available()) {// Read user input if available.
    delay(10);
    HM10.write(Serial.read());
  }

  digitalWrite(MOTOR1, HIGH);
  digitalWrite(MOTOR2, HIGH);
  // Clears the TRIG condition
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  // Sets the TRIG HIGH (ACTIVE) for 10 microseconds
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  // Reads the ECHO, returns the sound wave travel time in microseconds
  duration = pulseIn(ECHO, HIGH);
  // Calculating the distance from sensor to top level of trash
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  
  if (distance <= 1) { // Trash bin is full
    digitalWrite(MOTOR2, LOW);
    HM10.write("binFull");
  } 
}
  