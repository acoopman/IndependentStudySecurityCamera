#include "stats.h"
#include <math.h>

float image_mean(uint8_t * in, int count)
{
  float mean = 0;
  for(int i = 0; i < count; i++)
    {
      mean += in[i];
    }
  
  return mean/(float)count;
}

float image_std(uint8_t * in, int count, float mean)
{
  float std = 0;
  for(int i = 0; i < count; i++)
    {
      std += (in[i]-mean)*(in[i]-mean);
    }
  return sqrtf(std/(float)count);
}

void image_histogram(uint8_t * in, int count, int * histo)
{
  int n_bins = 256;
  for(int i = 0; i < n_bins; i++)
    {
      histo[i] = 0;
    }

  for(int i = 0; i < count; i++)
    {
      uint8_t pixel = in[i];
      histo[pixel] += 1;
    }
    
}

