#include <Adafruit_NeoPixel.h>
#include <Arduino.h>

void debug_show_line(Adafruit_NeoPixel* pixels)
{
    pixels->setPixelColor(4,pixels->Color(0,255,255));
    
}