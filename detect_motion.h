#ifndef DETECT_MOTION_H
#define DETECT_MOTION_H

#include <stdint.h>
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

typedef struct motion_detect_params
{
  int update_frequency;
  int blur_background;
  int use_exponential_filter;
  int num_of_blurs;
  int threshold_pixel_change;
  int pixel_value_threshold;
}  motion_detect_params_t;

typedef struct classification_params
{
  int label;
}classification_params_t;

//returns 1 if there is motion, otherwise 0 for no motion
int detect_motion(uint8_t * in, int height, int width, motion_detect_params_t * param);

void image_blur(Mat & in, int N);
void update_background(Mat & input, Mat & background, motion_detect_params_t * param, int motion_flag, int frame_count);

void subtract_background(Mat & diff_frame, Mat & input, Mat background_frame);

#endif
 
