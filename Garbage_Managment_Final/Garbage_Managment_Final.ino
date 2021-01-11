/* 2019
 * Authors: Kiril & Filip 
 * Project: Garbage managment system 
 * Mentor: prof. Igor Miskovski 
 *
 * RFID 7 wires: 
 *  Black wire - D53 (SDA)
 *  White wire - D52 (SCA)
 *  Blue wire - D51 (MOSI)
 *  Orange wire - D50 (MISO)
 *  Brown wire - GND (GND)
 *  Yellow wire - D49 (Reset)
 *  Red wire - 3.3 V (Vcc/3.3V) 
 *  
 * Fire sensor 1 wire: 
 *  Green wire - 5 V (Vcc/5V)  
 *  Blue wire - GND (GND)
 *  Yellow wire - D5 (Signal)
 *  
 * Buzzer 2 wires:
 *  White wire - GND (Short)
 *  Purple wire - D12 (Long)
 */
 
#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#define MAX_BUFFER_LENGTH 4  // change this to how many bytes you'd like to have in the buffer
char buffer[MAX_BUFFER_LENGTH];

// RFID -> MFRC522 instance
MFRC522 mfrc522(53, 49);   

//Servos
Servo myServo1; //For calculating volume 
Servo myServo2; //For opening system 

//RGB led -> 3 pins
int redPin = 9;
int greenPin = 10;
int bluePin = 11; 

//Buzzer
#define BUZZER 12 //purple wire 

//B1 & B2 for pressure plate
#define BUTTON1 46 
#define BUTTON2 48 

//USS1
#define trigPin1 42 //Blue wire
#define echoPin1 43 //Green wire 

//USS2
#define trigPin2 40 //Green wire  
#define echoPin2 41 //Yellow wire 

//USS3
#define trigPin3 38 //White wire 
#define echoPin3 39 //Grey white 

//USS4
#define trigPin4 36 //Grey wire 
#define echoPin4 37 //Purple wire 

//Variables for timmer 
unsigned long current;
unsigned long lastOccur=0; 

//Variables for ultra-sonic sensors
long Sensor1,Sensor2,Sensor3,Sensor4;
long duration, distance;


//Flame detector setup
int flame_sensor = 30;
int flame_detected;
int state = 0;//simple variable for displaying the state

//Global counters 
int i;
int counter=1;

void setup() 
{
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();          // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522

  //Servo for calculating 
  myServo1.attach(3);       

  //Servo for opening 
  myServo2.attach(4);
  myServo2.write(110);    
  
  //RGB
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  //Buzzer
  pinMode(BUZZER, OUTPUT);
  noTone(BUZZER);
  //Button 1 and 2
  pinMode(BUTTON1,INPUT);
  pinMode(BUTTON2,INPUT);
  //Flame
  pinMode(flame_sensor, INPUT);
  //Sensor1
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  //Sensor2
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  //Sensor3
  pinMode(trigPin3, OUTPUT);
  pinMode(echoPin3, INPUT);
  //Sensor4
  pinMode(trigPin4, OUTPUT);
  pinMode(echoPin4, INPUT);

  Serial.println("**********RGB node**********");
  Serial.println();

}

//SETCOLOR FOR RGB LED
void setColor(int red, int green, int blue){
    #ifdef COMMON_ANODE
      red = 255 - red;
      green = 255 - green;
      blue = 255 - blue;
    #endif
    analogWrite(redPin, red);
    analogWrite(greenPin, green);
    analogWrite(bluePin, blue);  
  }


//FLAME 
void flame(){
  flame_detected = digitalRead(flame_sensor);
  if (flame_detected == 1)
  {
    if(state==1){
    Serial.println("Flame detected...! take action immediately.");
    tone(BUZZER, 300);
   
   // myServo2.write(45);
    setColor(255, 0, 0);
    state=0;
    }
    
  }
  else
  {
 
  //  myServo2.write(0);
    noTone(BUZZER);
    delay(200);
    setColor(0, 0, 0);
    if(state==0){
    state=1;
    }
  }
  delay(100);
  }

