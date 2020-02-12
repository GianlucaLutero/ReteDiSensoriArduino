/*
  Sensore ambientale
 
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

#include <dht11.h>

// Modulo nrf20l01
#define CE 9
#define SCN 10
#define SCK 13
#define MOSI 11
#define MISO 12

// Sensore DTH11
#define S 4

RF24 radio(CE,SCN);

// Imposto l'indirizzo di comunicazione
const byte address[6] = "00001";

// Imposto l'id univoco
const int id = 1;

dht11 DHT11;


void setup() {

  Serial.begin(9600);

  // Inizalizzo il modulo radio
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();

  // Inizializzo modulo temperatura
  pinMode(S,INPUT);

  // Inizializzo fotoresistore
  pinMode(A6,INPUT);
}

void loop() {
  float temp;
  float umidity;
  float light;

  // Leggo la luminosità ambientale
  light = analogRead(A6);

  // Leggo la temperatura e l'umidità dal sensore dht11
  int chk = DHT11.read(S);

  float data[4];

  data[0] = id;
  data[1] = light;
  data[2] = (float)DHT11.temperature;
  data[3] = (float)DHT11.humidity;

  // Invio i dati
  radio.write(&data,sizeof(data));
  
  Serial.print("Luminosita': ");
  Serial.println(light);
  
  Serial.print("Temperatura : ");
  Serial.println((float)DHT11.temperature);
  
  Serial.print("Umidita' : ");
  Serial.println((float)DHT11.humidity);

  delay(5000);
  

}
