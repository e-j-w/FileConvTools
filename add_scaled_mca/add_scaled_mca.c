#include "add_scaled_mca.h"

int readMCA(FILE* inp,char* filename,float inpHist[NSPECT][S32K])
{
	int num_spect=0;
  for (int i=0; i<NSPECT; i++)
    if(fread(mcaHist[i],S32K*sizeof(int),1,inp)!=1)
      {
      	num_spect=i;
				break; // dont read in more spectra than there are in the input file
				/* printf("ERROR: Error reading file %s!\n",filename); */
				/* printf("For i=%d fread(mcaHist[i],S32K*sizeof(int),1,inp)=%lu\n",i,fread(mcaHist[i],S32K*sizeof(int),1,inp)); */
				/* exit(-1); */
      }
 
  for (int i=0; i<NSPECT; i++)
    for (int j=0; j<S32K; j++)
      inpHist[i][j]=(float)mcaHist[i][j];

  return num_spect;
}

/*******************************************************************************/
int readFMCA(FILE* inp,char* filename,float inpHist[NSPECT][S32K])
{
	int num_spect=0;
  for (int i=0; i<NSPECT; i++)
    if(fread(inpHist[i],S32K*sizeof(float),1,inp)!=1)
      {
      	num_spect=i;
				break; // dont read in more spectra than there are in the input file
				/* printf("ERROR: Error reading file %s!\n",filename); */
				/* printf("For i=%d fread(inpHist[i],S32K*sizeof(float),1,inp)=%lu\n",i,fread(inpHist[i],S32K*sizeof(float),1,inp)); */
				/* exit(-1); */
      }

  return num_spect;
}

