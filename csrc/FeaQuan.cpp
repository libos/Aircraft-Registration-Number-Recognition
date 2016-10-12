/*****************************************************************
** Copyright 2001-2002 Media Lab of ICT.	All rights reserved **
**																**
** version 1.0													**
** Date: November 20th, 2002									**
**																**
** C file for image color/texture feature quantilization		**
**																**
******************************************************************/
#include "FeaQuan.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
//////////////////////////////////////////////////////////////////
void FreeFMatrix(float **m, int nr)
{
	int i;
	
	for (i=0;i<nr;i++) free(m[i]);
	free(m);
}
float **FMatrix(int nr, int nc)
{
	int i,j;
	float **m;
	
	m=(float **) calloc(nr,sizeof(float *));
	if (!m) return NULL;
	for(i=0;i<nr;i++)
	{
		m[i]=(float *) calloc(nc,sizeof(float));
		if (!m[i]) return NULL;
		for (j=0;j<nc;j++) m[i][j]=0;
	}

	return m;
}
int **IMatrix(int nr, int nc)
{
	int i,j;
	int **m;
	
	m=(int **) calloc(nr,sizeof(int *));
	if (!m) return NULL;
	for(i=0;i<nr;i++)
	{
		m[i]=(int *) calloc(nc,sizeof(int));
		if (!m[i]) return NULL;
		for (j=0;j<nc;j++) m[i][j]=0;
	}

	return m;
}

void FreeIMatrix(int **m, int nr)
{
	int i;
	
	for (i=0;i<nr;i++) free(m[i]);
	free(m);
}
int Round(float x)
{
	int i;
	float y;
	
	y=(float)(floor(x));
	if ((x-y)>=0.5) x = (float)(ceil(x));
	else x = (float)(floor(x));
	i = (int) x;
	return i;
}

/* sort float from large to small */
void SortFloatL2S(int n, float *num, int *index)
{
	int i,j;
	int indextmp;
	float numtmp;
	
	for (i=0;i<n;i++) index[i]= i;
	for (j=1;j<n;j++)
	{
		numtmp=num[j];
		indextmp=index[j];
		i=j-1;
		while (i>=0 && num[i]<numtmp)
		{
			num[i+1]=num[i];
			index[i+1]=index[i];
			i--;
		}
		num[i+1]=numtmp;
		index[i+1]=indextmp;
	}
}

/* sort float from small to large */
void SortFloatS2L(int n, float *num, int *index)
{
	int i,j;
	int indextmp;
	float numtmp;
	
	for (i=0;i<n;i++) index[i]= i;
	for (j=1;j<n;j++)
	{
		numtmp=num[j];
		indextmp=index[j];
		i=j-1;
		while (i>=0 && num[i]>numtmp)
		{
			num[i+1]=num[i];
			index[i+1]=index[i];
			i--;
		}
		num[i+1]=numtmp;
		index[i+1]=indextmp;
	}
}

/* sort int from large to small */
void SortInL2S(int n, int *num, int *index)
{
	int i,j;
	int indextmp;
	int numtmp;
	
	for (i=0;i<n;i++) index[i]= i;
	for (j=1;j<n;j++)
	{
		numtmp=num[j];
		indextmp=index[j];
		i=j-1;
		while (i>=0 && num[i]<numtmp)
		{
			num[i+1]=num[i];
			index[i+1]=index[i];
			i--;
		}
		num[i+1]=numtmp;
		index[i+1]=indextmp;
	}
}

/* sort int from small to large */
void SortIntS2L(int n, int *num, int *index)
{
	int i,j;
	int indextmp;
	int numtmp;
	
	for (i=0;i<n;i++) index[i]= i;
	for (j=1;j<n;j++)
	{
		numtmp=num[j];
		indextmp=index[j];
		i=j-1;
		while (i>=0 && num[i]>numtmp)
		{
			num[i+1]=num[i];
			index[i+1]=index[i];
			i--;
		}
		num[i+1]=numtmp;
		index[i+1]=indextmp;
	}
}

