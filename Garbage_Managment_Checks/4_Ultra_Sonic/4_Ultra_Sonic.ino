
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

 
long duration, distance, Sensor1,Sensor2,Sensor3,Sensor4;
long sensorAverage;
long capacity = 100; 
void setup()
{
Serial.begin (9600);
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
}
 
void loop() {
  SonarSensor(trigPin1, echoPin1);
  Sensor1 = distance;
  
  SonarSensor(trigPin2, echoPin2);
  Sensor2 = distance;
  
  SonarSensor(trigPin3, echoPin3);
  Sensor3 = distance;
  
  SonarSensor(trigPin4, echoPin4);
  Sensor4 = distance; 

  Serial.print(Sensor1);
  Serial.print(" ");
  Serial.print(Sensor2);
  Serial.print(" ");
  Serial.print(Sensor3);
  Serial.println(" ");
//  Serial.print(Sensor4);
//  Serial.println(" ");
  //calculate(Sensor1, Sensor2, Sensor3,Sensor4);
  delay(1000); //1 sec

                                         
}

void calculate(long Sensor1, long Sensor2, long Sensor3, long Sensor4){


  sensorAverage = (Sensor1 + Sensor2 + Sensor3 + Sensor4)/4; 
  Serial.println(sensorAverage);
  delay(200);  

//  int procent = (sensorAverage*100)/capacity;
//  Serial.print("Garbage procent: ");
//  Serial.print(100-procent);
//  Serial.println(" %");
  delay(300);
  } 

  
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
