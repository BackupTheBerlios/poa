/* Demo of modified Lucas-Kanade optical flow algorithm.
   See the printf below */

#ifdef _CH_
#pragma package <opencv>
#endif

//#ifndef _EiC
// motion templates sample code
//#endif
#include "features.h"
#include <limits.h>


IplImage *image = 0, *grey = 0, *prev_grey = 0, *pyramid = 0, *prev_pyramid = 0, *swap_temp;

int win_size = 10;
const int MAX_COUNT = 500;
CvPoint2D32f* points[2] = {0,0}, *swap_points;
char* status = 0;
int need_to_init = 0;
int night_mode = 0;
int flags = 0;
//int add_remove_pt = 0;
CvPoint pt;


void iterate(IplImage *frame, int count)
{
    int i, c;

    if( !image ) {
        /* allocate all the buffers */
        image = cvCreateImage( cvGetSize(frame), 8, 3 );
        image->origin = frame->origin;
        grey = cvCreateImage( cvGetSize(frame), 8, 1 );
        prev_grey = cvCreateImage( cvGetSize(frame), 8, 1 );
        pyramid = cvCreateImage( cvGetSize(frame), 8, 1 );
        prev_pyramid = cvCreateImage( cvGetSize(frame), 8, 1 );
        //points[0] = (CvPoint2D32f*)cvAlloc(MAX_COUNT*sizeof(points[0][0]));
        //points[1] = (CvPoint2D32f*)cvAlloc(MAX_COUNT*sizeof(points[0][0]));
        status = (char*)cvAlloc(MAX_COUNT);
        flags = 0;
    }

    cvCopy( frame, image, 0 );
    cvCvtColor( image, grey, CV_BGR2GRAY );

    if( night_mode )
        cvZero( image );
        
/*  if( need_to_init ) {
        /* automatic initialization /
        IplImage* eig = cvCreateImage( cvGetSize(grey), 32, 1 );
        IplImage* temp = cvCreateImage( cvGetSize(grey), 32, 1 );
        double quality = 0.01;
        double min_distance = 10;
                        
        count = MAX_COUNT;
        cvGoodFeaturesToTrack( grey, eig, temp, points[1], &count,
                               quality, min_distance, 0 );
        cvFindCornerSubPix( grey, points[1], count,
                            cvSize(win_size,win_size), cvSize(-1,-1),
                            cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS,20,0.03));
        cvReleaseImage( &eig );
        cvReleaseImage( &temp );
                        
        add_remove_pt = 0;
    } else*/ 
    if( count > 0 ) {
        cvCalcOpticalFlowPyrLK( prev_grey, grey, prev_pyramid, pyramid,
                                points[0], points[1], count, cvSize(win_size,win_size), 3, status, 0,
                                cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS,20,0.03), flags );
        flags |= CV_LKFLOW_PYR_A_READY;
        for( i = 0; i < count; i++ ) {
/*            if( add_remove_pt ) {
                double dx = pt.x - points[1][i].x;
                double dy = pt.y - points[1][i].y;

                if( dx*dx + dy*dy <= 25 ) {
                    add_remove_pt = 0;
                    continue;
                }
            }*/
                
            if( !status[i] ) {
                points[1][i] = cvPoint2D32f(-100.0, -100.0);
            }
        }
    }

/*    if( add_remove_pt && count < MAX_COUNT ) {
        points[1][count++] = cvPointTo32f(pt);
        cvFindCornerSubPix( grey, points[1] + count - 1, 1,
                            cvSize(win_size,win_size), cvSize(-1,-1),
                            cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS,20,0.03));
        add_remove_pt = 0;
    }*/

    CV_SWAP( prev_grey, grey, swap_temp );
    CV_SWAP( prev_pyramid, pyramid, swap_temp );
    CV_SWAP( points[0], points[1], swap_points );
    need_to_init = 0;
}



int assignPoints(PointGroup groups[], CvPoint2D32f *points)
{
    int p = 0;
    for (int g = 0; g < MAX_GROUPS; ++g) {
        for (int i = 0; i < groups[g].size; ++i) {
            points[p++] = groups[g].points[i];
        }
    } 
    return p;
}

double sqr(double x)
{
    return x*x;
}


const double EPS = 1.0e-6;
double DEFAULT_MOVE_THRESHOLD, MOVE_THRESHOLD = DEFAULT_MOVE_THRESHOLD = 1.0;
char *DEFAULT_PATH, *PATH = DEFAULT_PATH = ".";
char *FEATURE_POINTS = 0;
bool DISPLAY=false;


