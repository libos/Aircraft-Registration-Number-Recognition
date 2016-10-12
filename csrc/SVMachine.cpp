#include "SVMachine.h"
/*
*
*	构造函数
*
*/
void CSVM::init_train()
{
	line=NULL;
	max_line_len = 1024;
	// default values
	param.svm_type = C_SVC;
	param.kernel_type = RBF;
	param.degree = 3;
	param.gamma = 0;	// 1/num_features
	param.coef0 = 0;
	param.nu = 0.5;
	param.cache_size = 100;
	param.C = 1;
	param.eps = 1e-3;
	param.p = 0.1;
	param.shrinking = 1;
	param.probability = 0;
	param.nr_weight = 0;
	param.weight_label = NULL;
	param.weight = NULL;
	cross_validation = 0;

	max_line_len = 1024;
	line = Malloc(char,max_line_len);
	/*
	*	init scale	
	*/
	this->lower = -1;
	this->upper = 1; 
}

void CSVM::setSaveModel(bool TorF)
{
	this->saveModel = TorF;
}
CSVM::CSVM(bool NoneParam)
{
	void (*print_func)(const char*) = NULL;	// default printing to stdout 
	init_train(); 

 	svm_set_print_string_function(print_func);
}
CSVM::CSVM(struct svm_parameter pamam)
{
	void (*print_func)(const char*) = NULL;	// default printing to stdout 
	init_train(); 
	this->param = param;
 	svm_set_print_string_function(print_func);
}
 
CSVM::CSVM(int NoneParam,int svm_type,int kernel_type,int degree,double gamma,double coef0)
{
	void (*print_func)(const char*) = NULL;	// default printing to stdout 
	init_train();
	param.svm_type = svm_type;
	param.kernel_type = kernel_type;
	param.degree = degree;
	param.gamma = gamma;	// 1/num_features
	param.coef0 = coef0; 

 	svm_set_print_string_function(print_func);
} 
CSVM::CSVM(int kernel_type,double C,double gamma)
{
	void (*print_func)(const char*) = NULL;	// default printing to stdout
	init_train(); 
	param.kernel_type = kernel_type;
	param.gamma = gamma;	// 1/num_features
	param.C = C; 
 	svm_set_print_string_function(print_func);
} 
void CSVM::setTrainDataFile(char *filename)
{
	this->SVM_TRAIN_FILENAME = filename;
}
void CSVM::setModelFile(char * filename)
{
	this->MODEL_FILENAME = filename;
}
void CSVM::setTestDataFile(char * filename)
{
	this->SVM_TEST_FILENAME = filename;
}
void CSVM::setScaledTrainData(char * filename)
{
	this->SCALED_TRAIN_DATA = filename;
}
void CSVM::setScaleTemplate(char *filename)	
{
	this->SCALE_TEMPLATE = filename;
}
CSVM::~CSVM()
{

}
/*
*
*	训练函数
*
*/
void CSVM::loadModel()
{
	loadModel(this->MODEL_FILENAME);
}
void CSVM::loadModel(char *filenamex)
{
	if((this->model=svm_load_model(filenamex))==0)
	{
		::AfxMessageBox("can't open model file!");
		return;
	} 
}
void CSVM::doCrossValidation()
{
	int i;
	int total_correct = 0;
	double total_error = 0;
	double sumv = 0, sumy = 0, sumvv = 0, sumyy = 0, sumvy = 0;
	double *target = Malloc(double,prob.l);

	svm_cross_validation(&prob,&param,nr_fold,target);
	if(param.svm_type == EPSILON_SVR ||
	   param.svm_type == NU_SVR)
	{
		for(i=0;i<prob.l;i++)
		{
			double y = prob.y[i];
			double v = target[i];
			total_error += (v-y)*(v-y);
			sumv += v;
			sumy += y;
			sumvv += v*v;
			sumyy += y*y;
			sumvy += v*y;
		}
		printf("Cross Validation Mean squared error = %g\n",total_error/prob.l);
		printf("Cross Validation Squared correlation coefficient = %g\n",
			((prob.l*sumvy-sumv*sumy)*(prob.l*sumvy-sumv*sumy))/
			((prob.l*sumvv-sumv*sumv)*(prob.l*sumyy-sumy*sumy))
			);
	}
	else
	{
		for(i=0;i<prob.l;i++)
			if(target[i] == prob.y[i])
				++total_correct;
		printf("Cross Validation Accuracy = %g%%\n",100.0*total_correct/prob.l);
	}
	free(target);
}
void CSVM::getModel()
{
	const char *error_msg;
	error_msg = svm_check_parameter(&prob,&param);

	if(error_msg)
	{
		::AfxMessageBox(error_msg);
		return;
	} 
 
	if(cross_validation)
	{
		doCrossValidation();
	}
	else
	{
		model = svm_train(&prob,&param);
		if(this->saveModel)
		{
			if(svm_save_model(this->MODEL_FILENAME,model))
			{
				::AfxMessageBox("can't save model to file");
				return;
			}
		}
		//svm_free_and_destroy_model(&model);
	}
	
}

