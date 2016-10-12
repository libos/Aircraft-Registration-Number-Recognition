#include "CharOCR.h"
#include <iostream>
#include <fstream>
CharOCR::CharOCR()
{

}
CharOCR::~CharOCR()
{
}
void CharOCR::GetFakeTrainingSample(IplImage *test,char *prefix)
{

	CSeqRect *seq = new CSeqRect();
	IplImage *gray = Split(test,seq);		
	SeqRect *seqrect = seq->seqNode;
	int i=0;
	while(seqrect)
	{
		cvSetImageROI(gray,seqrect->rect);
		char name[255];
		sprintf(name,"traintmp\\%s_%d.jpg",prefix,i);
		cvSaveImage(name, gray);
		cvResetImageROI(gray);
		i++;
		seqrect = seqrect->next;
	}
}
char CharOCR::Classlabel2Char(double label)
{
	return (char)label;
}
void CharOCR::GetTrainHu(Knn *knn)
{
	CString strPath;
	char model[MODEL_NUM] = MODEL;
	for(int i=0;i<MODEL_NUM-1;i++)
	{
		strPath.Format("train\\%c.jpg",model[i]);
		IplImage *tmp = cvLoadImage(strPath.GetBuffer(0));
		IplImage *tmp2 =  cvCreateImage(cvGetSize(tmp),8,1);
		
		cvCvtColor(tmp, tmp2, CV_BGR2GRAY);
		cvThreshold(tmp2,tmp2,100,255,CV_THRESH_BINARY);
/*		
	CvMemStorage* storage = cvCreateMemStorage(0);
		CvSeq* contours = NULL;		
		cvFindContours( tmp3, storage, &contours, sizeof(CvContour),CV_RETR_LIST, CV_CHAIN_APPROX_NONE );
		CvRect rect = cvBoundingRect( contours, 0 );
		IplROI ROI = cvRectToROI(rect,0);
		tmp2->roi = &ROI;
*/
		IplImage *tmp3 = cvCloneImage(tmp2);
		int values[16] = {	0,0,0,0,
							0,0,1,0,
							0,1,1,0,
                            0,0,0,0
                          };

        IplConvKernel* element = 0;								//声明结构元素

        int cols = 4,rows = 4 ,anchor_x = 2 ,anchor_y = 2;		//结构元素的参数
        element = cvCreateStructuringElementEx(cols , rows , anchor_x,anchor_y , CV_SHAPE_CUSTOM,values);//创建结构元素
        for(int j=0;j<1;j++)
		{
			CvMoments moment;
			cvMoments(tmp2,&moment,300);		//binary =>把0,255换成0,1
			CvHuMoments humoment;
			cvGetHuMoments(&moment,&humoment);
			knn->Import((double)i,humoment);
			cvErode(tmp2,tmp2,element);//腐蚀		//相反黑白和规则相反了，所以Erode成了Dilate了
		}
		//	printf("(%f,%f,%f,%f,%f,%f,%f)\n",humoment.hu1, humoment.hu2, humoment.hu3, humoment.hu4, humoment.hu5, humoment.hu6, humoment.hu7);

//		cvNamedWindow(strPath.GetBuffer(0));
//		cvShowImage(strPath.GetBuffer(0),tmp2);
  
	//	printf("%s\n",strPath.GetBuffer(0));
	}
}
void CharOCR::GenerateTrainData()
{
	CString strPath,data;
	
	CFile file;
	file.Open("..\\template\\train.data",CFile::modeCreate|CFile::modeWrite|CFile::modeNoTruncate,NULL);
//	file.SeekToEnd();

	char model[MODEL_NUM] = MODEL;
	for(int i=0;i<MODEL_NUM-1;i++)
	{
		strPath.Format("train\\%c.jpg",model[i]);
		IplImage *tmp = cvLoadImage(strPath.GetBuffer(0));
		IplImage *tmp2 =  cvCreateImage(cvGetSize(tmp),8,1);
		
		cvCvtColor(tmp, tmp2, CV_BGR2GRAY);
		cvThreshold(tmp2,tmp2,100,255,CV_THRESH_BINARY);
/*		//重新截取	
		CvMemStorage* storage = cvCreateMemStorage(0);
		CvSeq* contours = NULL;		
		cvFindContours( tmp3, storage, &contours, sizeof(CvContour),CV_RETR_LIST, CV_CHAIN_APPROX_NONE );
		CvRect rect = cvBoundingRect( contours, 0 );
		IplROI ROI = cvRectToROI(rect,0);
		tmp2->roi = &ROI;
*/
		IplImage *tmp3 = cvCloneImage(tmp2);
		int values[16] = {	0,0,0,0,
							0,0,1,0,
							0,1,1,0,
                            0,0,0,0
                          };

        IplConvKernel* element = 0;								//声明结构元素

        int cols = 4,rows = 4 ,anchor_x = 2 ,anchor_y = 2;		//结构元素的参数
        element = cvCreateStructuringElementEx(cols , rows , anchor_x,anchor_y , CV_SHAPE_CUSTOM,values);//创建结构元素
        for(int j=0;j<1;j++)
		{
			CvMoments moment;
			cvMoments(tmp2,&moment,300);		//binary =>把0,255换成0,1
			CvHuMoments humoment;
			cvGetHuMoments(&moment,&humoment);
			data.Format("%c:%d %f %f %f %f %f %f %f\n",model[i],(int)model[i],humoment.hu1, humoment.hu2, humoment.hu3, humoment.hu4, humoment.hu5, humoment.hu6, humoment.hu7);
			file.Write(data,data.GetLength());  
			cvErode(tmp2,tmp2,element);//腐蚀		//相反黑白和规则相反了，所以Erode成了Dilate了
		}
	}
	file.Close();
}
bool CharOCR::FileExist(CString fileName)
{
   CFile file;
   CFileStatus status;

   if(file.GetStatus(fileName,status))
       return true;
   else
       return false;
}
//单独加一个train数据
bool CharOCR::AddToTrainData(char *filename)
{
	char label = filename[0];
	if(AddToTrainData(label,filename))
		return true;
	else
		return false;
}

