#include <stdio.h>
#include "feature.h" 
#include "mser.h"
#include <ctime>
#include "cv.h"
#include "highgui.h"
#include "feaquan.h"

using namespace std;
//////////////////////////////////////////////////////////////////////////////////////////////////
int LVQ(FeaImage fea)
{
	int i; 
	int imgwid = fea.Wid;
	int imghei = fea.Hei;

	float * LUV = new float[imghei*imgwid*3];
	for(i = 0; i < imghei*imgwid*3; i++)
	{
		LUV[i] = (float)(int)(BYTE) fea.LUV->imageData[i];
	}

	unsigned char* feamap=  new unsigned char[imghei*imgwid];
	float** feaquan      =  FMatrix(256,3);
	int N                =  FeaQuan(LUV,feaquan,imghei,imgwid,3,1);	
	GetFeaMap(LUV,feamap,imgwid,imghei,feaquan,3,N);

	for(i = 0; i < imghei*imgwid; i++)
	{
		fea.QUA->imageData[i] = (BYTE)(int) (feamap[i]);
		fea.mserMap[3]->imageData[i] = (BYTE)(int) (feamap[i]);
	}

	free(feamap);
	free(LUV);
	FreeFMatrix(feaquan,256);	
	
	return 0;
}
//////////////////////////////////////////////////////////////////////////////////////////////////
int ComputerGradientMag(FeaImage fea)
{
	unsigned char *img = (BYTE*)fea.RGB->imageData;
	int  nBand = fea.RGB->nChannels;
	BYTE *gradient =(BYTE*)fea.gradientMag->imageData;
	int wid = fea.Wid;
	int hei = fea.Hei;
	
	int h,w,c;
	long pos1,pos2,pos3,pos4,loc;
	int max_dx,max_dy,max;
	int orient = -1;
	int g_x,g_y;
	long img_size = wid * hei;	
	
	///////////////////////////////////////////////////////////////
	//计算每个像素的梯度
	//gradient 一定要初始化
	for(int i = 0; i < wid*hei; i++){
		gradient[i] = 0;
	}
	for(h = 1; h < hei - 1; h++)
	{
		for(w = 1; w < wid - 1; w++)
		{
			max_dx = 0;         //注意!一定要在c循环外初始化!
			max_dy = 0;
			max = 0;
			
			loc = h * wid + w;	
			
			for(c = 0 ; c< nBand; c++)                //颜色分量
			{
				pos1 = (loc - 1) * nBand + c;
				pos2 = (loc + 1) * nBand + c;
				pos3 = (loc - wid) * nBand + c;
				pos4 = (loc + wid) * nBand + c;				
				g_x = (int)(img[pos2] - img[pos1]);    //梯度
				g_y = (int)(img[pos4] - img[pos3]);    //梯度								
				int temp = g_x*g_x + g_y*g_y;
				
				//计算梯度和最大的那个颜色通道
				if(temp > max)
				{
					max_dx = g_x;
					max_dy = g_y;
					max   = temp;					
				}
			}			

			float magnitude = sqrt(max_dx*max_dx + max_dy*max_dy);	
			gradient[loc] = (BYTE)(int)magnitude;	
		} //for-w
	} //for-h
	return 1;	
}
//////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
CvRect extractBox(const MSER::Region & region, int width, int height, int depth, IplImage *image)
{
	//Centroid (mean)
	const double x = region.moments_[0] / region.area_;//moments (x, y, x^2, xy, y^2)
	const double y = region.moments_[1] / region.area_;
	
	//Covariance matrix [a b; b c]   协方差矩阵[A B B C]
	const double a = region.moments_[2] / region.area_ - x * x;
	const double b = region.moments_[3] / region.area_ - x * y;
	const double c = region.moments_[4] / region.area_ - y * y;
	
	//Eigenvalues of the covariance matrix   协方差矩阵的特征值
	const double d  = a + c;
	const double e  = a - c;
	const double f  = sqrt(4.0 * b * b + e * e);
	const double e0 = (d + f) / 2.0; // First eigenvalue
	const double e1 = (d - f) / 2.0; // Second eigenvalue
	
	// Desired norm of the eigenvectors          期望中的特征向量范数
	const double e0sq = sqrt(e0);
	const double e1sq = sqrt(e1);
	
	// Eigenvectors
	double v0x = e0sq;
	double v0y = 0.0;
	double v1x = 0.0;
	double v1y = e1sq;
	
	if (b) {
		v0x = e0 - c;
		v0y = b;
		v1x = e1 - c;
		v1y = b;
		 
		// Normalize the eigenvectors         归一化的特征向量
		const double n0 = e0sq / sqrt(v0x * v0x + v0y * v0y);
		v0x *= n0;
		v0y *= n0;
		
		const double n1 = e1sq / sqrt(v1x * v1x + v1y * v1y);
		v1x *= n1;
		v1y *= n1;
	}

	
	BYTE level = region.level_;
	int nChannels = image->nChannels;

	/*
	int i;
	for (double t = 0.0; t < 2.0 * M_PI; t += 0.001) {
		int x2 = x + (cos(t) * v0x + sin(t) * v1x) * 2.0 + 0.5;
		int y2 = y + (cos(t) * v0y + sin(t) * v1y) * 2.0 + 0.5;
		
		if ((x2 >= 0) && (x2 < width) && (y2 >= 0) && (y2 < height))
			for (i = 0; i < nChannels; i++)
				image->imageData[(y2 * width + x2)*nChannels+i] = level;
	}

	
	int i;
	*/
    CvPoint pt0, pt;
	CvBox2D box;
	CvPoint2D32f box_vtx[4];
	
	box.center.x = x;
	box.center.y = y;	
	box.size.width = 4*(sqrt(v0x*v0x+v1x*v1x));
	box.size.height = 4*(sqrt(v0y*v0y+v1y*v1y));

	box.center.x = (region.maxX+region.minX)/2;
	box.center.y = (region.maxY+region.minY)/2;

	box.size.width = (region.maxX -region.minX);
	box.size.height = (region.maxY -region.minY);

	///////////////////////////////////////////////
	//angle normalization
	if(v0x ==0) box.angle = 90;
	else
		box.angle = 180*atanf(v0y/v0x)/M_PI;
	//if(fabs(box.angle) > 60) box.angle = 90;
	box.angle = 90;
	
	/*if(abs(box.angle) <	15) 
	{
		if(v1x ==0) box.angle = 90;
		else
			box.angle = 180*atanf(v1y/v1x)/M_PI;
		if(fabs(box.angle) > 60) box.angle = 90;
	}*/

	///////////////////////////////////////////////
		cvBoxPoints( box, box_vtx );

		pt0.x = cvRound(box_vtx[3].x);//cvRound 返回和参数最接近的整数值
        pt0.y = cvRound(box_vtx[3].y);
       /* for( i = 0; i < 4; i++ )
        {
            pt.x = cvRound(box_vtx[i].x);
            pt.y = cvRound(box_vtx[i].y);
            cvLine(image, pt0, pt, CV_RGB(255, 255, 255), 1, CV_AA, 0);
            pt0 = pt;
        }*/
		 pt.x = cvRound(box_vtx[1].x);
         pt.y = cvRound(box_vtx[1].y);
		 cvRectangle(image, pt0, pt,CV_RGB(255, 255, 255), 1, CV_AA, 0);
		 CvRect rect = cvRect(pt.x,pt.y,pt0.x-pt.x,pt0.y-pt.y);
		
		 return rect;

}
///////////////////////////////////////////////////////////////////////////////
int MSER2CC(FeaImage fea,CSeqRect *seq)
{
	int i, j;

	int width;
	int height;
	IplImage* inImage = fea.LUV;
	
	width = inImage->width;
	height = inImage->height;

	vector<uint8_t> original;	
	
	// Create a grayscale image
	vector<uint8_t> grayscale(width * height);

	int nChannels = inImage->nChannels;

	for (int c =0; c < inImage->nChannels; c++)
	{		
		if(c == 1)
			continue;
		for ( i = 0; i < width * height; ++i) {
			grayscale[i] = ((unsigned char*)inImage->imageData)[i*nChannels+c];
		}
		
		// Extract MSER
		clock_t start = clock();
	
		MSER mser8(true);
		//MSER mser4(false);
		
		vector<MSER::Region> regions[2];
		
		mser8(&grayscale[0], width, height, regions[0]);
		
		// Invert the pixel values
		for ( i = 0; i < width * height; ++i){
			grayscale[i] = ~grayscale[i];
		}
		
		mser8(&grayscale[0], width, height, regions[1]);
		
		clock_t stop = clock();
		int total =  regions[0].size() + regions[1].size();
		printf("Extracted %d in %lf s.\n",total,(double)(stop - start) / CLOCKS_PER_SEC);
		if(total > 80)
			return 0;
		// Draw boxes in the original image
		// extracing bounding boxes
		for ( i = 0; i < 2; ++i){
			for ( j = 0; j < regions[i].size(); ++j)
			{
				CvRect r = extractBox(regions[i][j], width, height, 1, fea.mserMap[c]);
				if ( c != 1)
				{ 
					//extractBox(regions[i][j], width, height, 1, fea.RGB);
					seq->AddNode(r,true);
				}
			}
		}
	}
 

	return 1;
}

