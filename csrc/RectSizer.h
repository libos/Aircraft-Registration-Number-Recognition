#ifndef __RECTSIZER_H_
#define __RECTSIZER_H_
/////////////////////////////////////////////////////
#include <cv.h>
#include "SeqRect.h"
////@@@@@@@@@@@@@@@@@@参数2
#define EPSILON_X 30
#define EPSILON_Y 10
#define ERROR_STATUS 9999
#define RIGHT_STATUS -9999

typedef struct regionSize
{
	int left,right,up,down;
	int id;
	int InnerCount;
	int per_width;
	int max_y;
	int min_y;
	struct regionSize *next;
	struct regionSize *prev;
}RegionSizer;
typedef struct roiRect
{
	CvRect rect[10];
	int minDist[10];
	int aveWidth;
	int id;
	int InnerCount;
	struct roiRect *next;
}ROIRect;
class CRectSizer
{
public:
	CRectSizer();
	~CRectSizer();
	CRectSizer(CSeqRect *seq);
public:
	void sizeRect2Region();												//最重要的函数之一，size意思是分炼，就是把矩形分到Region里，这是最初的那个函数
	void setHSV(IplImage *img);											// 将图像复制到函数的私有成员_hsv
	bool averageRectValue(SeqRect *prect,int *average,int *var);		//Get Rect HSV average value and Variance value 
public:		//Region	区域及感兴趣的区域（仅仅是一个区域范围）
	CvRect generateConnectedRegion(RegionSizer *region);				// 合并连通域
	int setRegionBorder(RegionSizer *region,ROIRect *proi);				// 重载
	RegionSizer *getRegion(int id);										//通过ID获取Region
	RegionSizer *getROI();												// 判断矩形是不是合格，以及矩形所在区域是不是合格，可不可以当做ROI
	void detectDistance();

public:		//ROI Rect感兴趣区域的Rect集合（不是区域，是一些矩形集合）
	void createROIRectList(SeqRect *prect);								//常见ROI矩形集合链表
	ROIRect *getROIRect(int id);										//通过ID获取矩形集合
	ROIRect *getLastROIRect();											//获取矩形集合最后面

	void setROIRect(SeqRect *prect);	//已经不用						// 为矩形合格&&矩形所在区域合格的矩形，根据最初Region划分，建立成ROI
	void setROIRect(ROIRect *proi);										// 把roi矩形集合整合成ROI Region
	

protected:
	void setRegionBorder(RegionSizer *region,CvRect rect);				//区域范围扩展，如果rect+epsilon在区域外，则加
	void initRegion(CvRect rect,RegionSizer *region,int regionid);		//初始化Region
	void initRegion(CvRect rect,RegionSizer *region,RegionSizer *regionOld);	//重载初始化Region
	bool inRegion(CvRect rect,RegionSizer *region);								//判断是不是在某个_region
	void transformXY(CvRect rect,RegionSizer *region);							//将region里面的left right up down 分别对应rect的 x y width height
public:
	RegionSizer *_region;												//对所有的矩形判断生成区域。[没有合并连通域]
private:
	ROIRect *_roiRect;					//存区域内的Rect
	RegionSizer *_ROI;					//用来存感兴趣的区域，合并连通域之后的

	CSeqRect *_seqRect;					//用来存所有的Rect
	SeqRect *_rects;					//暂时没用，本来是想用区域Rect的
	IplImage *_hsv;
	int region_id;						//生成区域的id
};


#endif