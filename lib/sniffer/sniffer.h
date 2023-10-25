#ifndef SNIFFER_H
#define SNIFFER_H

#define SNIFFER_STARTPIN    1
#define SNIFFER_PINS        5
#define SNIFFER_TRIGGER   200
void sniffer_read(char* sensors);
float sniffer_get_steering(const char* sensors, char* lost);

#endif