//单独加一个train数据
bool CharOCR::AddToTrainData(char label,char *filename)
{
	CString strPath,data;
	
	CFile file;
	file.Open("train.data",CFile::modeCreate|CFile::modeWrite|CFile::modeNoTruncate,NULL);
	file.SeekToEnd();
	
	strPath.Format("train\\%s.jpg",filename);
	if(!FileExist(strPath))
	{
		return false;				//文件不存在
	}
	IplImage *tmp = cvLoadImage(strPath.GetBuffer(0));
	IplImage *tmp2 =  cvCreateImage(cvGetSize(tmp),8,1);
	
	cvCvtColor(tmp, tmp2, CV_BGR2GRAY);
	cvThreshold(tmp2,tmp2,100,255,CV_THRESH_BINARY);
	//假定样本集不需要重新截取、不需要腐蚀了
	CvMoments moment;
	cvMoments(tmp2,&moment,300);		//binary =>把0,255换成0,1
	CvHuMoments humoment;
	cvGetHuMoments(&moment,&humoment);
	data.Format("%c:%d %f %f %f %f %f %f %f\n",label,(int)label,humoment.hu1, humoment.hu2, humoment.hu3, humoment.hu4, humoment.hu5, humoment.hu6, humoment.hu7);
	file.Write(data,data.GetLength());  
	file.Close();
	return true;
/*		//重新截取	
	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSeq* contours = NULL;		
	cvFindContours( tmp3, storage, &contours, sizeof(CvContour),CV_RETR_LIST, CV_CHAIN_APPROX_NONE );
	CvRect rect = cvBoundingRect( contours, 0 );
	IplROI ROI = cvRectToROI(rect,0);
	tmp2->roi = &ROI;

	IplImage *tmp3 = cvCloneImage(tmp2);
	int values[16] = {	0,0,0,0,
						0,0,1,0,
						0,1,1,0,
                        0,0,0,0
                      };

    IplConvKernel* element = 0;								//声明结构元素

    int cols = 4,rows = 4 ,anchor_x = 2 ,anchor_y = 2;		//结构元素的参数
    element = cvCreateStructuringElementEx(cols , rows , anchor_x,anchor_y , CV_SHAPE_CUSTOM,values);//创建结构元素
    for(int j=0;j<1;j++)
	{
		CvMoments moment;
		cvMoments(tmp2,&moment,300);		//binary =>把0,255换成0,1
		CvHuMoments humoment;
		cvGetHuMoments(&moment,&humoment);
		data.Format("%c:%f %f %f %f %f %f %f\n",model[i],humoment.hu1, humoment.hu2, humoment.hu3, humoment.hu4, humoment.hu5, humoment.hu6, humoment.hu7);
		file.Write(data,data.GetLength());  
		cvErode(tmp2,tmp2,element);//腐蚀		//相反黑白和规则相反了，所以Erode成了Dilate了
	}
*/

}
IplImage *CharOCR::Split(IplImage *img,CSeqRect *seq)
{
	IplImage *gray1 = cvCreateImage(cvGetSize(img),8,1);
	cvCvtColor(img, gray1, CV_BGR2GRAY);	
	IplImage *gray2 = cvCreateImage(cvGetSize(gray1),8,1);
	int thresh = graythresh(gray1);

	int type = CV_THRESH_BINARY;
	cvThreshold(gray1,gray2,thresh,255,type);

	if(cvFloor(cvGetReal2D(gray2,0,0)) == 0 && cvFloor(cvGetReal2D(gray2,gray2->height-1,gray2->width-1)) == 0 )
	{
		 type = CV_THRESH_BINARY_INV;
		 cvThreshold(gray1,gray1,thresh,255,type);
		 gray2 = cvCloneImage(gray1);
	}
	else
	{
		gray1 = cvCloneImage(gray2);
	}
 
	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSeq* contours = NULL , *contoursTemp=NULL;
	int total = cvFindContours( gray2, storage, &contours, 	sizeof(CvContour),CV_RETR_LIST, CV_CHAIN_APPROX_NONE );
	contoursTemp = contours;
    for(;contoursTemp->h_next!=NULL ; contoursTemp=contoursTemp->h_next)
    {
		CvRect rect = cvBoundingRect( contoursTemp, 0 );
		//printf("%d,%d,%d,%d\n",rect.x,rect.y,rect.width,rect.height);
		seq->AddNode(rect,true);
		//printf("%d,%d,%d\n",total, rect.y , rect.x);
		//cvRectangle( gray1, cvPoint( rect.x, rect.y ),cvPoint( rect.x + rect.width, rect.y + rect.height ), cvScalar(0,0,0), 0 );   
	}

 	//cvNamedWindow("CCCCC");
	//cvShowImage("CCCCC",gray1);
	return gray1;
}