/* sort array of int from small to large */
void SortInArrayS2L(int n, int **num, int *index,int m)
{
	int i,j,k,indextmp,*numtmp,bigger;
	
	numtmp=(int *)calloc(m,sizeof(int));
	for (i=0;i<n;i++) index[i]= i;
	for (j=1;j<n;j++)
	{
		for (k=0;k<m;k++) numtmp[k]=num[j][k];
		indextmp=index[j];
		i=j-1;
		bigger=0;
		if (i>=0)
		{
			for (k=0;k<m;k++)
			{
				if (num[i][k]>numtmp[k]) { bigger=1; break; }
				else if (num[i][k]<numtmp[k]) { break; }
			}
		}
		while (bigger)
		{
			for (k=0;k<m;k++) num[i+1][k]=num[i][k];
			index[i+1]=index[i];
			i--;
			bigger=0;
			if (i>=0)
			{
				for (k=0;k<m;k++)
				{
					if (num[i][k]>numtmp[k]) { bigger=1; break; }
					else if (num[i][k]<numtmp[k]) { break; }
				}
			}
		}
		for (k=0;k<m;k++) num[i+1][k]=numtmp[k];
		index[i+1]=indextmp;
	}
	free(numtmp);
}

/*Euliandistance of tow vector*/
float EulianDistance(float *a,float *b,int dim)
{
	int i;
	float dist=0.0;
	
	for (i=0;i<dim;i++) dist+=Sqr(a[i]-b[i]);
	dist = (float)(sqrt(dist));
	return dist;
}

float EulianDistance2(float *a,float *b,int dim)
{
	int i;
	float dist=0.0;
	
	for (i=0;i<dim;i++) dist+=Sqr(a[i]-b[i]);
	
	return dist;
}

double EuclidDistVec(int n, double** data1, double** data2, int index1, int index2) 
/*

The euclid routine calculates the weighted Euclidean distance between two
rows or columns in a matrix.

Arguments
=========

n      (input) int
The number of elements in a row 

data1  (input) double array
The data array containing the first vector.

data2  (input) double array
The data array containing the second vector.

index1     (input) int
Index of the first row or column.

index2     (input) int
Index of the second row or column.

============================================================================
*/
{ 
	double result = 0.;
	
	int i;	
	
	for (i = 0; i < n; i++)
	{		
		double term = data1[index1][i] - data2[index2][i];	
		result = result + term*term;		
		
	}		
	
	result = sqrt(result);
	return result;
}


int LocColor(int iy,int ix,int idim,int wid,int dim)
{
	return ( (iy*wid+ix)*dim + idim);
}

int LocPixel(int iy,int ix,int wid)
{
	return (iy*wid+ix);
}

int LocBlock(int it,int iy,int ix,int ny,int nx)
{
	return ((it*ny+iy)*nx+ix);
}

void ExtendBoundFloat(float *img,float *imgout,int ny,int nx,int offset,int dim)
{
  int iy,ix,i,j,sy,ey,sx,ex,nx2,jump,loc;
  float f1,f2;

  nx2 = nx+2*offset;
  sy = offset; ey = ny+offset; 
  sx = offset; ex = nx+offset;
  jump = 2*offset*dim;

  i = 0;
  loc = LocColor(sy,sx,0,nx2,dim);
  for (iy=sy;iy<ey;iy++)
  {
    for (ix=sx;ix<ex;ix++)
    {
      for (j=0;j<dim;j++)
        imgout[loc++]=img[i++];
    }
    loc += jump;
  }

  for (ix=sx;ix<ex;ix++)
  {
    for (j=0;j<dim;j++)
    {
      f1 = imgout[LocColor(offset,ix,j,nx2,dim)];
      f2 = imgout[LocColor(ny+offset-1,ix,j,nx2,dim)];
      for (iy=0;iy<offset;iy++)
      {
        imgout[LocColor(offset-1-iy,ix,j,nx2,dim)]  = f1;
        imgout[LocColor(ny+offset+iy,ix,j,nx2,dim)] = f2;
      }
    }
  }

  ey = ny+2*offset;
  for (iy=0;iy<ey;iy++)
  {
    for (j=0;j<dim;j++)
    {
      f1 = imgout[LocColor(iy,offset,j,nx2,dim)];
      f2 = imgout[LocColor(iy,nx+offset-1,j,nx2,dim)];
      for (ix=0;ix<offset;ix++)
      {
        imgout[LocColor(iy,offset-1-ix,j,nx2,dim)]  = f1;
        imgout[LocColor(iy,nx+offset+ix,j,nx2,dim)] = f2;
      }
    }
  }
}

