#include "mca.h"

int inpHist[NSPECT][S32K];
char str[256];
  
int main(int argc, char *argv[])
{

  FILE *input;
  FILE *output=NULL;
  int spect;
  bool mult_out=false;


  if((argc!=4)&&(argc!=3))
    {
      printf("\nmca2txt input_mca spectrum output_filename\n");
      printf("Converts the specified spectrum in the input .mca file to a tab delimited text file.\n");
      printf("\nmca2txt input_mca number_of_spectra\n");
      printf("Converts the input .mca file to a series of tab delimited text files, each containing data from a spectrum in the .mca file.  The text files are named after the original .mca file and the spectrum number.\n\n");
      exit(-1);
    }

  if(argc==3)
    mult_out=true;
  //read in command line arguments
  spect=atoi(argv[2]);
  
  if(spect>NSPECT)
    {
      if(mult_out==true)
        printf("ERROR: Number of spectra requested is larger than the maximum of %i!\n",NSPECT);
      if(mult_out==false)
        printf("ERROR: Spectrum number requested is larger than the maximum of %i!\n",NSPECT-1);
      printf("Either specify a lower number or increase the value of NSPECT in mca.h and recompile.\n");
      exit(-1);
    }
    
  //read in data from the .mca file
  if((input=fopen(argv[1],"r"))==NULL)
    {
      printf("ERROR: Cannot open the input file %s!\n",argv[1]);
      exit(-1);
    }
  for (int i=0;i<=spect;i++)
    if(fread(inpHist[i],S32K*sizeof(int),1,input)!=1)
      {
        printf("ERROR: Error reading file %s!\n",argv[1]);
        printf("The number of spectra in the file may be less than the specified number of %i.\n",spect);
        exit(-1);
      }
  fclose(input);
  
  //single output mode
  if(mult_out==false)
    {
      //open the output file   
      if((output=fopen(argv[3],"w"))==NULL)
        {
          printf("ERROR: Cannot open the output file!\n");
          exit(-1);
        }
      //write data to the output file
      fprintf(output,"CHANNEL VALUE\n");
      for (int i=0;i<S32K;i++)
        fprintf(output,"%i  %i\n", i, inpHist[spect][i]);
      fclose(output);
    }
  //multiple output mode
  if(mult_out==true)
    {
      for(int i=0;i<spect;i++)
        {
          sprintf(str,"%s%i.txt",argv[1],i);
          //open the output file   
          if((output=fopen(str,"w"))==NULL)
            {
              printf("ERROR: Cannot open the output file %s!\n",str);
              exit(-1);
            }
          //write data to the output file
          fprintf(output,"CHANNEL VALUE\n");
          for (int j=0;j<S32K;j++)
            fprintf(output,"%i  %i\n", j, inpHist[i][j]);
          fclose(output);
        }
    }

  return(0); //great success
}
