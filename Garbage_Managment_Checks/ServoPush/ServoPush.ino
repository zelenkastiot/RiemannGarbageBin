#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

 

#define LED_G 7 //define green LED pin D7
#define LED_R 6 //define red LED D6

#define BUZZER 2 //buzzer pin D2

MFRC522 mfrc522(53, 49);   // Create MFRC522 instance

Servo myServo1; //Servo 1
Servo myServo2; //Servo 2 

const int buttonPin = 6;

void setup() 
{
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  
  myServo1.attach(8); //servo1 - pin 8
  myServo1.write(0); //servo start position

  myServo2.attach(9);
  myServo2.write(180);
  
  pinMode(LED_G, OUTPUT);
  pinMode(LED_R, OUTPUT);

// Set up the pushbutton pins to be an input:
  pinMode(buttonPin, INPUT);
  
  pinMode(BUZZER, OUTPUT);
  noTone(BUZZER);
  Serial.println("Put your card to the reader...");
  Serial.println();

}
void loop() 
{
  if(digitalRead(buttonPin) == HIGH){
  
    Serial.println("Pressure plate pressed");
    Serial.println();
    delay(500);
    myServo1.write(90);
    myServo2.write(90);
    delay(5000);
    myServo1.write(0);
    myServo2.write(180);
  
  }
  
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content= "";
  byte letter;
 
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();

  if (content.substring(1) == "50 80 73 A5") //change here the UID of the card/cards that you want to give access
  {
    Serial.println("Authorized access");
    Serial.println();
    delay(500);
    digitalWrite(LED_G, HIGH);
    tone(BUZZER, 500);
    delay(300);
    noTone(BUZZER);
    myServo1.write(90);
    myServo2.write(90);
    delay(5000);
    myServo1.write(0);
    myServo2.write(180);
    digitalWrite(LED_G, LOW);
  }
  
else   {
    Serial.println(" Access denied");
    digitalWrite(LED_R, HIGH);
    tone(BUZZER, 300);
    delay(1000);
    digitalWrite(LED_R, LOW);
    noTone(BUZZER);
  }
  

 
 
}
