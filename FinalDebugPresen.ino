#include <Ultrasonic.h>
#include <SoftwareSerial.h>
#include <rgb_lcd.h>

SoftwareSerial SoftSerial(2, 3);
char buffer[64];       // buffer array for data receive over serial port
int count = 0;                    // counter for buffer array


int Min = 15;
int Sec = 59;
int Open = 0;
int Ecoulement = 1000;
int EcoulementWin = 4000;
int Condition = 1;
int Lose = 1;
int buttonState = 0; 
const int buttonPin = 4;
const int LedPerdu = 13;

const int colorR = 252;
const int colorG =  253;
const int colorB = 252;


Ultrasonic ultrasonic(7);
Ultrasonic ultrasonic2(8);
rgb_lcd lcd;


/*const int TouchBleu=4;
const int TouchRouge=3;
const int TouchJaune=2;*/


String stringToBinary(String str) {
  String binary = "";
  for (int i = 0; i < str.length(); i++) {
    byte asciiValue = str.charAt(i);
    String binaryValue = "";
    for (int j = 7; j >= 0; j--) {
      binaryValue += (asciiValue >> j) & 1;
    }
    binary += binaryValue;
  }
  return binary;
}



void setup()
{
    SoftSerial.begin(9600);     // the SoftSerial baud rate
    Serial.begin(9600);         // the Serial port of Arduino baud rate.
    const int buttonPin = 4;
    /*pinMode(TouchBleu, INPUT);
    pinMode(TouchRouge, INPUT);
    pinMode(TouchJaune, INPUT);*/
     
   lcd.begin(16, 2);
   lcd.setRGB(colorR, colorG, colorB);


   lcd.print(Min);
   lcd.print(" : ");
   lcd.print(Sec);

   pinMode(buttonPin, INPUT);
   pinMode(LedPerdu, OUTPUT);
   digitalWrite(LedPerdu, LOW);
}
    
  
  String valueOfEnd; 
  String Rouge = "0000001000110000001101010011000000110000001101110100010100111000010001000011010101000101010000010011100000000011";
  String Jaune = "0000001000110000001110000011000000110000001100010011010001000100001110000011001100110101010001100011000100000011";
  String Bleu = "0000001000110000001110000011000000110000001100010100010001000110001101110011011000110011001110000011000100000011";
  
  int Rougeok = 0;
  int Jauneok = 0;
  int Bleuok = 0;
  
  
void loop(){
  long Detection;
  long Detection2;
  
  buttonState = digitalRead(buttonPin);
  
  // Porte 
  Detection = ultrasonic.MeasureInCentimeters(); // two measurements should keep an interval
  Detection2 = ultrasonic2.MeasureInCentimeters(); //DetectionPoid Todo : Corriger le code avec,Led, Distribution de L'eau

  
  //Serial.println(Detection_poid);
  
    // if date is coming from software serial port ==> data is coming from SoftSerial shield
    if (SoftSerial.available())              
    {
        while(SoftSerial.available())               // reading data into char array
        {
            buffer[count++] = SoftSerial.read();      // writing data into array
            if(count == 64)break;
        }
        //Serial.write(buffer);     // if no data transmission ends, write buffer to hardware serial port
        String end = String(buffer);
        String endbin = stringToBinary(end);
        valueOfEnd = endbin;
        //Serial.print(endbin);

        endbin.remove(1, 80);
        //Serial.println("endbin");

        //Serial.print(end);
        clearBufferArray();             // call clearBufferArray function to clear the stored data from the array
        count = 0;                     // set counter of while loop to zero
    }



    Serial.println(valueOfEnd);
    
    if(valueOfEnd == Rouge && Detection2 < 5){
      Rougeok = 1;
    }else if(valueOfEnd == Jaune && Detection2 < 5){
      Jauneok = 1;
    }else if(valueOfEnd == Bleu && Detection2 < 5){
      Bleuok = 1;
    }else{
      Serial.print("No Connection !");
      //Serial.println(valueOfEnd);
    }


   /*int sensorBleu = digitalRead(TouchBleu);    
   int sensorJaune = digitalRead(TouchJaune);    
   int sensorRouge = digitalRead(TouchRouge);*/

   /*if(sensorBleu == 1){
    Bleuok = 1;
   }else if(sensorJaune == 1){
    Jauneok = 1;
   }else if(sensorRouge == 1){
    Rougeok =1;
    }*/
    
    
    if (Serial.available())             // if data is available on hardware serial port ==> data is coming from PC or notebook
    SoftSerial.write(Serial.read());    // write it to the SoftSerial shield

    // Code Timer

    
    if( Min == 0 && Sec <= 0 && Lose == 1){
        lcd.clear();
        lcd.println("Condamne !");
        digitalWrite(LedPerdu, HIGH);
    }

 
    if(Detection < 5){ //Detecte L'entrée dans la Salle 
    Open = 1;
    Serial.println(Open);
 }



if(Sec == 0 && Min > 0){ //Retour Aux Minutes
    Sec = 59;
    Min--;
}

  Serial.print(Detection);//0~400cm
  Serial.println(" cm");



 if(Bleuok == 1 && Jauneok == 1 && Rougeok == 1 && Open == 1){
  lcd.clear();
  lcd.print("ééééééééééééééééé");
  delay(800);
  Condition = 0;
  Min = 15;
  Sec = 59;
  Bleuok = 0;
  Jauneok = 0;
  Rougeok = 0;
 }

 
 if(Open == 1 && Sec <= 59 && Sec >= 0){ //Affiche des Temps //Sec Verrouillage ou non 
  lcd.clear();
  Sec--;
  lcd.print(Min);
  lcd.print(" : ");
  lcd.print(Sec);
  if(Condition == 1){
    delay(Ecoulement);
  }else{
    delay(EcoulementWin);
  }
 }



if (buttonState == HIGH) { 
  Sec = 5;
  Min = 0;

}


}

void clearBufferArray()                 // function to clear buffer array
{
    // clear all index of array with command NULL
    for (int i=0; i<count; i++)
    {
        buffer[i]=NULL;
    }                  
}
