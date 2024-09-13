#include <Arduino.h>

void setup(){
  pinMode(53, OUTPUT);
  pinMode(51, OUTPUT);
  Serial.begin(9600);
}

String readSerial();

void loop(){
  if (Serial.available() > 0){
    String fullcommand = Serial.readStringUntil('\n');

    if (fullcommand.length() >= 3){

      String spin = fullcommand.substring(0,2);
      int pin = spin.toInt();

      if  (pin != 0){
        String command = fullcommand.substring(2);
        Serial.println(command);
        Serial.println(pin);

        if (command=="open"){
          digitalWrite(pin-1, HIGH);
          digitalWrite(pin,HIGH);
        }
        else if(command=="close"){
          digitalWrite(pin-1, HIGH);
          digitalWrite(pin, LOW);
          
        }
        else if (command=="stop"){
          digitalWrite(pin-1, LOW);
        }

      }

    }
  }

}
