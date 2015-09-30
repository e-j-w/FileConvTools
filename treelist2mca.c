#include "tree2mca.h"
#define EntrySize   50000

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

  FILE *list, *output;
  const char *branch;
  char rootFile[132];
  Double_t data[EntrySize];

  int outHist[S32K];

  TFile *inp;
  TTree *stree;

  if(argc!=5)
    {
      printf("treelist2mca root_filename_list tree_name branch output_file\n");
      printf("\nTakes the specified branch of ROOT tree files specified in the list and converts it to a single spectrum .mca file.\n");
      exit(-1);
    }

  for (int i=0;i<S32K;i++)
    outHist[i]=0; //initialize all elements to 0

  //read in command line arguments
  branch=argv[3];

  //read in list
  if((list=fopen(argv[1],"r"))==NULL)
    {
      printf("ERROR: Cannot open the list file!\n");
      exit(-1);
    }


  while(fscanf(list,"%s",rootFile)!=EOF)
    {
      //strcpy(rootFile,n);
      //read in tree file
      inp = new TFile(rootFile,"read");
      if((stree = (TTree*)inp->Get(argv[2]))==NULL)
        {
          printf("The specified tree named %s doesn't exist, trying default name 'tree'.\n",argv[2]);
          if((stree = (TTree*)inp->Get("tree"))==NULL)//try the default tree name
            {
              printf("ERROR: The specified tree named %s (within the ROOT file) cannot be opened!\n",argv[2]);
              exit(-1);
            }
        }
      printf("Tree in file %s read out.\n", rootFile);
      stree->ResetBranchAddresses();
      stree->SetBranchAddress(branch,data);
      printf("Branch address set.\n");
      printf("Number of tree entries: %Ld\n",stree->GetEntries());

      for (int i=0;i<stree->GetEntries();i++)
        {
          stree->GetEntry(i);
          if(data[0]<S32K)
            outHist[(int)data[0]]++; //fill the output histogram
        }

    }

  //write the output histogram to disk
  if((output=fopen(argv[4],"w"))==NULL)
    {
      printf("ERROR: Cannot open the output mca file!\n");
      exit(-1);
    }
  fwrite(outHist,S32K*sizeof(int),1,output);
  fclose(output);

  return(0); //great success
}
