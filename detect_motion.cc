#include "detect_motion.h"


void image_blur(Mat & in, int N)
{
  //  for ( int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2 )
  for (int i  = 0; i < N; i++)
    medianBlur ( in, in, 9 );
}

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

