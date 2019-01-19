//security camera 
#include "opencv2/opencv.hpp"
#include <iostream>
#include "detect_motion.h"
#include <stdlib.h>
#include <string.h>
#include "parse_argv.h"
#include "feature_extract.h"
#include "decision.h"

using namespace cv;
using namespace std;

//to learn video in and out with opencv use this link:
//https://www.learnopencv.com/read-write-and-display-a-video-using-opencv-cpp-python/

//cap.set(cv2.CAP_PROP_AUTOFOCUS, 0) // turn the autofocus off

int main(int argc, char *argv[])
{
  video_in_params_t video_params;
  int motion_flag = 0;
  video_params.write_output = 0;
  video_params.outputfile = "output.avi";
  video_params.video_source = 1;
  
  //lets configure ther background struct 
  motion_detect_params_t detect_params;
  detect_params.update_frequency = 3000; //-1 should never update background
  detect_params.blur_background = 1;
  detect_params.num_of_blurs = 5;
  detect_params.threshold_pixel_diff = 25;
  detect_params.pixel_percent_threshold = 0.1f;
    
  features_t features;

  printf("argc = %i\n", argc);

  printf("usage:  ./video_in [-w foo.avi] [-source (0,1)] [-thresh (0-255)]\n");
  
  parse_argv(argc, argv, &detect_params, &video_params);

  //    VideoCapture cap(1); // open the default camera is 0, usb camera is 1
    VideoCapture cap(video_params.video_source); // open the default camera is 0, usb camera is 1
    if(!cap.isOpened())  // check if we succeeded
      return -1;

    //    if(write_output)
      VideoWriter video_out(video_params.outputfile,CV_FOURCC('M','J','P','G'),10, Size(640,480));
    
    
    //make an output window up
    namedWindow("frame",1);
    namedWindow("grayframe",1);
    namedWindow("background_frame",1);
    namedWindow("diff",1);
    
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

	//cout << "Image size is (width, height)" << width << " " << height << endl;
	
	//covert the input frame to a black and white frame
	cvtColor(frame, gray_frame, CV_BGR2GRAY);

	//blur the input image if the flag is on
	if(detect_params.blur_background == 1)
	  image_blur(gray_frame, detect_params.num_of_blurs);

	//if this is the first frame, set it as the background frame
	if(frame_count == 0)
	  background_frame = gray_frame.clone();
	
	//cout << "frame count = " << frame_count << endl;

	//subtract current frame from the background frame
	subtract_background(diff_frame, gray_frame, background_frame);

	
	//------------------------------------------------------------------------------
	
	//returns the number of pixels that are greater than the threshold

	//motion_flag = detect_motion(diff_frame.data, height, width, &detect_params);

	
	extract_features(&features, diff_frame.data, height, width, &detect_params);

	motion_flag = make_decision(&features,  &detect_params);

	cout << "Percent pixel changed = " <<  features.percent_pixels_changed
	     << " threshold = " <<  detect_params.threshold_pixel_diff << endl;
	cout << "std_x = " << features.std_x  << " std_y = " << features.std_y << endl;
	fflush(stdout);
	
	if(motion_flag)
	  {

	    cout << "Motion detected \n";
	    cout << "center_x = " << features.center_x  << " center_y = " << features.center_y << endl;
	    fflush(stdout);
	    circle(frame, Point(features.center_x, features.center_y), 10,
		   Scalar(0,255,0), 5);//, int lineType=8, int shift=0)¶


	    /*  
	    int radius = ((detect_params.std_x+detect_params.std_y)/2);
	    if(detect_params.center_x+radius > width)
	      {
		radius = width-detect_params.center_x -1;
	      }
	    if(detect_params.center_y+radius > height)
	      {
		radius = height-detect_params.center_y -1;
	      }
	    	    circle(frame, Point(detect_params.center_x, detect_params.center_y), radius,
	    	   Scalar(0,255,0), 5);//, int lineType=8, int shift=0)¶
	    */
	    Point pt1(features.center_x+features.std_x, features.center_y-features.std_y);
	    Point pt2(features.center_x-features.std_x, features.center_y+features.std_y);

	    //  cout << pt1.x << " " << pt1.y <<endl;
	    
	    rectangle(frame, pt1, pt2, Scalar(0,255,0));//, int lineType=8, int shift=0)¶)//, int thickness=1, int lineType=LINE_8, int shift=0 )coopman


	    
	  }

	//------------------------------------------------------------------------------
	//update background frame every 30 frames, and if theres no motion
	update_background(gray_frame, background_frame, &detect_params, motion_flag, frame_count);


	//display the frames ----------------------------------------
	imshow("frame",frame);
	imshow("grayframe",gray_frame);
	imshow("background_frame", background_frame);
	imshow("diff",10*diff_frame);  
       	
	// Write the frame into the file 'outcpp.avi'
	if(video_params.write_output)
	  video_out.write(frame);
    
	//press q to quit -------------------------------------------
	int key = waitKey(30);
	//	if(key == 113) //quit
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