float CaculateGlobalUniform(float *B,float *A,int ny,int nx,int offset,float *weight,int dim)
{
	int iy,ix,jy,jx,nuniform,window,j,k,winarea,*index,*index2;
	float *uniform,*dif,*dif2,avg=0.0,*D,D1,D2,**A1,mean1,mean2;
	int J1,J2,nnoise,J;
	float *difdif,difmean,difvar,difdift;
	float *vec;
	int ej,ej2,ey,ex,nx2,loc;
	
	uniform = (float *)calloc(dim,sizeof(float));
	
	window=2*offset+1;
	winarea=Sqr(window);
	nnoise=offset+1;
	A1=(float **) malloc(winarea*sizeof(float *));
	dif=(float *)calloc(winarea,sizeof(float));
	index=(int *)calloc(winarea,sizeof(int));
	D=(float *)calloc(winarea,sizeof(float));
	index2=(int *)calloc(winarea,sizeof(int));
	difdif=(float *)calloc(winarea-1,sizeof(int));
	
	nx2 = nx+2*offset;
	ej = winarea-1;
	
	for (iy=0;iy<ny;iy++)
	{
		for (ix=0;ix<nx;ix++)
		{
			j=0;
			ey = iy+window; ex = ix+window;
			for (jy=iy;jy<ey;jy++)
			{
				for (jx=ix;jx<ex;jx++)
				{
					A1[j]=A+LocColor(jy,jx,0,nx2,dim);
					dif[j]=EulianDistance(B,A1[j],dim);
					j++;
				}
			}
			SortFloatS2L(winarea,dif,index);
			
			for (j=0;j<ej;j++) difdif[j]=dif[j+1]-dif[j];
			
			difdift = (float)(12.0 * sqrt(dim/3.0));
			J1=0; 
			for (j=nnoise-1;j>=0;j--)
			{
				if (difdif[j]>difdift) { J1=j+1; break; }
			}
			J2=winarea-1;
			for (j=winarea-1-nnoise;j<ej;j++)      
			{
				if (difdif[j]>difdift) { J2=j; break; }
			}
			J=J2-2;
			
			if (EulianDistance2(A1[index[J1]],A1[index[J2]],dim)==0.0)
			{
				for (k=0;k<dim;k++) B[k] = A1[index[J1]][k];
			}
			else 
			{
				for (j=J1;j<=J;j++)
				{
					D1=0; D2=0;
					mean1=0; mean2=0;
					
					for (k=J1;k<=j;k++) mean1+=dif[k];
					mean1 /= (j-J1+1);
					for (k=J1;k<=j;k++) D1+=Sqr(dif[k]-mean1);
					
					for (k=j+1;k<=J2;k++) mean2+=dif[k];
					mean2 /= (J2-j);
					for (k=j+1;k<=J2;k++) D2+=Sqr(dif[k]-mean2);
					
					D[j-J1] = (D1+D2) / Sqr(mean2-mean1);        
				}
				
				SortFloatS2L(J-J1+1,D,index2);
				*weight = dif[index2[0]+J1]-dif[J1];
				avg += (*weight);
				
				for (k=0;k<dim;k++) uniform[k]=0;
				ej2 = index2[0]+J1;
				for (j=J1;j<=ej2;j++)
				{
					for (k=0;k<dim;k++) uniform[k] += A1[index[j]][k];
				}
				for (k=0;k<dim;k++) B[k] = uniform[k]/(index2[0]+1);
			}
			B += dim;
			weight ++;
		}
	}
	avg = avg/(ny*nx);
	free(dif);
	free(index);
	free(index2);
	free(difdif);
	free(D);
	free(A1);
	free(uniform);
	
	return avg;
}

