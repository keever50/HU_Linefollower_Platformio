#include <Arduino.h>
#include <sniffer.h>
#include <wheels.h>
#include <led.h>
#include <Adafruit_NeoPixel.h>

#include <behavior.cpp>

#define LED_MAX_BRIGHTNESS  100



void old_test();

void setup()
{
    
    Serial.begin(250000);
    wheels_initialize();
    
    //strip_pointer = leds_get_strip();
}

void loop()
{
    // char sensors[SNIFFER_PINS];
    // sniffer_read(sensors);
    // leds_update();    
    // debug_show_line(strip_pointer, sensors);
    // leds_show();

    behavior_update(strip_pointer);

}
