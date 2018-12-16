//security camera 
#include "opencv2/opencv.hpp"
#include <iostream>
#include "detect_motion.h"

using namespace cv;
using namespace std;

//to learn video in and out with opencv use this link:
//https://www.learnopencv.com/read-write-and-display-a-video-using-opencv-cpp-python/

int main(int, char**)
{

  int motion_flag = 0;
  
  //    VideoCapture cap(1); // open the default camera is 0, usb camera is 1
    VideoCapture cap(1); // open the default camera is 0, usb camera is 1
    if(!cap.isOpened())  // check if we succeeded
      return -1;

    VideoWriter video("outcpp.avi",CV_FOURCC('M','J','P','G'),10, Size(640,480));
    
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

    //lets configure ther background struct 
    motion_detect_params_t detect_params;
    detect_params.update_frequency = 30;
    detect_params.blur_background = 1;
    detect_params.num_of_blurs = 5;
    detect_params.threshold_pixel_change = 100;
    detect_params.pixel_value_threshold = 75;

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

	motion_flag = detect_motion(diff_frame.data, height, width, &detect_params);

	cout << "Pixel changes = " <<  detect_params.number_pixels_changed << " threshold = " <<  detect_params.threshold_pixel_change << endl;
	cout << "std_x = " << detect_params.std_x  << " std_y = " << detect_params.std_y << endl;
	
	if(motion_flag)
	  {
	    cout << "Motion detected \n";
	    cout << "center_x = " << detect_params.center_x  << "center_y = " << detect_params.center_y << endl;
	    circle(frame, Point(detect_params.center_x, detect_params.center_y), 10,
		   Scalar(0,255,0), 5);//, int lineType=8, int shift=0)¶
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
	video.write(frame);
    
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
    video.release();

    // Closes all the windows
    destroyAllWindows();
  
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
} 
