/************************************************************/
/* Sample program compatible with Microsoft and Borland C   */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "common.h"

#define SPX_DIM    16384
int main(int argc, char *argv[])
{

  FILE *input,*output;
  long int inpHist[SPX_DIM];
  int outHist[S32K];
  
  if (argc != 3 )
    {
      printf("\nspx2mca input_spx output_filename\n");
      printf("---------------------------------\nConverts the data in the specified .spx file into a single spectrum .mca file.\n\n");
	    exit(1);
    }
    
  memset(inpHist,0,sizeof(inpHist));
  memset(outHist,0,sizeof(outHist));
  
  if((input=fopen(argv[1],"r"))==NULL)
    {
      printf("ERROR: Cannot open the input file %s!\n",argv[1]);
      exit(-1);
    }
  if(fread(inpHist,SPX_DIM, sizeof(long int), input)<1)
    {
      printf("ERROR: Error reading input file %s, improper format!\n",argv[1]);
      exit(-1);
    }
  fclose(input);
     
  for(int ch=0;ch<SPX_DIM;ch++)
    {
	    outHist[ch]=(int)inpHist[ch];
    }
    
  if((output = fopen(argv[2], "w"))==NULL)
    {
      printf("ERROR: Cannot open the output .mca file!\n");
      exit(-1);
    }

  fwrite(outHist,S32K*sizeof(int),1,output);
  fclose(output);
  
  return 0;//great success
}
