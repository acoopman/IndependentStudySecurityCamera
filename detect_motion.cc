#include "detect_motion.h"


//returns the number of pixels above the threshold
int detect_motion(uint8_t * in, int height, int width, motion_detect_params_t * param)
{
  int count = 0;
  for(int k = 0; k < height; k++) //process height/vertical
    {
      for(int i = 0; i< width; i++) //process columns horizontal
	{
	  if(in[i + width*k  ] > param->pixel_value_threshold)
	    {
	      count++;
	    }
	  
	}
    }
  return count;
}
