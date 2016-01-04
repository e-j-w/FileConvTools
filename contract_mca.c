#include "mca.h"

int inpHist[NSPECT][S32K];
char str[256];
  
int main(int argc, char *argv[])
{

  FILE *input;
  FILE *output=NULL;
  int num_spect, cntr;


  if(argc!=5)
    {
      printf("\ncontract_mca number_of_spectra input contraction_factor output\n");
      printf("Contracts the input .mca file by the specified contraction factor.\n\n");
      exit(-1);
    }

  //read in command line arguments
  num_spect=atoi(argv[1]);
  cntr=atoi(argv[3]);
  printf("Using a contraction factor of %i.\n",cntr);
  if(num_spect>NSPECT)
    {
      printf("ERROR: Number of spectra requested is larger than the maximum of %i!\n",NSPECT);
      printf("Either specify a lower number or change the value of NSPECT in mca.h and recompile.\n");
      exit(-1);
    }


  //read in the .mca file
  if((input=fopen(argv[2],"r"))==NULL)
    {
      printf("ERROR: Cannot open the input file %s!\n",argv[2]);
      exit(-1);
    }
  for (int i=0;i<num_spect;i++)
    if(fread(inpHist[i],S32K*sizeof(int),1,input)!=1)
      {
        printf("ERROR: Error reading file %s!\n",argv[2]);
        printf("The number of spectra in the file may be less than the specified number of %i.\n",num_spect);
        exit(-1);
      }
  fclose(input);
  
  //initialize the output histogram
  for (int i=0;i<num_spect;i++)
    for (int j=0;j<S32K;j++)
      outHist[i][j]=0;
  
  //generate a contracted output histogram
  for (int i=0;i<num_spect;i++)
    for (int j=0;j<((int)(S32K/(float)cntr));j++)
      for (int k=0; k<cntr; k++)
        outHist[i][j]+=inpHist[i][cntr*j+k];
      
  //open the output file   
  if((output=fopen(argv[4],"w"))==NULL)
    {
      printf("ERROR: Cannot open the output .mca file!\n");
      exit(-1);
    }
    
  //write the output histogram to disk
  for (int i=0;i<NSPECT;i++)
    fwrite(outHist[i],S32K*sizeof(int),1,output);
  fclose(output);

  return 0; //great success
}
