#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#include <string>

using namespace std; //need this for string

typedef struct motion_detect_params
{
  int update_frequency;
  int blur_background;
  int use_exponential_filter;
  int num_of_blurs;
  int min_threshold_diff; //if the diff frame is greater than this, increase count
  int std_factor;
  float pixel_percent_threshold; //this is a threshold to determine if there is motion
  float std_x_thresh;
  float std_y_thresh;
  float alpha;
}  motion_detect_params_t;

typedef struct classification_params
{
  int label;
}classification_params_t;

typedef struct video_in_params
{
  int write_output;
  int read_input;
  string outputfile;
  string inputfile;
  int video_source;

}video_in_params_t;

typedef struct features
{
  int center_x;
  int center_y;
  float std_x;
  float std_y;
  float percent_pixels_changed;

}features_t;






#endif
