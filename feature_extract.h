#ifndef FEATURE_EXTRACT_H
#define FEATURE_EXTRACT_H

#include "typedefs.h"

void extract_features(features_t * features,
		      uint8_t * in,
		      int height,
		      int width,
		      motion_detect_params_t * param);







#endif
