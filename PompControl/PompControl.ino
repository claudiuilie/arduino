#include <SimpleTimer.h>

const int pingPin = 7; // Trigger Pin of Ultrasonic Sensor
const int echoPin = 6; // Echo Pin of Ultrasonic Sensor
const int rangeInCm = 10; // range senzor distanta
const int distanceDelayInMils = 1000;  // delay senzor distanta
const int timerInMinutes = 1; // interval pompa activa
const int repeatTimer = 60;
const int countdownRange = timerInMinutes * repeatTimer; // interval in s rulare pompa
int countdown = 0;
boolean isStarted = false;
SimpleTimer timer;

void setup() {
   Serial.begin(9600); // Starting Serial Terminal
}

void loop() {
   timer.run();
   measureDistance();
   Serial.println();
   delay(distanceDelayInMils);
   
}


void startTimer(){
  int numTimers = timer.getNumTimers();

  if(countdown < countdownRange - 1){
    countdown += 1;
    Serial.print("Running: "); 
    Serial.print(countdown);
    Serial.print(" s");
    Serial.println();
  }else{
    Serial.println("Timer exausted.");
    stopPomp();
    countdown = 0;
  }
  
  Serial.println();
}

void startPomp() {
  
  if(timer.getNumTimers() == 0 ){
    timer.setTimer(1000, startTimer, timerInMinutes * repeatTimer );
    Serial.println("Pomp Started");
  }else{
    Serial.println("Pomp is already active");  
  }
}

void stopPomp() {
  Serial.println("Pomp Stopped");
}


void measureDistance(){
  
   long duration, cm;
   pinMode(pingPin, OUTPUT);
   digitalWrite(pingPin, LOW);
   delayMicroseconds(2);
   digitalWrite(pingPin, HIGH);
   delayMicroseconds(10);
   digitalWrite(pingPin, LOW);
   pinMode(echoPin, INPUT);
   duration = pulseIn(echoPin, HIGH);
   cm = microsecondsToCentimeters(duration);
   
   if(isInRange(cm)){
      Serial.print("In range");
      Serial.println();
      startPomp();
   }else{
      Serial.print(cm);
      Serial.print("cm");
      Serial.println();
   }
}

long microsecondsToCentimeters(long microseconds) {
   return microseconds / 29 / 2;
}


boolean isInRange(long cm) {
  if(cm < rangeInCm){
    return true;  
  }else{
    return false;  
  }
}
