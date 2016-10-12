#include <stdio.h>
#include <ctime>
#include "RectSizer.h"

CRectSizer::CRectSizer()
{
	region_id = 0;
	this->_region = (RegionSizer *)malloc(sizeof(RegionSizer));
	this->_ROI = (RegionSizer *)malloc(sizeof(RegionSizer));
	this->_ROI->next = NULL;
	this->_ROI->InnerCount = -1;

	this->_roiRect = (ROIRect *)malloc(sizeof(ROIRect));
	this->_roiRect->id = -1;
	this->_roiRect->InnerCount = 0;
	this->_roiRect->next = NULL;
	initRegion(cvRect(0,0,0,0),this->_region,region_id);
}
CRectSizer::CRectSizer(CSeqRect *seq)
{
	this->_seqRect = seq;
	region_id = 0;
	this->_region = (RegionSizer *)malloc(sizeof(RegionSizer));
	this->_rects = NULL;
	this->_ROI = (RegionSizer *)malloc(sizeof(RegionSizer));
	this->_ROI->next = NULL;
	this->_ROI->InnerCount = -1;

	this->_roiRect = (ROIRect *)malloc(sizeof(ROIRect));
	this->_roiRect->id = -1;
	this->_roiRect->InnerCount = 0;
	this->_roiRect->next = NULL;
	initRegion(this->_seqRect->seqNode->rect,this->_region,region_id);
}
CRectSizer::~CRectSizer()
{
	free(this->_region);
}
void CRectSizer::setHSV(IplImage *img)
{
	IplImage *hsv =  cvCloneImage(img);
	/*
	IplImage *_h =  cvCreateImage(cvGetSize(inImage),8,1);
	IplImage *_s =  cvCreateImage(cvGetSize(inImage),8,1);
	IplImage *_v =  cvCreateImage(cvGetSize(inImage),8,1);
	cvSplit(hsv,_h,_s,_v,NULL);
	*/
	cvCvtColor(img, hsv, CV_BGR2HSV);	
	this->_hsv = hsv;
}
void CRectSizer::initRegion(CvRect rect,RegionSizer *region,int regionid)
{
	region->left = rect.x-EPSILON_X;
	region->right = rect.x + EPSILON_X;
	region->up =	rect.y + EPSILON_Y;
	region->down =  rect.y - EPSILON_Y;
	region->id = regionid;
	region->InnerCount = 1;
	region->per_width =  rect.width;
	region->max_y =  rect.y + rect.height;
	region->min_y = rect.y;
	region->prev = region->next = NULL;
}
void CRectSizer::initRegion(CvRect rect,RegionSizer *region,RegionSizer *regionOld)
{
	region->left = rect.x-EPSILON_X;
	region->right = rect.x + EPSILON_X;
	region->up =	rect.y + EPSILON_Y;
	region->down =  rect.y - EPSILON_Y;
	region->id = region_id;
	region->InnerCount = 1;
	region->next = NULL;
	region->per_width =  rect.width;
	region->max_y =  rect.y + rect.height;
	region->min_y = rect.y;
	RegionSizer *tmp = regionOld;
	while(tmp->next)
	{
		tmp = tmp->next;
	}
	region->prev = tmp;
	tmp->next = region;
}
RegionSizer *CRectSizer::getROI()														//这个是主要的。
{
	SeqRect *prect = this->_seqRect->seqNode;

	while(prect) 
	{
		int average[3] = {0,0,0};
		int var[3] = {0,0,0};
		
		averageRectValue(prect,average,var);
		
		RegionSizer *cur_region = getRegion(prect->region_id);
		
		int innercount = cur_region->InnerCount;

		bool average_condition =  (average[0]>=150 && average[2]>=80) || (average[0] <= 150 && average[2]>=90 );	//因为opencv的HSV空间的H范围是0~180，红色的H范围大概是(0~8)∪(160,180),S饱和度很重要，一般是大于一个值,S过低就是灰色（参考值S>80)，V就亮度，过低就是黑色，过高就是白色(参考值220>V>50) http://zhidao.baidu.com/question/468254593.html 
		bool var_condition = var[0] < 1500 && var[2] > 30;
		bool count_condition = innercount>0 && innercount<10;			 				//字符个数
		bool height_condition = (cur_region->max_y - cur_region->min_y) > 16;
		bool condition = var_condition && average_condition && count_condition && height_condition;
		
		if(condition)
		{
			createROIRectList(prect);
		}
		prect = prect->next;
	}

	detectDistance();			//很重要，横向距离检测函数

	ROIRect *proi = this->_roiRect;
	while(proi)
	{
		setROIRect(proi);										//从proi转换到curROI里面
		proi = proi->next;
	}

	RegionSizer *curROI = this->_ROI;
	while(curROI!=NULL)											//生成区域	
	{
		CvRect tmpRect = generateConnectedRegion(curROI);
		transformXY(tmpRect,curROI);
		curROI = curROI->next;
	}
	return this->_ROI;
}
void CRectSizer::detectDistance()
{
	ROIRect *proi = this->_roiRect;
	while(proi)
	{
		
		for(int i=0;i<proi->InnerCount;i++)
		{
			proi->minDist[i] = INFINIT;
			for(int j=0;j<proi->InnerCount;j++)
			{	
				if(i!=j)
				{
					int md = abs(proi->rect[i].x - proi->rect[j].x);
					proi->minDist[i] =(proi->minDist[i] <= md) ? proi->minDist[i] : md;
				}
			}
		}
		if(proi->InnerCount == 1)
		{
			proi->minDist[0] = 0 ;
		}
		for(int k=0;k<proi->InnerCount;k++)
		{
			proi->minDist[k] -= cvRound(proi->aveWidth*4);		//假设条件，用3居然能去掉好多多余的，用2就不行	
			if(proi->minDist[k] > 0)							
			{
				proi->minDist[k] = ERROR_STATUS;
			}
			else
			{
				proi->minDist[k] = RIGHT_STATUS;
			}
		}
		proi = proi->next;
	}
}
ROIRect *CRectSizer::getROIRect(int id)								//返回ROI的id = id的ROIRect集合，如果_ROI->id=-1，返回ROI，如果没有，新建一个节点
{
	ROIRect *proi = this->_roiRect;
	if(proi->id == -1)
	{
		proi->id=id;
		return proi;
	}
	if(id == proi->id)
	{
		return proi;
	}
	while(proi->next)
	{
		proi = proi->next;
		if(id == proi->id)
		{
			return proi;
		}
	}
	ROIRect *newroi = (ROIRect *)malloc(sizeof(ROIRect));
	newroi->id=id;
	newroi->InnerCount = 0;
	newroi->next = NULL;
	proi->next = newroi;
	proi = newroi;
	return proi;
}
ROIRect *CRectSizer::getLastROIRect()								//已经不用
{
	ROIRect *proi = this->_roiRect;
	while(proi->next)
	{
		proi = proi->next;
	}
	return proi;
}
void CRectSizer::createROIRectList(SeqRect *prect)					//用来记录每个区域的矩形
{	
	SeqRect *tmpprect = prect;
	ROIRect *proi ;
	proi = getROIRect(tmpprect->region_id);// this->_roiRect;  //获取ROI的id是region_id的ROI, 不是则新建
	proi->aveWidth = 0;
	proi->rect[proi->InnerCount] = tmpprect->rect;
	proi->InnerCount++;
	proi->aveWidth = (proi->aveWidth > tmpprect->rect.width) ? proi->aveWidth :tmpprect->rect.width;// cvRound((proi->aveWidth*(proi->InnerCount-1) + tmpprect->rect.width)/proi->InnerCount);
	
}
void CRectSizer::setROIRect(ROIRect *proi)
{
	RegionSizer *curROI = this->_ROI;
	RegionSizer *oldROI =  this->_ROI;
	while(curROI)
	{ 		
 
		if(curROI->InnerCount == -1)
		{ 
			curROI->id = proi->id;
			int count = setRegionBorder(curROI,proi);
			curROI->InnerCount = count;
			break;
		}
		else
		{ 
			oldROI = curROI;
			curROI = curROI->next;
			if(curROI == NULL) 
			{
				RegionSizer *regionNew = (RegionSizer *)malloc(sizeof(RegionSizer));
				regionNew->prev = oldROI;
				oldROI->next = regionNew;
				regionNew->next=NULL;

				int count = setRegionBorder(regionNew,proi);
				regionNew->InnerCount = count;
				regionNew->id = proi->id;
			}
		}
	}

}