void CSVM::readTrainData(bool scale)
{
	int elements, max_index, inst_max_index, i, j;
	FILE *fp = NULL;
	if(scale)
	{
		fp = fopen(this->SCALED_TRAIN_DATA,"r");
	}
	else
	{
		fp = fopen(this->SVM_TRAIN_FILENAME,"r");
	}
	char *endptr;
	char *idx, *val, *label;

	if(fp == NULL)
	{
		CString tt;
		tt.Format("can't open input file %s",this->SVM_TRAIN_FILENAME);
		::AfxMessageBox(tt);
		return;
	}

	prob.l = 0;
	elements = 0;

	while(readline(fp)!=NULL)
	{
		char *p = strtok(line," \t"); // label

		// features
		while(1)
		{
			p = strtok(NULL," \t");
			if(p == NULL || *p == '\n') // check '\n' as ' ' may be after the last feature
				break;
			++elements;
		}
		++elements;
		++prob.l;
	}
	rewind(fp);

	prob.y = Malloc(double,prob.l);
	prob.x = Malloc(struct svm_node *,prob.l);
	x_space = Malloc(struct svm_node,elements);

	max_index = 0;
	j=0;
	for(i=0;i<prob.l;i++)
	{
		inst_max_index = -1; // strtol gives 0 if wrong format, and precomputed kernel has <index> start from 0
		readline(fp);
		prob.x[i] = &x_space[j];
		label = strtok(line," \t\n");
		if(label == NULL) // empty line
			exit_input_error(i+1);

		prob.y[i] = strtod(label,&endptr);
		if(endptr == label || *endptr != '\0')
			exit_input_error(i+1);

		while(1)
		{
			idx = strtok(NULL,":");
			val = strtok(NULL," \t");

			if(val == NULL)
				break;

			errno = 0;
			x_space[j].index = (int) strtol(idx,&endptr,10);
			if(endptr == idx || errno != 0 || *endptr != '\0' || x_space[j].index <= inst_max_index)
				exit_input_error(i+1);
			else
				inst_max_index = x_space[j].index;

			errno = 0;
			x_space[j].value = strtod(val,&endptr); 
			if(endptr == val || errno != 0 || (*endptr != '\0' && !isspace(*endptr)))
				exit_input_error(i+1);
 
			++j;
		}

		if(inst_max_index > max_index)
			max_index = inst_max_index;
		x_space[j++].index = -1;
	}

	if(param.gamma == 0 && max_index > 0)
		param.gamma = 1.0/max_index;

	if(param.kernel_type == PRECOMPUTED)
		for(i=0;i<prob.l;i++)
		{
			if (prob.x[i][0].index != 0)
			{
				::AfxMessageBox("Wrong input format: first column must be 0:sample_serial_number\n");
				return;
			}
			if ((int)prob.x[i][0].value <= 0 || (int)prob.x[i][0].value > max_index)
			{
				::AfxMessageBox("Wrong input format: sample_serial_number out of range\n");
				return;
			}
		}

	fclose(fp);
}
/*
*
*	分类函数
*
*/
double CSVM::Classify(svm_vector svm)
{
	double target_label = svm.label;
	struct svm_node *x = svm.x;
	double error=0;
	int total=0;
	int correct = 0;  
	double sump = 0, sumt = 0, sumpp = 0, sumtt = 0, sumpt = 0;

	int svm_type=svm_get_svm_type(model);
	int nr_class=svm_get_nr_class(model);
 

	double predict_label;
	//max_nr_attr=64;
	//x = (struct svm_node *) malloc(max_nr_attr*sizeof(struct svm_node));

	if(svm_check_probability_model(model)!=0)
		printf("Model supports probability estimates, but disabled in prediction.\n");
	
	predict_label = svm_predict(model,x);
//	printf("%g\n",predict_label);

	if(predict_label == target_label)
		++correct;
	error += (predict_label-target_label)*(predict_label-target_label);
	sump += predict_label;
	sumt += target_label;
	sumpp += predict_label*predict_label;
	sumtt += target_label*target_label;
	sumpt += predict_label*target_label; 
	if (svm_type==NU_SVR || svm_type==EPSILON_SVR)
	{
		printf("Mean squared error = %g (regression)\n",error/total);
		printf("Squared correlation coefficient = %g (regression)\n",
			((total*sumpt-sump*sumt)*(total*sumpt-sump*sumt))/
			((total*sumpp-sump*sump)*(total*sumtt-sumt*sumt))
			);
	}
	
	return predict_label;
}
/*
*
*	归一化函数
*
*/

