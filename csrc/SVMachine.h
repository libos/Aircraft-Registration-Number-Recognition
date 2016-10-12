#ifndef __SVMACHINE_H_
#define __SVMACHINE_H_

#include "../stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <float.h>

#include "../svm/svm.h"
//#define SVM_TRAIN_FILENAME "svm_train.data"
//#define MODEL_FILENAME "svm.model"
#define Malloc(type,n) (type *)malloc((n)*sizeof(type))
#ifndef NOMINMAX

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#endif  /* NOMINMAX */


typedef struct {
	double label;
	struct svm_node *x;
}svm_vector;


#define SKIP_TARGET\
	while(isspace(*p)) ++p;\
	while(!isspace(*p)) ++p;

#define SKIP_ELEMENT\
	while(*p!=':') ++p;\
	++p;\
	while(isspace(*p)) ++p;\
	while(*p && !isspace(*p)) ++p;

class CSVM
{
public:
	void init_train();
	CSVM(bool NoneParam);
	~CSVM();
	//这个是设置常用参数tcg,t=>kernel_type,c=>C,g=>gamma
	CSVM(int kernel_type = RBF,double C=1,double gamma=0 );
	//下面是最普通的设置
	CSVM(int NoneParam,int svm_type=C_SVC,int kernel_type=RBF,int degree=3,double gamma=0,double coef0=0);
	//这个是全面设置
	CSVM(struct svm_parameter param);


public:
	int ScaleTrainData(char* save_filename);
	int ScaleTestData(svm_vector *sv);
public:
	void readTrainData(bool scale);
	void getModel();
	void loadModel();
	void loadModel(char *filename);
	double Classify(svm_vector svm);

public:
	void setSaveModel(bool TorF);
	void setTrainDataFile(char *filename);
	void setModelFile(char *filename);
	void setTestDataFile(char * filename);
	void setScaledTrainData(char * filename);
	void setScaleTemplate(char *filename);
/*下面这个public 导致scale没法用*/
public:
	void output(FILE *fp,int index, double value);
	void output(svm_node *snode,int index, double value);
	char* readline(FILE *input);
private:
	void exit_input_error(int line_num);
	void doCrossValidation();
	
protected: 
	struct svm_parameter param;		
	struct svm_problem prob;
	struct svm_model *model;
	struct svm_node *x_space;
	int cross_validation;
	int nr_fold;

public:
	 char *SVM_TRAIN_FILENAME;
	 char *MODEL_FILENAME;
	 char *SVM_TEST_FILENAME;
	 char *SCALED_TRAIN_DATA;
	 char *SCALE_TEMPLATE;
	bool saveModel;
	char *line;
	int max_line_len;

//predict start

	int max_nr_attr;
//predict end
//scale begin
	
	double *feature_max;
	double *feature_min;
	int max_index;
	int min_index;
	double lower;
	double upper;
	long int new_num_nonzeros;
	long int num_nonzeros;
//scale end
};

#endif