/*******************************************************************************/
int main(int argc, char *argv[])
{

  FILE *input1,*input2;
  FILE *output=NULL;
  FILE *sumOutput=NULL;
  bool writeWeights = false;

  if((argc!=7)&&(argc!=6)&&(argc!=3))
    {
      printf("\nadd_mca input1 scale_factor_1 input_2 scale_factor_2 output_sp output_weight_sp\n");
      printf("------------------------------\n");
      printf("Adds the two input .mca files together spectrum by spectrum and outputs a combined .fmca or .mca file given by output = scale_factor_1*input_1 + scale_factor_2*input_2.  Integer format will be saved if the output file extension is .mca.\n");
      printf("output_weight_sp is an optional parameter.  If used, a weighting spectrum will be written which can be used in RadWare or similar programs to weight background subtracted data.\n");
      printf("\nadd_mca input_list output output_weight_sp\n");
      printf("-------------------------\n");
      printf("Adds the all of the .fmca or .mca files apecified in the list file together spectrum by spectrum with the corresponding scale factors and outputs a combined .fmca or .mca file.\n");
      printf("The list file should contain the relative file paths to the individual .fmca or .mca files seperated by line.  Integer format will be saved if the output file extension is .mca.\n");
      printf("output_weight_sp is an optional parameter.  If used, a weighting spectrum will be written which can be used in RadWare or similar programs to weight background subtracted data.\n\n");
      exit(-1);
    }

  
  //initialize the output histograms
  for (int j=0;j<S32K;j++){
    for (int i=0;i<NSPECT;i++){
      outHist[i][j]=0.;
      weightHist[i][j]=0.;
      mcaHist[i][j]=0;
    }
    weightSumHist[j]=0;
  }

  //read in the .mca files
  if((argc==6)||(argc==7)) //two spectrum mode
    {
      

      if(argc==7){
        writeWeights = true;
      }
    	
      float scale1 = atof(argv[2]);
      float scale2 = atof(argv[4]);
      if((input1=fopen(argv[1],"r"))==NULL)
        {
          printf("ERROR: Cannot open the input file %s!\n",argv[1]);
          exit(-1);
        }
      if((input2=fopen(argv[3],"r"))==NULL)
        {
          printf("ERROR: Cannot open the input file %s!\n",argv[3]);
          exit(-1);
        }

      // read in input1
      const char *dot1 = strrchr(argv[1], '.'); // get the file extension
      if(strcmp(dot1 + 1,"mca")==0)
				readMCA(input1,argv[1],inpHist1);
      else if(strcmp(dot1 + 1,"fmca")==0)
				readFMCA(input1,argv[1],inpHist1);
      else
				{
					printf("ERROR: Improper type of input file: %s\n",argv[1]);
					printf("Integer array (.mca) and float array (.fmca) files are supported.\n");
					exit(-1);
				}  
      fclose(input1);
      
      
      
      //append values to the output histogram
      for (int i=0;i<NSPECT;i++)
        for (int j=0;j<S32K;j++){
          outHist[i][j]+=scale1*inpHist1[i][j];
          if(writeWeights){
            weightHist[i][j]+=scale1*scale1*fabs(inpHist1[i][j]);
            weightSumHist[j]+=scale1*scale1*fabs(inpHist1[i][j]);
          }
        }
          
      
      // read in input2
      const char *dot2 = strrchr(argv[3], '.'); // get the file extension
      if(strcmp(dot2 + 1,"mca")==0)
				readMCA(input2,argv[3],inpHist2);
      else if(strcmp(dot2 + 1,"fmca")==0)
				readFMCA(input2,argv[3],inpHist2);
      else
				{
					printf("ERROR: Improper type of input file: %s\n",argv[2]);
					printf("Integer array (.mca) and float array (.fmca) files are supported.\n");
					exit(-1);
				}  
      fclose(input2);

      //append values to the output histogram
      for (int i=0;i<NSPECT;i++)
        for (int j=0;j<S32K;j++){
          outHist[i][j]+=scale2*inpHist2[i][j];
          if(writeWeights){
            weightHist[i][j]+=scale2*scale2*fabs(inpHist2[i][j]);
            weightSumHist[j]+=scale2*scale2*fabs(inpHist2[i][j]);
          }
        }
          
      
      //open the output file   
      if((output=fopen(argv[5],"w"))==NULL)
        {
          printf("ERROR: Cannot open the output file (%s)!\n",argv[5]);
          exit(-1);
        }
      
      strncpy(outName,argv[5],256);
      if(writeWeights){
        strncpy(weightOutName,argv[6],256);
        /*for (int j=0;j<S32K;j++){
          for (int i=0;i<NSPECT;i++){
            weightHist[i][j]=sqrtf(weightHist[i][j]);
          }
          weightSumHist[j]=sqrtf(weightSumHist[j]);
        }*/
      }
        
    }

  if((argc==3)||(argc==4)) //spectrum list mode
    {

      if(argc==4){
        writeWeights = true;
      }

      char str[256];
      float scale=0.;
      //open the list file
      if((input1=fopen(argv[1],"r"))==NULL)
        {
          /* printf("ERROR: Cannot open the input list file %s!\n",argv[1]); */
          exit(-1);
        }
        
      //scan the list file for .mca files and add them to the output histogram
      while(fscanf(input1,"%s %f",str,&scale)!=EOF)
        {
          //open and read in the .mca file
          if((input2=fopen(str,"r"))==NULL)
            {
              /* printf("ERROR: Cannot open the input file %s!\n",str); */
              exit(-1);
            }
          
          //read in spectra
          //read in input2
					const char *dot2 = strrchr(str, '.'); // get the file extension
					if(strcmp(dot2 + 1,"mca")==0)
						readMCA(input2,str,inpHist2);
					else if(strcmp(dot2 + 1,"fmca")==0)
						readFMCA(input2,str,inpHist2);
					else
						{
							printf("ERROR: Improper type of input file: %s\n",argv[2]);
							printf("Integer array (.mca) and float array (.fmca) files are supported.\n");
							exit(-1);
						}
					fclose(input2);

          //add the .mca file values to the output histogram    
          for (int i=0;i<NSPECT;i++)
            for (int j=0;j<S32K;j++){
              outHist[i][j]+=scale*inpHist2[i][j];
              if(writeWeights){
                weightHist[i][j]+=scale*scale*fabs(inpHist2[i][j]);
                weightSumHist[j]+=scale*scale*fabs(inpHist2[i][j]);
              }
            }
              
        }
      fclose(input1);
        
      //open the output file
      if((output=fopen(argv[2],"w"))==NULL)
        {
          printf("ERROR: Cannot open the output .mca file!\n");
          exit(-1);
        }
      strncpy(outName,argv[2],256);
      if(writeWeights){
        strncpy(weightOutName,argv[3],256);
        /*for (int j=0;j<S32K;j++){
          for (int i=0;i<NSPECT;i++){
            weightHist[i][j]=sqrtf(weightHist[i][j]);
          }
          weightSumHist[j]=sqrtf(weightSumHist[j]);
        }*/
          
      }
    }

  // replace bins with less than zero counts
  // e.g. small negative data from bg subtraction
  /* for (int i=0;i<NSPECT;i++) */
  /*   for (int j=0;j<S32K;j++) */
  /*     if(outHist[i][j] < 0.) */
  /*     outHist[i][j]=0.; */
  
  //open the output file   
  if((output=fopen(outName,"w"))==NULL)
    {
      printf("ERROR: Cannot open the output file: %s!\n",outName);
      exit(-1);
    }
    
  const char *dot = strrchr(outName, '.'); // get output file extension
  if(strcmp(dot + 1,"mca")==0)
  	{
  		printf("Writing data in integer (.mca) format.\n");
  		
  		//convert data to integer
  		for (int i=0;i<NSPECT;i++)
  			for (int j=0;j<S32K;j++)
  				mcaHist[i][j]=outHist[i][j];
  		
  		//write the output histogram to disk
			for (int i=0;i<NSPECT;i++)
				fwrite(mcaHist[i],S32K*sizeof(int),1,output);
			fclose(output);
			printf("Wrote %i spectra to file %s\n",NSPECT,outName);
		}
	else if(strcmp(dot + 1,"fmca")==0)
  	{
  		printf("Writing data in floating-point (.fmca) format.\n");
  		
  		//write the output histogram to disk
			for (int i=0;i<NSPECT;i++)
				fwrite(outHist[i],S32K*sizeof(float),1,output);
			fclose(output);
			printf("Wrote %i spectra to file %s\n",NSPECT,outName);
  	}
  

  if(writeWeights){
    //open the weight spectrum file   
    if((output=fopen(weightOutName,"w"))==NULL){
      printf("ERROR: Cannot open the weight spectrum file: %s!\n",weightOutName);
      exit(-1);
    }
    
    char weightSumName[256];
    sprintf(weightSumName, "sum_%s",weightOutName);
    if((sumOutput=fopen(weightSumName,"w"))==NULL){
      printf("ERROR: Cannot open the weight sum spectrum file: %s!\n",weightOutName);
      exit(-1);
    }
      
    const char *dot = strrchr(weightOutName, '.'); // get output file extension
    if(strcmp(dot + 1,"mca")==0){
      printf("Writing weight data in integer (.mca) format.\n");
      
      //convert data to integer
      for (int i=0;i<NSPECT;i++){
        for (int j=0;j<S32K;j++){
          mcaHist[i][j]=weightHist[i][j];
        }
      }
      //write the output histogram to disk
      for (int i=0;i<NSPECT;i++)
        fwrite(mcaHist[i],S32K*sizeof(int),1,output);
      fclose(output);
      printf("Wrote %i spectra to weight spectrum file %s\n",NSPECT,weightOutName);

      //convert data to integer
      for (int j=0;j<S32K;j++){
        mcaHist[0][j]=weightSumHist[j];
      }
      //write the output histogram to disk
      fwrite(mcaHist[0],S32K*sizeof(int),1,sumOutput);
      fclose(sumOutput);
      printf("Wrote 1 spectrum to summed weight spectrum file %s\n",weightSumName);

    }else if(strcmp(dot + 1,"fmca")==0){
      printf("Writing weight data in floating-point (.fmca) format.\n");
      
      //write the output histogram to disk
      for (int i=0;i<NSPECT;i++)
        fwrite(weightHist[i],S32K*sizeof(float),1,output);
      fclose(output);
      printf("Wrote %i spectra to weight spectrum file %s\n",NSPECT,weightOutName);

      fwrite(weightSumHist,S32K*sizeof(float),1,sumOutput);
      fclose(sumOutput);
      printf("Wrote 1 spectrum to summed weight spectrum file %s\n",weightSumName);

    }
  }
   
  
  return 0; //great success
}
