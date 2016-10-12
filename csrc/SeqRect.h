#ifndef SEQRECT_H_
#define SEQRECT_H_
/////////////////////////////////////////////////////
#include <cv.h>
#include <math.h>

typedef struct rectnode{
	int region_id;
	CvRect rect;
	int avarage[3];					//hsv avarage
	int var[3];						//hsv variance 
	struct rectnode *next;					//主链->前后无包含关系
	struct rectnode *prev;		
	struct rectnode *out_next;				//辅链->辅链上所有的节点都包含主链的节点
	struct rectnode *out_prev;
	struct rectnode *first_out_layer;
}SeqRect; 

#define INFINIT 65535
#define MARGIN_OFFSET 10
#define PADDING_OFFSET 10
#define RATIO_AREA 0.001
#define EPSILON 0.00000001		//极小数
class CSeqRect
{
public:
	CSeqRect();
	CSeqRect(int fea_width,int fea_height);
	CSeqRect(CvRect rect);
	~CSeqRect();
public:
	SeqRect *seqNode;
public:
	SeqRect *CreateSeqRect();
	SeqRect *CreateSeqRect(CvRect rect);
	

public:
	void AddNode(CvRect rect);
	void AddNode(CvRect rect,bool simple);//重载不加判断
	bool IsRectAbnormal(CvRect rect);
	bool IsRect1In2(CvRect rect1,CvRect rect2);
	bool IsRect1In2(CvRect rect1,CvRect rect2,bool simple);
	void InsertInineEnd(SeqRect *inlineRect,SeqRect *newRect);
	void InsertInOrder(SeqRect *inlineRect,SeqRect *newRect);	//如果最后没有发现包含关系的，按照rect的x值大小加入主队列
	void InsertOutEnd(SeqRect *inlineRect,SeqRect *newRect);
	void ReplaceBy2(SeqRect *inlineRect,SeqRect *newRect);
	void calcArea();
	double calcRatio(CvRect rect);
	//Get Rect HSV average value and Variance value 
//	void averageRectValue(IplImage *img,CvRect rect,int *average,int *var);
public:
	long int _area;
private:
	bool firstNode;
	int FeaWidth;
	int FeaHeight;
	int bug;
};
#endif