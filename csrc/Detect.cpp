#include "Detect.h" 
#include <stdio.h>
#include "cv.h"
#include "highgui.h"
#include "integralFea.h"

/////////////////////////////////////////////////////////////////////////////////
bool WriteResults(FeaImage fea, char path[255], char filename[255], int index)
{
	char dir[255];
	//sprintf(dir,"%s\\%s",path,filename);	
	sprintf(dir,"%s\\%s",path,"result");	//for test
	//create a dictinoary by file name
	if( CreateDirectory(dir, NULL) )
	{
		printf("%s: Not Exist\n",dir);
		CreateDirectory(dir, NULL);
		printf("Create dictionary %s: succeed!\n",dir);
	}

	if(0 == index){		
		
		char name[255];
			sprintf(name,"%s\\%s-%d.jpg",dir,filename,index);			
			cvSaveImage(name, fea.RGB);			
	} 
	else 	if(1 == index){		
		
		char name[255];
	/*		for(int c =0;c< 4;c++){
			sprintf(name,"%s\\%s-%d-%d.jpg",dir,filename,index,c);			
			cvSaveImage(name, fea.mserMap[c]);	
		}			

		sprintf(name,"%s\\%s-%d.jpg",dir,filename,index);			
			cvSaveImage(name, fea.QUA);	*/
	 	//this comment just for test
			sprintf(name,"%s\\%s-RGB2.jpg",dir,filename);			
			cvSaveImage(name, fea.RGB);	
	}



	return 1;
}