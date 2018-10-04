#include "detect_motion.h"


//returns the number of pixels above the threshold
int detect_motion(uint8_t * in, int height, int width, uint8_t threshold)
{
  int count = 0;
  for(int k = 0; k < height; k++) //process height/vertical
    {
      for(int i = 0; i< width; i++) //process columns horizontal
	{
	  if(in[i + width*k  ] > threshold)
	    {
	      count++;
	    }
	  
	}
    }
  return count;
}
