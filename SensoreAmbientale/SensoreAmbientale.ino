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

// Fotoresistore

RF24 radio(CE,SCN);

const byte address[6] = "00001";
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

  light = analogRead(A6);

  int chk = DHT11.read(S);

  float data[4];

  data[0] = id;
  data[1] = light;
  data[2] = (float)DHT11.temperature;
  data[3] = (float)DHT11.humidity;

  radio.write(&data,sizeof(data));
  
  Serial.print("Luminosita': ");
  Serial.println(light);
  
  Serial.print("Temperatura : ");
  Serial.println((float)DHT11.temperature);
  
  Serial.print("Umidita' : ");
  Serial.println((float)DHT11.humidity);

  delay(5000);
  

}
