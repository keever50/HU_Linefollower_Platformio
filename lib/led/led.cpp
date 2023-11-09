#include <led.h>
#include <Adafruit_NeoPixel.h>
#include <Arduino.h>

#define LED_PIN         6
#define LED_NUMPIXELS   15
#define LED_DELAY       50
#define LED_DELAY2      5
#define LED_WIDTH       1

Adafruit_NeoPixel pixels(LED_NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

Adafruit_NeoPixel *leds_get_strip()
{
    return &pixels;
}

void leds_update()
{
    
    pixels.begin();
    pixels.setBrightness(50);
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
        //pixels.clear();
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
        //pixels.show();
        i++;

    }

   
}

void leds_show()
{
    pixels.show();
}

void leds_update2()
{  

    pixels.setBrightness(50);
    pixels.begin();
    static long firstPixelHue;
    static unsigned long next_led_update;
    if (millis()>=next_led_update)
    {
        next_led_update = millis()+LED_DELAY2;
        //pixels.clear();
        unsigned char rand_sat = random(210,255);
        unsigned char rand_bri = random(50,200);
        int rand_pix = random(0, LED_NUMPIXELS);
        int rand_hue = random(-5000,5000);
        pixels.setPixelColor(rand_pix,pixels.ColorHSV((firstPixelHue+rand_hue),rand_sat,rand_bri));
        firstPixelHue += 500;

        
    }
    if(firstPixelHue>65536) firstPixelHue=0;
    //pixels.show();

   
}
