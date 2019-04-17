//feature_extract.cc  Written by Aidan Coopman 2019
#include "feature_extract.h"
#include <math.h>
#include "stats.h"

//features so far
//1) number pixels changed
//2) center of cluster
//3) std of cluster
void extract_features(features_t * features,
		      uint8_t * in,
		      int height,
		      int width,
		      motion_detect_params_t * param)
{
  int histo[256];

  //PIXEL VALUES ---------------------------------------
  //extract the mean and the std of the diff frame
  float diff_mean = image_mean(in, (width*height));
  float diff_std  = image_std(in, (width*height), diff_mean);

  //create a histogram of the different pixel values
  image_histogram(in, (width*height), histo);
  
  float threshold = diff_mean + param->std_factor*diff_std;

  if(param->min_threshold_diff > threshold)
    threshold = param->min_threshold_diff;

  //plot the histogram, helps debug
  plot_histogram(histo, 256);

  
  //LOCATION -------------------------------------------
  //returns the number of pixels above the threshold
  //figure out how many pixels are greater than the threshold
  int count = 0;
  float sum_x = 0;
  float sum_y = 0;
  features->std_x = 0;
  features->std_y = 0;

  //find the MEAN LOCATION: gives the cluster middle
  for(int y = 0; y < height; y++) //process height/vertical
    {
      for(int x = 0; x< width; x++) //process columns horizontal
	{
	  if(in[x + width*y  ] > threshold)
	    {
	      sum_x += x;
	      sum_y += y;
	      count++;
	    }
	  
	}
    }
  
  //make sure we dont divide by 0
  if(count == 0)
    {
      features->center_x = 0;
      features->center_y = 0;
    }
  else
    {
      features->center_x = (sum_x/count);
      features->center_y = (sum_y/count);
    }

  //store the percent of pixels that have changed
  features->percent_pixels_changed = ((float)count/(float)(width*height))*100.0f;

  printf("fe.cc: mean=%f  std=%f  min_thresh=%i  calc_thresh=%f, pixel_change_percent=%f\n",
	 diff_mean,diff_std, param->min_threshold_diff, threshold, features->percent_pixels_changed);
	    fflush(stdout);  
  //-----------------------------------------
  //find the standard deviation of the cluster
  sum_x = 0;
  sum_y = 0;
  
  for(int y = 0; y < height; y++) //process height/vertical
    {
      for(int x = 0; x< width; x++) //process columns horizontal
	{
	  if(in[x + width*y] > threshold)
	    {
	      sum_x += (features->center_x - x)*(features->center_x - x);
	      sum_y += (features->center_y - y)*(features->center_y - y);
	      in[x + width*y] = 200; 
	    }
	  else
	    {
	      in[x + width*y] = 0; 
	    }
	}
    }
  
  if(count == 0)
    {
      features->std_x = 0;
      features->std_y = 0;
    }
  else
    {
      features->std_x = sqrtf(sum_x/(float)count);
      features->std_y = sqrtf(sum_y/(float)count);
    }
}//function
