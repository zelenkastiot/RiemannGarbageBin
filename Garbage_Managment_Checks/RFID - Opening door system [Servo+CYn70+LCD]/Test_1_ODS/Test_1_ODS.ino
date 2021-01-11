#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal.h>
#include <Servo.h>
#define RST_PIN   5
#define SDA_PIN    53
byte readCard[4];
char* myTags[100] = {};
int tagsCount = 0;
String tagID = "";
boolean successRead = false;
boolean correctTag = false;
int proximitySensor;
boolean doorOpened = false;
// Create instances

MFRC522 mfrc522(SDA_PIN, RST_PIN);
LiquidCrystal lcd(1, 2, 4, 6, 7, 8);
Servo myServo; // Servo motor


void setup() {
  // Initiating
  SPI.begin();        // SPI bus
  mfrc522.PCD_Init(); //  MFRC522
  lcd.begin(16, 2);   // LCD screen
  myServo.attach(9);  // Servo motor
  myServo.write(10); // Initial lock position of the servo motor
  // Prints the initial message
  lcd.print("-No Master Tag!-");
  lcd.setCursor(0, 1);
  lcd.print("    SCAN NOW");
  // Waits until a master card is scanned
  while (!successRead) {
    successRead = getID();
    if ( successRead == true) {
      myTags[tagsCount] = strdup(tagID.c_str()); // Sets the master tag into position 0 in the array
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Master Tag Set!");
      tagsCount++;
    }
  }
  successRead = false;
  printNormalModeMessage();
}
void loop() {
  int proximitySensor = analogRead(A2);
  // If door is closed...
  if (proximitySensor > 200) {
    if ( ! mfrc522.PICC_IsNewCardPresent()) { //If a new PICC placed to RFID reader continue
      return;
    }
    if ( ! mfrc522.PICC_ReadCardSerial()) {   //Since a PICC placed get Serial and continue
      return;
    }
    tagID = "";
    // The MIFARE PICCs that we use have 4 byte UID
    for ( uint8_t i = 0; i < 4; i++) {  //
      readCard[i] = mfrc522.uid.uidByte[i];
      tagID.concat(String(mfrc522.uid.uidByte[i], HEX)); // Adds the 4 bytes in a single String variable
    }
    tagID.toUpperCase();
    mfrc522.PICC_HaltA(); // Stop reading
    correctTag = false;
    // Checks whether the scanned tag is the master tag
    if (tagID == myTags[0]) {
      lcd.clear();
      lcd.print("Program mode:");
      lcd.setCursor(0, 1);
      lcd.print("Add/Remove Tag");
      while (!successRead) {
        successRead = getID();
        if ( successRead == true) {
          for (int i = 0; i < 100; i++) {
            if (tagID == myTags[i]) {
              myTags[i] = "";
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print("  Tag Removed!");
              printNormalModeMessage();
              return;
            }
          }
          
          myTags[tagsCount] = strdup(tagID.c_str());
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("   Tag Added!");
          printNormalModeMessage();
          tagsCount++;
          return;
        }
      }
    }
    successRead = false;
    // Checks whether the scanned tag is authorized
    for (int i = 0; i < 100; i++) {
      if (tagID == myTags[i]) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(" Access Granted!");
        myServo.write(170); // Unlocks the door
        printNormalModeMessage();
        correctTag = true;
      }
    }
    if (correctTag == false) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(" Access Denied!");
      printNormalModeMessage();
    }
  }
  // If door is open...
  else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(" Door Opened!");
    while (!doorOpened) {
      proximitySensor = analogRead(A2);
      if (proximitySensor > 200) {
        doorOpened = true;
      }
    }
    doorOpened = false;
    delay(500);
    myServo.write(10); // Locks the door
    printNormalModeMessage();
  }
}
uint8_t getID() {
  // Getting ready for Reading PICCs
  if ( ! mfrc522.PICC_IsNewCardPresent()) { //If a new PICC placed to RFID reader continue
    return 0;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) {   //Since a PICC placed get Serial and continue
    return 0;
  }
  tagID = "";
  for ( uint8_t i = 0; i < 4; i++) {  // The MIFARE PICCs that we use have 4 byte UID
    readCard[i] = mfrc522.uid.uidByte[i];
    tagID.concat(String(mfrc522.uid.uidByte[i], HEX)); // Adds the 4 bytes in a single String variable
  }
  tagID.toUpperCase();
  mfrc522.PICC_HaltA(); // Stop reading
  return 1;
}

void printNormalModeMessage() {
  delay(1500);
  lcd.clear();
  lcd.print("-Access Control-");
  lcd.setCursor(0, 1);
  lcd.print(" Scan Your Tag!");
}
