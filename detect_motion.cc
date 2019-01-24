#include "detect_motion.h"


void image_blur(Mat & in, int N)
{
  /*
  //  for ( int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2 )
  for (int i  = 0; i < N; i++)
    medianBlur ( in, in, 9 );
  */
  
      GaussianBlur( in, in, Size( 11, 11 ), 0, 0 );
     
}

//Updating the background, we combine two algorithms. The first one is a copy of the gray frame.
// The issue with that is if someone leaves an object then it will always detect motion.
//To counter this issue i added in a decay, so the object will slowly decay into the background,
//and at that point motion will no longer be detected and the background will update.
//-------------------------------------------------------------------------------
void update_background(Mat & input, Mat & background, motion_detect_params_t * params,
		       int motion_flag, int frame_count)
{

  if(( (frame_count % params->update_frequency) == 0) && (!motion_flag))
    {
      background = input.clone();
      cout << "Background has changed\n";
    }


  //expontial use_exponential_filter
  float alpha = 0.992;  //.99 - 4 seconds .999 - 
    background = alpha*background + (1-alpha)*input;


  
}

//-------------------------------------------------------------------------------
void subtract_background(Mat & diff_frame, Mat & input, Mat background_frame)
{
  diff_frame = abs(input - background_frame);
}
