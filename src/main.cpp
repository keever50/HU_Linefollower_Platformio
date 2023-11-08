#include <Arduino.h>
#include <sniffer.h>
#include <wheels.h>
#include <led.h>
#include <behavior.cpp>

#define LED_MAX_BRIGHTNESS  100


void setup()
{
    Serial.begin(250000);
    wheels_initialize();
    
    pinMode(7, OUTPUT);
    pinMode(12, OUTPUT);

    pinMode(2,OUTPUT);
    pinMode(3,INPUT);
    //strip_pointer = leds_get_strip();
}

void loop()
{
    behavior_update();
}