//归一化训练集
int CSVM::ScaleTrainData(char* save_filename)
{
	this->num_nonzeros = 0;
	this->new_num_nonzeros = 0;
	char *processFile;
	int i,index;
	FILE *fp  = NULL;
	FILE *fscale = NULL;
	if(!(upper > lower))
	{
		::AfxMessageBox("inconsistent lower/upper specification\n");
		return 0;
	}
	if(save_filename)
	{
		processFile = this->SVM_TRAIN_FILENAME;
	}
	fp=fopen(processFile,"r");
	
	if(fp==NULL)
	{
		CString tt;
		tt.Format("can't open file %s\n", processFile);
		::AfxMessageBox(tt);
		return 0;
	}

	line = (char *) malloc(max_line_len*sizeof(char));
	
	/* assumption: min index of attributes is 1 */
	/* pass 1: find out max index of attributes */
	max_index = 0;
	min_index = 1;

	while(readline(fp)!=NULL)
	{
		char *p=line;

		SKIP_TARGET

		while(sscanf(p,"%d:%*f",&index)==1)
		{
			max_index = max(max_index, index);
			min_index = min(min_index, index);
			SKIP_ELEMENT
			num_nonzeros++;
		}
	}

	if(min_index < 1)
	{
		CString tt;
		tt.Format("WARNING: minimal feature index is %d, but indices should start from 1\n", min_index);
		::AfxMessageBox(tt); 
	}
	rewind(fp);

	feature_max = (double *)malloc((max_index+1)* sizeof(double));
	feature_min = (double *)malloc((max_index+1)* sizeof(double));

	if(feature_max == NULL || feature_min == NULL)
	{
		::AfxMessageBox("can't allocate enough memory\n");
		return 0; 
	}

	for(i=0;i<=max_index;i++)
	{
		feature_max[i]=-DBL_MAX;
		feature_min[i]=DBL_MAX;
	}

	/* pass 2: find out min/max value */
	while(readline(fp)!=NULL)
	{
		char *p=line;
		int next_index=1;
		double target;
		double value;

		sscanf(p,"%lf",&target);
		
		SKIP_TARGET

		while(sscanf(p,"%d:%lf",&index,&value)==2)
		{
			for(i=next_index;i<index;i++)
			{
				feature_max[i]=max(feature_max[i],0);
				feature_min[i]=min(feature_min[i],0);
			}
			
			feature_max[index]=max(feature_max[index],value);
			feature_min[index]=min(feature_min[index],value);

			SKIP_ELEMENT
			next_index=index+1;
		}		

		for(i=next_index;i<=max_index;i++)
		{
			feature_max[i]=max(feature_max[i],0);
			feature_min[i]=min(feature_min[i],0);
		}	
	}

	rewind(fp);

	/* pass 2.5: save/restore feature_min/feature_max */
	
	if(save_filename)
	{
		FILE *fp_save = fopen(save_filename,"w");
		if(fp_save==NULL)
		{
			CString tt;
			tt.Format("can't open file %s\n", save_filename);
			::AfxMessageBox(tt);
			return 0;
		}
	 
		fprintf(fp_save, "x\n");
		fprintf(fp_save, "%.16g %.16g\n", lower, upper);
		for(i=1;i<=max_index;i++)
		{
			if(feature_min[i]!=feature_max[i])
				fprintf(fp_save,"%d %.16g %.16g\n",i,feature_min[i],feature_max[i]);
		}

		if(min_index < 1)
			printf("WARNING: scaling factors with indices smaller than 1 are not stored to the file %s.\n", save_filename);

		fclose(fp_save);
	}

	fscale=fopen(this->SCALED_TRAIN_DATA,"w");
	if(fscale==NULL)
	{
		CString tt;
		tt.Format("can't open file %s\n", save_filename);
		::AfxMessageBox(tt);
		return 0; 
	}
	/* pass 3: scale */
	while(readline(fp)!=NULL)
	{
		char *p=line;
		int next_index=1;
		double target;
		double value;
		
		sscanf(p,"%lf",&target);
		fprintf(fscale,"%g ",target);
		//output_target(target);

		SKIP_TARGET

		while(sscanf(p,"%d:%lf",&index,&value)==2)
		{
			for(i=next_index;i<index;i++)
				output(fscale,i,0);
			output(fscale,index,value);

			SKIP_ELEMENT
			next_index=index+1;
		}		

		for(i=next_index;i<=max_index;i++)
			output(fscale,i,0);

		fprintf(fscale,"\n");
	}
	fclose(fscale);
	if (new_num_nonzeros > num_nonzeros)
		printf( 
			"WARNING: original #nonzeros %ld\n"
			"         new      #nonzeros %ld\n"
			"Use -l 0 if many original feature values are zeros\n",
			num_nonzeros, new_num_nonzeros);

	free(line);
	free(feature_max);
	free(feature_min);
	fclose(fp);
	return 0;
}