int Greedy(float *A,int nvec,int ndim,int N,float **codebook,float t,unsigned char *P,
		   float *weight)
{
	int iv,in,jn,imax,nsplit,*index2,k,i,retgla,kn;
	float *totalw,*d,**buf, *variance;
	
	buf=FMatrix(N,ndim);
	d=(float *)calloc(ndim,sizeof(float));
	variance=(float *)calloc(N,sizeof(float));
	totalw=(float *)calloc(N,sizeof(float));
	index2=(int *)calloc(N,sizeof(int));
	
	/* Calculate the initial centroid */
	for (k=0;k<ndim;k++) codebook[0][k]=0.0;
	totalw[0]=0; i=0;
	for (iv=0;iv<nvec;iv++)
	{
		P[iv]= 0;
		totalw[0]+=weight[iv];
		for (k=0;k<ndim;k++) codebook[0][k] += weight[iv]*A[i++];
	}
	for (k=0;k<ndim;k++) codebook[0][k]/=totalw[0];
	
	in=1;
	while (in<N)
	{
		/*  find the maximum variance */
		for (jn=0;jn<in;jn++) 
		{ 
			variance[jn]=0; 
			totalw[jn]=0;
			for (k=0;k<ndim;k++) buf[jn][k]=0.0;
		}
		i = 0;
		for (iv=0;iv<nvec;iv++)
		{
			for (k=0;k<ndim;k++) d[k] = codebook[P[iv]][k] - A[i++];
			for (k=0;k<ndim;k++) 
			{
				buf[P[iv]][k] += weight[iv]*Sqr(d[k]);
				variance[P[iv]]+= buf[P[iv]][k];
			}
			totalw[P[iv]] += weight[iv];
		}
		for (jn=0;jn<in;jn++) 
		{
			for (k=0;k<ndim;k++) buf[jn][k] = (float)(sqrt(buf[jn][k]/totalw[jn]));
		}
		SortFloatL2S(in,variance,index2);
		
		/*  split */
		nsplit=in/2+1;
		if ((nsplit+in)>N) nsplit=N-in;
		for (jn=0;jn<nsplit;jn++)
		{
			imax=index2[jn];
			for (k=0;k<ndim;k++) codebook[in+jn][k] = codebook[imax][k] - buf[imax][k]; 
			for (k=0;k<ndim;k++) codebook[imax] [k] = codebook[imax][k] + buf[imax][k];
		}
		
		/*  run gla on the codebook */
		in=in+nsplit;
		retgla=GLA_VQ(A,nvec,ndim,in,codebook,t,P,weight);
		
		/*  find the code vectors same, remove them and stop */
		if (retgla)
		{
			for (jn=0;jn<in-1;jn++)
			{
				for (kn=jn+1;kn<in;kn++)
				{
					if (EulianDistance2(codebook[jn],codebook[kn],ndim)==0)
					{
						for (i=kn;i<in-1;i++) 
							for (k=0;k<ndim;k++) codebook[i][k]=codebook[i+1][k];
							in--;
					}
				}
			}
			break;
		}
	}
	FreeFMatrix(buf,N);
	free (d);
	free (index2);
	free(variance);
	free(totalw);
	return in;
}

