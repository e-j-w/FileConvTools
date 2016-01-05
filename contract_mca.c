#include "mca.h"

int inpHist[NSPECT][S32K];
  
int main(int argc, char *argv[])
{

  FILE *input;
  FILE *output=NULL;
  int num_spect;
  int cntr;


  if(argc!=4)
    {
      printf("\ncontract_mca input_file contraction_factor output_file\n");
      printf("Contracts the input .mca file by the specified contraction factor.\n\n");
      exit(-1);
    }

  //read in command line arguments
  cntr=atoi(argv[2]);
  printf("Using a contraction factor of %i.\n",cntr);


  //read in the .mca file
  if((input=fopen(argv[1],"r"))==NULL)
    {
      printf("ERROR: Cannot open the input file %s!\n",argv[1]);
      exit(-1);
    }
  //get the number of spectra in the .mca file
  num_spect=NSPECT;
  for (int i=0;i<num_spect;i++)
    if(fread(inpHist[i],S32K*sizeof(int),1,input)!=1)
      {
        num_spect=i;
        break;
      }
  fclose(input);
  input=fopen(argv[1],"r");
  for (int i=0;i<num_spect;i++)
    if(fread(inpHist[i],S32K*sizeof(int),1,input)!=1)
      {
        printf("ERROR: Error reading file %s!\n",argv[1]);
        exit(-1);
      }
  printf("Read %i spectra from file %s\n",num_spect,argv[1]);
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
  if((output=fopen(argv[3],"w"))==NULL)
    {
      printf("ERROR: Cannot open the output .mca file!\n");
      exit(-1);
    }
    
  //write the output histogram to disk
  for (int i=0;i<num_spect;i++)
    fwrite(outHist[i],S32K*sizeof(int),1,output);
  fclose(output);
  printf("Wrote %i spectra to file %s\n",num_spect,argv[3]);

  return 0; //great success
}
