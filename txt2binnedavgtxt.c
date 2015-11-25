#include "binnedavgtxt.h"
#include "read_config.c"

double val[MAXFILELENGTH];
double avg[MAXFILELENGTH],stdev[MAXFILELENGTH];
int bin[MAXFILELENGTH],numEntriesPerBin[MAXFILELENGTH];
char str[2][256];
double fileValues[MAXNUMCOLUMNS];
FILE *input;

int main(int argc, char *argv[])
{
  
  FILE *output=NULL;

  if(argc!=2)
    {
      printf("\ntxt2binnedavgtxt parameter_file\n");
      printf("-------------------------------\nConverts the data in the specified text file into a histogram of averaged values, using bin size and input file formatting information specified in the parameter file.\n\n");
      exit(-1);
    }

  //initialize arrays
  memset(bin,0,sizeof(bin));
  memset(val,0,sizeof(val));
  memset(avg,0,sizeof(avg));
  memset(stdev,0,sizeof(stdev));
  memset(numEntriesPerBin,0,sizeof(numEntriesPerBin));
  
  //read in command line arguments
  readConfigFile(argv[1],"txt2binnedavgtxt"); //grab data from the parameter file
  //check for out of bounds values
  if(binSize<=0)
    {
      printf("ERROR: Bin size cannot be 0 or negative!\n");
      exit(-1);
    }
  
  //read in data from the .txt file
  if((input=fopen(inp_filename,"r"))==NULL)
    {
      printf("ERROR: Cannot open the input file %s!\n",inp_filename);
      exit(-1);
    }
    
  //read data from the input file
  if(file_handler_specified==true)
    readFileToArrays(file_handler);
  else
    readFileToArrays("default");
  fclose(input);
  
  //build the average and standard deviation arrays
  for(int i=0;i<numLines;i++)
    if(bin[i]>=0)
      {
        avg[bin[i]]+=val[i];
        numEntriesPerBin[bin[i]]++;
      }
  for(int i=0;i<=maxBin;i++)
    avg[i]=avg[i]/numEntriesPerBin[i];
  for(int i=0;i<numLines;i++)
    if(bin[i]>=0)
      stdev[bin[i]]+=(val[i] - avg[bin[i]])*(val[i] - avg[bin[i]]);
  for(int i=0;i<=maxBin;i++)
    stdev[i]=sqrt(stdev[i]/numEntriesPerBin[i]);
  
  
  //write the data to the output file on disk
  if((output=fopen(out_filename,"w"))==NULL)
    {
      printf("ERROR: Cannot open the output file!\n");
      exit(-1);
    }
  fprintf(output,"HISTOGRAM DATA\nBin i ranges from i*bin_size to i+1*bin_size.\n");
  fprintf(output,"Bin size: %f\n",binSize);
  fprintf(output,"---------------\nBIN VALUE STDEV\n");
  for(int i=0;i<maxBin;i++)
    fprintf(output,"%i %f %f\n",i,avg[i],stdev[i]);
  fclose(output);

  return(0); //great success
}

void readFileToArrays(const char * fileType)
{
  maxBin=0;
  bool negBin=false;
  
  //regular two column data
  if(strcmp(fileType,"default")==0)
    {
      if(fgets(str[0],256,input)!=NULL)
        {
          int i=0;
          while(fscanf(input,"%s %s\n",str[0],str[1])!=EOF)
            if(atof(str[1])!=0)//ignore values of 0 and text strings
	            if(i<MAXFILELENGTH)
	              {
                  bin[i]=(int)(atof(str[0])/binSize);
                  val[i]=atof(str[1]);
                  if(bin[i]>maxBin)
                    maxBin=bin[i];
                  if(bin[i]<0)
                    if(negBin==false)
                      {
                        negBin=true;
                        printf("x data contains negative values, these will be ignored...\n");
                      }
                  i++;
                }
          numLines=i;
        }      
      else
        {
        	printf("Wrong structure of file %s, aborting...\n",inp_filename);
          exit(-1);
        }
    }
    
  //SRIM energy vs depth data file
  if(strcmp(fileType,"SRIM_EXYZ")==0)
    {
      int i=0;
          
      //skip file header
      printf("Skipping input file header...\n");
      for(i=0;i<15;i++)
        if(fgets(str[0],256,input)!=NULL);//skip line (encapsulated in if to avoid comiler warning)
          
      i=0;
      int numFileValues=0;
      while(numFileValues!=EOF)
        if(i<MAXFILELENGTH)
	        {
	          numFileValues = fscanf(input,"%lf %lf %lf %lf %lf %lf %lf\n",&fileValues[0],&fileValues[1],&fileValues[2],&fileValues[3],&fileValues[4],&fileValues[5],&fileValues[6]);
	          if(numFileValues==7)
	            {
                bin[i]=(int)(fileValues[2]/binSize);
                val[i]=fileValues[1];
                if(bin[i]>maxBin)
                  maxBin=bin[i];
                if(bin[i]<0)
                  if(negBin==false)
                    {
                      negBin=true;
                      printf("x data contains negative values, these will be ignored...\n");
                    }
                i++;
              }
            else
              {
                printf("Unexpected number of entries on line following data point %i, skipping line.\n",i);
                if(fgets(str[0],256,input)!=NULL);//skip line
              }
          }
      numLines=i;
      printf("Read %i lines of data from input file, sorted into %i bins.\n",numLines,maxBin);
    }
}
