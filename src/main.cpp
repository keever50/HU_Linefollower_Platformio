#include <Arduino.h>
#include <sniffer.h>
#include <wheels.h>
#include <led.h>
#include <Adafruit_NeoPixel.h>
#include <debug_leds.cpp>

#define LED_MAX_BRIGHTNESS  100

Adafruit_NeoPixel* strip_pointer;

void setup()
{
    Serial.begin(9600);

    
    strip_pointer = leds_get_strip();
}

void loop()
{
    
    leds_update();    
    debug_show_line(strip_pointer);
    leds_show();
}