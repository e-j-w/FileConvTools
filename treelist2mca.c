#include "tree2mca.h"
char rootFile[132];

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
  int numLeaf;


  TFile *inp;
  TTree *stree;

  if(argc!=6)
    {
      printf("treelist2mca root_filename_list tree_name branch number_of_leaves output_file\n");
      printf("\nTakes the specified branch of ROOT tree files specified in the list and converts it to an .mca file, where each spectrum in the .mca file is a leaf in the ROOT tree branch.\n");
      exit(-1);
    }

  for (int i=0;i<S32K;i++)
    for (int j=0;j<NSPECT;j++)
        outHist[j][i]=0; //initialize all elements to 0

  //read in command line arguments
  branch=argv[3];
  numLeaf=atoi(argv[4]);

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
          for (int j=0;j<numLeaf;j++)
            if((data[j]<S32K)&&(j<NSPECT))
              outHist[j][(int)data[j]]++; //fill the output histogram
        }

    }

  //write the output histogram to disk
  if((output=fopen(argv[5],"w"))==NULL)
    {
      printf("ERROR: Cannot open the output mca file!\n");
      exit(-1);
    }
  for (int i=0;i<numLeaf;i++)
    if(i<NSPECT)
      fwrite(outHist[i],S32K*sizeof(int),1,output);
  fclose(output);

  return(0); //great success
}