int CharOCR::Recognize(IplImage *img,char *result)
{
	Knn knn;

	CSeqRect *seq = new CSeqRect();
	IplImage *gray = Split(img,seq);		
	
//	GetTrainHu(&knn);
	knn.GetTrainData();
	
	SeqRect *seqrect = seq->seqNode;

	char *a[20] = {"p1","p2","p3","p4","p5","p6","p7","p8","p9"};	//printf("%d,%d,%d,%d\n",seqrect->rect.x,seqrect->rect.y,seqrect->rect.width,seqrect->rect.height);
	
	int i =0;
	while(seqrect)
	{
	    cvSetImageROI(gray,seqrect->rect);
#if 0
		int values[16] = {0,0,0,0,0,0,1,0,0,1,1,0,0,0,0,0};
        IplConvKernel* element = 0;								//声明结构元素
        int cols = 4,rows = 4 ,anchor_x = 2 ,anchor_y = 2;		//结构元素的参数
        element = cvCreateStructuringElementEx(cols , rows , anchor_x,anchor_y , CV_SHAPE_CUSTOM,values);//创建结构元素
		for(int o=0;o<3;o++)
			cvDilate(gray,gray,element);//腐蚀
#endif
		double m00,m10,m01;
		CvMoments moment;
		/*
		/	m00, 0阶矩m10, m01,1阶矩 m20, m11, m02,2阶矩 m30, m21, m12, m03,3阶矩普通矩阵
		/	mu20, mu11, mu02,2阶中心矩 mu30, mu21, mu12, mu03：3阶中心距 中心矩-平移不变性
		*/
		cvMoments(gray,&moment,300);	//binary =>把0,255换成0,1
		m00 = cvGetSpatialMoment(&moment,0,0);
		m10 = cvGetSpatialMoment(&moment,1,0);
		m01 = cvGetSpatialMoment(&moment,0,1);

		//计算质心坐标
		float x = (float)(m10/m00);
		float y = (float)(m01/m00);

		//定义hu矩变量
		CvHuMoments humoment;

		//计算hu矩
		cvGetHuMoments(&moment,&humoment);
		vectors	unknow = knn.CreateVector(-1,humoment);
		double classlabel = knn.Classify(unknow);
		//cout<<"The classlabel of the testv is:	";
		result[i]=Classlabel2Char(classlabel);
		//cout<<result[i]<<endl;
		//printf("(%f,%f,%f,%f,%f,%f,%f)\n",humoment.hu1, humoment.hu2, humoment.hu3, humoment.hu4, humoment.hu5, humoment.hu6, humoment.hu7);
		
	 	//cvNamedWindow(a[i]);
	 	//cvShowImage(a[i],gray);
		
		cvResetImageROI(gray);
		i++;
		seqrect = seqrect->next;
	}

	//cvWaitKey(0);
	return 1;
}


