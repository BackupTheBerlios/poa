#ifdef _CH_
#pragma package <opencv>
#endif

//#ifndef _EiC
// motion templates sample code
//#endif

#include "features.h"

bool DISPLAY=true;

char *PATH = 0;
char *OUTPUT_PATH = 0;
char *INPUT_PATH = 0;


bool parseParameters(int argc, char *argv[])
{
    bool goon=true;
    const static struct option END_MARKER = { 0, 0, 0, 0 };
    const static struct option long_options[] = { {"help", 0, 0, 'h' },
                                                  {"input", 1, 0, 'i'},
						  {"output", 1, 0, 'o'},
						  END_MARKER };
    while (1) {
	char *endptr;
	int option_index;
	int c = getopt_long (argc, argv, "ho:i:", long_options, &option_index);
	if (c==-1) break;
	switch (c) {
	case 'h':
	    goon=false;
	    printf ("Options:\n"
		    "%s \n"
		    "  -h | --help               show help, no default\n"
                    "  -i | --image     <path>   background image to display\n"
		    "<path>                      feature point file\n",
		    argv[0]);
	    break;
        case 'i':
            PATH=optarg;
            break;
	default:
	    printf("Unrecognized option %o\n", c);
	}
    }
    if (optind < argc) {
	INPUT_PATH = OUTPUT_PATH = argv[optind];
    }
    if (goon) {
        if (PATH == 0) {
            printf("Please specify image with -i\n");
        }
        if (OUTPUT_PATH == 0) {
            printf("Please specify feature point file name");
        }
    }
    return PATH != 0 && OUTPUT_PATH != 0 && goon;
}


PointGroup point_groups[MAX_GROUPS];
unsigned gnum;

void onmouse(int event, int x, int y, int flags)
{
    if ((event & CV_EVENT_LBUTTONDOWN) &&
        (gnum < MAX_GROUPS) && (point_groups[gnum].size < MAX_PER_GROUP)) {
        
        point_groups[gnum].points[point_groups[gnum].size++] =
            cvPoint2D32f((double)x, (double)y);
        printf("Current point group: %u, points: %u\n", gnum, point_groups[gnum].size);
    }
}

int main(int argc, char** argv)
{
    for (gnum = 0; gnum < MAX_GROUPS; ++gnum) {
        point_groups[gnum] = createPointGroup();
    }
    gnum = 0;

    int c;

    if( parseParameters(argc,argv) )
    {
        if (INPUT_PATH != 0) {
            readPointGroups(INPUT_PATH, point_groups);
        }
        
        cvNamedWindow( "Current", 1 );
        IplImage *image = cvLoadImage(PATH);
        IplImage *point_image = cvCreateImage(cvGetSize (image), IPL_DEPTH_8U, 3);
        gnum = 0;
        cvSetMouseCallback("Current", onmouse);
        printf("Current point group: %u, points: %u\n", gnum, point_groups[gnum].size);
        do {
            paintPoints(point_image, image, point_groups, gnum);
            cvShowImage("Current", point_image);
            c = cvWaitKey(5) & 0xFFFF;
            switch (c) {
            case 'n':
                if (gnum + 1 < MAX_GROUPS) ++gnum;
                printf("Current point group: %u, points: %u\n", gnum, point_groups[gnum].size);
                break;
            case 'b':
                if (gnum > 0) --gnum;
                printf("Current point group: %u, points: %u\n", gnum, point_groups[gnum].size);
                break;
            case 'd':
                if (point_groups[gnum].size > 0) point_groups[gnum].size--;
                printf("Current point group: %u, points: %u\n", gnum, point_groups[gnum].size);
                break;
            }
        } while (c != 27 && c != 'q');

	if (c == 'q') {
            FILE *output_file = fopen(OUTPUT_PATH, "w");
            if (output_file != 0) {
                printf("Output file %s\n", OUTPUT_PATH);
            } else {
                printf("Couldn't open file %s\n", OUTPUT_PATH);
                exit(1);
            }
            char c = 0;
            for (unsigned g = 0; g < MAX_GROUPS; ++g) {
                for (unsigned p = 0; p < point_groups[g].size; ++p) {
                    if (c != 0) fprintf(output_file, "%c", c);
                    fprintf(output_file,
                            "(%f %f)",
                            point_groups[g].points[p].x, point_groups[g].points[p].y);
                    c = ',';
                }
                if (c != 0) c = ';';
            }
            if (c != 0) fprintf(output_file, ".");
            fclose(output_file);
        }

        cvDestroyWindow( "Current" );
        cvReleaseImage(&image);
        cvReleaseImage(&point_image);
    }

    return 0;
}
                                
#ifdef _EiC
main(1,"setfeature.c");
#endif
