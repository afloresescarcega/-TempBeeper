#include <Adafruit_ESP8266.h>

#include <OneWire.h>

#include <NewPing.h>

#define TRIGGER_PIN  12
#define ECHO_PIN     11
#define MAX_DISTANCE 200
#define inPin 10 // for temp sensor
#define speakerPin =  8;      //Buzzer pin;

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

OneWire ds(inPin);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(speakerPin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
//  printDistance();
  printTemp();
  
}

void printDistance(){
  delay(50);
  int uS = sonar.ping();
  Serial.print("Ping: ");
  Serial.print(uS / US_ROUNDTRIP_CM);
  Serial.println("cm");
}

void printTemp(){
  int HighByte, LowByte, TReading, SignBit, Tc_100, Whole, Fract;
  byte i;
  byte present = 0;
  byte data[12];
  byte addr[8];
 
  if ( !ds.search(addr)) {
      ds.reset_search();
      return;
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
  Serial.print("Temperature: ");
  LowByte = data[0];
  HighByte = data[1];
  TReading = (HighByte << 8) + LowByte;
  SignBit = TReading & 0x8000;  
  if (SignBit)
  {
    TReading = (TReading ^ 0xffff) + 1;
  }
  Tc_100 = (6 * TReading) + TReading / 4; 
  Whole = Tc_100 / 100; 
  Fract = Tc_100 % 100;
  if (SignBit)
  {
     Serial.print("-");
  }
  Serial.print(Whole);
  Serial.print(".");
  if (Fract < 10)
  {
     Serial.print("0");
  }
  Serial.print(Fract);
  Serial.print(" C\n");
}

