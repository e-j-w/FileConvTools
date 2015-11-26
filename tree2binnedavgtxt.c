#include "tree2binnedavgtxt.h"
#include "read_config.c"



int main(int argc, char *argv[])
{
  
  //Creating an instance of TApplication
  //This is evidently needed for auto-loading of ROOT libraries, 
  //otherwise the program may crash upon execution depending on how ROOT
  //is set up.
  //http://root.cern.ch/phpBB3/viewtopic.php?f=3&t=14064
  int ac;
  char* av[10];
  theApp=new TApplication("App", &ac, av);
  
  FILE *output=NULL;
  TFile *inp;

  if(argc!=2)
    {
      printf("\ntree2binnedavgtxt parameter_file\n");
      printf("--------------------------------\nConverts the data in the specified ROOT tree file into a histogram of averaged values, using bin size and ROOT tree branch information specified in the parameter file.\n\n");
      exit(-1);
    }

  //initialize arrays
  memset(avg,0,sizeof(avg));
  memset(stdev,0,sizeof(stdev));
  memset(numEntriesPerBin,0,sizeof(numEntriesPerBin));
  
  readConfigFile(argv[1],"tree2binnedavgtxt"); //grab data from the parameter file
  //check for out of bounds values
  if(binSize<=0)
    {
      printf("ERROR: Bin size cannot be 0 or negative!\n");
      exit(-1);
    }
    
  if(strcmp(x_branch,y_branch)==0)
    same_branches=true;
  
  //read in tree file
  inp = new TFile(inp_filename,"read");
  if((stree = (TTree*)inp->Get(tree_name))==NULL)
    {
      printf("The specified tree named %s doesn't exist, trying default name 'tree'.\n",tree_name);
      if((stree = (TTree*)inp->Get("tree"))==NULL)//try the default tree name
        {
          printf("ERROR: The specified tree named %s (within the ROOT file) cannot be opened!\n",tree_name);
          exit(-1);
        }
    }
  printf("Tree in %s read out.\n",inp_filename);

  stree->ResetBranchAddresses();
  if (!same_branches)
    stree->SetBranchAddress(y_branch,yVal);
  stree->SetBranchAddress(x_branch,xVal);
  printf("Branch address set.\n");
  printf("Number of tree entries: %Ld\n",stree->GetEntries());
  
  //read data from tree into data arrays
  maxBin=0;
  negBin=false;
  for(int i=0;i<stree->GetEntries();i++)
    { 
      getXYTreedata(i);
      //construct running sums
      if(bin>=0)
        {
          avg[bin]+=val;
          numEntriesPerBin[bin]++;
        }
         
    }
  
  //compute the average and standard deviation arrays
  for(int i=0;i<=maxBin;i++)
    avg[i]=avg[i]/numEntriesPerBin[i];
  for(int i=0;i<stree->GetEntries();i++)
    { 
      getXYTreedata(i);
      if(bin>=0)
        stdev[bin]+=(val-avg[bin])*(val-avg[bin]);
    }
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

//function which gets data from the ROOT tree at the specified entry and
//puts it in the bin and val arrays
void getXYTreedata(int entry)
{
  stree->GetEntry(entry);
        
  //if the branches are the same, copy data from one to the other
  if(same_branches)
    for (int j=0;j<MAXLEAVES;j++)
      yVal[j]=xVal[j];
          
  bin=(int)(xVal[x_leaf]/binSize);
  val=yVal[y_leaf];
  if(bin>maxBin)
    maxBin=bin;
  if(bin<0)
    if(negBin==false)
      {
        negBin=true;
        printf("x data contains negative values, these will be ignored...\n");
      }
  if(maxBin>MAXNUMBINS)
    {
      printf("ERROR: The number of bins in the output histogram is larger than the maximum value of %i!  Increase the bin size in the parameter file, or increase the value of MAXNUMBINS in tree2binnedavg.h and recompile.\n",MAXNUMBINS);
      exit(-1);
    }
}