int CSVM::ScaleTestData(svm_vector *sv)
{
	this->num_nonzeros = 0;
	this->new_num_nonzeros = 0;
	int i,index;
	FILE  *fp_restore = NULL; 
	 
	char* restore_filename = this->SCALE_TEMPLATE;

	max_index = 0;
	min_index = 1; 

	if(restore_filename)
	{
		int idx, c;

		fp_restore = fopen(restore_filename,"r");
		if(fp_restore==NULL)
		{
			CString tt;
			tt.Format("can't open file %s\n", restore_filename);
			::AfxMessageBox(tt);
			return 0;  
		}

		c = fgetc(fp_restore);
		if(c == 'y')
		{
			readline(fp_restore);
			readline(fp_restore);
			readline(fp_restore);
		}
	
		readline(fp_restore);
		readline(fp_restore);

		while(fscanf(fp_restore,"%d %*f %*f\n",&idx) == 1)
			max_index = max(idx,max_index);
		rewind(fp_restore);
	} 
	i=0;
	while(sv->x[i].index != -1)
	{
			max_index = max(max_index, sv->x[i].index);
			min_index = min(min_index, sv->x[i].index);
			num_nonzeros++;
			i++;
	}
	if(min_index < 1)
		printf("WARNING: minimal feature index is %d, but indices should start from 1\n", min_index);

	feature_max = (double *)malloc((max_index+1)* sizeof(double));
	feature_min = (double *)malloc((max_index+1)* sizeof(double));

	if(feature_max == NULL || feature_min == NULL)
	{
		CString tt;
		tt.Format("can't allocate enough memory\n");
		::AfxMessageBox(tt);
		return 0;   
	}

	for(i=0;i<=max_index;i++)
	{
		feature_max[i]=-DBL_MAX;
		feature_min[i]=DBL_MAX;
	}

	/* pass 2: find out min/max value */
	int k=0;
	int next_index=1;
	double target = sv->label;
	while(sv->x[k].index != -1)
	{ 
		index = sv->x[k].index;
		double value = sv->x[k].value ;
		
		for(i=next_index;i<index;i++)
		{
			feature_max[i]=max(feature_max[i],0);
			feature_min[i]=min(feature_min[i],0);
		}
			
		feature_max[index]=max(feature_max[index],value);
		feature_min[index]=min(feature_min[index],value);

		next_index=index+1;
		k++;
	}
 	for(i=next_index;i<=max_index;i++)
	{
		feature_max[i]=max(feature_max[i],0);
		feature_min[i]=min(feature_min[i],0);
	}	

	/* pass 2.5: restore feature_min/feature_max */
	
	if(restore_filename)
	{
		/* fp_restore rewinded in finding max_index */
		int idx, c;
		double fmin, fmax;
		int next_index = 1;
		int y_min,y_lower,y_upper,y_max;
		int y_scaling=0;
		if((c = fgetc(fp_restore)) == 'y')
		{
			fscanf(fp_restore, "%lf %lf\n", &y_lower, &y_upper);
			fscanf(fp_restore, "%lf %lf\n", &y_min, &y_max);
			y_scaling = 1;
		}
		else
			ungetc(c, fp_restore);

		if (fgetc(fp_restore) == 'x') 
		{
			fscanf(fp_restore, "%lf %lf\n", &lower, &upper);
			while(fscanf(fp_restore,"%d %lf %lf\n",&idx,&fmin,&fmax)==3)
			{
				for(i = next_index;i<idx;i++)
					if(feature_min[i] != feature_max[i])
						printf(
							"WARNING: feature index %d appeared in svm_vector was not seen in the scaling factor file %s.\n",
							i,  restore_filename);

				feature_min[idx] = fmin;
				feature_max[idx] = fmax;

				next_index = idx + 1;
			}
			
			for(i=next_index;i<=max_index;i++)
				if(feature_min[i] != feature_max[i])
					printf(
						"WARNING: feature index %d appeared in svm_vector was not seen in the scaling factor file %s.\n",
						i , restore_filename);
		}
		fclose(fp_restore);
	}
	k = 0;
	next_index=1;
	/* pass 3: scale */
	while(sv->x[k].index != -1)
	{
		index = sv->x[k].index;
		double value = sv->x[k].value;

		for(i=next_index;i<index;i++)
				output(&(sv->x[i]),i,0);
			
		output(&(sv->x[k]),index,value);
	
		next_index=index+1;
		k++;
	}
	for(i=next_index;i<=max_index;i++)
		output(&(sv->x[i]),i,0);

	if (new_num_nonzeros > num_nonzeros)
		printf( 
			"WARNING: original #nonzeros %ld\n"
			"         new      #nonzeros %ld\n"
			"Use -l 0 if many original feature values are zeros\n",
			num_nonzeros, new_num_nonzeros);
 
	free(feature_max);
	free(feature_min); 
	return 0;
}

