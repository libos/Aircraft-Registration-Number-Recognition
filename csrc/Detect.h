#ifndef detect_H_
#define detect_H_
/////////////////////////////////////////////////////
 
#include <afx.h>
#include <stdio.h>

#include <iostream>
#include <fstream>
#include "feature.h"

#include "cv.h"
#include "cxcore.h"
#include "highgui.h" 
#include "integralfea.h"
#include "SeqRect.h"
#include "RectSizer.h" 
#include <math.h>
#include "CharOCR.h"
#include "Knn.h"
#include "Sobel.h"
#include "..\stdafx.h"
/////////////////////////////////////////////////////
//some write out functions
//index =0; RGB		 images 
//index =1; stroke   images 
//index =2; corner   images 
//index =3; edge     images 
bool WriteResults(FeaImage fea, char path[255], char filename[255], int index);

void Process(CSeqRect *seqRect,	FeaImage *fea,char *result,CvRect *rectReturn,bool useSvm,bool onlyFind);
int Detect(IplImage *inImage,char *result , bool useSvm,CvRect *rect = NULL , bool onlyFind = false);
#endif