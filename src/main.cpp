#include <Arduino.h>
#include <sniffer.h>
#include <wheels.h>
#include <led.h>
#include <Adafruit_NeoPixel.h>

#include <behavior.cpp>

#define LED_MAX_BRIGHTNESS  100



void setup()
{
    
    Serial.begin(250000);
    wheels_initialize();
    
    pinMode(7, OUTPUT);

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

    //SIREN//

    // static unsigned long siren_switch;
    // if(millis()>=siren_switch)
    // {
    //     siren_switch=millis()+750;
    //     siren_tone=!siren_tone;
    // }

    // static unsigned long siren_delay;
    // if(micros()>=siren_delay)
    // {
    //     switch(siren_tone)
    //     {
    //         case 0:
    //         {
                
    //             digitalWrite(7,HIGH);
                
    //             digitalWrite(7,LOW);
    //             break;
    //         }
    //         case 1:
    //         {
    //             siren_half=!siren_half;
    //             if(siren_half)
    //             {
                    
    //                 digitalWrite(7,HIGH);
                    
    //                 digitalWrite(7,LOW);
    //             }

    //             break;
    //         }
    //     }
    //     siren_delay=micros()+1000-(delta/1000);
    // }


}
