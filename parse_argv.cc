//parse_argv.cc  Written by Aidan Coopman 2019
#include "parse_argv.h"

//parse argv/argc from main()
void parse_argv(int argc, char *argv[],
		motion_detect_params_t * params,
		video_in_params_t * video_params)
{
  for (int i = 1; i < argc; i++)
    {
      printf("input string = %s\n",argv[i]);
      if( (argv[i][0] == '-')  && (argv[i][1] == 'w'))
	{
	  video_params->write_output = 1;
	  printf("Will write output\n");

	  video_params->outputfile = argv[i+1];
	  i++;
	}
	
      if( (argv[i][0] == '-')  && (argv[i][1] == 'f'))
	{
	  video_params->read_input = 1;
	  printf("Reading input\n");

	  video_params->inputfile = argv[i+1];
	  i++;
	}
      
      if( (argv[i][0] == '-')  && (argv[i][1] == 's') && (argv[i][2] == 'o'))
	{
	  video_params->video_source = atoi(argv[i+1]);
	  i++;
	  printf("video source = %i\n",video_params->video_source);
	}


      if( (argv[i][0] == '-')  && (argv[i][1] == 't') && (argv[i][2] == 'h'))
	{
	  params->pixel_percent_threshold = atof(argv[i+1]);;
	  i++;
	  printf("threshold = %f\n",params->pixel_percent_threshold);
	}
      
    }


}
