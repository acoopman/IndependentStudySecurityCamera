#ifndef STATS_H
#define STATS_H

#include <stdint.h>

float image_mean(uint8_t * in, int count);

float image_std(uint8_t * in, int count, float mean);

void image_histogram(uint8_t * in, int count, int * histo);






#endif
