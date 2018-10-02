#include "opencv2/opencv.hpp"
#include <iostream>

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

int detect_motion(uint8_t * in, int height, int width, uint8_t threshold)
{
  int count = 0;
  for(int k = 0; k < height; k++) //process height/vertical
    {
      for(int i = 0; i< width; i++) //process columns horizontal
	{
	  if(in[i + width*k  ] > threshold)
	    {
	      count++;
	      if(count > 60)
		{
		  return 1;
		}
	      
	    }
	  
	}
    }
  return 0;
}

int main(int, char**)
{
    VideoCapture cap(1); // open the default camera is 0, usb camera is 1
    if(!cap.isOpened())  // check if we succeeded
      return -1;

    //make an output window up
    namedWindow("frame",1);
    namedWindow("grayframe",1);
    namedWindow("previous_frame",1);
    namedWindow("diff",1);
    
    int frame_count = 0;
    int bj_count = 100;
    
    Mat frame;
    Mat background_frame;
    Mat gray_frame;
    Mat diff_frame;
    
      while(1)
	{
	cap >> frame; // get a new frame from camera

	int height  = frame.rows;
	int columns = frame.cols;

	//cout << "Image size is (width, height)" << columns << " " << height << endl;
	
	//covert the input frame to a black and white frame
	cvtColor(frame, gray_frame, CV_BGR2GRAY);
	if(frame_count == 0)
	  background_frame = gray_frame.clone();
	
	cout << "frame count = " << frame_count << endl;
	
	//-----------------------------------------
	//subtract current frame from previous frame
	diff_frame = abs(gray_frame - background_frame);
	
       
	uint8_t * data = diff_frame.data;
	int motion = detect_motion(data, height, columns, 128);
	if(motion)
	  {
	    cout << "Motion detected \n";
	    imwrite( "./Motion.jpg", frame );
	  }

	//display the frames
	imshow("frame",frame);
	imshow("grayframe",gray_frame);
	imshow("previous_frame", background_frame);
	imshow("diff",diff_frame);  
       	
	
	//press q to quit
	int key = waitKey(30);
	if(key == 113) //quit
	  break;

	frame_count++;
	if(( (frame_count % bj_count) == 0) && (!motion))
	  {
	    background_frame = gray_frame.clone();
	  }
	
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
} 
