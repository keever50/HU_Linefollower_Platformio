#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
#include <sniffer.h>
#include <sniffer.h>
#include <wheels.h>
#include <led.h>
#include <debug_leds.cpp>

#define BEHAVE_TRACK_MEMORY_SIZE    300

#define BEHAVE_FOLLOW_LINE          0
#define BEHAVE_LOST                 1
#define BEHAVE_OBSTACLE             2
#define BEHAVE_STOP                 3
#define BEHAVE_RECOVERY             4

unsigned char behavior_track_memory[BEHAVE_TRACK_MEMORY_SIZE];
Adafruit_NeoPixel* strip_pointer;

void behavior_print_sensors(const char* sensors)
{
    Serial.print("[");
    for(int i=0;i<SNIFFER_PINS;i++)
    {
        if(sensors[i])
        {
            Serial.print("#");
        }else{
            Serial.print("-");
        }
    }
    Serial.print("]");
}

void behavior_print_bits(unsigned char bits)
{
    unsigned char bits_print=bits;
    for(int i=0;i<SNIFFER_PINS;i++)
    {
        unsigned char bit = bits_print & 0x01; //mask
        bits_print=bits_print>>1;
        if(bit) 
        {
            Serial.print("1");
        }else{
            Serial.print("0");
        }      
    }
    Serial.println("");  
}

void behavior_set_track_memory(int addr, const char* sensors)
{
    unsigned char sensor_bits=0;
    for(int i=0;i<SNIFFER_PINS;i++)
    {
        char sensor = sensors[i];
        sensor_bits = sensor_bits<<1;
        sensor_bits = sensor_bits | (sensor>0);
    }
    behavior_track_memory[addr]=sensor_bits;

}

void behavior_get_track_memory(int addr, char* sensors)
{
    unsigned char sensor_bits = behavior_track_memory[addr];

    for(int i=0;i<SNIFFER_PINS;i++)
    {
        sensors[i]=sensor_bits&0x01;
        sensor_bits=sensor_bits>>1;    
    }
}

void behavior_update(Adafruit_NeoPixel* strip_pointer)
{
    static int state;
    static char lost;
    static int ticks;
    static char recovery_direction;
    strip_pointer = leds_get_strip();

    switch(state)
    {
        case BEHAVE_FOLLOW_LINE: /*Default line-following behavior*/
        {
            //Sensor update
            char sensors[SNIFFER_PINS];
            sniffer_read(sensors);
            behavior_set_track_memory(ticks%BEHAVE_TRACK_MEMORY_SIZE,sensors);
            ticks++;

            leds_update();

            //debug
            debug_show_line(strip_pointer, sensors);
            leds_show();

            //Apply steering
            float steering = sniffer_get_steering(sensors, &lost);

            //Lost track
            if(lost)
            {
                state=BEHAVE_LOST;
                wheels_move(0, 0);
                break;
            }
            wheels_move(0.2, steering);
            break;
        }

        case BEHAVE_LOST: /*Lost behavior*/
        {
            Serial.println("START");
            for(int i=0;i<BEHAVE_TRACK_MEMORY_SIZE;i++)
            {
                char sensors[SNIFFER_PINS];
                behavior_get_track_memory((ticks-BEHAVE_TRACK_MEMORY_SIZE+i)%BEHAVE_TRACK_MEMORY_SIZE,sensors);
                behavior_print_sensors(sensors);
                if(sensors[0]==1 && sensors[SNIFFER_PINS-1]==0 && recovery_direction==0)
                {
                    Serial.print("<-MARK L");
                    recovery_direction=1;
                }
                if(sensors[0]==0 && sensors[SNIFFER_PINS-1]==1 && recovery_direction==0)
                {
                    Serial.print("<-MARK R");
                    recovery_direction=-1;
                }  
                Serial.println("");              
            }
            Serial.println("END");
            state = BEHAVE_RECOVERY;
            break;
        }

        case BEHAVE_RECOVERY:
        {
            //Sensor update
            char sensors[SNIFFER_PINS];
            sniffer_read(sensors);  
            float steering = sniffer_get_steering(sensors, &lost);
            if(!lost){
                wheels_move(0, 0);  
                break;
            }
            wheels_move(0.1, recovery_direction);          
            break;
        }        
        

    }
}