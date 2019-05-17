//security camera 
#include "opencv2/opencv.hpp"
#include <iostream>
#include "detect_motion.h"
#include <stdlib.h>
#include <string.h>
#include "parse_argv.h"
#include "feature_extract.h"
#include "decision.h"
#include "stats.h"

using namespace cv;
using namespace std;


int main(int argc, char *argv[])
{
  video_in_params_t video_params;
  int motion_flag = 0;
  video_params.write_output = 0;
  video_params.read_input = 0;
  video_params.outputfile = "output.avi";
  video_params.video_source = 1;
  
  //lets configure ther background struct 
  motion_detect_params_t detect_params;
  detect_params.update_frequency = 30000; //-1 should never update background
  detect_params.blur_background = 1;
  detect_params.num_of_blurs = 5;
  detect_params.min_threshold_diff = 40;
  detect_params.std_factor = 4;
  detect_params.pixel_percent_threshold = 0.25f;
  detect_params.std_x_thresh = 200;
  detect_params.std_y_thresh = 200;
  detect_params.alpha = .992;
    
  features_t features;

  printf("usage:  ./video_in [-f in.avi] [-w out.avi] [-source (0,1)] [-thresh (0-255)]\n");
  
  parse_argv(argc, argv, &detect_params, &video_params);
  
  VideoCapture cap;
  
  //either read in from file or use the default camera
  if(video_params.read_input == 1)
    cap.open(video_params.inputfile); // open the default camera is 0, usb camera is 1
  else
    cap.open(1); // open the default camera is 0, usb camera is 1
  
  if(!cap.isOpened())  // check if we succeeded
    return -1;
  
  VideoWriter video_out(video_params.outputfile,CV_FOURCC('M','J','P','G'),25, Size(640*2,480*2));
    
  int frame_count = 0;
  Mat frame;
  Mat background_frame;
  Mat gray_frame;
  Mat diff_frame;
  
  //main loop of the program
    while(1)
      {
	//use openCV's VideoCapture class to get an input frame from camera
	cap >> frame; 

	//get the height and width parameters
	int height  = frame.rows;
	int width = frame.cols;

	//covert the input frame to a black and white frame
	cvtColor(frame, gray_frame, CV_BGR2GRAY);

	//blur the input image if the flag is on
	if(detect_params.blur_background == 1)
	  image_blur(gray_frame, detect_params.num_of_blurs);

	//if this is the first frame, set it as the background frame
	if(frame_count == 0)
	  background_frame = gray_frame.clone();
	
	//subtract current frame from the background frame
	subtract_background(diff_frame, gray_frame, background_frame);
	
	//------------------------------------------------------------------------------
	
	extract_features(&features, diff_frame.data, height, width, &detect_params);
	motion_flag = make_decision(&features,  &detect_params);
	
	if(motion_flag)
	  {
	    circle(frame, Point(features.center_x, features.center_y), 10,
		   Scalar(0,255,0), 5);//, int lineType=8, int shift=0)¶

	    Point pt1(features.center_x+features.std_x, features.center_y-features.std_y);
	    Point pt2(features.center_x-features.std_x, features.center_y+features.std_y);

	    rectangle(frame, pt1, pt2, Scalar(0,255,0));	    
	  }

	//------------------------------------------------------------------------------

	update_background(gray_frame, background_frame, &detect_params, motion_flag, frame_count);
	
	//display the frames ----------------------------------------
	Mat top(frame.rows,frame.cols*2,3);
	Mat bottom(frame.rows,frame.cols*2,3);
	Mat final_out(frame.rows*2,frame.cols*2,3);
	Mat back_color;
	Mat diff_color;
	Mat gray_color;
	cv::cvtColor(background_frame, back_color, cv::COLOR_GRAY2BGR);
	cv::cvtColor(gray_frame, gray_color, cv::COLOR_GRAY2BGR);
	cv::cvtColor(diff_frame, diff_color, cv::COLOR_GRAY2BGR);

	//----------------------------------------------------
	std::string text = "Frame = ";
	text += std::to_string(frame_count);	
	//add frame number to video
	cv::putText(frame, 
		    text,
		    cv::Point(50,400), // Coordinates
		    cv::FONT_HERSHEY_COMPLEX_SMALL, // Font
		    1.0, // Scale. 2.0 = 2x bigger
		    cv::Scalar(255,255,255), // BGR Color
		    1); // Line Thickness (Optional)

	//----------------------------------------------------
	std::string blurred_text = "Blurred Gray Input";
	cv::putText(gray_color, 
		    blurred_text,
		    cv::Point(50,400), // Coordinates
		    cv::FONT_HERSHEY_COMPLEX_SMALL, // Font
		    1.0, // Scale. 2.0 = 2x bigger
		    cv::Scalar(255,255,255), // BGR Color
		    1); // Line Thickness (Optional)

	//----------------------------------------------------
	std::string back_text = "Background Frame: alpha = ";
	back_text += std::to_string(detect_params.alpha);	
	//add diff percent to video
	cv::putText(back_color, 
		    back_text,
		    cv::Point(50,400), // Coordinates
		    cv::FONT_HERSHEY_COMPLEX_SMALL, // Font
		    1.0, // Scale. 2.0 = 2x bigger
		    cv::Scalar(255,255,255), // BGR Color
		    1); // Line Thickness (Optional)

	//----------------------------------------------------
	std::string diff_text = "Active Pixels % = ";
	diff_text += std::to_string(features.percent_pixels_changed);	
	//add diff percent to video
	cv::putText(diff_color, 
		    diff_text,
		    cv::Point(50,400), // Coordinates
		    cv::FONT_HERSHEY_COMPLEX_SMALL, // Font
		    1.0, // Scale. 2.0 = 2x bigger
		    cv::Scalar(255,255,255), // BGR Color
		    1); // Line Thickness (Optional)

       
	//--------------------------------------------------

	cv::hconcat(frame, gray_color, top);
	cv::hconcat(diff_color, back_color, bottom);
	cv::vconcat(top, bottom, final_out);
	imshow("SecurityCamera:",final_out);
       	
	// Write the final output
	if(video_params.write_output)
	  video_out.write(final_out);
    
	//press q to quit -------------------------------------------
	int key = waitKey(30);
	  if(key == 'q') //quit	  
	  break;

	if(key == ' ')
	  {
	    cout << "space bar was pressed, make a new background\n";
	    background_frame = gray_frame.clone();
	  }
	
	//update frame counter
	frame_count++;	
    }
    // When everything done, release the video capture and write object
    cap.release();
    video_out.release();

    // Closes all the windows
    destroyAllWindows();
  
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
} 
