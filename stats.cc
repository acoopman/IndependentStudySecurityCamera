#include "stats.h"
#include <math.h>
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

float image_mean(uint8_t * in, int count)
{
  float mean = 0;
  for(int i = 0; i < count; i++)
    {
      mean += in[i];
    }
  
  return mean/(float)count;
}

float image_std(uint8_t * in, int count, float mean)
{
  float std = 0;
  for(int i = 0; i < count; i++)
    {
      std += (in[i]-mean)*(in[i]-mean);
    }
  return sqrtf(std/(float)count);
}

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

void plot_histogram(int * hist, int bins)
{

    int hist_w = 512; int hist_h = 400;
    int bin_w = cvRound( (double) hist_w/bins );
 
    Mat histImage( hist_h, hist_w, CV_8UC1, Scalar( 0,0,0) );
    //normalize(hist, hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
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
    
    namedWindow( "Result", 1 );    imshow( "Result", histImage );    
}