void CRectSizer::sizeRect2Region()								//将矩形框根据EPSILON分成不同的区域
{
	SeqRect *prect = this->_seqRect->seqNode;
	while(prect)
	{
		RegionSizer *region = this->_region;
		RegionSizer *regionOld =this->_region;
		while(region)
		{
			if(inRegion(prect->rect,region))					//判断是否在区域内
			{
					prect->region_id = region->id;
					break;
			}
			else												//不在这个节点的话，继续扫
			{
				regionOld = region;
				region = region->next;
			}
			if(region == NULL)									//彻底不在，新建一个区域范围
			{
				region_id ++;
				RegionSizer *regionNew = (RegionSizer *)malloc(sizeof(RegionSizer));
				initRegion(prect->rect,regionNew,regionOld);
				prect->region_id = regionNew->id;
			}
			
		}
		prect = prect->next;
	}
}
int CRectSizer::setRegionBorder(RegionSizer *region,ROIRect *proi) 
{
	int count = 0;
	for(int i=0; i<proi->InnerCount;i++)
	{
		if(0==i)
		{
			region->left	=	proi->rect[i].x - EPSILON_X;
			region->right	=	proi->rect[i].x + proi->rect[i].width +EPSILON_X;
			region->up		=	proi->rect[i].y + EPSILON_Y;
			region->down	=	proi->rect[i].y - EPSILON_Y;
			region->max_y	=	proi->rect[i].y + proi->rect[i].height;
			region->min_y	=	proi->rect[i].y;
		}
#if debug
		printf("xxx%d\n",proi->minDist[i]);
		printf("(%d,%d,%d,%d)\n",region->left,region->right,region->up,region->down);
#endif
		if(proi->minDist[i] == RIGHT_STATUS)
		{
			setRegionBorder(region,proi->rect[i]);
			count++;
		}
	}
	return count;

}
void CRectSizer::setRegionBorder(RegionSizer *region,CvRect rect)
{
			region->left  = (region->left  <= rect.x - EPSILON_X) ? (region->left ):(rect.x - EPSILON_X);
			region->right = (region->right >= rect.x + rect.width +  EPSILON_X) ? (region->right) : (rect.x + rect.width +  EPSILON_X);
			region->up	  = (region->up    >= rect.y + rect.height) ? (region->up   ):(rect.y + rect.height);
			region->down  = (region->down  <= rect.y - EPSILON_Y) ? (region->down ):(rect.y - EPSILON_Y);
			
			region->max_y = (region->max_y >= rect.y + rect.height) ? region->max_y :  rect.y + rect.height + EPSILON_Y;
			region->min_y = (region->min_y <= rect.y + rect.height) ? region->min_y :  rect.y + rect.height + EPSILON_Y;
			
			//region->per_width =(int)((region->per_width + rect.width)/2);									//近似平均值
			region->per_width = (region->per_width >=rect.width)? (region->per_width) : rect.width;			//取最大值保险
}
bool CRectSizer::inRegion(CvRect rect,RegionSizer *region)
{
#if debug
	printf("(%d,%d)(%d,%d,%d,%d)\n",rect.x,rect.y,region->left, region->right,region->down,region->up);

#endif
	if(rect.x >= region->left && rect.x <= region->right && rect.y >= region->down && rect.y <= region->up)
	{
		region->InnerCount ++;
		setRegionBorder(region,rect);
		return true;
	}
	else
	{
		return false;
	}
}

