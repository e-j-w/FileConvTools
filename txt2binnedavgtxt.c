#include "txt2binnedavgtxt.h"
#include "read_config.c"


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
  if(file_handler_specified==false)
    strcpy(file_handler,"default");
  
  
  //read in data from the .txt file
  if((input=fopen(inp_filename,"r"))==NULL)
    {
      printf("ERROR: Cannot open the input file %s!\n",inp_filename);
      exit(-1);
    }
    
  //read data from the input file
  setupFileRead();
  while(!readFileData(file_handler))
    if((bin>=0)&&(skipLine==false))
      {
        avg[bin]+=val; //construct running sums
        numEntriesPerBin[bin]++;
      }
  printf("Read %i lines of data from input file, sorted into %i bins.\n",numLines,maxBin);
  
  
  //compute the average and standard deviation arrays 
  for(int i=0;i<=maxBin;i++)
    avg[i]=avg[i]/numEntriesPerBin[i];
  setupFileRead();
  while(!readFileData(file_handler))
    if((bin>=0)&&(skipLine==false))
      stdev[bin]+=(val-avg[bin])*(val-avg[bin]);
  for(int i=0;i<=maxBin;i++)
    stdev[i]=sqrt(stdev[i]/numEntriesPerBin[i]);
  
  fclose(input);
  
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
    {
      if(numEntriesPerBin[i]>0)
        fprintf(output,"%i %f %f\n",i,avg[i],stdev[i]);
      else
        printf("No entries in histogram bin %i, skipping this bin!\n",i);
    }
  fclose(output);

  return 0; //great success
}

//read data from the file
//returns true when end of file is reached
bool readFileData(const char * fileType)
{
  skipLine=false; 
  
  //regular two column data
  if(strcmp(fileType,"default")==0)
    {
      int numFileValues = fscanf(input,"%s %s\n",str[0],str[1]);
      if(numFileValues!=EOF)
	      if(numFileValues==2)
	        {
	          if((atof(str[0])<max_x)||(use_max_x==false))
	            {
                bin=(int)(atof(str[0])/binSize);
                val=atof(str[1]);
                if(bin>maxBin)
                  maxBin=bin;
                numLines++;
              }
            else
              skipLine=true;
          }
        else
          {
            printf("Unexpected number of entries on line following data point %i, skipping line.\n",numLines);
            if(fgets(str[0],256,input)!=NULL);//skip line
          }
      else
        return true;
    }
    
  //SRIM energy vs depth data file
  if(strcmp(fileType,"SRIM_EXYZ")==0)
    {
          
      //skip file header if neccesary
      if(skipHeader)
        {
          skipHeader=false;
          printf("Skipping input file header...\n");
          for(int i=0;i<15;i++)
            if(fgets(str[0],256,input)!=NULL);//skip line (encapsulated in if to avoid comiler warning)
        }
      
      int numFileValues = fscanf(input,"%lf %lf %lf %lf %lf %lf %lf\n",&fileValues[0],&fileValues[1],&fileValues[2],&fileValues[3],&fileValues[4],&fileValues[5],&fileValues[6]);
      if(numFileValues!=EOF)
	      if(numFileValues==7)
	        {
	          if((fileValues[2]<max_x)||(use_max_x==false))
	            {
                bin=(int)(fileValues[2]/binSize);
                val=fileValues[1];
                if(bin>maxBin)
                  maxBin=bin;
                numLines++;
              }
            else
              skipLine=true;
          }
        else
          {
            printf("Unexpected number of entries on line following data point %i, skipping line.\n",numLines);
            if(fgets(str[0],256,input)!=NULL);//skip line
            skipLine=true;
          }
      else
        return true;
    }
    
  if(bin<0)
    if(negBin==false)
      {
        negBin=true;
        printf("x data contains negative values, these will be ignored...\n");
      }
    
  return false;
}

//sets up the file for reading again
void setupFileRead()
{
  skipHeader=true;
  numLines=0;
  maxBin=0;
  if(fseek(input, 0, SEEK_SET)) 
    {
      printf("Error seeking to start of file");
      exit(-1);
    }
}
