#include "parse_argv.h"


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
      
      if( (argv[i][0] == '-')  && (argv[i][1] == 's') && (argv[i][2] == 'o'))
	{
	  video_params->video_source = atoi(argv[i+1]);
	  i++;
	  printf("video source = %i\n",video_params->video_source);
	}


      if( (argv[i][0] == '-')  && (argv[i][1] == 't') && (argv[i][2] == 'h'))
	{
	  params->pixel_value_threshold = atoi(argv[i+1]);;
	  i++;
	  printf("threshold = %i\n",params->pixel_value_threshold);
	}
      
    }


}