//CALCULATE VOLUME DEMO
long plot=0; //plot to be calculated 
int distance_between_sensors=2; //2cm 
int distance_between_mid_sensors=4; //4cm

float radius = 9.5; //cm
float height = 19.0; //cm
float my_volume; //where the acctual volume will be stored

//Function for setting volume
void init_volume(float current_plot){
  my_volume = current_plot * PI * radius;
}
//Function for returning the float value of the current volume
float c_volume(float plot){
  return plot * PI * radius;
}

//Function for measuring current plot
float calculate(long Sensor1, long Sensor2, long Sensor3){
  //Trapeziod rule 
  plot = 0;
  //1st trapezoid
  plot+=(Sensor1+Sensor2)*distance_between_sensors/2;
  //2nd trapeziod
  plot+=(Sensor2+Sensor3)*distance_between_mid_sensors/2;
  //3rd trapeziod
  //plot+=(Sensor3+Sensor4)*distance_between_sensors/2;
  Serial.print("The plot from this measuring is: P=");
  Serial.println(plot);
  return plot;
  } 

void calc_capacity(float current_volume){
  Serial.print("The bin was: ");
  if(current_volume/my_volume == 1) Serial.print(" 0");
  else{
    float procent;
    procent = current_volume/my_volume;
    procent = 100-procent*100;
    //sprintf(buffer," %.2f%",procent);
    Serial.print(procent);
  }
  Serial.println("% full");
}


//ULTRA SONIC SENSOR
void SonarSensor(int trigPin,int echoPin)
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(20000);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(20000);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1; 
}

//PRESSURE PLATE 

void pressure_plate(){
if((digitalRead(BUTTON1) == LOW)||(digitalRead(BUTTON2) == LOW)){
    
    Serial.println("Pressure plate activated");
    Serial.println();
    delay(500);
    setColor(0, 255, 0);
    tone(BUZZER, 600);
    delay(300);
    noTone(BUZZER);
    myServo2.write(180);
    delay(5000);
    myServo2.write(110);
    setColor(0, 0, 0);;
    }
}


//RFID 
void rfid(){
  
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
    setColor(0, 255, 0); //Green light
    tone(BUZZER, 500);
    delay(300);
    noTone(BUZZER);
    myServo2.write(180);
    delay(5000);
    myServo2.write(110);
    setColor(0, 0, 0); //Green light off
  }
 
 else   {
    Serial.println(" Access denied");
    setColor(255, 0, 0); //Red light
    tone(BUZZER, 300);
    delay(1000);
    setColor(0, 0, 0); //Red light off
    noTone(BUZZER);
  }
}

//MAIN 
float current_plot=0;
float current_volume;
void loop() 
{
  setColor(0,0,0);
  current=millis();
  
   flame();
   pressure_plate();
   rfid();

  //15 sec
   
   if(current - lastOccur > 20000){
    Serial.print(counter);
    Serial.println(" Mesuring");
    for(i=180;i>=45;i-=45){
   
    SonarSensor(trigPin1, echoPin1);
    Sensor1 = distance;
    
    SonarSensor(trigPin2, echoPin2);
    Sensor2 = distance;
    
    SonarSensor(trigPin3, echoPin3);
    Sensor3 = distance;
    
    SonarSensor(trigPin4, echoPin4);
    Sensor4 = distance; 
    
    Serial.print(i);
    Serial.print("-> Distance: ");
    current_plot+=calculate(Sensor1,Sensor2,Sensor3,Sensor4);
    setColor(150,0,150);
    delay(1000); //1 sec
    setColor(0,0,0);
    myServo1.write(i);
    delay(4000); //5 sec
 }
    if(counter==1){
      init_volume(current_plot);
      Serial.print("The volume of this RGB node is set to: ");
      Serial.print(my_volume);
      Serial.println(" cm^3");
    }
    current_volume = c_volume(current_plot);
    calc_capacity(current_volume);
    counter++;
    setColor(0,0,255);
    delay(1000); //1 sec
    myServo1.write(180);
    delay(1000); //1 sec
    Serial.println();
    lastOccur = millis(); 
    current_plot = 0;
  }
}
