#include <arm.h>
#include <Arduino.h>

void arm_set_angle(int angle)
{
    static unsigned long next;
    if(millis()>=next)
    {
        next=millis()+20;
        float verhouding_graden= 2000/180;
        float microseconds= verhouding_graden*angle +500;
        digitalWrite(12,HIGH);
        delayMicroseconds(microseconds);    
        digitalWrite(12,LOW);
    }

}