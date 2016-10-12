#ifndef __KNN_H_
#define __KNN_H_

#include "cv.h"
#include <iostream.h>
#include <math.h>
#include <fstream.h>
#define  NATTRS 7 //number of attributes
#define  MAXSZ  1000 //max size of training set
#define  MAXVALUE  1000.0 //the biggest attribute's value is below 10000(int)
#define  K  1

typedef struct vector {
	double attributes[NATTRS];
	double classlabel;
}vectors;
typedef struct item {
	double distance;
	double classlabel;
}items;

class Knn
{
public:
	Knn();
	~Knn();

public:
	int AddtoTSet(vectors v);
	double Distance(vectors v1, vectors v2);
	int KnnMax(items knn[]);
	double Classify(vectors v);//decide which class label will be assigned to a given input vetor with the knn method
	bool Import(double label,CvHuMoments humoment);
	vectors CreateVector(double label,CvHuMoments humoment);
	void GetTrainData();
	
public:
	vectors trSet[MAXSZ];//global variable,the training set
	items knn[K];//global variable,the k-neareast-neighbour set
	int curTSize; //current size of the training set
};

#endif