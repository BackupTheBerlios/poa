#ifdef _CH_
#pragma package <opencv>
#endif

#ifndef _EiC
#include "cv.h"
#include "highgui.h"
#include <math.h>
#endif

#include <stdio.h>

#define CVNW_OPTION CV_WINDOW_AUTOSIZE

IplImage*  image[3];
IplImage*  gray[3];
IplImage*  hue[3];
IplImage*  temp8[3];
IplImage*  temp24[3];
IplImage*  velocityX;
IplImage*  velocityY;

typedef unsigned char us;
typedef unsigned int ui;

void picDiff(IplImage* d, IplImage* s1, IplImage* s2) {
	int i;
	for (i = 0; i < s1->imageSize; i++) {
		if (*((us*)s1->imageData + i) > *((us*)s2->imageData + i))
			*(d->imageData + i) = *((us*)s1->imageData + i) - *((us*)s2->imageData + i);
		else
			*(d->imageData + i) = *((us*)s2->imageData + i) - *((us*)s1->imageData + i);
	}
}

void thresholdFilter(IplImage* s, IplImage* d, int Threshold) {
	int i;
	for (i = 0; i < s->imageSize; i++) {
		if (*((us*)s->imageData + i) > Threshold)
			*((us*)d->imageData + i) = 255;
		else
			*((us*)d->imageData + i) = 0;
	}
}


void removeNoise(IplImage* s, IplImage* d, int Threshold) {
	int i;
	for (i = 0; i < s->imageSize; i++) {
		if (abs(*((us*)s->imageData + i) - *((us*)d->imageData + i)) <= Threshold) {
			*(d->imageData + i) = *(s->imageData + i);
		}
	}
}

void scanContour(IplImage* s, IplImage* d) {
	CvMemStorage* storage;
	CvSeq* contours = 0;
	CvContourScanner scanner;

    storage = cvCreateMemStorage(0);
	scanner = cvStartFindContours(s, storage, sizeof(CvContour), 
		CV_RETR_TREE, CV_CHAIN_APPROX_NONE, cvPoint(0,0));

    cvNamedWindow("Contour", CVNW_OPTION);
	while ((contours = cvFindNextContour(scanner)) != NULL) {
		cvDrawContours(d, contours, CV_RGB(255,0,0), CV_RGB(0,255,0), 5, 3, CV_AA );
	    cvShowImage("Contour", d);
	    cvWaitKey(0);
	}
	cvEndFindContours(&scanner);

//    cvFindContours(s, storage, &contours, sizeof(CvContour),
//                    CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0) );
//    cvDrawContours(d, contours, CV_RGB(255,0,0), CV_RGB(0,255,0), 5, 3, CV_AA );
}

int countPixels(IplImage* s, int x, int y, int h, int w) {
	int i, j, c;
	
	c = 0;
	for (i = x; i < x + w; i++) {
		for (j = y; j < y + h; j++) {
			if (*((us*)s->imageData + j * s->widthStep + i) > 0) {
				c++;
			}
		}
	}
	return c;
}

void fillPixels(IplImage* s, int x, int y, int h, int w) {
	int i, j;

	for (i = x; i < x + w; i++) {
		for (j = y; j < y + h; j++) {
			*((ui*)((us*)s->imageData + j * s->widthStep + i * 3)) = 0xFF0000;
		}
	}
}

void rasterizePixels(IplImage* s, IplImage* d, int BlockSize, int Percentage) {
	int i, j, h, w;
	for (i = 0; i < s->width; i += BlockSize) {
		for (j = 0; j < s->height; j += BlockSize) {
			h = BlockSize;
			w = BlockSize;
			if (i + w > s->width) w = s->width - i;
			if (j + h > s->height) h = s->height - j;
			if (countPixels(s, i, j, h, w) * 100 / (h * w) >= Percentage) {
				fillPixels(d, i, j, h, w);
			}
		}
	}
}

void opticalFlowBM(IplImage* s1, IplImage* s2, int BlockSize) {
	CvSize size = cvGetSize(s1);
	size.height /= BlockSize;
	size.width /= BlockSize;
    velocityX = cvCreateImage(size, IPL_DEPTH_32F, 1);
    velocityY = cvCreateImage(size, IPL_DEPTH_32F, 1);
	cvCalcOpticalFlowBM(s1, s2, cvSize(BlockSize, BlockSize), cvSize(1, 1), 
		cvSize(1, 1), 0, velocityX, velocityY);

	cvNamedWindow("HorFlowBM", CVNW_OPTION);
    cvShowImage("HorFlowBM", velocityX);

    cvNamedWindow("VerFlowBM", CVNW_OPTION);
    cvShowImage("VerFlowBM", velocityY);
}

