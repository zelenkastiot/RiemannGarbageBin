#include <dht.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(1, 2, 4, 5, 6, 7); // Creates an LCD object. Parameters: (rs, enable, d4, d5, d6, d7)
const int trigPin = 12; 
const int echoPin = 11;

long duration;
int distanceCm, distanceInch;


dht DHT;

#define DHT11_PIN 8

void setup() {
  lcd.begin(16,2); // Initializes the interface to the LCD screen, and specifies the dimensions (width and height) of the display
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}



void loop() {
  
  int readData = DHT.read11(DHT11_PIN);
  float t = DHT.temperature;
  float h = DHT.humidity;
  lcd.setCursor(0,0); // Sets the location at which subsequent text written to the LCD will be displayed
  lcd.print("T:"); // Prints string "Temp." on the LCD
  lcd.print(t); // Prints the temperature value from the sensor
  lcd.print("C");
  lcd.setCursor(9,0);
  lcd.print("H:");
  lcd.print(h);
  lcd.print("%");
  delay(2000);

digitalWrite(trigPin, LOW);
delay(20);
digitalWrite(trigPin, HIGH);
delay(20);
digitalWrite(trigPin, LOW);
duration = pulseIn(echoPin, HIGH);
distanceCm= duration*0.034/2;
distanceInch = duration*0.0133/2;
lcd.setCursor(0,1); // Sets the location at which subsequent text written to the LCD will be displayed
lcd.print("Distance: "); // Prints string "Distance" on the LCD
lcd.print(distanceCm); // Prints the distance value from the sensor
lcd.print(" cm");
delay(10);
lcd.setCursor(0,1);
lcd.print("Distance: ");
lcd.print(distanceInch);
lcd.print(" inch");
delay(10);
  

}
