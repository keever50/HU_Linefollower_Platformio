#include <led.h>
#include <Adafruit_NeoPixel.h>
#include <Arduino.h>

#define LED_PIN         6
#define LED_NUMPIXELS   8
#define LED_DELAY       40
#define LED_WIDTH       1

Adafruit_NeoPixel pixels(LED_NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

void leds_update()
{
    
    pixels.begin();
    pixels.setBrightness(20);
    unsigned long time=millis();
    static unsigned long next_led_update;
    if (time>=next_led_update)
    {
        next_led_update = time+LED_DELAY;
        static unsigned char i,h=0;
        if(i>=LED_NUMPIXELS-(h%2)) 
        {
            h++;
            i=h%2;
        }
        pixels.clear();
        if(h%2)
        {
            pixels.setPixelColor(LED_NUMPIXELS-i+1, pixels.Color(0, 0, 0));
            pixels.setPixelColor(i-2, pixels.Color(0, 0, 0));

            pixels.setPixelColor(LED_NUMPIXELS-i-2, pixels.Color(50, 0, 0));
            pixels.setPixelColor(i-1, pixels.Color(0, 0, 50)); 
            pixels.setPixelColor(LED_NUMPIXELS-i, pixels.Color(50, 0, 0));
            pixels.setPixelColor(i+1, pixels.Color(0, 0, 50));

            pixels.setPixelColor(LED_NUMPIXELS-i-1, pixels.Color(255, 0, 0));
            pixels.setPixelColor(i, pixels.Color(0, 0, 255));
        }
        else
        {
            pixels.setPixelColor(LED_NUMPIXELS-i+1, pixels.Color(0, 0, 0));
            pixels.setPixelColor(i-2, pixels.Color(0, 0, 0));

    
            pixels.setPixelColor(LED_NUMPIXELS-i-2, pixels.Color(0, 0, 50));
            pixels.setPixelColor(i-1, pixels.Color(50, 0, 0)); 
            pixels.setPixelColor(LED_NUMPIXELS-i, pixels.Color(0, 0, 50));
            pixels.setPixelColor(i+1, pixels.Color(50, 0, 0));

            pixels.setPixelColor(LED_NUMPIXELS-i-1, pixels.Color(0, 0, 255));
            pixels.setPixelColor(i, pixels.Color(255, 0, 0));    
        }
        pixels.show();
        i++;

    }

   
}




