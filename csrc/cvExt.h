#ifndef __CV_EXT_H_
#define __CV_EXT_H_
/////////////////////////////////////////////////////
#include "cv.h"

//Otsu算法求二值化阈值
static double cvGetThreshVal_Otsu( const CvHistogram* hist );
int graythresh(IplImage *src);

#endif