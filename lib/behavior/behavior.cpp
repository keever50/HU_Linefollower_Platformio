#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
#include <sniffer.h>
#include <sniffer.h>
#include <wheels.h>
#include <led.h>
#include <debug_leds.cpp>
#include <Ultrasonic.h>
#include <arm.h>

#define BEHAVE_DEBUG_RECOVERY

#define BEHAVE_SPEED                    0.3
#define BEHAVE_STEERING_MULTIPLIER      1.2

#define BEHAVE_TRACK_MEMORY_SIZE        300
#define BEHAVE_POST_RECOVERY_TIME_MS    500
#define BEHAVE_RECOVERY_TIMEOUT_FLIP    1000    

#define BEHAVE_OBSTACLE_START_CM        15
#define BEHAVE_ECHO_INTERVAL_MS         100

#define BEHAVE_FOLLOW_LINE          0
#define BEHAVE_LOST                 1
#define BEHAVE_OBSTACLE             2
#define BEHAVE_STOP                 3
#define BEHAVE_RECOVERY             4
#define BEHAVE_OBSTACLE_REMOVAL     5
#define BEHAVE_OBSTACLE_REMOVAL_RES 6


unsigned char behavior_track_memory[BEHAVE_TRACK_MEMORY_SIZE+1];
Adafruit_NeoPixel* strip_pointer;
Ultrasonic obstacle_sensor(2,3,10000);

/*
    This helps printing the line tracking sensor detections.
*/
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

/*
    This will print the binary bits of the input
*/
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

/*
    This will store a sensor array as one unsigned char of bits at addr in memory.
    This is more compact than storing one char per bit, per memory location.
    Because we are unsigned chars, there is only space for 8 sensors per location.
*/
void behavior_set_track_memory(unsigned long addr, const char* sensors)
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

/*
    This recalls bits from memory and puts them back into the sensor array.
*/
void behavior_get_track_memory(unsigned long addr, char* sensors)
{
    unsigned char sensor_bits = behavior_track_memory[addr];

    for(int i=0;i<SNIFFER_PINS;i++)
    {
        sensors[i]=sensor_bits&0x01;
        sensor_bits=sensor_bits>>1;    
    }
}

