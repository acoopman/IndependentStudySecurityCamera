//decision.cc  Written by Aidan Coopman 2019
#include "decision.h"

//-------------------------------------------------------------------------------
//features so far
//1) number pixels changed
//2) center of cluster
//3) std of cluster
int make_decision(features_t * features,  motion_detect_params_t * param)
{

  //if the camera moves or a bug lands on the
  //camera lens this gets rid of the false positives
    if(features->std_x > 200.0f)
      return 0;
    if(features->std_y > 200.0f)
      return 0;

    int percent_change = features->percent_pixels_changed;
    int motion_flag = 0;
    
    //if the pixels changed is greater than the set threshold, say there is motion
    if(percent_change > param->pixel_percent_threshold)
      {
	motion_flag = 1;
      }
    else
      {
	motion_flag = 0;
      }
    
    return motion_flag;
    
}
