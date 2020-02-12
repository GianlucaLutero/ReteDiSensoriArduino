/*
    Sensore di parcheggio e luminosita'
    
    Copyright (C) 2020  Gianluca Lutero
 
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

 */
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Moduli ad ultrasuoni 
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

// Imposto l'indirizzo di comunicazione
const byte address[6] = "00001";

// Imposto l'id univoco del sensore
const int id = 2;

// Funzione per calcolare la distanza rilevata dal modulo ad ultrasuoni
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

  // Imposto il modulo nRF24L01
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
  
  Serial.begin(9600);
  Serial.println("Distanze misurate dal sensore ad ultrasuoni:");

}

void loop() {
  float dist00 = 9999;
  float dist01 = 9999;
  float dist02 = 9999;
  float data[4];

  // Rilevo le distanze dai tre sensori
  Serial.print("Distanza sensore 00:");
  dist00 = distance(TRIG00,ECHO00);
  Serial.println(dist00);
  
  Serial.print("Distanza sensore 01:");
  dist01 = distance(TRIG01,ECHO01);
  Serial.println(dist01);

  Serial.print("Distanza sensore 02:");
  dist02 = distance(TRIG02,ECHO02);
  Serial.println(dist02);

  data[0] = id;
  data[1] = dist00;
  data[2] = dist01;
  data[3] = dist02;  

  // Invio i  dati
  radio.write(&data,sizeof(data));
  delay(1000);

  if (dist00 <= 10 || dist01 <= 10 || dist02 <= 10){
    Serial.println("Troppo vicino!!!");

  }
  
}
