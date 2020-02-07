/*
 * Sensore di parcheggio e luminosita'
 */
#define TRIG00 13
#define ECHO00 12
#define TRIG01 11
#define ECHO01 10



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
 
  pinMode(TRIG00,OUTPUT);
  pinMode(ECHO00,INPUT);
  
  pinMode(TRIG01,OUTPUT);
  pinMode(ECHO01,INPUT);
  
  Serial.begin(9600);
  Serial.println("Distanze misurate dal sensore ad ultrasuoni:");

}

void loop() {
  // put your main code here, to run repeatedly:
  float dist00 = 9999;
  float dist01 = 9999;
  
  Serial.print("Distanza sensore 00:");
  dist00 = distance(TRIG00,ECHO00);
  Serial.println(dist00);
  
  Serial.print("Distanza sensore 01:");
  dist01 =distance(TRIG01,ECHO01);
  Serial.println(dist01);
  delay(1000);

  if (dist00 <= 10 || dist01 <= 10){
    Serial.println("Troppo vicino!!!");
  }
  
}