void behavior_update( )
{
    static int state;
    static char lost;
    static unsigned long ticks;
    static char recovery_direction;
    static unsigned long next_memory;
    static unsigned long recovery_timerout_next;
    static unsigned long next_distance;

    Adafruit_NeoPixel* strip_pointer = leds_get_strip();

    switch(state)
    {
        case BEHAVE_FOLLOW_LINE: /*Default line-following behavior*/
        {
            //state = BEHAVE_OBSTACLE; break;

            //Distance update//
            if(millis()>=next_distance)
            {
                next_distance=millis()+BEHAVE_ECHO_INTERVAL_MS;
                unsigned int dist = obstacle_sensor.read(); 
                if(dist<=BEHAVE_OBSTACLE_START_CM)
                {
                    state = BEHAVE_OBSTACLE;
                    break;
                }
            }


            //Sensor update//
            char sensors[SNIFFER_PINS];
            sniffer_read(sensors);

            //Memory//
            if(millis()>=next_memory) /*Some behaviors can delay track recording*/
            {
                unsigned long address = ticks%BEHAVE_TRACK_MEMORY_SIZE;
                behavior_set_track_memory(address,sensors); /*Records previous detections in memory as bits*/
                ticks++;                
            }

            //Visuals//
            leds_update();
            debug_show_line(strip_pointer, sensors);
            leds_show();

            //Calculate steering//
            float steering = sniffer_get_steering(sensors, &lost);

            //Lost track//
            if(lost)
            {
                state=BEHAVE_LOST;/*Change behavior by changing switch case state*/
                wheels_move(0, 0);
                break;
            }

            //Track is still there//
            wheels_move(BEHAVE_SPEED, steering*BEHAVE_STEERING_MULTIPLIER);
            break;
        }

        /*
            In case we lose the line we are tracking, this behavior will recall our previous recorded sensor outputs.
            This way we can find out where and how we lost track.
        */
        case BEHAVE_LOST: /*Lost behavior*/
        {
            recovery_direction=0;
            #ifdef BEHAVE_DEBUG_RECOVERY
                Serial.println("START");
            #endif

            for(int i=0;i<BEHAVE_TRACK_MEMORY_SIZE;i++)
            {
                char sensors[SNIFFER_PINS];
                behavior_get_track_memory((ticks-BEHAVE_TRACK_MEMORY_SIZE+i)%BEHAVE_TRACK_MEMORY_SIZE,sensors);

                #ifdef BEHAVE_DEBUG_RECOVERY
                    behavior_print_sensors(sensors);
                #endif

                if(sensors[0]==1 && sensors[SNIFFER_PINS-1]==0 && recovery_direction==0)
                {
                    #ifdef BEHAVE_DEBUG_RECOVERY
                        Serial.print("<-MARK L"); 
                    #endif
                    recovery_direction=1;
                }
                if(sensors[0]==0 && sensors[SNIFFER_PINS-1]==1 && recovery_direction==0)
                {
                    #ifdef BEHAVE_DEBUG_RECOVERY
                        Serial.print("<-MARK R"); 
                    #endif
                    recovery_direction=-1;
                }  

                #ifdef BEHAVE_DEBUG_RECOVERY
                    Serial.println("");  
                #endif            
            }
            #ifdef BEHAVE_DEBUG_RECOVERY
                Serial.println("END");  
            #endif       

            //Clear track memory
            for(int i=0;i<BEHAVE_TRACK_MEMORY_SIZE;i++)
            {
                behavior_track_memory[i]=0;
            }

            recovery_timerout_next = millis()+BEHAVE_RECOVERY_TIMEOUT_FLIP;
            state = BEHAVE_RECOVERY;
            //break;
            //Continue down
        }

        /*
            This is the behavior that will apply the calculated move from BEHAVIOR_LOST till we found the line back.
        */
        case BEHAVE_RECOVERY:
        {
            //Sensor update
            char sensors[SNIFFER_PINS];
            sniffer_read(sensors);  
            sniffer_get_steering(sensors, &lost);
            if(!lost){
                wheels_move(0, 0);  
                state = BEHAVE_FOLLOW_LINE;
                next_memory=millis()+BEHAVE_POST_RECOVERY_TIME_MS;
                break;
            }
            if(recovery_direction==0){
                recovery_direction=1;
            }

            //Timeout flip
            if(millis()>=recovery_timerout_next)
            {
                recovery_direction=-recovery_direction;
                recovery_timerout_next = recovery_timerout_next + BEHAVE_RECOVERY_TIMEOUT_FLIP*2;
            }

            wheels_move(0.1, recovery_direction);          
            break;
        }        
        
        static unsigned long obstacle_removal_time_next;
        case BEHAVE_OBSTACLE:
        {
            wheels_move(0,0);
            obstacle_removal_time_next=millis()+1000;

            state = BEHAVE_OBSTACLE_REMOVAL;
        }

        case BEHAVE_OBSTACLE_REMOVAL:
        {
            //Move arm OUT
            arm_set_angle(180);
        

            wheels_move(0.3,1);

            if(millis()>=obstacle_removal_time_next)
            {
                state = BEHAVE_OBSTACLE_REMOVAL_RES;
                obstacle_removal_time_next=obstacle_removal_time_next+1000;
            }

            break;
        }

        case BEHAVE_OBSTACLE_REMOVAL_RES:
        {
            //Move arm IN
            arm_set_angle(0);

            wheels_move(0.3,-1);

            if(millis()>=obstacle_removal_time_next)
            {
                state = BEHAVE_FOLLOW_LINE;
            }
           
        }

    }
}