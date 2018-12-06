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
  //figure out how many pixels are greater than the threshold
  int count = 0;
  int sum_x = 0;
  int sum_y = 0;
  
  for(int y = 0; y < height; y++) //process height/vertical
    {
      for(int x = 0; x< width; x++) //process columns horizontal
	{
	  if(in[x + width*y  ] > param->pixel_value_threshold)
	    {
	      sum_x += x;
	      sum_y += y;
	      count++;
	    }
	  
	}
    }
  if(count != 0)
    {
      param->center_x = (sum_x/count);
      param->center_y = (sum_y/count);
    }
  //-----------------------------------------

  int pixels_change = count;
  int motion_flag = 0;
  
  //if the pixels changed is greater than the set threshold, say there is motion
  if(pixels_change > param->threshold_pixel_change)
    {
      motion_flag = 1;
    }
  else
    {
      motion_flag = 0;
    }



  
  return motion_flag;
}

//-------------------------------------------------------------------------------
void update_background(Mat & input, Mat & background, motion_detect_params_t * params,
		       int motion_flag, int frame_count)
{
  
  if(( (frame_count % params->update_frequency) == 0) && (!motion_flag))
    {
      background = input.clone();
      cout << "Background has changed\n";
    }
  
	
}

//-------------------------------------------------------------------------------
void subtract_background(Mat & diff_frame, Mat & input, Mat background_frame)
{
  diff_frame = abs(input - background_frame);
}
