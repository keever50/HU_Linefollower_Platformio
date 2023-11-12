//geschreven door Milan van belkom
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

void leds_update()                              //sirene effect, rode en blauwe pixels gaan van links naar rechts
{
    pixels.begin();
    pixels.setBrightness(50);
    unsigned long time=millis();
    static unsigned long next_led_update;
    if (time>=next_led_update)                  //delay zonder gebruik te maken van de arduino delay
    {
        next_led_update = time+LED_DELAY; 
        static unsigned char i,h=0;
        if(i>=LED_NUMPIXELS-(h%2))              //zorgt ervoor dat de in de sirene rood eerst van links naar rechts
        {                                       //en dan van rechts naar links gaat
            h++;    
            i=h%2;
        }
        if(h%2)                                 //blauw begint op positie 1 en loopt tot de een na laaste
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
        else                                    //rood begint op positie 0 en loopt tot de laatste positie.
        {                                       //positie van rood tov i is hier gespiegeld
            pixels.setPixelColor(LED_NUMPIXELS-i+1, pixels.Color(0, 0, 0));
            pixels.setPixelColor(i-2, pixels.Color(0, 0, 0));

    
            pixels.setPixelColor(LED_NUMPIXELS-i-2, pixels.Color(0, 0, 50));
            pixels.setPixelColor(i-1, pixels.Color(50, 0, 0)); 
            pixels.setPixelColor(LED_NUMPIXELS-i, pixels.Color(0, 0, 50));
            pixels.setPixelColor(i+1, pixels.Color(50, 0, 0));

            pixels.setPixelColor(LED_NUMPIXELS-i-1, pixels.Color(0, 0, 255));
            pixels.setPixelColor(i, pixels.Color(255, 0, 0));    
        }
        i++;
    }

   
}

void leds_show()                                //omdat er meerdere functies zijn die de leds gebruiken worden ze pas
{                                               //na al die functies getoond
    pixels.show();
}

void leds_update2()                             //willekeurige leds krijgen steeds een andere kleur die cyclisch
{                                               //rond het kleurenspectrum loopt
    pixels.setBrightness(50);
    pixels.begin();
    static long firstPixelHue;
    static unsigned long next_led_update;
    if (millis()>=next_led_update)              //zelfde delay oplossing als eerder
    {
        next_led_update = millis()+LED_DELAY2;      
        unsigned char rand_sat = random(210,255);   //willekeurige saturation waarde
        unsigned char rand_bri = random(50,200);    //willekeurige brightness waarde
        int rand_pix = random(0, LED_NUMPIXELS);    //willekeurige pixel
        int rand_hue = random(-5000,5000);          //willekeurige hue waarde
        pixels.setPixelColor(rand_pix,pixels.ColorHSV((firstPixelHue+rand_hue),rand_sat,rand_bri)); //zet de eerder bepaalde waarde in pixel
        firstPixelHue += 500;                       //loopt cyclisch door het kleurspectrum
    }
    if(firstPixelHue>65536) firstPixelHue=0;       
}
