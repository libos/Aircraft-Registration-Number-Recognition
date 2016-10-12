#include "Knn.h"
#include "stdio.h" 
#include "..\\stdafx.h"
Knn::Knn()
{
	curTSize = 0;
}
Knn::~Knn()
{
}
int Knn::AddtoTSet(vectors v)
{
	if(this->curTSize>=MAXSZ) {
		cout<<endl<<"The training set has "<<MAXSZ<<" examples!"<<endl<<endl; 
		return 0;
	}
	trSet[curTSize] = v;
	curTSize++;
	return 1;
}
double Knn::Distance(vectors v1,vectors v2)
{
	double d = 0.0;
	double tem = 0.0;
	for(int i = 0;i < NATTRS;i++)
		tem += (v1.attributes[i]-v2.attributes[i])*(v1.attributes[i]-v2.attributes[i]);
	d = sqrt(tem);
	return d;
}
int Knn::KnnMax(items knn[]) //return the no. of the item which has biggest distance(
                           //should be replaced) 
{
	int maxNo = 0;
	if(K > 1)
	for(int i = 1;i < K;i++)
		if(knn[i].distance>knn[maxNo].distance)
			maxNo = i;
    return maxNo;
}

double Knn::Classify(vectors v)//decide which class label will be assigned to a given input vetor with the knn method
{
	double dd = 0;
	int maxn = 0;
	int freq[K];
	double mfreqC = 0;//the class label appears most frequently 
	int i;
	for(i = 0;i < K;i++)
		knn[i].distance = MAXVALUE;
	for(i = 0;i < curTSize;i++)
	{
		dd = Distance(trSet[i],v);
		maxn = KnnMax(knn);//for every new state of the training set should update maxn
		if(dd < knn[maxn].distance) {
				knn[maxn].distance = dd;
				knn[maxn].classlabel = trSet[i].classlabel;
            }
	}
	for(i = 0;i < K;i++)//freq[i] represents knn[i].classlabel appears how many times 
		freq[i] = 1;
	for(i = 0;i < K;i++)  
		for(int j = 0;j < K;j++)
			if((i!=j)&&(knn[i].classlabel == knn[j].classlabel))
				freq[i]+=1;
#if debug		
		for(i = 0;i < K;i++)  
			cout<<"freq:"<<freq[i]<<endl;
#endif


	int mfreq = 1;
	mfreqC = knn[0].classlabel;
	for(i = 0;i < K;i++)
		if(freq[i] > mfreq)  {
			mfreq = freq[i];//mfreq represents the most frepuences
 			mfreqC = knn[i].classlabel; //mfreqNo is the item no. with the most frequent
			                             //classlabel
		}
#if debug	
	for(i = 0;i < K;i++)
		cout<<knn[i].distance<<"\t"<<knn[i].classlabel<<endl;
#endif
	return mfreqC;
}
void Knn::GetTrainData()
{
	TCHAR exePath[256] = { 0 };  ///exeÎÄ¼þÂ·¾¶  
	DWORD dwResult  = GetModuleFileName( AfxGetApp()->m_hInstance , exePath , 256 );  
	CString path = exePath;  
	CString apName  = AfxGetApp()->m_pszAppName;  
	path = path.Left(path.ReverseFind( _T('\\') ));  
	path = path.Left(path.ReverseFind( _T('\\') ));  
	CString filexx = path + _T("\\templateh\\knntrain.data");
	FILE *fList = fopen(filexx.GetBuffer(0),"r");
	if(NULL == fList) 
	{
		printf("open train.data file failed!\n"); 
		::AfxMessageBox("open knn train.data file failed!");
		return;
	}
	
	while(!feof(fList))
	{
		double label;
		int labeltmp;
		char ctmp,tmp;
		CvHuMoments humoment;
		fscanf(fList,"%c%c%d %lf %lf %lf %lf %lf %lf %lf ",&ctmp,&tmp,&labeltmp,&humoment.hu1, &humoment.hu2, &humoment.hu3, &humoment.hu4, &humoment.hu5, &humoment.hu6, &humoment.hu7);
		label=(double)labeltmp;
		//printf("tttttt=>%c,%c,%d\n",ctmp,tmp,labeltmp);
		Import(label,humoment);
	}

	fclose(fList);
}
bool Knn::Import(double label,CvHuMoments humoment)
{
	vectors trExmp = CreateVector(label,humoment);

	if(!AddtoTSet(trExmp))
		return false;
	return true;
}
vectors Knn::CreateVector(double label,CvHuMoments humoment)
{
	vectors trExmp;	
	trExmp.classlabel = label;


	trExmp.attributes[0] = humoment.hu1;
	trExmp.attributes[1] = humoment.hu2;
	trExmp.attributes[2] = humoment.hu3;
	trExmp.attributes[3] = humoment.hu4;
	trExmp.attributes[4] = humoment.hu5;
	trExmp.attributes[5] = humoment.hu6;
	trExmp.attributes[6] = humoment.hu7;

#if debug
	cout<<"lable:"<<trExmp.classlabel<<"| ";
	for(int j = 0;j < NATTRS;j++) 
	{
		cout<<trExmp.attributes[j]<<" ";
	}
	cout<<endl;

#endif
	return trExmp;
}