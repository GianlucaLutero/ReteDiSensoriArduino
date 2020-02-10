/*
 * Sensori per auto:
 *  - parcheggio
 *  - luminosita'
 *  - angolo di sterzata
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

RF24 radio(CE,SCN);
LiquidCrystal_I2C lcd(0x27, 16, 2);

const byte address[6] ="00001";

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
  radio.openReadingPipe(0,address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

  // Inizializzo il led
  pinMode(RED,OUTPUT);
  pinMode(GREEN,OUTPUT);
  pinMode(BLUE,OUTPUT);

}

void loop() {

  if(radio.available()){
    float data[3];

    // Ricevo i dati
    radio.read(&data,sizeof(data));

    // Stampo i dati su display
    lcd.clear();
    lcd.setCursor(0,0);

    if (data[0] <= 10 || data[1] <= 10 || data[2] <= 10){
      lcd.print("Porco Dio");
      lcd.setCursor(0,1);
      lcd.print("sbatti !!!!");
    }else{
      lcd.print("L:");
      lcd.print(data[0]);
      lcd.print(" C:");
      lcd.print(data[1]);
      lcd.setCursor(0,1);
      lcd.print("R:");
      lcd.print(data[2]);
    }

    // Accendo il led in base alla distanza
    float min = data[0];

    for(int i = 1; i<= 2; ++i){
      if(data[i] <= min){
        min = data[i];
      }
    }

    if(min >= 60){
      setColor(255,0,255);
    }
    
    if(min >= 10 && min < 60){
      setColor(0,0,255);  
    }

    if(min < 10){
      setColor(0,255,255);
    }
    
    Serial.print("Dist 00: ");
    Serial.println(data[0]);
    Serial.print("Dist 01: ");
    Serial.println(data[1]);
    Serial.print("Dist 02: ");
    Serial.println(data[2]);
  }

}
