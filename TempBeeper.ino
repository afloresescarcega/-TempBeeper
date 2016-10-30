#include <Adafruit_ESP8266.h>

#include <OneWire.h>

#include <NewPing.h>

#define TRIGGER_PIN  12
#define ECHO_PIN     11
#define MAX_DISTANCE 200
#define inPin 10 // for temp sensor
#define speakerPin 8      //Buzzer pin;

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

OneWire ds(inPin);



double getDistanceInCM(){
  delay(50);
  int uS = sonar.ping() / US_ROUNDTRIP_CM;
  return(uS);
}
void printDistance(){
//  delay(50);
//  int uS = sonar.ping();
  Serial.print("Ping: ");
//  Serial.print(uS / US_ROUNDTRIP_CM);
  Serial.print(getDistanceInCM());
  Serial.println("cm");
}

int returnTc_100(){
    int HighByte, LowByte, TReading, SignBit, Tc_100, Whole, Fract;
  byte i;
  byte present = 0;
  byte data[12];
  byte addr[8];
 
  if ( !ds.search(addr)) {
      ds.reset_search();
      return 0;
  }
 
 
  ds.reset();
  ds.select(addr);
  ds.write(0x44,1); 
 
  delay(1000);  
 
 
  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);  
 
 
 
  for ( i = 0; i < 9; i++) { 
    data[i] = ds.read();
  }
  LowByte = data[0];
  HighByte = data[1];
  TReading = (HighByte << 8) + LowByte;
  SignBit = TReading & 0x8000;  
  if (SignBit)
  {
    TReading = (TReading ^ 0xffff) + 1;
  }
  Tc_100 = (6 * TReading) + TReading / 4; 
  return Tc_100;
}

double getCelsius(int Tc_100){
  return Tc_100/100 + Tc_100%100/100.0;
  
}

void printTemp(){

//  if (SignBit)
//  {
//     Serial.print("-");
//  }
  int Tc_100 = returnTc_100();
  int Whole = Tc_100 / 100; 
  int Fract = Tc_100 % 100;
  Serial.print("Temperature: ");
  Serial.print(Whole);
  Serial.print(".");
  if (Fract < 10)
  {
     Serial.print("0");
  }
  Serial.print(Fract);
  Serial.print(" C\n");
}

void makeSiren(){
    digitalWrite(speakerPin,LOW);
    digitalWrite(speakerPin, HIGH);
    delay(1000);
    digitalWrite(speakerPin,LOW);
    digitalWrite(speakerPin, HIGH);
    delay(.75);
    digitalWrite(speakerPin,LOW);
    digitalWrite(speakerPin, HIGH);
    delay(.75);
    digitalWrite(speakerPin,LOW);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(speakerPin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
//  printDistance();
//  printTemp();
//  Serial.print("this is the temp: "); 
//  Serial.print(getCelsius(returnTc_100()));
//  Serial.print("\n");
  int distance = getDistanceInCM();
  if( distance > 30 && distance < 60 && getCelsius(returnTc_100()) > 28){
    Serial.println("Yes");
    makeSiren();
  }
  
}
