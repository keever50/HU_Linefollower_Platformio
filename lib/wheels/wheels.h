#ifndef WHEELS_H
#define WHEELS_H

#define WHEELS_PIN_L                    9
#define WHEELS_PIN_R                    10

#define WHEELS_PULSE_MS                 20
#define WHEELS_PULSE_WIDTH_CENTER       1.5
#define WHEELS_PULSE_WIDTH_MAX_RANGE    0.5
#define WHEELS_PULSE_TRIM               0.001

void wheels_initialize();
void wheels_move(float speed, float steering);

#endif