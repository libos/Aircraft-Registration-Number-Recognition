// GTL.cpp : Defines the entry point for the console application.
//
//////////////////////////////////////////////////////////////////////////////////

#include "detect.h"


#define BORDER 10
//#include "svm.h"
//CvMemStorage* storage;
//////////////////////////////////////////////////////////////////////////////////
void Process(CSeqRect *seqRect,	FeaImage *fea,char *result,int inImageArea,CvRect *rectReturn,bool useSvm,bool onlyFind)
{ 
	SeqRect *prect = seqRect->seqNode;
#if 0
	while(prect != NULL)
	{
		printf("rect(%d,%d,%d,%d)\n",prect->rect.x,prect->rect.y,prect->rect.width,prect->rect.height);
	  	cvRectangle(fea->RGB, cvPoint(prect->rect.x,prect->rect.y),cvPoint(prect->rect.x+prect->rect.width,prect->rect.y+prect->rect.height),CV_RGB(255,0, 0), 1, CV_AA, 0);
		prect =prect->next;
	}
#endif
	prect  = seqRect->seqNode;
	CRectSizer *sizer = new CRectSizer(seqRect);
	sizer->setHSV(fea->RGB);
	sizer->sizeRect2Region();
	RegionSizer *cc = sizer->_region;
#if 0
	while(cc != NULL)
	{
		printf("region:(%d,%d,%d,%d)\n",cc->left+ EPSILON_X,cc->down+EPSILON_Y,cc->right - EPSILON_X,cc->up );
		//CvRect cx = cvRect(cc->left,cc->down,cc->right-cc->left,cc->up- cc->down);
 		cvRectangle(fea->RGB, cvPoint(cc->left + EPSILON_X,cc->down +EPSILON_Y),cvPoint(cc->right -EPSILON_X,cc->up  ),CV_RGB(255,255, 0), 1, CV_AA, 0);
		cc = cc->next;
	}
#endif
	RegionSizer *region = sizer->_region;	
	region = sizer->_region;
	RegionSizer *roi = sizer->getROI();
	CharOCR *ocr = new CharOCR();

	while(roi)
	{/////@@@@@@@@@@@@@@@@参数4 
		float arearatio = ((roi->up * roi->down * 1.0 ) / inImageArea) ;
		bool area =  arearatio < 0.01;
		if(roi->InnerCount >= 1 && (roi->up / roi->down > 2) && area )												//最后在判断一次内部个数
		{
			CvRect tmp = cvRect(roi->left,roi->right,roi->up,roi->down);
			//printf("ROI:(%d,%d,%d,%d)\n",tmp.x,tmp.y,tmp.width,tmp.height);
			
			if(rectReturn != NULL)
			{				
				rectReturn->x = tmp.x;
				rectReturn->y = tmp.y;
				rectReturn->width = tmp.width;
				rectReturn->height = tmp.height;
			}
			if(onlyFind)
			{
				roi = roi->next;
				continue;
			}
			cvSetImageROI(fea->RGB,tmp);
			

			if(useSvm)
			{
				ocr->SVMRecognize(fea->RGB,result);
			}	
			else
			{
				ocr->Recognize(fea->RGB,result);
			}
		 
			if(strlen(result) > 3)
			{
				//printf("");
				//::AfxMessageBox(result);
			}
			else
			{
				::AfxMessageBox("Recognize Error");
			}

			cvResetImageROI(fea->RGB);
		 	//cvRectangle(fea->RGB, cvPoint(tmp.x,tmp.y),cvPoint(tmp.x+tmp.width,tmp.y+tmp.height),CV_RGB(255,255, 255), 1, CV_AA, 0);
		}
		roi = roi->next;
	}
//	cvNamedWindow("hi");
//	cvShowImage("hi",fea->RGB);
//	cvWaitKey(0);
}

int Detect(IplImage *inImage,char *result, bool useSvm ,CvRect *rect,bool onlyFind)
{	 
	if(NULL == inImage) {printf("open file failed!\n"); return 0;}
	int area = inImage->width * inImage->height;

	CSobel *sobel = new CSobel();
	sobel->Sobel(inImage);
 
	SobelRects *tmp = sobel->rects;
	char *oldresult;
	while(tmp != NULL)
	{
		int x = tmp->rect.x - 10;
		int y = tmp->rect.y - 10;
		int w = tmp->rect.width  + 60;/////@@@@@@@@@@@@@@@@参数6
		int h = 0;
		if(x < 0)
			x = 10;
		if(y < 0)
			y = 10;
		int half = w > h ? w / 16 : h /16 ;
		w = half * 16;
		h = half * 9;		
		if(x + w > inImage->width)
		{
			w =  inImage->width - x - 10;
		}
		if(y+h > inImage->height)
		{
			h =  inImage->height - y - 10;
		}
		
		
		printf("16:9 ====>  (%d,%d)\n",w,h);
		CvRect tmprect = cvRect(x,y,w,h);
		IplImage *part = cvCreateImage(cvSize(tmprect.width,tmprect.height),8,3);
		
		cvSetImageROI(inImage,tmprect);
		cvCopy(inImage,part);
		cvResetImageROI(inImage);
		
		FeaImage fea;
		
		fea.Wid = part->width;
		fea.Hei = part->height;

	 
		fea.RGB = cvCloneImage(part);
		fea.LUV = cvCloneImage(part);
		fea.QUA = cvCreateImage(cvGetSize(part),8,1);
		cvCvtColor(part, fea.LUV, CV_RGB2Luv);
		 
		for(int c = 0;c< 4; c++){
			fea.mserMap[c] = cvCreateImage(cvGetSize(part),8,1);
		} 

		cvSplit(fea.LUV,fea.mserMap[0],fea.mserMap[1],fea.mserMap[2],NULL);
 

		CSeqRect *seqRect = new CSeqRect(fea.RGB->width,fea.RGB->height);
		MSER2CC(fea,seqRect);
 
		Process(seqRect,&fea,result,area,rect,useSvm,onlyFind);
		if(rect != NULL && result !=oldresult)
		{	
			rect->x += tmprect.x ;
			rect->y += tmprect.y; 
		}
		//WriteResults(fea,resDir,fileName,1);

		oldresult = result;
		cvReleaseImage(&fea.RGB);
		cvReleaseImage(&fea.LUV);	
		cvReleaseImage(&fea.QUA);
			 
		for(c = 0;c< 4; c++){
			cvReleaseImage(&fea.mserMap[0]);
		} 

		tmp = tmp->next;
		
	}
  
	return 1;

}

//////////////////////////////////////////////////////////////////////////////