//stats.h  Written by Aidan Coopman 2019
#ifndef STATS_H
#define STATS_H

#include <stdint.h>

//------------------------------------------------------------------------------
//gets the mean of the image
float image_mean(uint8_t * in, int count);

//------------------------------------------------------------------------------
//gets the std of the image
float image_std(uint8_t * in, int count, float mean);

//------------------------------------------------------------------------------
//creates the histogram for pixel values
void image_histogram(uint8_t * in, int count, int * histo);

//------------------------------------------------------------------------------
//plots the histogram with the data from the image
void plot_histogram(int * hist, int bins);

#endif