int GLA_VQ(float *A,int nvec,int ndim,int N,float **codebook,float t,unsigned char *P,
		float *weight)
{
	int iv,in,i,j,jn,codeword_exist=0,k,l;
	float *totalw,d1,d2,rate,lastmse,mse,*centroid,*d;
	
	totalw=(float *)calloc(N,sizeof(float));
	d=(float *)calloc(ndim,sizeof(float));
	
	for (i=0;i<5;i++)
	{
		/*  get the new partition and total distortion using NN */
		mse=0.0; j=0;
		for (iv=0;iv<nvec;iv++)
		{
			for (k=0;k<ndim;k++) d[k] = A[j++]-codebook[0][k];
			d1=0;
			for (k=0;k<ndim;k++) d1 += Sqr(d[k]);
			P[iv]=0; 
			for (in=1;in<N;in++)
			{
				d2=0; l=j-ndim;
				for (k=0;k<ndim;k++) 
				{
					d2+=Sqr(A[l]-codebook[in][k]); l++;
					if (d2>=d1) break;
				}
				if (d2<d1)  { d1=d2; P[iv]= (unsigned char) in; }
			}
			mse+=d1;
		}
		
		/*  get the new codebook using centroid */
		for (in=0;in<N;in++) 
		{
			totalw[in]=0.0;
			for (k=0;k<ndim;k++) codebook[in][k]=0;
		}
		j = 0;
		for (iv=0;iv<nvec;iv++)
		{
			for (k=0;k<ndim;k++) codebook[P[iv]][k] += weight[iv]*A[j++];
			totalw[P[iv]]+=weight[iv];
		}
		for (in=0;in<N;in++)
		{
			if (totalw[in]>0.0)
			{
				for (k=0;k<ndim;k++) codebook[in][k] /= totalw[in]; 
			}
			else
			{
				/*assign a training vector not in the codebook as code vector */
				codeword_exist=1;
				iv= Round ( ((float) rand()) *(nvec-1)/RAND_MAX);
				while (codeword_exist<=2 && codeword_exist>0)
				{
					j = iv*ndim;
					for (k=0;k<ndim;k++) codebook[in][k] = A[j++];
					for (jn=0;jn<N;jn++)
					{
						if (jn!=in)
						{
							for (k=0;k<ndim;k++) d[k]=codebook[jn][k]-codebook[in][k];
							d1=0;
							for (k=0;k<ndim;k++) d1 += Sqr(d[k]);
							if (d1==0) break;
						}
					}
					if (jn==N) codeword_exist=0;
					else 
					{ 
						iv = Round( ((float) rand()) *(nvec-1)/RAND_MAX);
						codeword_exist++;
					}
				}
			}
		}
		if (i>0)
		{
			rate=(lastmse-mse)/lastmse;
			if (rate<t) break;
		}
		lastmse=mse;
	}
	
	for (in=0;in<N;in++)
	{
		totalw[in]=0.0;
		for (k=0;k<ndim;k++) codebook[in][k]=0;
	}
	j = 0;
	for (iv=0;iv<nvec;iv++)
	{
		for (k=0;k<ndim;k++) codebook[P[iv]][k] += A[j++];
		totalw[P[iv]]+=1.0;
	}
	for (in=0;in<N;in++)
	{
		if (totalw[in]>0)
		{
			for (k=0;k<ndim;k++) codebook[in][k] /= totalw[in];
		}
	}
	
	free(d);
	free(totalw);
	return codeword_exist; 
}
////////////////////////////////////////////////////////////////////////////////////
int FeaQuan(float *B,float **feaquan,int imghei,int imgwid,int dim,int precision)
{
	int N;	
	int i,offset, imgsize;
	float *A,*weight,avgweight;
	int imghei2,imgwid2;
	unsigned char * P;
	int mincolornum;
	float coeff;
	
	imgsize= imghei*imgwid;
	
	//printf ("please wait, feature quantization\n");	
	
	offset=2;
	imghei2 = imghei+2*offset; imgwid2 = imgwid+2*offset;
	
	weight = (float *)calloc(imgsize,sizeof(float));
	A      = (float *)calloc(imghei2*imgwid2*dim,sizeof(float));
	P      = (unsigned char *)calloc(imgsize,sizeof(unsigned char));	
	avgweight = 0;	
	
	/*add edges for image*/
	ExtendBoundFloat(B,A,imghei,imgwid,offset,dim);
	
	/*caculate average weight*/
	avgweight = CaculateGlobalUniform(B,A,imghei,imgwid,offset,weight,dim);
	for (i=0;i<imgsize;i++) weight[i] = (float)(exp(-weight[i]));

	/*caculate the feature quantilization number*/
	mincolornum = 2;coeff=1.0;

	N=MAX(Round(coeff*avgweight),mincolornum)-1;
	if (N>MAX_QUAN_NUM) N=MAX_QUAN_NUM;
	
	if(N > 6) N = 6;
	printf("quantilized color number is %d\n",N);	

	N=Greedy(B,imgsize,dim,N,feaquan,(float)(0.05),P,weight);

	/*use GLA VQ for feature quantilization*/
	GLA_VQ(B,imgsize,dim,N,feaquan,(float)(0.03),P,weight);

	free(A);
	free(weight);
	free(P);
	
	return N;
}

////////////////////////////////////////////////
void GetFeaMap(float *B,unsigned char *feamap, int wid, int hei, 
			   float **feaquan,int dim,int N)
{
	int i,j,k;
	float mindif,dif;

	//long mean[256];
	float exchange[3];
	//long num [256];
	//for(i = 0; i < 256;i++) {mean[i] = 0; num[i] = 0;};

	/*//ordering of feaquan
	for(i = 0; i < 256; i++)
	{
		for(j = 0; j < 256; j++)
		{
			if(feaquan[i][0] > feaquan[j][0])
			{
				for(k = 0 ; k < 3; k++)
				{
					exchange[k] = feaquan[i][k];
					feaquan[i][k]= feaquan[j][k];
					feaquan[j][k]= exchange[k];
					
				}
			}
		}
	}*/
	
	for (i=0;i<hei;i++)
	{
		for (j=0;j<wid;j++)
		{
			mindif = EulianDistance2(B,feaquan[0],dim);
			
			feamap[i*wid+j]=0;
			for (k=1;k<N;k++)
			{
				dif = EulianDistance2(B,feaquan[k],dim);
				if (dif<mindif) { feamap[i*wid+j]=(unsigned char)(feaquan[k][0]); mindif=dif; }				
			}

			B += dim;	
		}
	}
}