int CharOCR::SVMRecognize(IplImage *img,char *result)
{
	TCHAR exePath[256] = { 0 };  ///exe文件路径  
	DWORD dwResult  = GetModuleFileName( AfxGetApp()->m_hInstance , exePath , 256 );  
	CString path = exePath;  
	CString apName  = AfxGetApp()->m_pszAppName;  
	path = path.Left(path.ReverseFind( _T('\\') ));  
	path = path.Left(path.ReverseFind( _T('\\') ));  

	CString filex,filexx;
	filexx.Format("%s\\templateh\\svmtrain.model",path);

	CSVM *svm = new CSVM(RBF,32.0,2.0);  
	char *modelfile = filexx.GetBuffer(0);
	svm->setModelFile(modelfile);
	svm->setSaveModel(true);
//	svm->setTrainDataFile("svm_train.data");
//	svm->setTestDataFile("test.data");
//	svm->setScaledTrainData("scaledtrain.data");
	filex.Format("%s\\templateh\\svmtrain.scale",path);
	svm->setScaleTemplate(filex.GetBuffer(0));

	//svm->ScaleTrainData("train.scale");
	//svm->readTrainData(true);
	//svm->getModel();
	
	svm->loadModel();
	

	CSeqRect *seq = new CSeqRect();
	IplImage *gray = Split(img,seq);		
	
	SeqRect *seqrect = seq->seqNode;

	char *a[20] = {"p1","p2","p3","p4","p5","p6","p7","p8","p9"};	//printf("%d,%d,%d,%d\n",seqrect->rect.x,seqrect->rect.y,seqrect->rect.width,seqrect->rect.height);
	
	int i =0;
	while(seqrect)
	{
	    cvSetImageROI(gray,seqrect->rect);
#if 0
		int values[16] = {0,0,0,0,0,0,1,0,0,1,1,0,0,0,0,0};
        IplConvKernel* element = 0;								//声明结构元素
        int cols = 4,rows = 4 ,anchor_x = 2 ,anchor_y = 2;		//结构元素的参数
        element = cvCreateStructuringElementEx(cols , rows , anchor_x,anchor_y , CV_SHAPE_CUSTOM,values);//创建结构元素
		for(int o=0;o<3;o++)
			cvDilate(gray,gray,element);//腐蚀
#endif
		double m00,m10,m01;
		CvMoments moment;
		/*
		/	m00, 0阶矩m10, m01,1阶矩 m20, m11, m02,2阶矩 m30, m21, m12, m03,3阶矩普通矩阵
		/	mu20, mu11, mu02,2阶中心矩 mu30, mu21, mu12, mu03：3阶中心距 中心矩-平移不变性
		*/
		cvMoments(gray,&moment,300);	//binary =>把0,255换成0,1
		m00 = cvGetSpatialMoment(&moment,0,0);
		m10 = cvGetSpatialMoment(&moment,1,0);
		m01 = cvGetSpatialMoment(&moment,0,1);

		//定义hu矩变量
		CvHuMoments humoment;

		//计算hu矩
		cvGetHuMoments(&moment,&humoment);		
		svm_node *x;
		int max_nr_attr =64;
		x = (struct svm_node *) malloc(max_nr_attr*sizeof(struct svm_node)); 
		x[0].index = 1;
		x[0].value = humoment.hu1;

		x[1].index = 2;
		x[1].value = humoment.hu2;

		x[2].index = 3;
		x[2].value =  humoment.hu3;

		x[3].index = 4;
		x[3].value =  humoment.hu4;

		x[4].index = 5;
		x[4].value =  humoment.hu5;

		x[5].index = 6;
		x[5].value =  humoment.hu6;

		x[6].index = 7;
		x[6].value =  humoment.hu7;
		
		x[7].index = -1;
		svm_vector sv;
		sv.label = -1;
		sv.x = x;
		svm->ScaleTestData(&sv);
		
		double classlabel = svm->Classify(sv);
		//cout<<"The classlabel of the testv is:	";

		result[i]=Classlabel2Char(classlabel);
		//cout<<result[i]<<endl;
		//printf("(%f,%f,%f,%f,%f,%f,%f)\n",humoment.hu1, humoment.hu2, humoment.hu3, humoment.hu4, humoment.hu5, humoment.hu6, humoment.hu7);
		
	//	cvNamedWindow(a[i]);
	//	cvShowImage(a[i],gray);
		free(x);
		cvResetImageROI(gray);
		i++;
		seqrect = seqrect->next;
	}
 
	//cvWaitKey(0);
	return 1;
}