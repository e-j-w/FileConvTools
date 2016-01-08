#include "mca.h"

int inpHist[S32K][S32K];
  
int main(int argc, char *argv[])
{

  FILE *input;
  int num_spect;


  if(argc!=2)
    {
      printf("\nanalyze_mca input_file\n");
      printf("----------------------\nPrints useful (?) information about the input .mca file.\n\n");
      exit(-1);
    }

  //read in the .mca file
  if((input=fopen(argv[1],"r"))==NULL)
    {
      printf("ERROR: Cannot open the input file %s!\n",argv[1]);
      exit(-1);
    }
  //get the number of spectra in the .mca file
  num_spect=S32K;
  for (int i=0;i<num_spect;i++)
    if(fread(inpHist[i],S32K*sizeof(int),1,input)!=1)
      {
        num_spect=i;
        break;
      }
  fclose(input);
  if(num_spect<=0)
    printf("File %s contains no spectra or isn't an .mca file.\n",argv[1]);
  else if(num_spect>NSPECT)
    printf("File %s contains %i spectra.  This value is higher than NSPECT defined in common.h.\nIn order for the file to be properly read by other codes, change NSPECT in common.h to a value of %i or higher, and recompile.\n",argv[1],num_spect,num_spect);
  else
    printf("File %s contains %i spectra.\n",argv[1],num_spect);

  return 0; //great success
}