RegionSizer *CRectSizer::getRegion(int id)
{
	int num = 0 ;
	RegionSizer *region = this->_region;
	while (region)
	{
		if(num == id)
			break;
		num ++;
		region = region ->next;
	}
	return region;
}
//把left right up down 变成 x y width height
void CRectSizer::transformXY(CvRect rect,RegionSizer *region)
{
	region->left = rect.x;
	region->right = rect.y;
	region->up = rect.width;
	region->down = rect.height;
}

//将矩形生成ROI区域
CvRect CRectSizer::generateConnectedRegion(RegionSizer *region)
{
	if(!region)
	{
		return cvRect(0,0,0,0);
	}/////@@@@@@@@@@@@@@@@参数3
	int x = region->left + EPSILON_X-5 > 0 ? region->left + EPSILON_X-5  : 0;
	int y = region->down + EPSILON_Y-5 > 0 ? region->down + EPSILON_Y -5: 0;
	int width = region->right - EPSILON_X + 10 - (region->left + EPSILON_X)   ;// + region->per_width;
	int height = region->max_y + 10 - region->min_y ;
	return cvRect(x,y,width,height);
}

//需要先调用SetHSV
bool CRectSizer::averageRectValue(SeqRect *prect,int *average,int *var)  //average 平均值 var 方差
{
	if(!prect)
	{
		return false;
	}
	IplImage *img = this->_hsv;
	CvRect rect = prect->rect;
	int x,y;
	int sum[3]={0,0,0};
	long int sum2[3] = {0,0,0};
	int amount = 0;
	for(y=rect.y+4;y<rect.y+rect.height-4;y++)				//  图片矩形内元素均值 和 方差
	{
		 unsigned char* line = (unsigned char*)(img->imageData + y*img->widthStep);
		 for( x=rect.x + 4;x<rect.x+rect.width - 4;x++)
		 {
			 sum[0] = sum[0] + line[3*x + 0];
			 sum[1] = sum[1] + line[3*x + 1];
			 sum[2] = sum[2] + line[3*x + 2];
	
			 sum2[0] = sum2[0] + line[3*x + 0] * line[3*x + 0];
			 sum2[1]= sum2[1] + line[3*x + 1] * line[3*x + 1];
			 sum2[2] = sum2[2] + line[3*x + 2] * line[3*x + 2];
			 amount++;
		 }
	}	

	prect->avarage[0] = average[0] = (int)floor(sum[0] *1.0 / (amount *1.0));
	prect->avarage[1] = average[1] = (int)floor(sum[1] *1.0 / (amount *1.0));
	prect->avarage[2] = average[2] = (int)floor(sum[2] *1.0 / (amount *1.0));
	prect->var[0] = var[0] = (int)floor(sum2[0] * 1.0 / (amount * 1.0) - (average[0] * average[0] * 1.0 ));			//方差 E(X^2) - E(X)^2
	prect->var[1] = var[1] = (int)floor(sum2[1] * 1.0 / (amount * 1.0) - (average[1] * average[1] * 1.0 ));
	prect->var[2] = var[2] = (int)floor(sum2[2] * 1.0 / (amount * 1.0) - (average[2] * average[2] * 1.0 ));
	
	return true;
}


void CRectSizer::setROIRect(SeqRect *prect)		//已经不用了
{
	RegionSizer *curROI = this->_ROI;
	RegionSizer *oldROI =  this->_ROI;
	while(curROI)
	{
		if(curROI->InnerCount == -1)
		{
			initRegion(prect->rect,curROI,prect->region_id);
			break;
		}
		else if(prect->region_id == curROI->id)
		{
			setRegionBorder(curROI,prect->rect);
			curROI->InnerCount = curROI->InnerCount ++ ;
			break;
		}
		else
		{ 
			oldROI = curROI;
			curROI = curROI->next;
			if(curROI == NULL) 
			{
				RegionSizer *regionNew = (RegionSizer *)malloc(sizeof(RegionSizer));
				initRegion(prect->rect,regionNew,oldROI);
				regionNew->id = prect->region_id;
			}
		}
	}
}