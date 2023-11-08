#include <Arduino.h>
#include <sniffer.h>
#include <wheels.h>
#include <led.h>
#include <Adafruit_NeoPixel.h>
#include <Servo.h>
#include <behavior.cpp>

#define LED_MAX_BRIGHTNESS  100

Servo arm;

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

char siren_tone = 0;
char siren_half = 0;
unsigned long delta = 0;
void loop()
{


    
    unsigned long delta_start;
    delta_start = micros();

    behavior_update();

    delta = micros()-delta_start;



}
