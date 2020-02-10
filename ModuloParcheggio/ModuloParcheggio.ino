/*
 * Sensore di parcheggio e luminosita'
 */
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
 
#define TRIG00 8
#define ECHO00 7
#define TRIG01 6
#define ECHO01 5
#define TRIG02 4
#define ECHO02 3

// Modulo nrf24l01
#define CE 9
#define SCN 10
#define SCK 13
#define MOSI 11
#define MISO 12

RF24 radio(CE,SCN);

const byte address[6] = "00001";

float distance(int trig, int echo){
  int duration = 0;
  float distance = 0;

  digitalWrite(trig,LOW);
  delayMicroseconds(2);
  digitalWrite(trig,HIGH);
  delayMicroseconds(10);
  digitalWrite(trig,LOW);

  duration = pulseIn(echo,HIGH);
  distance = (duration*0.034)/2;

  return distance;
}

void setup() {

  // Left sensor
  pinMode(TRIG00,OUTPUT);
  pinMode(ECHO00,INPUT);

  // Center sensor
  pinMode(TRIG01,OUTPUT);
  pinMode(ECHO01,INPUT);

  // Right sensor.h
  pinMode(TRIG02,OUTPUT);
  pinMode(ECHO02,INPUT);

  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
  
  Serial.begin(9600);
  Serial.println("Distanze misurate dal sensore ad ultrasuoni:");

}

void loop() {
  // put your main code here, to run repeatedly:
  float dist00 = 9999;
  float dist01 = 9999;
  float dist02 = 9999;
  float data[3];
  
  Serial.print("Distanza sensore 00:");
  dist00 = distance(TRIG00,ECHO00);
  Serial.println(dist00);
  
  Serial.print("Distanza sensore 01:");
  dist01 = distance(TRIG01,ECHO01);
  Serial.println(dist01);

  Serial.print("Distanza sensore 02:");
  dist02 = distance(TRIG02,ECHO02);
  Serial.println(dist02);

  data[0]=dist00;
  data[1]=dist01;
  data[2]=dist02;  
  
  radio.write(&data,sizeof(data));
  delay(1000);

  if (dist00 <= 10 || dist01 <= 10 || dist02 <= 10){
    Serial.println("Troppo vicino!!!");

  }
  
}
