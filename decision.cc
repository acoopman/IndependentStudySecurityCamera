#include "decision.h"
//features so far
//1) number pixels changed  TODO want it in percentage
//2) center of cluster
//3) std of cluster


int make_decision(features_t * features,  motion_detect_params_t * param)
{

  //no pixels have changed, return no motion
  if(features->number_pixels_changed == 0)
    return 0;

    //check to make sure the std are widthin the threshold

    if(features->std_x > 100.0f)
      return 0;
    if(features->std_y > 100.0f)
      return 0;

      int pixels_change = features->number_pixels_changed;
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