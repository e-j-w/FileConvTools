#include "mca.h"

int inpHist1[NSPECT][S32K],inpHist2[NSPECT][S32K];
char str[256];
  
int main(int argc, char *argv[])
{

  FILE *input1,*input2;
  FILE *output=NULL;
  int num_spect;


  if((argc!=5)&&(argc!=4))
    {
      printf("\nadd_mca number_of_spectra input_1 input_2 output\n");
      printf("Adds the two input .mca files together spectrum by spectrum and outputs a combined .mca file.\n");
      printf("\nadd_mca number_of_spectra input_list output\n");
      printf("Adds the all of the .mca files apecified in the list file together spectrum by spectrum and outputs a combined .mca file.\n");
      printf("The list file should contain the relative file paths to the individual .mca files seperated by line.\n\n");
      exit(-1);
    }

  //read in command line arguments
  num_spect=atoi(argv[1]);
  if(num_spect>NSPECT)
    {
      printf("ERROR: Number of spectra requested is larger than the maximum of %i!\n",NSPECT);
      printf("Either specify a lower number or change the value of NSPECT in tree2mca.h and recompile.\n");
      exit(-1);
    }


  //read in the .mca files
  if(argc==5) //two spectrum mode
    {
      if((input1=fopen(argv[2],"r"))==NULL)
        {
          printf("ERROR: Cannot open the input file %s!\n",argv[2]);
          exit(-1);
        }
      if((input2=fopen(argv[3],"r"))==NULL)
        {
          printf("ERROR: Cannot open the input file %s!\n",argv[3]);
          exit(-1);
        }
      for (int i=0;i<num_spect;i++)
        if(fread(inpHist1[i],S32K*sizeof(int),1,input1)!=1)
          {
            printf("ERROR: Error reading file %s!\n",argv[2]);
            printf("The number of spectra in the file may be less than the specified number of %i.\n",num_spect);
            exit(-1);
          }
      for (int i=0;i<num_spect;i++)
        if(fread(inpHist2[i],S32K*sizeof(int),1,input2)!=1)
          {
            printf("ERROR: Error reading file %s!\n",argv[3]);
            printf("The number of spectra in the file may be less than the specified number of %i.\n",num_spect);
            exit(-1);
          }
      fclose(input1);
      fclose(input2);

      //add the .mca files together
      for (int i=0;i<num_spect;i++)
        for (int j=0;j<S32K;j++)
          outHist[i][j]=inpHist1[i][j]+inpHist2[i][j];
      
      //open the output file   
      if((output=fopen(argv[4],"w"))==NULL)
        {
          printf("ERROR: Cannot open the output .mca file!\n");
          exit(-1);
        }
    }
  if(argc==4) //spectrum list mode
    {
    
      //initialize the output histogram
      for (int i=0;i<num_spect;i++)
        for (int j=0;j<S32K;j++)
          outHist[i][j]=0;
    
      //open the list file
      if((input1=fopen(argv[2],"r"))==NULL)
        {
          printf("ERROR: Cannot open the input list file %s!\n",argv[2]);
          exit(-1);
        }
        
      //scan the list file for .mca files and add them to the output histogram
      while(fscanf(input1,"%s",str)!=EOF)
        {
          //open and read in the .mca file
          if((input2=fopen(str,"r"))==NULL)
            {
              printf("ERROR: Cannot open the input file %s!\n",argv[3]);
              exit(-1);
            }
          for (int i=0;i<num_spect;i++)
            if(fread(inpHist2[i],S32K*sizeof(int),1,input2)!=1)
              {
                printf("ERROR: Error reading file %s!\n",argv[3]);
                printf("The number of spectra in the file may be less than the specified number of %i.\n",num_spect);
                exit(-1);
              }  
          //add the .mca file values to the output histogram    
          for (int i=0;i<num_spect;i++)
            for (int j=0;j<S32K;j++)
              outHist[i][j]+=inpHist2[i][j];
              
        }
        
      fclose(input1);
        
      //open the output file   
      if((output=fopen(argv[3],"w"))==NULL)
        {
          printf("ERROR: Cannot open the output .mca file!\n");
          exit(-1);
        }
        
    }
    
  //write the output histogram to disk
  for (int i=0;i<NSPECT;i++)
    fwrite(outHist[i],S32K*sizeof(int),1,output);
  fclose(output);

  return(0); //great success
}
