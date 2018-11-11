//security camera 
#include "opencv2/opencv.hpp"
#include <iostream>
#include "detect_motion.h"

using namespace cv;
using namespace std;

//img.at<array<uint8_t,3>>(0,0);
/*
class Mat
{
public:
    // ... a lot of methods ...
    ...

    //! includes several bit-fields:
    //     - the magic signature
    //     - continuity flag
    //     - depth
    //     - number of channels
     
    int flags;
    //! the array dimensionality, >= 2
    int dims;
    //! the number of height and columns or (-1, -1) when the array has more than 2 dimensions
    int height, width;
    //! pointer to the data
    uchar* data;

    //! pointer to the reference counter;
    // when array points to user-allocated data, the pointer is NULL
    int* refcount;
q
    // other members
    ...
};
*/


struct background_configuration
{
  int update_frequency;
  int blur_background;
  int use_exponential_filter;
  int num_of_blurs;
};

#define MAX_KERNEL_LENGTH 31;
void image_blur(Mat & in, int N)
{
  //  for ( int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2 )
  for (int i  = 0; i < N; i++)
    medianBlur ( in, in, 9 );
      

}

int main(int, char**)
{
  //threshold values to control things:
  const int threshold_pixel_change = 200;
  int motion_flag = 0;
  
  //    VideoCapture cap(1); // open the default camera is 0, usb camera is 1
    VideoCapture cap(1); // open the default camera is 0, usb camera is 1
    if(!cap.isOpened())  // check if we succeeded
      return -1;

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
    background_configuration bg_config;
    bg_config.update_frequency = 30;
    bg_config.blur_background = 1;
    bg_config.num_of_blurs = 5;
      

    while(1)
      {
	cap >> frame; // get a new frame from camera

	int height  = frame.rows;
	int width = frame.cols;

	//cout << "Image size is (width, height)" << width << " " << height << endl;
	
	//covert the input frame to a black and white frame
	cvtColor(frame, gray_frame, CV_BGR2GRAY);

	
	if(bg_config.blur_background == 1)
	  image_blur(gray_frame, bg_config.num_of_blurs);
	
	if(frame_count == 0)
	  background_frame = gray_frame.clone();
	
	cout << "frame count = " << frame_count << endl;


	//-----------------------------------------
	//subtract current frame from previous frame
	diff_frame = abs(gray_frame - background_frame);


       
	uint8_t * data = diff_frame.data;
	int pixels_change = detect_motion(data, height, width, 50);
	printf("Number of pixels changed = %i\n", pixels_change);
	if(pixels_change > threshold_pixel_change)
	  {
	    cout << "Motion detected \n";
	    imwrite( "./Motion.jpg", frame );
	    motion_flag = 1;
	  }
	else
	  {
	    motion_flag = 0;
	  }

	//display the frames
	imshow("frame",frame);
	imshow("grayframe",gray_frame);
	imshow("background_frame", background_frame);
	imshow("diff",10*diff_frame);  
       	
	
	//press q to quit
	int key = waitKey(30);
	//	if(key == 113) //quit
	  if(key == 'q') //quit	  
	  break;


	if(key == ' ')
	  {
	    cout << "space bar was pressed, make a new background\n";
	    background_frame = gray_frame.clone();
	  }
	
	//update background frame every 30 frames, and if theres no motion
	if(( (frame_count % bg_config.update_frequency) == 0) && (!motion_flag))
	  {
	    background_frame = gray_frame.clone();	   
	  }

	//update frame counter
	frame_count++;
	
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
} 
