#include "scale_mca.h"

float inpHist[NSPECT][S32K];
  
int main(int argc, char *argv[])
{

  FILE *input,*scaleFactor;
  FILE *output=NULL;
  float scale[NSPECT]={};

  if(argc!=4)
    {
      printf("\nscale_mca input scale_factor_list output\n");
      printf("-------------------------\nScales the input mca file using the channel-by-channel scaling factors defined in the file and writes the spectrum to the output (fmca) file name.\n");
      printf("The list file should contain channels and corresponding scaling factors numbered the same way as in the mca file.\n\n");
      exit(-1);
    }

  //initialize the output histogram
  for (int i=0;i<NSPECT;i++)
    for (int j=0;j<S32K;j++)
      outHist[i][j]=0.;

  if(argc==4) // try to read in scaling factors
    {
       if((input=fopen(argv[1],"r"))==NULL)
        {
          printf("ERROR: Cannot open the input file %s!\n",argv[1]);
          exit(-1);
        }
       // read in input file
       const char *dot = strrchr(argv[1], '.'); // get the file extension
       if(strcmp(dot + 1,"mca")==0)
	 readMCA(input,argv[1],inpHist);
       else if(strcmp(dot + 1,"fmca")==0)
	 readFMCA(input,argv[1],inpHist);
       else
	 {
	   printf("ERROR: Improper type of input file: %s\n",argv[1]);
	   printf("Integer array (.mca) and float array (.fmca) files are supported.\n");
	   exit(-1);
	 }  
       fclose(input);

       if((scaleFactor=fopen(argv[2],"r"))==NULL)
        {
          printf("ERROR: Cannot open the scaling factor file %s!\n",argv[2]);
          exit(-1);
        }

       int sp=0;
       float sf=0.;
       while(fscanf(scaleFactor,"%d %f",&sp,&sf)!=EOF)
	 scale[sp]=sf;
       fclose(scaleFactor);
       /* for(int i=0;i<NSPECT;i++) */
       /* 	 printf("scaling factor %d %f\n",i,scale[i]);  */ 
    }

  //append values to the output histogram
  for (int i=0;i<NSPECT;i++)
    for (int j=0;j<S32K;j++)
      outHist[i][j]+=scale[i]*inpHist[i][j];

  //open the output file
  if((output=fopen(argv[3],"w"))==NULL)
    {
      printf("ERROR: Cannot open the output .mca file!\n");
      exit(-1);
    }
//write the output histogram to disk
  for (int i=0;i<NSPECT;i++)
    fwrite(outHist[i],S32K*sizeof(float),1,output);
  fclose(output);
  if(argc==4)
    printf("Wrote %i spectra to file %s\n",NSPECT,argv[3]);
      
      
  return 0;
}

/*******************************************************************************/
int readMCA(FILE* inp,char* filename,float inpHist[NSPECT][S32K])
{
  int mcaHist[NSPECT][S32K];
  for (int i=0; i<NSPECT; i++)
    if(fread(mcaHist[i],S32K*sizeof(int),1,inp)!=1)
      {
	printf("ERROR: Error reading file %s!\n",filename);
	exit(-1);
      }
 
  for (int i=0; i<NSPECT; i++)
    for (int j=0; j<S32K; j++)
      	inpHist[i][j]=(float)mcaHist[i][j];

  return 1;
}

/*******************************************************************************/
int readFMCA(FILE* inp,char* filename,float inpHist[NSPECT][S32K])
{
  for (int i=0; i<NSPECT; i++)
    if(fread(inpHist[i],S32K*sizeof(float),1,inp)!=1)
      {
	printf("ERROR: Error reading file %s!\n",filename);
	exit(-1);
      }

  return 1;
}
