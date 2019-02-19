//detect_motion.cc  Written by Aidan Coopman 2019
#include "detect_motion.h"

//-------------------------------------------------------------------------------
//Using standard openCV gaussian blurring with kernel size 11 by 11
void image_blur(Mat & in, int N)
{
  GaussianBlur( in, in, Size( 11, 11 ), 0, 0 );
}

//-------------------------------------------------------------------------------
//Updating the background, we combine two algorithms. The first one is a copy of the gray frame.
// The issue with that is if someone leaves an object then it will always detect motion.
//To counter this issue i added in a decay, so the object will slowly decay into the background,
//and at that point motion will no longer be detected and the background will update.
void update_background(Mat & input, Mat & background, motion_detect_params_t * params,
		       int motion_flag, int frame_count)
{
  if(( (frame_count % params->update_frequency) == 0) && (!motion_flag))
    {
      background = input.clone();
      cout << "Background has changed\n";
    }
  
  //expontial use_exponential_filter
  float alpha = 0.9899;  //.99 - 4 seconds .999 - 
  background = alpha*background + (1-alpha)*input;
}

//-------------------------------------------------------------------------------
//subtracts the blurred image and the background frame 
void subtract_background(Mat & diff_frame, Mat & input, Mat & background_frame)
{
  diff_frame = abs(input - background_frame);
}