int updatePoints(const PointGroup groups[], CvPoint2D32f *points, PointGroup translation[],
                 bool moved[])
{
    int count = 0;
    int p = 0;
    for (int g = 0; g < MAX_GROUPS; ++g) {
        translation[g].size = groups[g].size;
        double diff = 0.0;
        for (int i = 0; i < groups[g].size; ++i) {
            translation[g].points[i] = points[p];
            diff += sqr(points[p].x - groups[g].points[i].x) +
                    sqr(points[p].y - groups[g].points[i].y);
            ++p;
        }
        moved[g] = diff > MOVE_THRESHOLD * (double)groups[g].size + EPS;
        if (moved[g]) {
            ++count;
            p -= groups[g].size;
            // movement in group g --> reset g's points
            for (int i = 0; i < groups[g].size; ++i) {
                points[p++] = groups[g].points[i];
            }
        }
    } 
    return count;
}


bool parseParameters(int argc, char *argv[])
{
    bool goon=true;
    const static struct option END_MARKER = { 0, 0, 0, 0 };
    const static struct option long_options[] = { {"threshold", 1, 0, 't'},
                                                  {"feature-points", 1, 0, 'f'},
                                                  {"visualize", 0, 0, 'v'},
                                                  {"help", 0, 0, 'h' },
                                                  END_MARKER };
    while (1) {
        char *endptr;
        int option_index;
        int c = getopt_long (argc, argv, "t:f:vh", long_options, &option_index);
        if (c==-1) break;
        switch (c) {
        case 't':
	    MOVE_THRESHOLD = strtod(optarg, &endptr);
	    goon &= (endptr != optarg);
	    break;
        case 'f':
            FEATURE_POINTS=optarg;
            break;
        case 'v':
            DISPLAY=true;
            break;
        case 'h':
            goon=false;
            printf ("Options:\n"
                    "%s \n"
                    "  -t | --threshold      <double> threshold for group movement, default=%f\n"
                    "  -f | --feature-points <path>   file containing feature points, no default\n"
                    "  -v | --visualize               display windows\n"
                    "  -h | --help                    show help\n"
                    "<path>                           recording directory, default = %s\n",
                    argv[0], DEFAULT_MOVE_THRESHOLD, DEFAULT_PATH);
            break;
        default:
            printf("Unrecognized option %o\n", c);
        }
    }
    if (optind < argc) {
        PATH = argv[optind];
    }
    return FEATURE_POINTS != 0 && goon;
}



PointGroup point_groups[MAX_GROUPS];
PointGroup translated[MAX_GROUPS];
bool moved[MAX_GROUPS];

int main( int argc, char** argv )
{
    for (unsigned gnum = 0; gnum < MAX_GROUPS; ++gnum) {
        point_groups[gnum] = createPointGroup();
        translated[gnum] = createPointGroup();
    }
    unsigned count;

    if( parseParameters(argc,argv) ) {

        points[0] = (CvPoint2D32f*)cvAlloc(MAX_GROUPS*MAX_PER_GROUP*sizeof(points[0][0]));
        points[1] = (CvPoint2D32f*)cvAlloc(MAX_GROUPS*MAX_PER_GROUP*sizeof(points[0][0]));
        readPointGroups(FEATURE_POINTS, point_groups);
        int point_count = assignPoints(point_groups, points[0]);
        if (point_count != assignPoints(point_groups, points[1])) exit(1);

        char old_dir[PATH_MAX];
        if (!getcwd(old_dir, PATH_MAX)) {
            assert(PATH_MAX >= 2);
            old_dir[0] = '.';
            old_dir[1] = 0;
        }
        chdir(PATH);

        if (DISPLAY) cvNamedWindow( "Pyramid Motion", 0 );

        struct dirent **name_list;
        int file_count = scandir(".", &name_list, 0, alphasort);
        IplImage *frame;
        
        bool first_image = true;
        for (int i = 0; i < file_count; ++i) {
            if (frame = cvLoadImage(name_list[i]->d_name)) {

                clock_t time_before = clock();
                iterate(frame, point_count);
                count = updatePoints(point_groups, points[0], translated, moved);
                if (first_image) {
                    count = 0;
                    first_image = false;
                }
                double efficiency = ((double)(clock() - time_before)) / (double)CLOCKS_PER_SEC;
                printf("%s\t%u\t%f\n", name_list[i]->d_name, count, efficiency);

                if (DISPLAY) {
                    paintPoints(image, frame, translated, UINT_MAX);
                    paintRects(image, translated, moved);
                    cvShowImage( "Pyramid Motion", image );
                    if( cvWaitKey(500) >= 0 )
                        break;
                }

                cvReleaseImage(&frame);
                free(name_list[i]);
                name_list[i] = 0;
            }
        }

        free(name_list);

        chdir(old_dir);

        if (DISPLAY) cvDestroyWindow("Pyramid Motion");

        return 0;
    }
}

#ifdef _EiC
main(1,"pyramid.c");
#endif
