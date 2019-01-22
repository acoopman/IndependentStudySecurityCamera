#include "feature_extract.h"
#include <math.h>
#include "stats.h"

//features so far
//1) number pixels changed  TODO want it in percentage
//2) center of cluster
//3) std of cluster


void extract_features(features_t * features,
		      uint8_t * in,
		      int height,
		      int width,
		      motion_detect_params_t * param)
{
  int histo[256];
  //extract the mean and the std of the diff frame
  float diff_mean = image_mean(in, (width*height));

  float diff_std = image_std(in, (width*height), diff_mean);

  image_histogram(in, (width*height), histo);
  
  printf("mean = %f  std = %f  thresh = %i---------\n", diff_mean, diff_std, param->threshold_pixel_diff);

  param->threshold_pixel_diff = diff_mean + 4*diff_std; 
  
	//returns the number of pixels above the threshold
	  //figure out how many pixels are greater than the threshold
	  int count = 0;
	  float sum_x = 0;
	  float sum_y = 0;
	  features->std_x = 0;
	  features->std_y = 0;
	  //find how many pixels have changed and calculate the mean
	  for(int y = 0; y < height; y++) //process height/vertical
	    {
	      for(int x = 0; x< width; x++) //process columns horizontal
		{
		  if(in[x + width*y  ] > param->threshold_pixel_diff)
		    {
		      sum_x += x;
		      sum_y += y;
		      count++;
		    }

		}
	    }

	  features->percent_pixels_changed = ((float)count/(float)(width*height))*100.0f;
	  // printf("count = %i   percent = %f   width = %i height = %i\n", count, features->percent_pixels_changed, width, height);
	  //fflush(stdout);
	  //get the mean/center of the cluster

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
	  //-----------------------------------------
	  //find the standard deviation of the cluster
		sum_x = 0;
		sum_y = 0;

	  for(int y = 0; y < height; y++) //process height/vertical
	    {
	      for(int x = 0; x< width; x++) //process columns horizontal
		{
		  if(in[x + width*y] > param->threshold_pixel_diff)
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
	  //---------------------------------------
	  //check to make sure the std are widthin the threshold


}
