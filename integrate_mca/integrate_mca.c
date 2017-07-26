#include "integrate_mca.h"

float inpHist[NSPECT][S32K];
  
int main(int argc, char *argv[])
{

  FILE *input,*integralLimits;
  float limits[2][NSPECT]={}; // 0 is low, 1 is high
  float integral[2][NSPECT]={}; // 0 is integral, 1 is error

  if(argc!=3)
    {
      printf("\nintegrate_mca input integration_limit_list\n");
      printf("-------------------------\nIntegrates the input mca file between the channel-by-channel (inclusive) integration limits (in channels!) defined in the file and prints the integrals and Poisson error sqrt(Cts) to the screen in a convienient way for the deorientation analysis performed for 84Kr.\n");
      printf("The list file should contain channels and corresponding integration limits numbered the same way as in the mca file.\n\n");
      exit(-1);
    }

  if(argc==3) // try to read in scaling factors
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

       if((integralLimits=fopen(argv[2],"r"))==NULL)
        {
          printf("ERROR: Cannot open the scaling factor file %s!\n",argv[2]);
          exit(-1);
        }

       // get integration limits
       int sp=0;
       int low=0.;
       int high=0.;
       while(fscanf(integralLimits,"%d %d %d",&sp,&low,&high)!=EOF)
	 {
	   limits[0][sp]=low;
	   limits[1][sp]=high;
	 }
       fclose(integralLimits);

       // do integral
       for(int i=0;i<NSPECT;i++)
	 {
	   for(int j=limits[0][i];j<=limits[1][i];j++) // limits inclusive
	     {
	       integral[0][i] += inpHist[i][j];
	     }
	   integral[1][i] = sqrt(integral[0][i]);
	 }
    }

  // printing for deorientation analysis
  printf("%f %f %f %f %f %f %f %f %f %f\n",integral[0][1],integral[1][1],integral[0][2],integral[1][2],integral[0][3],integral[1][3],integral[0][4],integral[1][4],integral[0][5],integral[1][5]);

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
