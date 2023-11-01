#include <wheels.h>
#include <TimerOne.h>
#include <Arduino.h>

void wheels_initialize()
{
    Timer1.initialize(WHEELS_PULSE_MS*1000);
    Timer1.pwm(WHEELS_PIN_L, 1024*(WHEELS_PULSE_TRIM+WHEELS_PULSE_WIDTH_CENTER/WHEELS_PULSE_MS)); 
    Timer1.pwm(WHEELS_PIN_R, 1024*(WHEELS_PULSE_TRIM+WHEELS_PULSE_WIDTH_CENTER/WHEELS_PULSE_MS)); 
}

void wheels_move(float speed, float steering)
{
    float L, R;
    float V = speed;
    if(steering>=0)
    {
        L=V; 
        R=V-2*steering*V;
    }else{
        L=V-2*(-steering)*V; 
        R=V;
    }
    
    // if(speed==0){
    //     Timer1.pwm(WHEELS_PIN_L, 0); 
    //     Timer1.pwm(WHEELS_PIN_R, 0);     
    //     return; 
    // }
    
    if(L>1){L=1;}
    if(L<-1){L=-1;}
    if(R>1){R=1;}
    if(R<-1){R=-1;}   

    // Serial.println(L);
    // Serial.println(R);

    float steering_out_L = WHEELS_PULSE_WIDTH_CENTER-(WHEELS_PULSE_WIDTH_MAX_RANGE*L);
    float steering_out_R = WHEELS_PULSE_WIDTH_CENTER+(WHEELS_PULSE_WIDTH_MAX_RANGE*R);
    // Serial.println(steering_out_L);
    // Serial.println(steering_out_R);
    Timer1.pwm(WHEELS_PIN_L, 1024*((steering_out_L)/WHEELS_PULSE_MS)); 
    Timer1.pwm(WHEELS_PIN_R, 1024*((steering_out_R)/WHEELS_PULSE_MS));      
}