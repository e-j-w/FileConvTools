#include "mca.h"

double ch[S32K],val[S32K];
char str[2][256];
  
int main(int argc, char *argv[])
{

  FILE *input;
  FILE *output=NULL;
  double scaling=1.0;
  double val_scaling=1.0;

  if((argc!=5)&&(argc!=4)&&(argc!=3))
    {
      printf("\ntxt2mca input_txt output_filename\n");
      printf("---------------------------------\nConverts the data in the specified text file into .mca format.\n");
      printf("\ntxt2mca input_txt output_filename bin_scaling_factor\n");
      printf("txt2mca input_txt output_filename bin_scaling_factor value_scaling_factor\n");
      printf("-------------------------------------------------------------------------\nConverts the data in the specified text file into .mca format, scaling the bins and/or values in the text files by the specified scaling factor(s).\n");
      printf("\nThis code assumes a two column text file, space or tab delimited, with the first column being the bin number and the second column being the bin value.  The output .mca file contains a single spectrum.\n\n");
      exit(-1);
    }

  //read in command line arguments
  if(argc==4)
    scaling=atof(argv[3]);
  if(argc==5)
    {
      scaling=atof(argv[3]);
      val_scaling=atof(argv[4]);
    }
  
  //read in data from the .txt file
  if((input=fopen(argv[1],"r"))==NULL)
    {
      printf("ERROR: Cannot open the input file %s!\n",argv[1]);
      exit(-1);
    }
  if(fgets(str[0],256,input)!=NULL)
    {
      int i=0;
      while(fscanf(input,"%s %s\n",str[0],str[1])!=EOF)
        if(atof(str[1])!=0)//ignore bin values of 0 and text strings
	        if(i<S32K)
	          {
              ch[i]=atof(str[0]);
              val[i]=atof(str[1]);
              i++;
            }
    }      
  else
    {
    	printf("Wrong structure of file %s, aborting...\n",argv[1]);
      exit(-1);
    }
  fclose(input);
  
  
  //put data into .mca format
  for(int i=0;i<S32K;i++)
    outHist[0][i]=0;//initialize all elements to 0
  for(int i=0;i<S32K;i++)
    if((ch[i]*scaling)>0.0)
      if((ch[i]*scaling)<S32K)
        outHist[0][(int)round(ch[i]*scaling)]+=(int)round(val[i]*val_scaling);
  
  //write the output histogram to disk
  if((output=fopen(argv[2],"w"))==NULL)
    {
      printf("ERROR: Cannot open the output .mca file!\n");
      exit(-1);
    }
  fwrite(outHist[0],S32K*sizeof(int),1,output);
  fclose(output);

  return(0); //great success
}
