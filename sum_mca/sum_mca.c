#include "sum_mca.h"

int inpHist1[NSPECT][S32K],inpHist2[NSPECT][S32K];
char str[256];
  
int main(int argc, char *argv[])
{

  FILE *input1;
  FILE *output=NULL;
  int num_spect;
  int startSp,endSp;


  if(argc!=5)
    {
      printf("\nsum_mca input_file start_sp end_sp output_file\n");
      printf("----------------------------------------------\nSums the spectra ranging from start_sp to end_sp in the input .mca file and writes the result to a single spectrum .mca file.\n\n");
      exit(-1);
    }
    
  //initialize the output histogram
  for (int i=0;i<NSPECT;i++)
    for (int j=0;j<S32K;j++)
      outHist[i][j]=0;
      
  //read in parameters
  startSp=atoi(argv[2]);
  endSp=atoi(argv[3]);
  if((startSp<0)||(endSp<0))
    {
      printf("ERROR: Start and end spectra cannot be negative!\n");
      exit(-1);
    }
  if((endSp-startSp)<0)
    {
      int tmp=startSp;
      startSp=endSp;
      endSp=tmp;
      printf("WARNING: Start and end spectra specified in the wrong order.  Summing from spectrum %i to %i.\n",startSp,endSp);
    }

  //read in the .mca file
  if((input1=fopen(argv[1],"r"))==NULL)
    {
      printf("ERROR: Cannot open the input file %s!\n",argv[1]);
      exit(-1);
    }

  //get the number of spectra in the .mca file and then read them in
  num_spect=NSPECT;
  for (int i=0;i<num_spect;i++)
    if(fread(inpHist1[i],S32K*sizeof(int),1,input1)!=1)
      {
        num_spect=i;
        break;
      }
  fclose(input1);
  input1=fopen(argv[1],"r");
  for (int i=0;i<num_spect;i++)
    if(fread(inpHist1[i],S32K*sizeof(int),1,input1)!=1)
      {
        printf("ERROR: Error reading file %s!\n",argv[1]);
        exit(-1);
      }
  printf("Read %i spectra from file %s\n",num_spect,argv[1]);
  fclose(input1);
  
  if(startSp>=num_spect)
    startSp=num_spect;
  if(endSp>=num_spect)
    endSp=num_spect;
     
  //append values to the output histogram
  for (int i=startSp;i<endSp;i++)
    for (int j=0;j<S32K;j++)
      outHist[0][j]+=inpHist1[i][j];
      
  //open the output file   
  if((output=fopen(argv[4],"w"))==NULL)
    {
      printf("ERROR: Cannot open the output .mca file!\n");
      exit(-1);
    }
    
  //write the output histogram to disk
  fwrite(outHist[0],S32K*sizeof(int),1,output);
  fclose(output);
    
  return 0; //great success
}
