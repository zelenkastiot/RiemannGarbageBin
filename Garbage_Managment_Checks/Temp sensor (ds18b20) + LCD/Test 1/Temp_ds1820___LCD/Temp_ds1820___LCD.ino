#include <LiquidCrystal.h> // includes the LiquidCrystal Library 
LiquidCrystal lcd(1, 2, 4, 5, 6, 7); // Creates an LC object. Parameters: (rs, enable, d4, d5, d6, d7) 

const int Sensor = d12; 
byte degree_symbol[8] = 
              {
                0b00111,
                0b00101,
                0b00111,
                0b00000,
                0b00000,
                0b00000,
                0b00000,
                0b00000
              };
void setup()
{
  pinMode(Sensor, INPUT);
  lcd.begin(16,2);
  lcd.createChar(1, degree_symbol);
  lcd.setCursor(0,0);
  lcd.print("    Digital    ");
  lcd.setCursor(0,1);
  lcd.print("  Thermometer   ");
  delay(4000);
  lcd.clear();
}
void loop()
{
  
     float temp_reading=digitalRead(Sensor);
     float temperature=temp_reading*(5.0/1023.0)*100;
     delay(10); 
  
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Temperature in C");
    lcd.setCursor(4,1);
    lcd.print(temperature);
    lcd.write(1);
    lcd.print("C");
    delay(1000);
}
