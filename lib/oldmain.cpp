#include <Arduino.h>
#include <sniffer.h>
#include <wheels.h>
#include <time.h>
#include <led.h>

//#define MAIN_DEBUG
#define RECOVERY_STEPS  200
unsigned long get_performance()
{
  static unsigned long previous = 0;
  unsigned long now = micros();
  unsigned long lasted = now - previous;
  previous = now;
  return lasted;
}

void setup() {
  Serial.begin(250000);
  wheels_initialize();

  
}

float last_steerings[RECOVERY_STEPS];
int tick;
char recovery=0;
float recover_steering=0;
void loop() {

  //Reset terminal position
  //Serial.print("\e[0J");
  //Serial.print("\e[H");

  //Serial.print("uS per cycle: ");
  //Serial.println(get_performance());

  char IR_sensors[SNIFFER_PINS];
  sniffer_read(IR_sensors);

  #ifdef MAIN_DEBUG
    Serial.print("[");
    for(int i=0;i<SNIFFER_PINS;i++)
    {
      if(IR_sensors[i])
      {
        Serial.print(" # ");
      }else{
        Serial.print("   ");
      }
    }
    Serial.print("] ");
  #endif

  char lost;
  float steering = sniffer_get_steering(IR_sensors, &lost);
  
  #ifdef MAIN_DEBUG
    if(lost)
    {
      Serial.print("  LOST  \n\r");
    }else
    {
      Serial.print("TRACKING\n\r");
    }
    Serial.print("Steering: ");
    Serial.println(steering);
  #endif 

  if(!lost)
  {
    wheels_move(0.1, steering*1);
    last_steerings[tick%RECOVERY_STEPS]=steering;
    tick=tick+1;
    recovery=0;
  }else{

    if(recovery==0)
    {
      
      for(int i=RECOVERY_STEPS-2;i>=0;i--)
      {
        float s=last_steerings[(tick-i)%RECOVERY_STEPS];  
        if(s>=1)
        {
          recover_steering=1;
          break;  
        }
        if(s<=-1)
        {
          recover_steering=-1;
          break;  
        }        
        //recover_steering=recover_steering+s;
      }
      //recover_steering=recover_steering/RECOVERY_STEPS;
      recovery=1;
    }

    wheels_move(0.1, recover_steering);
  }
  
  leds_update();
  
  delay(30);
}
