/*****************************************************************
** Copyright 2001-2002 Media Lab of ICT.	All rights reserved **
**																**
** version 1.0													**
** Date: November 20th, 2002									**
**																**
** header file for image color/texture feature quantilization   **
**																**
******************************************************************/

#ifndef _FEA_QUAN_
#define _FEA_QUAN_

#define MAX_QUAN_NUM    256

/*Get the larger / smaller value */
#ifndef MAX
#define MAX(x, y) (((x) > (y)) ? (x) : (y))          
#endif
#ifndef MIN
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#endif

/*get squre*/
#ifndef Sqr
#define Sqr(x) ( (x)*(x) )  
#endif                            

/*get sign of a data*/
#ifndef Sgn
#define Sgn(x) ((x)>0 ? 1 : -1)                      
#endif

/*get round a data*/
int  Round(float x);   


/*alloc and free float type matrix*/
float **FMatrix(int nr, int nc);
void FreeFMatrix(float **m, int nr);

/*alloc and free int type matrix*/
int **IMatrix(int nr, int nc);
void FreeIMatrix(int **m, int nr);

void GetFeaMap(float *B,unsigned char *feamap, int wid, int hei, 
			   float **feaquan,int dim,int N);

/*caculate the global uniform of the image, the uniform is represent by weight*/
float CaculateGlobalUniform(float *B,float *A,int ny,int nx,int offset,
							float *weight,int dim);

/*GLA algorithm for Vector quantilization*/
int GLA_VQ(float *A,int nvec,int ndim,int N,float **codebook,float t,
		   unsigned char *P, float *weight);

/*greedy algorithm */
int Greedy(float *A,int nvec,int ndim,int N,float **codebook,float t,unsigned char *P,
    float *weight);

/*feature quantilization function*/
int FeaQuan(float *B,float **feaquan,int imghei,int imgwid,int dim,int precision);





#endif