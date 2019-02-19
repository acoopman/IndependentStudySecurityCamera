//detect_motion.h  Written by Aidan Coopman 2019
#ifndef DETECT_MOTION_H
#define DETECT_MOTION_H

#include <stdint.h>
#include "opencv2/opencv.hpp"
#include "typedefs.h"

using namespace cv;
using namespace std;

//overwrites the input and uses an 11x11 gaussian kernal from openCV 
void image_blur(Mat & in, int N);

//Updating the background, we combine two algorithms.
//The first one is a copy of the gray frame.
// The issue with that is if someone leaves an object then it
//will always detect motion.
//To counter this issue i added in a decay, so the object will
//slowly decay into the background,
//and at that point motion will no longer be detected and the
//background will update.
void update_background(Mat & input, Mat & background,
		       motion_detect_params_t * param,
		       int motion_flag,int frame_count);

//subtracts the blurred image and the background frame 
void subtract_background(Mat & diff_frame, Mat & input, Mat & background_frame);

#endif
 
