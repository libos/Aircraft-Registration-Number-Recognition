#include "sobel.h"
 


CSobel::CSobel()
{
	rects = (SobelRects *)malloc(sizeof(SobelRects));
	rects->normal = false;
	rects->next = NULL;
	rects->prev = NULL; 
	last = rects;
}
void CSobel::Add(CvRect rect)
{	 
	if(rects->normal){
		SobelRects *rectnew = (SobelRects *)malloc(sizeof(SobelRects));
		rectnew->normal = true;
		rectnew->rect = rect;
		rectnew->next = NULL;
		rectnew->prev = last;
		last->next = rectnew;
		last = rectnew;
	}
	else
	{
		rects->normal = true;
		rects->rect = rect;
	}
}
int CSobel::getLength()
{
	int count = 0;
	SobelRects *tmp = this->rects;
	while(tmp != NULL)
	{	
		if(tmp->normal)
			count ++ ;
		tmp = tmp->next;
	}
	return count;
}

void CSobel::Sobel(IplImage *rgb){
	clock_t start = clock();
    int i,j;   
    CvScalar sc;   
	IplImage *copy_rgb = cvCloneImage(rgb);
    IplImage *img1, *img3;    
    IplImage *sobv, *sobvh;//,*sobh
    IplImage* sob_edge;
	IplImage* histImg;
    double thresh = -1;   

	img1 = cvCreateImage(cvGetSize(rgb),IPL_DEPTH_8U,1);   
	
    CvSize sz = cvGetSize(img1);   
    sob_edge = cvCreateImage(cvGetSize(img1),IPL_DEPTH_8U,1);   
    sobv = cvCreateImage(cvGetSize(img1),IPL_DEPTH_32F,1);   //sobh = cvCreateImage(cvGetSize(img1),IPL_DEPTH_32F,1);   
    sobvh = cvCreateImage(cvGetSize(img1),IPL_DEPTH_32F,1);   
    img3 = cvCloneImage(sobv);      
	cvCvtColor(rgb,img1,CV_BGR2GRAY); 
	IplImage *copy_gray = cvCloneImage(img1);
	histImg = cvCreateImage(cvGetSize(copy_gray),copy_gray->depth,1); 

	IplImage *binaryImg = cvCloneImage(img1);
	cvSmooth(binaryImg,binaryImg,CV_MEDIAN);
	cvThreshold(binaryImg,binaryImg,0,255,CV_THRESH_OTSU | CV_THRESH_BINARY_INV );
 
	
    cvSobel(img1,sobv,0,1,7);    
//	cvSobel(img1,sobh,1,0,7);    
    
	cvSet(img3,cvScalar(8,0,0,0),0);   //这个在干嘛？
    cvDiv(sobv,img3,sobv,1);
  //  cvDiv(sobh,img3,sobh,1);    

    for(i=0;i<sz.height;i++){   
        for(j=0;j<sz.width;j++){   
			double val =   pow((cvGet2D(sobv,i,j).val[0]),2) ;// + pow((cvGet2D(sobh,i,j).val[0]),2);//
            cvSet2D(sobvh,i,j,  cvScalar(val,0,0,0));     
        }   
    }

    thresh = (6*cvAvg(sobvh).val[0]); 
    cvCmpS(sobvh,thresh,sob_edge,CV_CMP_GE);   

	int valueErode[16] = {	0,0,0,0,
							0,1,0,0,
							0,1,1,0,
							0,0,0,0
						};
	int valueDilate[25] = {	0,0,0,0,0,
							1,1,1,1,1,
							1,1,1,1,1,
							1,1,1,1,1,
							0,0,0,0,0
						};
    IplConvKernel* elementErode = 0;
	IplConvKernel* elementDilate= 0;
    int cols = 4,rows = 4 ,anchor_x = 2 ,anchor_y = 2;	 
    elementErode = cvCreateStructuringElementEx(cols , rows , anchor_x,anchor_y , CV_SHAPE_CUSTOM,valueErode);
	cols = 5;
	rows = 5;
	anchor_x = 4;
	anchor_y = 3;
	elementDilate = cvCreateStructuringElementEx(cols , rows , anchor_x,anchor_y , CV_SHAPE_CUSTOM,valueDilate);
 	cvErode(sob_edge,sob_edge,elementErode); 
 	cvDilate(sob_edge,sob_edge,elementDilate,5);
	cvRectangle(sob_edge,cvPoint(0,0),cvPoint(sob_edge ->width-MARGIN_OFFSET-10,sob_edge ->height-MARGIN_OFFSET-10),cvScalarAll(0),10);//边框
 

	IplImage *copy_sob = cvCloneImage(sob_edge);
	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSeq* contours = NULL , *contoursTemp=NULL;
	int total = cvFindContours( copy_sob, storage, &contours, 	sizeof(CvContour),CV_RETR_LIST, CV_CHAIN_APPROX_NONE );
	contoursTemp = contours;
    for(;contoursTemp->h_next!=NULL ; contoursTemp=contoursTemp->h_next)
    { 
		CvRect rect = cvBoundingRect( contoursTemp, 0 ); 
		int area = rect.width * rect.height;
		if(area < 2500 || rect.height < MARGIN_OFFSET)
			continue;
		bool positive = (((rect.x - MARGIN_OFFSET) <= 0) || ((rect.y - MARGIN_OFFSET) <=0 ));
		bool inImage = ( (rect.x + rect.width + MARGIN_OFFSET >= copy_rgb->width) || (rect.y + rect.height + MARGIN_OFFSET >= copy_rgb->height ) );
		if(positive || inImage)
			continue;

		int max = rect.width > rect.height ? rect.width : rect.height;
		int min = max ==  rect.width ? rect.height : rect.width;
		double wh_ratio =  (max *1.0)/min ;
		if( wh_ratio  > 8 || wh_ratio < 2 )
			continue;
		
		cvSetImageROI(sob_edge,rect);
		CvScalar tmp = cvSum(sob_edge);
		float ratio = (tmp.val[0]/255) / area; 
		cvResetImageROI(sob_edge);
	 	if(ratio < 0.4)
	 		continue;

		if(this->maxRect.width > rect.width && this->maxRect.height > rect.height)
		{
			this->maxRect = rect;
		}
		int inner_count = getPartInnerCount(binaryImg,rect);
		if(inner_count < 5 || inner_count > 15 )			//加上最外面还有一个轮廓
			continue;

		cvZero(histImg);

 

		printf("(x,y,w,h):(%d,%d,%d,%d)\ninnercount:%d\n", rect.x,rect.y,rect.width,rect.height,inner_count);
		
		Add(rect);
		cvRectangle( sob_edge, cvPoint( rect.x, rect.y ),cvPoint( rect.x + rect.width, rect.y + rect.height ), cvScalarAll(255), 0 );   
		cvRectangle( copy_rgb, cvPoint( rect.x, rect.y ),cvPoint( rect.x + rect.width, rect.y + rect.height ), cvScalarAll(255), 0 );   
	}
	

	clock_t stop = clock();
	int rectCount = getLength();
	if(rectCount == 0)
	{
		Add(this->maxRect);
	}
	printf("Sobel Done.Extracted %d in %lf s.\n",rectCount,(double)(stop - start) / CLOCKS_PER_SEC);
//	cvNamedWindow("inImage"); 
//	cvShowImage("inImage",copy_rgb);
//	cvNamedWindow("edge",1);
//	cvShowImage("edge",sob_edge);   
 
//  cvWaitKey(0);    
 	cvReleaseImage(&sob_edge);
	cvReleaseImage(&copy_gray);
	cvReleaseImage(&copy_rgb);
	cvReleaseImage(&copy_sob);
	cvReleaseImage(&histImg);
	cvReleaseImage(&img1); 
	cvReleaseImage(&img3);
 	cvReleaseImage(&sobv);
 	cvReleaseImage(&sobvh);
}
int CSobel::getPartInnerCount(IplImage *img,CvRect rect)
{ 
	cvSetImageROI(img,rect);
	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSeq* contours = NULL ;
	int total = cvFindContours(img, storage, &contours, sizeof(CvContour),CV_RETR_LIST, CV_CHAIN_APPROX_NONE );

	return total;
}
