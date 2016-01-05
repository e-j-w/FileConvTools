#include "mca.h"

int inpHist[NSPECT][S32K];
  
int main(int argc, char *argv[])
{

  FILE *input;
  int num_spect;


  if(argc!=2)
    {
      printf("\nanalyze_mca input_file\n");
      printf("Prints useful (?) information about the input .mca file.\n\n");
      exit(-1);
    }

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
  if(num_spect<=0)
    printf("File %s contains no spectra or isn't an .mca file.\n",argv[1]);
  else
    printf("File %s contains %i spectra.\n",argv[1],num_spect);

  return 0; //great success
}
