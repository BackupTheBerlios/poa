#ifndef __FEATURE_H
#define __FEATURE_H

#include "cv.h"
#include "highgui.h"
#include <time.h>
#include <math.h>
#include <ctype.h>
#include <stdio.h>
#include <getopt.h>
#include <unistd.h>
#include <dirent.h>


const unsigned MAX_GROUPS = 8;
const unsigned MAX_PER_GROUP = 40;

typedef struct
{
    unsigned size;
    CvPoint2D32f *points;
} PointGroup;

PointGroup createPointGroup()
{
    PointGroup group;
    group.size = 0;
    group.points = (CvPoint2D32f*)malloc(MAX_PER_GROUP*sizeof(CvPoint2D32f));
    return group;
}


void readPointGroups(const char *input_name, PointGroup point_groups[])
{
    unsigned gnum;
    for (gnum = 0; gnum < MAX_GROUPS; ++gnum) {
        point_groups[gnum].size = 0;
    }
    FILE *input_file = fopen(input_name, "r");
    if (input_file != 0) {
        char c = 0;
        bool first_read = true;          
        for (gnum = 0; c != '.'; ++gnum) {
            if (gnum >= MAX_GROUPS) {
                printf("Too many groups!\n");
                exit(1);
            }
            do {
                unsigned curr_index = point_groups[gnum].size;
                if (curr_index >= MAX_PER_GROUP) {
                    printf("Too many points in group!\n");
                    exit(1);
                }
                CvPoint2D32f &current = point_groups[gnum].points[curr_index];
                float x, y;
                int result = fscanf(input_file, " ( %f %f ) %c", &x, &y, &c);
                if (first_read && result == EOF) {
                    c = '.';
                } else {
                    first_read = false;
                    if (result != 3) {
                        printf("Read error\n");
                        exit(1);
                    }
                    current.x = x;
                    current.y = y;
                    point_groups[gnum].size++;
                }

            } while (c == ',');
        }

        fclose(input_file);
    }
}


void paintPoints(IplImage *point_image, const IplImage *image,
                 PointGroup point_groups[], unsigned gnum)
{
    static const CvScalar colors[MAX_GROUPS] =
        { CV_RGB(255,255,255), CV_RGB(255,0,0), CV_RGB(0,255,0), CV_RGB(0,0,255),
          CV_RGB(0,255,255), CV_RGB(255,0,255), CV_RGB(255,255,0), CV_RGB(0,0,0) };
    cvCopy(image, point_image);
    for (unsigned g = 0; g < MAX_GROUPS; ++g) {
        for (unsigned p = 0; p < point_groups[g].size; ++p) {
            int radius = g == gnum ? 4 : 2;
            cvCircle(point_image,
                     cvPointFrom32f(point_groups[g].points[p]),
                     radius, colors[g], -1, 8,0);
        }
    }
}

void paintRects(IplImage *image, const PointGroup point_groups[], const bool moved[])
{
    int done;
    int i;
    int count;
    CvPoint2D32f box_vtx[4];
    CvBox2D box;
    CvPoint* points = (CvPoint*)malloc( MAX_PER_GROUP * sizeof(points[0]));    
    for (unsigned g = 0; g < MAX_GROUPS; ++g) if (moved[g]) {
        count = 0;
        for (i = 0; i < (int)point_groups[g].size; ++i) {
            if (point_groups[g].points[i].x >= 0) ++count;
        }
        if (count > 1) {
            CvMat pointMat = cvMat( 1, count, CV_32SC2, points );

            for( i = 0, done = 0; done < count; i++ ) {
                if (point_groups[g].points[i].x >= 0) {
                    points[i] = cvPointFrom32f(point_groups[g].points[i]);
                    ++done;
                }
            }

            box = cvMinAreaRect2( &pointMat, 0 );
            cvBoxPoints( box, box_vtx );
            CvPoint pt0;
            pt0.x = cvRound(box_vtx[3].x);
            pt0.y = cvRound(box_vtx[3].y);
            for( i = 0; i < 4; i++ ) {
                CvPoint pt;
                pt.x = cvRound(box_vtx[i].x);
                pt.y = cvRound(box_vtx[i].y);
                cvLine(image, pt0, pt, CV_RGB(0, 255, 0), 1, CV_AA, 0);
                pt0 = pt;
            }
        }
    }
    free(points);
}




#endif
