#include "sniffer.h"
#include <Arduino.h>

void sniffer_read(char* sensors)
{
    for(int i=0;i<SNIFFER_PINS;i++)
    {
        int reading=analogRead(i+SNIFFER_STARTPIN);
        sensors[i]=reading<SNIFFER_TRIGGER;
    }
}

/*
This provides a float, which provides a value between -1 to 1, depending on where we need to steer towards.
It takes 2 pointers. const char* sensors containing IR sensor bits. char* lost where we write the lost(no line) bit to.
*/
float sniffer_get_steering(const char* sensors, char* lost)
{
    *lost = 0;
    int L, R;
    float s;
    for(L=0;L<SNIFFER_PINS;L++)
    {
        if(sensors[L])
        {
            break;
        }
    }    

    for(R=0;R<SNIFFER_PINS;R++)
    {
        if(sensors[SNIFFER_PINS-R-1])
        {
            break;
        }
    }     
  
    if(sensors[SNIFFER_PINS-1]==1 && sensors[0]==0)
    {
        R=0;
        L=4;
    }
    if(sensors[SNIFFER_PINS-1]==0 && sensors[0]==1)
    {
        L=0;
        R=4;
    }

    //When both R and L reached 4, we know no zeroes have been detected, thus no lines. Therefore it is safe to assume we are lost.
    // Serial.println(L);
    // Serial.println(R);
    if(R==5 && L==5)
    {
        *lost = 1;
    }

    s = (float)(L-R)/(float)(SNIFFER_PINS-1);
    return s;

}