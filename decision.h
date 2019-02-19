//decision.h  Written by Aidan Coopman 2019
#ifndef DECISION_H
#define DECISION_H

#include "feature_extract.h"

//takes in the features and the video params
//returns 0 if there is no motion and 1 if there is motion
int make_decision(features_t * features,  motion_detect_params_t * param);

#endif
