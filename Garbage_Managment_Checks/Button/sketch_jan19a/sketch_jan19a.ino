int BUTTON1 = 7;

void setup(){

Serial.begin(9600);

pinMode(BUTTON1,INPUT);

}

void loop(){

Serial.println( );

if(digitalRead(BUTTON1) == HIGH)

{ Serial.println("Button1 1");

}else{

Serial.println("Button1 0");

} delay(200);

}
