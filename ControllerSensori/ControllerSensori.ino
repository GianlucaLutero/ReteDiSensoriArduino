/*
   Sensori per auto:
    - parcheggio
    - luminosita'
    - angolo di sterzata
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#include <LiquidCrystal_I2C.h>

// Modulo nrf24l01
#define CE 9
#define SCN 10
#define SCK 13
#define MOSI 11
#define MISO 12

// Display I2C
#define SDA A4
#define SCL A5

// LED RGB
#define RED 7
#define GREEN 6
#define BLUE 5

// Buzzer
#define BUZZER 2

// Button 
#define BUTTON 3

RF24 radio(CE, SCN);
LiquidCrystal_I2C lcd(0x27, 16, 2);

const byte address[6] = "00001";
const int n_transmitter = 2;

int button_state = 0;
int select = 0;

float parking[3];
float ambiental[3];

void setColor(int redValue, int greenValue, int blueValue) {
  analogWrite(RED, redValue);
  analogWrite(GREEN, greenValue);
  analogWrite(BLUE, blueValue);
}

void setup() {

  Serial.begin(9600);

  // Inizializzo il display lcd
  lcd.init();
  lcd.backlight();

  // Inizializzo il modulo radio
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

  // Inizializzo il led
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);

  // Inizializzo il buzzer
  pinMode(BUZZER, OUTPUT);

  // Inizializzo il bottone
  pinMode(BUTTON,INPUT);

  // Inizializzo le variabili dati
  for(int i=0;i<3;++i){
    parking[i] = 9999;
    ambiental[i] = 9999;  
  }
  

}

void loop() {
  int id;

  button_state = digitalRead(BUTTON);

  if(button_state == HIGH){
    select = (select + 1) % n_transmitter;
    Serial.print("Mostro i dati relativi alla trasmittente: ");
    Serial.println(select);
  }

  if (radio.available()) {
    float data[4];

    // Ricevo i dati
    radio.read(&data, sizeof(data));

    id = data[0];
    
    Serial.print("Id sensore: ");
    Serial.println(data[0]);

    switch (id) {
      case 1:

        ambiental[0] = data[1];
        ambiental[1] = data[2];
        ambiental[2] = data[3];

        Serial.print("Temperatura: ");
        Serial.println(data[2]);
        
        Serial.print("Umidita' : ");
        Serial.println(data[3]);
        
        Serial.print("Luminosita' : ");
        Serial.println(data[1]);
        
        break;

      case 2:
        // Stampo i dati su display
        parking[0] = data[1];
        parking[1] = data[2];
        parking[2] = data[3]; 

        // Accendo il led in base alla distanza
        float min = data[1];

        for (int i = 2; i <= 3; ++i) {
          if (data[i] <= min) {
            min = data[i];
          }
        }

        if (min >= 60) {
          setColor(255, 0, 255);
        }

        if (min >= 10 && min < 60) {
          setColor(255, 255, 0);
          tone(BUZZER, 1000, 50 + min);
        }

        if (min < 10) {
          setColor(0, 255, 255);
          tone(BUZZER, 1000, 500);
        }

        Serial.print("Dist 00: ");
        Serial.println(data[1]);
        Serial.print("Dist 01: ");
        Serial.println(data[2]);
        Serial.print("Dist 02: ");
        Serial.println(data[3]);
    }
  }

  // Stampo i dati sul display in base alla scelta
  lcd.clear();
  lcd.setCursor(0, 0);
  
  switch(select){
    case 0:
        
        lcd.print("T: ");
        lcd.print(ambiental[1]);
        
        lcd.print("U: ");
        lcd.print(ambiental[2]);
        
        lcd.setCursor(0,1);
        
        if(ambiental[0] < 200)
           lcd.print("Buio");
        else
           lcd.print("Luminoso");
      break;
    case 1:

        if (parking[0] <= 10 || parking[1] <= 10 || parking[2] <= 10) {
          lcd.print("    Pericolo    ");
          lcd.setCursor(0, 1);
          lcd.print("    Ostacolo    ");
        } else {
          lcd.print("L:");
          lcd.print(parking[0]);
          lcd.print(" C:");
          lcd.print(parking[1]);
          lcd.setCursor(0, 1);
          lcd.print("R:");
          lcd.print(parking[2]);
        }
      break;
  }

  delay(1000);

}
