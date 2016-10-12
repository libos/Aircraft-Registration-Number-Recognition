#ifndef train_H_
#define train_H_
#include <stdio.h>
#include <ctime>
#include "cv.h"
#include "highgui.h"
#include "SeqRect.h"

typedef struct node{
	CvRect rect;
	bool normal; 
	struct node *next;					//主链->前后无包含关系
	struct node *prev;		
}SobelRects; 

class CSobel
{
public:
	CSobel();
	void Sobel(IplImage *rgb);
	void Add(CvRect rect);
	int getLength();
	int getPartInnerCount(IplImage *img,CvRect rect);
public:
	SobelRects *rects;
private:
	SobelRects *last;
	CvRect maxRect;
 
};
#endif