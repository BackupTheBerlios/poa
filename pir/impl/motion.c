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
//#endif

#include <dirent.h>

// various tracking parameters (in seconds)
const double MHI_DURATION = 2;
const double MAX_TIME_DELTA = 0.5;
const double MIN_TIME_DELTA = 0.05;
// number of cyclic frame buffer used for motion detection
// (should, probably, depend on FPS)
const int N = 2;

// ring image buffer
IplImage **buf = 0;
int last = 0;

// temporary images
IplImage *mhi = 0; // MHI
IplImage *orient = 0; // orientation
IplImage *mask = 0; // valid orientation mask
IplImage *segmask = 0; // motion segmentation map
CvMemStorage* storage = 0; // temporary storage

// parameters:
//  img - input video frame
//  timestamp - timestamp of imp in seconds
//  dst - resultant motion picture
//  args - optional parameters
void  update_mhi( IplImage* img, double timestamp, IplImage* dst, IplImage* dst_mask,
		  int diff_threshold )
{
    CvSize size = cvSize(img->width,img->height); // get current frame size
    int i, idx1 = last, idx2;
    IplImage* silh;
    CvSeq* seq;
    CvRect comp_rect;
    double count;
    double angle;
    CvPoint center;
    double magnitude;          
    CvScalar color;

    // allocate images at the beginning or
    // reallocate them if the frame size is changed
    if( !mhi || mhi->width != size.width || mhi->height != size.height ) {
        if( buf == 0 ) {
            buf = (IplImage**)malloc(N*sizeof(buf[0]));
            memset( buf, 0, N*sizeof(buf[0]));
        }
        
        for( i = 0; i < N; i++ ) {
            cvReleaseImage( &buf[i] );
            buf[i] = cvCreateImage( size, IPL_DEPTH_8U, 1 );
            cvZero( buf[i] );
        }
        cvReleaseImage( &mhi );
        cvReleaseImage( &orient );
        cvReleaseImage( &segmask );
        cvReleaseImage( &mask );
        
        mhi = cvCreateImage( size, IPL_DEPTH_32F, 1 );
        cvZero( mhi ); // clear MHI at the beginning
        orient = cvCreateImage( size, IPL_DEPTH_32F, 1 );
        segmask = cvCreateImage( size, IPL_DEPTH_32F, 1 );
        mask = cvCreateImage( size, IPL_DEPTH_8U, 1 );
    }

    cvCvtColor( img, buf[last], CV_BGR2GRAY ); // convert frame to grayscale

    idx2 = (last + 1) % N; // index of (last - (N-1))th frame
    last = idx2;

    silh = buf[idx2];
    cvAbsDiff( buf[idx1], buf[idx2], silh ); // get difference between frames
    
    cvThreshold( silh, silh, diff_threshold, 1, CV_THRESH_BINARY ); // and threshold it
    cvUpdateMotionHistory( silh, mhi, timestamp, MHI_DURATION ); // update MHI

    // convert MHI to blue 8u image
    cvCvtScale( mhi, mask, 255./MHI_DURATION,
                (MHI_DURATION - timestamp)*255./MHI_DURATION );
    cvZero( dst_mask );
    cvCvtPlaneToPix( mask, 0, 0, 0, dst_mask );
    cvCopy(img, dst);

    // calculate motion gradient orientation and valid orientation mask
    cvCalcMotionGradient( mhi, mask, orient, MAX_TIME_DELTA, MIN_TIME_DELTA, 3 );
    
    if( !storage )
        storage = cvCreateMemStorage(0);
    else
        cvClearMemStorage(storage);
    
    // segment motion: get sequence of motion components
    // segmask is marked motion components map. It is not used further
    seq = cvSegmentMotion( mhi, segmask, storage, timestamp, MAX_TIME_DELTA );

    // iterate through the motion components,
    // One more iteration (i == -1) corresponds to the whole image (global motion)
    for( i = /*-1 DO NOT USE ENTIRE RECTANGLE*/0; i < seq->total; i++ ) {

        if( i < 0 ) { // case of the whole image
            comp_rect = cvRect( 0, 0, size.width, size.height );
            color = CV_RGB(255,255,255);
            magnitude = 100;
        }
        else { // i-th motion component
            comp_rect = ((CvConnectedComp*)cvGetSeqElem( seq, i ))->rect;
            if( comp_rect.width + comp_rect.height < 100 ) // reject very small components
                continue;
            color = CV_RGB(255,0,0);
            magnitude = 30;
        }

        // select component ROI
        cvSetImageROI( silh, comp_rect );
        cvSetImageROI( mhi, comp_rect );
        cvSetImageROI( orient, comp_rect );
        cvSetImageROI( mask, comp_rect );

        // calculate orientation
        angle = cvCalcGlobalOrientation( orient, mask, mhi, timestamp, MHI_DURATION);
        angle = 360.0 - angle;  // adjust for images with top-left origin

        count = cvNorm( silh, 0, CV_L1, 0 ); // calculate number of points within silhouette ROI

        cvResetImageROI( mhi );
        cvResetImageROI( orient );
        cvResetImageROI( mask );
        cvResetImageROI( silh );

	cvRectangle( dst,
                     cvPoint(comp_rect.x, comp_rect.y),
                     cvPoint(comp_rect.x+comp_rect.width-1, comp_rect.y+comp_rect.height-1),
		     CV_RGB(0,255,0) );
	cvRectangle( dst_mask,
                     cvPoint(comp_rect.x, comp_rect.y),
                     cvPoint(comp_rect.x+comp_rect.width-1, comp_rect.y+comp_rect.height-1),
		     CV_RGB(0,255,0) );

        // check for the case of little motion
        if( count < comp_rect.width*comp_rect.height * 0.05 )
            continue;

        // draw a clock with arrow indicating the direction
        center = cvPoint( (comp_rect.x + comp_rect.width/2),
                          (comp_rect.y + comp_rect.height/2) );

        cvCircle( dst, center, cvRound(magnitude*1.2), color, 3, CV_AA, 0 );
        cvLine( dst, center, cvPoint( cvRound( center.x + magnitude*cos(angle*CV_PI/180)),
                cvRound( center.y - magnitude*sin(angle*CV_PI/180))), color, 3, CV_AA, 0 );
        cvCircle( dst_mask, center, cvRound(magnitude*1.2), color, 3, CV_AA, 0 );
        cvLine( dst_mask, center, cvPoint( cvRound( center.x + magnitude*cos(angle*CV_PI/180)),
                cvRound( center.y - magnitude*sin(angle*CV_PI/180))), color, 3, CV_AA, 0 );
    }
}


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