void opticalFlowLK(IplImage* s1, IplImage* s2, int BlockSize) {
	CvSize size = cvGetSize(s1);
    velocityX = cvCreateImage(size, IPL_DEPTH_32F, 1);
    velocityY = cvCreateImage(size, IPL_DEPTH_32F, 1);
	cvCalcOpticalFlowLK(s1, s2, cvSize(BlockSize, BlockSize), velocityX, velocityY);

	cvNamedWindow("HorFlowLK", CVNW_OPTION);
    cvShowImage("HorFlowLK", velocityX);

    cvNamedWindow("VerFlowLK", CVNW_OPTION);
    cvShowImage("VerFlowLK", velocityY);
}

void doHSV() {
	hue[0] = cvCreateImage(cvGetSize(image[0]), IPL_DEPTH_8U, 1);
	hue[1] = cvCreateImage(cvGetSize(image[1]), IPL_DEPTH_8U, 1);
	cvCvtColor(image[0], temp24[0], CV_BGR2HSV);
	cvCvtColor(image[1], temp24[1], CV_BGR2HSV);
	cvCvtPixToPlane(temp24[0], hue[0], NULL, NULL, NULL);
	cvCvtPixToPlane(temp24[1], hue[1], NULL, NULL, NULL);

	picDiff(temp8[0], hue[0], hue[1]);
	thresholdFilter(temp8[0], temp8[0], 10);

	cvNamedWindow("Hue0", CVNW_OPTION);
    cvShowImage("Hue0", hue[0]);
	cvNamedWindow("HueDiff", CVNW_OPTION);
    cvShowImage("HueDiff", temp8[0]);
}

int main( int argc, char** argv ) {
//    char* filename1 = "current1.jpg";
//    char* filename2 = "current2.jpg";
    char* filename1;
    char* filename2;

	if (argc != 3) {
		printf("Illegal parameter: xxx file1.jpg file2.jpg");
		return 1;
	}
	filename1 = argv[1];
	filename2 = argv[2];

    if ((image[0] = cvLoadImage( filename1, 1)) == 0)
        return -1;

    if ((image[1] = cvLoadImage( filename2, 1)) == 0)
        return -1;

	gray[0] = cvCreateImage(cvGetSize(image[0]), IPL_DEPTH_8U, 1);
	gray[1] = cvCreateImage(cvGetSize(image[1]), IPL_DEPTH_8U, 1);
	cvCvtColor(image[0], gray[0], CV_BGR2GRAY);
	cvCvtColor(image[1], gray[1], CV_BGR2GRAY);
	temp8[0] = cvCreateImage(cvGetSize(image[0]), IPL_DEPTH_8U, 1);
	temp8[1] = cvCreateImage(cvGetSize(image[1]), IPL_DEPTH_8U, 1);
	temp24[0] = cvCreateImage(cvGetSize(image[0]), IPL_DEPTH_8U, 3);
	temp24[1] = cvCreateImage(cvGetSize(image[1]), IPL_DEPTH_8U, 3);

	cvSmooth(gray[0], gray[0], CV_GAUSSIAN, 3, 3, 0);
	cvSmooth(gray[1], gray[1], CV_GAUSSIAN, 3, 3, 0);

	removeNoise(gray[0], gray[1], 10);

	picDiff(temp8[0], gray[0], gray[1]);
	thresholdFilter(temp8[0], temp8[0], 10);
	cvNamedWindow("PicDiff", CVNW_OPTION);
    cvShowImage("PicDiff", temp8[0]);

	cvNamedWindow("Original", CVNW_OPTION);
    cvShowImage("Original", image[0]);

	rasterizePixels(temp8[0], image[1], 10, 10);
	cvNamedWindow("Rasterization", CVNW_OPTION);
    cvShowImage("Rasterization", image[1]);

//	opticalFlowBM(gray[0], gray[1], 4);
//	opticalFlowLK(gray[0], gray[1], 3);
	
	cvWaitKey(0);

/*    cvDestroyWindow("Image2");
    cvDestroyWindow("Pic1");
    cvDestroyWindow("Pic2");
    cvDestroyWindow("Horizontal");
    cvDestroyWindow("Vertical");

	cvReleaseImage(&temp24[1]);
    cvReleaseImage(&temp24[0]);
	cvReleaseImage(&temp8[1]);
    cvReleaseImage(&temp8[0]);
	cvReleaseImage(&image[1]);
    cvReleaseImage(&image[0]);
    cvReleaseImage(&velocityY);
    cvReleaseImage(&velocityX);*/
    return 0;
}
