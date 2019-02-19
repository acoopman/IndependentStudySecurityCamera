//feature_extract.h  Written by Aidan Coopman 2019
#ifndef FEATURE_EXTRACT_H
#define FEATURE_EXTRACT_H

#include "typedefs.h"

//extracts key features:
//1: Center of cluster: features->center_x = 0; features->center_y = 0;
//2: Cluster: features->std_x = 0; features->std_y = 0;
//3: cluster size, percent of pixels that have changed: features->percent_pixels_changed
void extract_features(features_t * features,
		      uint8_t * in,
		      int height,
		      int width,
		      motion_detect_params_t * param);

#endif
