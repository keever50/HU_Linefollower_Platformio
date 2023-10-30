#include <Arduino.h>
#include <wheels.h>

#define ECHO_RESPONSETIME   1000
#define ECHO_TIMEOUT        20000


float echo()
{
    digitalWrite(2,HIGH);
    delayMicroseconds(10);
    digitalWrite(2,LOW);

    
    
    int start = micros();
    int end = 0;
    while(digitalRead(3)==LOW)
    {
        end = micros();
        if(end-start > ECHO_RESPONSETIME)
        {
            return -2;
        }
    };

    

    start = micros();
    end = 0;
    while(digitalRead(3)==HIGH)
    {
        end = micros();
        if(end-start > ECHO_TIMEOUT)
        {
            return -1;
        }        
    };
    float dist=0;
    dist = (end-start)*343.0;


    static unsigned long click_delay=0;
    if(millis()>click_delay)
    {
        click_delay=millis()+((dist/2000000)*50);
        digitalWrite(4,HIGH);
        delay(1);
        digitalWrite(4,LOW);
    }

    return dist/2000000;
    
}

void setup()
{
    Serial.begin(9600);
    pinMode(2,OUTPUT);
    pinMode(3,INPUT);
    pinMode(4,OUTPUT);

    wheels_initialize();
}

float t=0;

char search=0;
void loop()
{


    float d = echo();

    Serial.println(d);

    delay(1);
}