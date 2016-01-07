#include "mca.h"

int inpHist1[NSPECT][S32K],inpHist2[NSPECT][S32K];
char str[256];
  
int main(int argc, char *argv[])
{

  FILE *input1,*input2;
  FILE *output=NULL;
  int num_spect;
  int max_num_spect=0;


  if((argc!=4)&&(argc!=3))
    {
      printf("\nadd_mca input_1 input_2 output\n");
      printf("------------------------------\nAdds the two input .mca files together spectrum by spectrum and outputs a combined .mca file.\n");
      printf("\nadd_mca input_list output\n");
      printf("-------------------------\nAdds the all of the .mca files apecified in the list file together spectrum by spectrum and outputs a combined .mca file.\n");
      printf("The list file should contain the relative file paths to the individual .mca files seperated by line.\n\n");
      exit(-1);
    }
    
  //initialize the output histogram
  for (int i=0;i<NSPECT;i++)
    for (int j=0;j<S32K;j++)
      outHist[i][j]=0;

  //read in the .mca files
  if(argc==4) //two spectrum mode
    {
      if((input1=fopen(argv[1],"r"))==NULL)
        {
          printf("ERROR: Cannot open the input file %s!\n",argv[1]);
          exit(-1);
        }
      if((input2=fopen(argv[2],"r"))==NULL)
        {
          printf("ERROR: Cannot open the input file %s!\n",argv[2]);
          exit(-1);
        }
      //get the number of spectra in the .mca files and then read them in
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
      if(num_spect>max_num_spect)
        max_num_spect=num_spect;
      fclose(input1);
      
      //append values to the output histogram
      for (int i=0;i<num_spect;i++)
        for (int j=0;j<S32K;j++)
          outHist[i][j]+=inpHist1[i][j];
      
      num_spect=NSPECT;
      for (int i=0;i<num_spect;i++)
        if(fread(inpHist2[i],S32K*sizeof(int),1,input2)!=1)
          {
            num_spect=i;
            break;
          }
      fclose(input2);
      input2=fopen(argv[2],"r");
      for (int i=0;i<num_spect;i++)
        if(fread(inpHist2[i],S32K*sizeof(int),1,input2)!=1)
          {
            printf("ERROR: Error reading file %s!\n",argv[2]);
            exit(-1);
          }
      printf("Read %i spectra from file %s\n",num_spect,argv[2]);
      if(num_spect>max_num_spect)
        max_num_spect=num_spect;
      fclose(input2);
      
      //append values to the output histogram
      for (int i=0;i<num_spect;i++)
        for (int j=0;j<S32K;j++)
          outHist[i][j]+=inpHist2[i][j];
      
      //open the output file   
      if((output=fopen(argv[3],"w"))==NULL)
        {
          printf("ERROR: Cannot open the output .mca file!\n");
          exit(-1);
        }
    }
  if(argc==3) //spectrum list mode
    {
      //open the list file
      if((input1=fopen(argv[1],"r"))==NULL)
        {
          printf("ERROR: Cannot open the input list file %s!\n",argv[1]);
          exit(-1);
        }
        
      //scan the list file for .mca files and add them to the output histogram
      while(fscanf(input1,"%s",str)!=EOF)
        {
          //open and read in the .mca file
          if((input2=fopen(str,"r"))==NULL)
            {
              printf("ERROR: Cannot open the input file %s!\n",str);
              exit(-1);
            }
            
          //get the number of spectra in the .mca file
          num_spect=NSPECT;
          for (int i=0;i<num_spect;i++)
            if(fread(inpHist2[i],S32K*sizeof(int),1,input2)!=1)
              {
                num_spect=i;
                break;
              }
          fclose(input2);
          
          //read in spectra
          input2=fopen(str,"r");
          for (int i=0;i<num_spect;i++)
            if(fread(inpHist2[i],S32K*sizeof(int),1,input2)!=1)
              {
                printf("ERROR: Error reading file %s!\n",str);
                exit(-1);
              }
          printf("Read %i spectra from file %s\n",num_spect,str);
          if(num_spect>max_num_spect)
            max_num_spect=num_spect;
          fclose(input2);
              
          //add the .mca file values to the output histogram    
          for (int i=0;i<num_spect;i++)
            for (int j=0;j<S32K;j++)
              outHist[i][j]+=inpHist2[i][j];
              
          
        }
        
      fclose(input1);
        
      //open the output file   
      if((output=fopen(argv[2],"w"))==NULL)
        {
          printf("ERROR: Cannot open the output .mca file!\n");
          exit(-1);
        }
        
    }
    
  //write the output histogram to disk
  for (int i=0;i<max_num_spect;i++)
    fwrite(outHist[i],S32K*sizeof(int),1,output);
  fclose(output);
  if(argc==3)
    printf("Wrote %i spectra to file %s\n",max_num_spect,argv[2]);
  if(argc==4)
    printf("Wrote %i spectra to file %s\n",max_num_spect,argv[3]);
    
  return(0); //great success
}
