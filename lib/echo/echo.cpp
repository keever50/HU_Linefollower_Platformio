#include <Arduino.h>

const int echo_trigger = 2;
const int echo_pin = 3;

double echo() 
{
    
    digitalWrite(echo_trigger, HIGH);  
    delayMicroseconds(10);                      
    digitalWrite(echo_trigger, LOW);   
    
    while(!digitalRead(echo_pin)){}
    unsigned long start = micros();
    while(digitalRead(echo_pin)){}
    unsigned long delta = micros()-start;
    
    double milisec = delta/1000.0;
    double afstand=milisec*17.15;
    
    
    return afstand;
  


}




