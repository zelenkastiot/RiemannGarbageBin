/*
 * --Kiril & Flip--
 * HC-05 ili HC-06 so Arduino Mega 2560
 *
 *  Pred testiranje
 *   1. Proverka na povrzuvanjeto na site senzori
 *   2. Povrzuvanje na zica od 3.3 v do pin 34 na HC-05 (ili stiskanje na kopceto) kaj hc06 nonstop 3.3v vo 34 pin
 *   3. Go vadis Ground na HC-05
 *   4. Vrakjas Ground 
 *   5. Se povruzvas od telefon 
 *   6. So 1 se pali, so 0 se gasi 
 *   7. Ako e poblisku od 15 cm isto i e upalena pak ja gasi 
 */

int triggerPin = 7; //trigger pin na ultra sonic vo D7
int echoPin = 8;    //echo pin na ultra sonic vo D8
int LED = 9; //led pin vo D9
int info = 0;//promenliva za informacijata koja ja dobivame od hc05
int sostojba = 0;//ke ni ja kazuva sostojbata dokolku veke upalena led da ne kazuva celo vreme (obratno istotaka)
bool flag = false; //false e ako led e izgasena a true e ako led e upalena

void setup() { 
  
  Serial.begin(9600);  //Pocnuvanje na serial comunication, za da mozeme da ja gledame distancata sto se meri

  //definiranje na pinovite (input ili output)  
  pinMode(triggerPin, OUTPUT); 
  pinMode(echoPin, INPUT);
  pinMode(LED, OUTPUT);    

  //na pocetokot na programata sijalicata e isklucena za da nema zbunki
  digitalWrite(LED, LOW);  
  
}

void loop(){ 
  bluetooth();
  sensor();
}



void bluetooth() { 

  
  if(Serial.available() > 0){  //proveruva dali ima bilo kakva povratna informacija od serial liniite
    info = Serial.read();   //ako ima informacija (0 ili 1) od hc05 se smestuva vo info
    
    sostojba = 0;   //prethodnata sostojba ne se znae (ako e 1 ke se znae dali e upalena ili izgasena sijalicata)
  }
  if(info == '1'){                //ako dobie broj 1 
    digitalWrite(LED, HIGH);    //ke ja upali sijalicata
    if(sostojba == 0){              //ako sostojbata e 0, togas prikazi deka LED e upalena i potoa smeni ja vo 1
      Serial.println("LED ON");  //^^so sostojba promenlivata se pravi prevencija da ne pisuva celo vreme deka sijalicata e upalena
      sostojba = 1;
      flag = true;               //Flagot se koristi za dali e upalena ili izgasena sijalcata (true e za on, false e za off)
    }
   }
  else if(info == '0'){           //ako dobie broj 0
    digitalWrite(LED, LOW);      //ke ja izgasi sijalicata
    if(sostojba == 0){
      Serial.println("LED OFF"); //prikazuva LED OFF
      sostojba = 1;
      flag = false;              //izgasena sijalica
     }
  }
}


void sensor() { 
  
  int vreme, distanca;    //vremetranje = vreme, distanca = dalecina
  
  digitalWrite(triggerPin, HIGH); //prakjanje na bran 
  delay(1000);
  digitalWrite(triggerPin, LOW);
  
  vreme = pulseIn(echoPin, HIGH); //pulseIn - specijalna funkcija za slusanje na branot (echo pinot)
  distanca = (vreme/2) / 29.1; //transformiranje na brojot vo cm (za inchi, se menja brojot 29.1 )
  
  Serial.print(distanca);    //printajki gi brojkite
  Serial.print("cm");       //i ednicata
  Serial.println(" ");      //nova linija 
  
  //dodaten del za gasenje na led sijalicata ako ima prepreka poblisku od 15 cm
  //ako se trgne preprekata pak si sveti ako svetela prethodno
  if(distanca <= 15){  
    digitalWrite(LED, LOW);
    Serial.println("BLISKA PREPREKA!!");
    delay(200); //pogolem delay za da se bide vidlivo gasenjeto, ako nema delay nema da se primeti samo ke trepne
  }

  else if(flag==true)   //ako prethodno svetela povtorno upali ja
  digitalWrite(LED,HIGH);

  }  
