#ifdef _CH_
#pragma package <opencv>
#endif

//#ifndef _EiC
// motion templates sample code
#include "cv.h"
#include "highgui.h"
#include <time.h>
#include <math.h>
#include <ctype.h>
#include <stdio.h>
#include <getopt.h>
#include <unistd.h>
//#endif

#include <dirent.h>

bool DISPLAY=true;

char *DEFAULT_PATH, *PATH = DEFAULT_PATH = ".";

char *OUTPUT_PATH = 0;


double first_time_stamp = 0.0;
bool first_time_stamp_initialized = false;

double calcTimeStamp(const char *file_name)
{
    const char *s = file_name;
    while ((*s != 0) && !(*s >= '0' && *s<='9')) ++s;
    char *endptr;
    double value = strtod(s, &endptr);
    assert(endptr != 0 && endptr != s);

    if (!first_time_stamp_initialized) {
        first_time_stamp = value;
        first_time_stamp_initialized = true;
    }

    return value - first_time_stamp;
}


bool parseParameters(int argc, char *argv[])
{
    bool can_goon=false;
    bool goon=true;
    const static struct option END_MARKER = { 0, 0, 0, 0 };
    const static struct option long_options[] = { {"help", 0, 0, 'h' },
						  {"output", 1, 0, 'o'},
						  END_MARKER };
    while (1) {
	char *endptr;
	int option_index;
	int c = getopt_long (argc, argv, "ho:", long_options, &option_index);
	if (c==-1) break;
	switch (c) {
	case 'h':
	    goon=false;
	    printf ("Options:\n"
		    "%s \n"
		    "  -h | --help               show help, no default\n"
		    "  -o | --output    <path>   file to save output to\n"
		    "<path>                      recording directory, default = %s\n",
		    argv[0], DEFAULT_PATH);
	    break;
	case 'o':
	    can_goon=true;
	    OUTPUT_PATH=optarg;
	    break;
	default:
	    printf("Unrecognized option %o\n", c);
	}
    }
    if (optind < argc) {
	PATH = argv[optind];
    }
    return can_goon && goon;
}


typedef struct 
{
    int index;
    unsigned count;
    double efficiency;
} image_info;

int main(int argc, char** argv)
{
    IplImage* motion = 0;
    IplImage* mask_motion=0;
    
    if( parseParameters(argc,argv) )
    {
	FILE *output_file = fopen(OUTPUT_PATH, "w");
	if (output_file != 0) {
	    printf("Output file %s\n", OUTPUT_PATH);
	} else {
	    printf("Couldn't open file %s\n", OUTPUT_PATH);
	    exit(1);
	}

	char old_dir[PATH_MAX];
	if (!getcwd(old_dir, PATH_MAX)) {
	    assert(PATH_MAX >= 2);
	    old_dir[0] = '.';
	    old_dir[1] = 0;
	}
	chdir(PATH);

	if (DISPLAY) {
	    cvNamedWindow( "Current", 1 );
	}

        struct dirent **name_list;
        int file_count = scandir(".", &name_list, 0, alphasort);
        IplImage *image;

	image_info *all = (image_info*)malloc (file_count * sizeof (image_info));
	unsigned all_count = 0;
	unsigned current;
        
        for (int i = 0; i < file_count; ++i) {
            if (image = cvLoadImage(name_list[i]->d_name)) {

		printf("Image: %s\n", name_list[i]->d_name);
		current = all_count;
		all[current].index = i;

		int count;

		do {
		
		    double time_stamp = calcTimeStamp(name_list[all[current].index]->d_name);


		    if (DISPLAY) {
			cvShowImage ("Current", image);
			count = 0;
			count = (cvWaitKey(0) & 0xFFFF);
		    }

		    if (count == 27) break;

		    if (count >= '0' && count <='9') {
			all[current].count = count - '0';
			all[current].efficiency = 0.0;
			printf("Image: %s, Persons: %u\n", name_list[all[current].index]->d_name,
			       all[current].count);
			if (current == all_count) ++all_count;
			else {
			    ++current;
			    count = 'n';
			}
		    } else {
			if (count == 'b' && current > 0) --current;
			else if (count =='n' && current < all_count) ++current;

			cvReleaseImage(&image);
			image = cvLoadImage(name_list[all[current].index]->d_name);
			assert(image != 0);
			if (current < all_count) {
			    printf("Image: %s, Persons: %u\n",
				   name_list[all[current].index]->d_name,
				   all[current].count);
			} else {
			    printf("Image: %s\n", name_list[all[current].index]->d_name);
			}
		    }

		} while (count != 27 && (count < '0' || count > '9'));

		cvReleaseImage(&image);
		if (count == 27) break;
            }

        }

	chdir(old_dir);

	
	for (int j = 0; j < all_count; ++j) {
	    fprintf(output_file,
		    "%s\t%i\t%f\n", name_list[all[j].index]->d_name,
		    all[j].count,
		    all[j].efficiency);
	}
	fclose(output_file);

	if (DISPLAY) {
	    cvDestroyWindow( "Motion" );
	    cvDestroyWindow( "Mask" );
	}

        free(name_list);
    }

    return 0;
}
                                
#ifdef _EiC
main(1,"motion.c");
#endif
