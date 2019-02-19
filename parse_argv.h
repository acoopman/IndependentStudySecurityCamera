//parse_argv.h  Written by Aidan Coopman 2019
#ifndef PARSE_ARGV_H
#define PARSE_ARGV_H

#include "detect_motion.h"

//parse argv/argc from main()
void parse_argv(int argc, char *argv[],
		motion_detect_params_t * params,
		video_in_params_t * video_params);


#endif
