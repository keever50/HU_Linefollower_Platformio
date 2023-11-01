#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
#include <sniffer.h>

void debug_show_line(Adafruit_NeoPixel* pixels, char* sensors)
{
    
    for(int i=0;i<SNIFFER_PINS;i++)
    {
        char state = sensors[SNIFFER_PINS-i-1];
        pixels->setPixelColor(1+i*3,pixels->Color(255*state,255*state,255*state));
    }   
}