#include "tree2mca.h"
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

  FILE *output;
  TFile *inp;
  //randGen = new TRandom();

  if(argc!=2)
    {
      printf("tree2mca parameter_file\n");
      printf("\nTakes the specified branch of the specified ROOT tree file and converts it to an .mca file, where each spectrum in the .mca file is a leaf in the ROOT tree branch.\n");
      exit(-1);
    }
    
  readConfigFile(argv[1],"tree2mca"); //grab data from the parameter file

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
  printf("Tree read out.\n");

  stree->ResetBranchAddresses();
  stree->SetBranchAddress(sort_branch,data);
  printf("Branch address set.\n");
  printf("Number of tree entries: %Ld\n",stree->GetEntries());


  for (int i=0;i<S32K;i++)
    for (int j=0;j<NSPECT;j++)
        outHist[j][i]=0; //initialize all elements to 0

  for (int i=0;i<stree->GetEntries();i++)
    {
      stree->GetEntry(i);
      for (int j=0;j<numLeaf;j++)
        if(((data[j]*scaling)<S32K)&&((data[j]*scaling)>=0.0)&&(j<NSPECT))
          outHist[j][(int)(data[j]*scaling)]++; //fill the output histogram
    }

  //write the output histogram to disk
  if((output=fopen(out_filename,"w"))==NULL)
    {
      printf("ERROR: Cannot open the output .mca file!\n");
      exit(-1);
    }
  for (int i=0;i<numLeaf;i++)
    if(i<NSPECT)
      fwrite(outHist[i],S32K*sizeof(int),1,output);
  fclose(output);

  return(0); //great success
}
