/*****************************************************************
** Copyright 2012-2013 LAMP of UMICAS.UMD	All rights reserved **
**																**
** version 1.0													**
** Date: DEC 30th, 2012		by Qixiang ye						**
**																**
** header	 file for		 feature extraction					**
**																**
******************************************************************/
#ifndef FEATURE_H_
#define FEATURE_H_
/////////////////////////////////////////////////////
#include <cv.h>
//#include "libsvm\\linear.h"
#include "SeqRect.h"
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
#define ABS(a) (a>0?a:-a)

///////////////////////////////////////////////////////////////////
#define DIAT_RADIUS 3
///////////////////////////////////////////////////////////////////
typedef struct{
	CvBox2D box;
	CvPoint center;
	float   reponse;

}Character;

typedef struct{
	
	int Wid;
	int Hei;	
	
	IplImage* RGB;			//Image of RGB	
	IplImage* LUV;			//Image of LUV
	IplImage* QUA;			//Image of quantilized map
	IplImage* gradientMag;  //Image of gradient
	IplImage* mserMap[4];	//Image of mserImage
	IplImage**ROIImages ;   //Images of ROI

	CvMemStorage* storage	;  //for sequences
	CvSeq* seqCharacter     ;  //sequence of characters
	CvSeq* seqText          ;  //seuence of texts
}FeaImage;
///////////////////////////////////////////////////////////////////
int MSER2CC(FeaImage fea,CSeqRect *seq);
int LVQ(FeaImage fea);

/////////////////////////////////////////////////////
int ComputerGradientMag(FeaImage fea);
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////


#endif