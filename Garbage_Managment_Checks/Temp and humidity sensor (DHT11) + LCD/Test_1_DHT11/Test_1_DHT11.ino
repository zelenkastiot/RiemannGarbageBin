#include <dht.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(1, 2, 4, 5, 6, 7); // Creates an LCD object. Parameters: (rs, enable, d4, d5, d6, d7)

dht DHT;

#define DHT11_PIN 8

void setup() {
  lcd.begin(16,2); // Initializes the interface to the LCD screen, and specifies the dimensions (width and height) of the display
}

void loop() {
  int readData = DHT.read11(DHT11_PIN);
  float t = DHT.temperature;
  float h = DHT.humidity;
  lcd.setCursor(0,0); // Sets the location at which subsequent text written to the LCD will be displayed
  lcd.print("Temp.: "); // Prints string "Temp." on the LCD
  lcd.print(t); // Prints the temperature value from the sensor
  lcd.print(" C");
  lcd.setCursor(0,1);
  lcd.print("Humi.: ");
  lcd.print(h);
  lcd.print(" %");
  delay(2000);
}
