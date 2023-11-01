#ifndef LEDS_H
#define LEDS_H
#include <Adafruit_NeoPixel.h>


Adafruit_NeoPixel *leds_get_strip();

void leds_update();
void leds_show();

#endif