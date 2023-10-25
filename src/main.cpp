#include <Arduino.h>
#include <sniffer.h>
#include <wheels.h>
#include <time.h>

//#define MAIN_DEBUG

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

void loop() {
  //Reset terminal position
  Serial.print("\e[0J");
  Serial.print("\e[H");

  Serial.print("uS per cycle: ");
  Serial.println(get_performance());

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


  wheels_move(0.08, steering*1.4);
 // delay(1);
}
