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
    //strip_pointer = leds_get_strip();
}

char siren_tone = 0;
char siren_half = 0;
unsigned long delta = 0;
void loop()
{
    static unsigned long Th;
    static unsigned long next_angle;
    if(millis()>=next_angle)
    {
        static char toggle;
        toggle=!toggle;
        next_angle = millis()+2000;
        Th=500+(2000*toggle);
    }

    static unsigned long next_servo;
    if(millis()>=next_servo)
    {
        next_servo = millis()+20;
        digitalWrite(12,HIGH);
        delayMicroseconds(Th);
        digitalWrite(12,LOW);
    }

    
    unsigned long delta_start;
    delta_start = micros();

    behavior_update();

    delta = micros()-delta_start;



}
