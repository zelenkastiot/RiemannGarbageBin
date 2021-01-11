/*
* Ultrasonic Sensor HC-SR04 and Arduino Tutorial, Test 1 
*/

// defines pins numbers
const int trigPin = 10;
const int echoPin = 9;

// defines variables
long duration;
int distance;

void setup() {
pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin, INPUT); // Sets the echoPin as an Input
Serial.begin(9600); // Starts the serial communication
}

void loop() {
// Clears the trigPin
digitalWrite(trigPin, LOW);
delay(200);

// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPin, HIGH);
delay(200);
digitalWrite(trigPin, LOW);

// Reads the echoPin, returns the sound wave travel time in microseconds
duration = pulseIn(echoPin, HIGH);

// Calculating the distance
distance= duration*0.034/2;
if(distance < 20) {
  Serial.print("Too close!!\n");
  }

else{
Serial.print("Distance: ");
Serial.println(distance);
}
// Prints the distance on the Serial Monitor


}
