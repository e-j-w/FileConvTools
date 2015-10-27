#include "tree2mca.h"

Int_t gate_data[NSPECT];

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
  const char *sort_branch, *gate_branch;
  int sort_leaf, gate_leaf;
  bool same_branches = false;

  TFile *inp;
  TTree *stree;

  if(argc!=8)
    {
      printf("tree2mca_gated root_filename tree_name sort_branch_name sort_leaf_number gate_branch_name gate_leaf_number output_file\n");
      printf("\nTakes the data in the specified branch and leaf of the specified ROOT tree and sorts it to an .mca file with spectra gated on the data in the specified gate branch and leaf.\n");
      printf("Eg. the sort data could refer to gamma ray energy, while the gate data could refer to detector number.  Output is then an .mca file containing gamma-ray spectra, where the spectrum number corresponds to the detector number.  The gate data should contain integer values.\n");
      exit(-1);
    }

  //read in command line arguments
  sort_branch=argv[3];
  sort_leaf=atoi(argv[4]);
  gate_branch=argv[5];
  gate_leaf=atoi(argv[6]);

  //read in tree file
  inp = new TFile(argv[1],"read");
  if((stree = (TTree*)inp->Get(argv[2]))==NULL)
    {
      printf("The specified tree named %s doesn't exist, trying default name 'tree'.\n",argv[2]);
      if((stree = (TTree*)inp->Get("tree"))==NULL)//try the default tree name
        {
           printf("ERROR: The specified tree named %s (within the ROOT file) cannot be opened!\n",argv[2]);
           exit(-1);
        }
    }
  printf("Tree read out.\n");

  if(strcmp(sort_branch,gate_branch)==0)
    same_branches=true;

  stree->ResetBranchAddresses();
  if (!same_branches)
    stree->SetBranchAddress(sort_branch,data);
  stree->SetBranchAddress(gate_branch,gate_data);
  printf("Branch address set.\n");
  printf("Number of tree entries: %Ld\n",stree->GetEntries());


  for (int i=0;i<S32K;i++)
    for (int j=0;j<NSPECT;j++)
        outHist[j][i]=0; //initialize all elements to 0

  for (int i=0;i<stree->GetEntries();i++)
    {
      stree->GetEntry(i);

      //if the branches are the same, copy data from one to the other
      if(same_branches)
        for (int j=0;j<NSPECT;j++)
          data[j]=(double)gate_data[j];

      for (int j=0;j<NSPECT;j++)
        if(gate_data[gate_leaf]==j)
          if(data[sort_leaf]<S32K)
            outHist[j][(int)data[sort_leaf]]++; //fill the output histogram
    }

  //write the output histogram to disk
  if((output=fopen(argv[7],"w"))==NULL)
    {
      printf("ERROR: Cannot open the output .mca file!\n");
      exit(-1);
    }
  for (int i=0;i<NSPECT;i++)
    fwrite(outHist[i],S32K*sizeof(int),1,output);
  fclose(output);

  return(0); //great success
}
