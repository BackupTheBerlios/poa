#ifdef _CH_
#pragma package <opencv>
#endif

#ifndef _EiC
#include "cv.h"
#include "highgui.h"
#include <math.h>
#endif

IplImage*  image[2];
IplImage*  source[2];
IplImage*  velocityX;
IplImage*  velocityY;
CvSize size;

int main( int argc, char** argv )
{
    if (argc<2) return 17;

    char* filename1 = argv[1];
    char* filename2 = argv[2];
    
    if( (image[0] = cvLoadImage( filename1, 1)) == 0 )
        return -1;

    if( (image[1] = cvLoadImage( filename2, 1)) == 0 )
        return -1;

    source[0] = cvCreateImage(cvGetSize(image[0]), IPL_DEPTH_8U, 1);
    source[1] = cvCreateImage(cvGetSize(image[1]), IPL_DEPTH_8U, 1);
    cvCvtColor(image[0], source[0], CV_BGR2GRAY);
    cvCvtColor(image[1], source[1], CV_BGR2GRAY);

    cvNamedWindow("Pic1", 0);
    cvShowImage("Pic1", source[0]);

    cvNamedWindow("Pic2", 0);
    cvShowImage("Pic2", source[1]);

    size = cvGetSize(source[0]);

    size.height /= 5;
    size.width /= 5;

    velocityX = cvCreateImage(size, IPL_DEPTH_32F, 1);
    velocityY = cvCreateImage(size, IPL_DEPTH_32F, 1);

//  cvCalcOpticalFlowLK(source[0], source[1], cvSize(3, 3), velocityX, velocityY);
    cvCalcOpticalFlowBM(source[0], source[1], cvSize(5, 5), cvSize(1, 1), cvSize(1, 1), 0, velocityX, velocityY);

    cvNamedWindow("Horizontal", 0);
    cvShowImage("Horizontal", velocityX);

    cvNamedWindow("Vertical", 0);
    cvShowImage("Vertical", velocityY);

    cvWaitKey(0);

    cvDestroyWindow("Pic1");
    cvDestroyWindow("Pic2");
    cvDestroyWindow("Horizontal");
    cvDestroyWindow("Vertical");

    cvReleaseImage(&source[1]);
    cvReleaseImage(&source[0]);
    cvReleaseImage(&image[1]);
    cvReleaseImage(&image[0]);
    cvReleaseImage(&velocityY);
    cvReleaseImage(&velocityX);
    return 0;
}

#ifdef _EiC
main(1,"pyramid_segmentation.c");
#endif