int main(int argc, char** argv)
{
    IplImage* motion = 0;
    IplImage* mask_motion=0;
    
    if( 1 )
    {
 
        cvNamedWindow( "Motion", 1 );
	cvNamedWindow( "Mask", 1 );

	struct dirent **name_list;
	int file_count = scandir(".", &name_list, 0, alphasort);
	IplImage *image;
	
        for (int i = 0; i < file_count; ++i) {
	    if (image = cvLoadImage(name_list[i]->d_name)) {

	      double time_stamp = /*clock()/CLOCKS_PER_SEC;*/calcTimeStamp(name_list[i]->d_name);

	        printf("Current file: %s, time: %fs\n", name_list[i]->d_name, time_stamp);

		free(name_list[i]);
		name_list[i] = 0;

		// is motion and mask_motion allocated already?
		if( !motion )
		{
		    motion = cvCreateImage( cvSize(image->width,image->height), 8, 3 );
		    mask_motion = cvCreateImage( cvSize(image->width,image->height), 8, 3 );
		    cvZero( motion );
		    cvZero( mask_motion );
		    motion->origin = image->origin;
		    mask_motion->origin = image->origin;
		}


		update_mhi( image, time_stamp, motion, mask_motion, 5 );
		cvShowImage( "Motion", motion );
		cvShowImage( "Mask", mask_motion );

		if( cvWaitKey(500) >= 0 )
		  break;
            }
        }

        cvDestroyWindow( "Motion" );
	cvDestroyWindow( "Mask" );

	free(name_list);
    }

    return 0;
}
                                
#ifdef _EiC
main(1,"motempl.c");
#endif
