#ifndef __CHAROCR_H_
#define __CHAROCR_H_

#include <stdio.h>
#include <afx.h>
#include "cv.h"
#include "highgui.h"
#include "SeqRect.h"
#include "cvExt.h"
#include "Knn.h"
#include "SVMachine.h"
#include "..\stdafx.h"
#define MODEL_NUM 38
#define MODEL "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-";
class CharOCR
{

public:
	CharOCR();
	~CharOCR();
public:
	//返回二值化后的结果图像
	IplImage *Split(IplImage *img,CSeqRect *seq);
	void GetTrainHu(Knn *knn);
	char Classlabel2Char(double label);
	void GetFakeTrainingSample(IplImage *test,char *prefix);
	int Recognize(IplImage *img,char *result);
	int SVMRecognize(IplImage *img,char *result);
	void GenerateTrainData();
	bool FileExist(CString fileName);
	bool AddToTrainData(char label,char *filename);
	bool AddToTrainData(char *filename);
};

#endif