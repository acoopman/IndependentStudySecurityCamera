//stats.cc  Written by Aidan Coopman 2019
#include "stats.h"
#include <math.h>
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

//------------------------------------------------------------------------------
//gets the mean of the image
float image_mean(uint8_t * in, int count)
{
  float mean = 0;
  for(int i = 0; i < count; i++)
    {
      mean += in[i];
    }
  
  return mean/(float)count;
}

//------------------------------------------------------------------------------
//gets the std of the image
float image_std(uint8_t * in, int count, float mean)
{
  float std = 0;
  for(int i = 0; i < count; i++)
    {
      std += (in[i]-mean)*(in[i]-mean);
    }
  return sqrtf(std/(float)count);
}

//------------------------------------------------------------------------------
//creates the histogram for pixel values
void image_histogram(uint8_t * in, int count, int * histo)
{
  int n_bins = 256;
  for(int i = 0; i < n_bins; i++)
    {
      histo[i] = 0;
    }

  for(int i = 0; i < count; i++)
    {
      uint8_t pixel = in[i];
      histo[pixel] += 1;
    }
    
}

//------------------------------------------------------------------------------
//plots the histogram with the data from the image
void plot_histogram(int * hist, int bins)
{

    int hist_w = 512; int hist_h = 400;
    int bin_w = cvRound( (double) hist_w/bins );
 
    Mat histImage( hist_h, hist_w, CV_8UC1, Scalar( 0,0,0) );
    //normalize(hist, hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );

    //set first 10 bins to 0
    for(int i = 0; i<128; i++)
      {
	hist[i]=1;
      }

    int max = 0;
    for(int i = 0; i < bins; i++)
      {
	if( hist[i] > max)
	  max = hist[i];
      }
    for(int i = 0; i < bins; i++)
      hist[i] = hist_h*hist[i]/max;
    
    for( int i = 1; i < bins; i++ )
    {
      line( histImage, Point( bin_w*(i-1), hist_h - cvRound(hist[i-1] )) ,
	    Point( bin_w*(i), hist_h - cvRound(hist[i]) ),
	    Scalar( 255, 0, 0), 2, 8, 0  );
    }
    
    /*
    cv::putText(histImage, //target image
		"Hello, OpenCV!", //text
		cv::Point(10, histImage.rows / 2), //top-left position
		cv::FONT_HERSHEY_DUPLEX,
		1.0,
		CV_RGB(118, 185, 0), //font color
		2);

    // putText(histImage,'OpenCV',Point(10,500), FONT_HERSHEY_SIMPLEX, 4,(255,255,255),2);
    */
    
    // namedWindow( "Result", 1 );    imshow( "Result", histImage );    
}