/*
*
*	下面是工具函数Util
*
*/
void CSVM::exit_input_error(int line_num)
{
	CString tt;
	tt.Format("Wrong input format at line %d\n", line_num);
	::AfxMessageBox(tt);
	return;  
}


char* CSVM::readline(FILE *input)
{
	int len;

	if(fgets(line,max_line_len,input) == NULL)
		return NULL; 

	while(strrchr(line,'\n') == NULL)
	{
		max_line_len *= 2;
		line = (char *) realloc(line,max_line_len);
		len = (int) strlen(line);
		if(fgets(line+len,max_line_len-len,input) == NULL)
			break;
	}
	return line;
}

void CSVM::output(FILE *fp,int index, double value)
{
	/* skip single-valued attribute */
	if(feature_max[index] == feature_min[index])
		return;

	if(value == feature_min[index])
		value = lower;
	else if(value == feature_max[index])
		value = upper;
	else
		value = lower + (upper-lower) * 
			(value-feature_min[index])/
			(feature_max[index]-feature_min[index]);

	if(value != 0)
	{
		fprintf(fp,"%d:%g ",index, value);
		new_num_nonzeros++;
	}
}
void CSVM::output(svm_node *snode,int index, double value)
{
	/* skip single-valued attribute */
	if(feature_max[index] == feature_min[index])
		return;

	if(value == feature_min[index])
		value = lower;
	else if(value == feature_max[index])
		value = upper;
	else
		value = lower + (upper-lower) * 
			(value-feature_min[index])/
			(feature_max[index]-feature_min[index]);

	if(value != 0)
	{
		snode->index = index;
		snode->value = value;
		new_num_nonzeros++;
	}
}

