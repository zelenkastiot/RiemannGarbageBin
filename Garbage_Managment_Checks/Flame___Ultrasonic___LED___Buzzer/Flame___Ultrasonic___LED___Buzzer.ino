//Ultrasonic & LED
int triggerPin = 7; //triggering on pin 7
int echoPin = 8;    //echo on pin 8
int LED = 12; 

//FLame sensor & buzzer
int flame_sensor = 4;
int buzzer = 48;
int flame_detected;
int state = 0;//simple variable for displaying the state



void setup() {
  
  Serial.begin(9600);

  //Ultra & LED
  pinMode(triggerPin, OUTPUT); //defining pins
  pinMode(echoPin, INPUT);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);

  //Flame & buzzer
  pinMode(buzzer, OUTPUT);
  noTone(buzzer);
  pinMode(flame_sensor, INPUT);


}

void loop() {
  sensor();
  flame();
}

// ULTRA

void sensor() { //loop from the sensor code is renamed to the "sensor" void
  
  int duration, distance;    //Adding duration and distance
  digitalWrite(triggerPin, HIGH); //triggering the wave(like blinking an LED)
  delay(1000);
  digitalWrite(triggerPin, LOW);
  
  duration = pulseIn(echoPin, HIGH); //a special function for listening and waiting for the wave
  distance = (duration/2) / 29.1; //transforming the number to cm(if you want inches, you have to change the 29.1 with a suitable number
  
  Serial.print(distance);    //printing the numbers
  Serial.print("cm");       //and the unit
  Serial.println(" ");      //just printing to a new line
  
  //adding for mesuring distance where the led will turn off, even if we tell it to turn off when we chose so in the app
  
  if(distance <= 15){  //if we get too close, the LED will turn on
    digitalWrite(LED, HIGH);
    Serial.println("TOO CLOSE!!!");
     //so the stopping is visable
  }

  else {
  digitalWrite(LED,LOW);
  }

  delay(200);
  
  }  


//FLAME 

void flame(){
  
  flame_detected = digitalRead(flame_sensor);
  if (flame_detected == 1)
  {
    if(state==1){
    Serial.println("Flame detected...! take action immediately.");
    tone(buzzer, 600);
    delay(2000);
    noTone(buzzer);
    state=0;
    }
  }
  else
  {
    if(state==0){
    Serial.println("No flame detected. stay cool");
    digitalWrite(buzzer, LOW);
    state=1;
    }
  }
  delay(100);
  }
