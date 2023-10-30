#ifndef TAIL_H
#define TAIL_H

#define TAIL_PIN                      12

#define TAIL_PULSE_MS                 20
#define TAIL_PULSE_WIDTH_CENTER       1.5
#define TAIL_PULSE_WIDTH_MAX_RANGE    0.5
#define TAIL_MIN_ANGLE                -60
#define TAIL_MAX_ANGLE                60

void tail_initialize();
void tail_set_angle(int angle);

#endif