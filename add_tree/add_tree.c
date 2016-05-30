#include "common.h"

#include "TTree.h"
#include "TFile.h"
#include "TApplication.h"

char str[256];
//Creating an instance of TApplication
//This is evidently needed for auto-loading of ROOT libraries, 
//otherwise the program may crash upon execution depending on how ROOT
//is set up.
//http://root.cern.ch/phpBB3/viewtopic.php?f=3&t=14064
int ac;
char* av[10];
TApplication *theApp=new TApplication("App", &ac, av);
  
int main(int argc, char *argv[])
{

  FILE *list;
  TFile *inp;
  TTree *tree;

  if(argc!=4)
    {
      printf("\nadd_tree input_root_file_list tree_name output_file\n");
      printf("-----------------------------\nAdds the specified ROOT trees together into one big ROOT tree (assuming they have the same branches/leaves).\n\n");
      exit(-1);
    }
    
  //read in tree list file
  if((list=fopen(argv[1],"r"))==NULL)
    {
      printf("ERROR: Cannot open the input list file %s!\n",argv[1]);
      exit(-1);
    } 
  
  TList tree_list;
  while(fscanf(list,"%s",str)!=EOF)
    {

      inp = new TFile(str,"read");
      if((tree = (TTree*)inp->Get(argv[2]))==NULL)
        {
          printf("The specified tree named %s doesn't exist, trying default name 'tree'.\n",gate_tree_name);
          if((tree = (TTree*)inp->Get("tree"))==NULL)//try the default tree name
            {
              printf("ERROR: The specified tree named %s (within the ROOT file) cannot be opened!\n",gate_tree_name);
              exit(-1);
            }
        }
      tree_list.Add(tree);
    }
  
  //set up the output file (must be done before setting up the output tree)
  TFile *f;
  f = new TFile(argv[3],"recreate");
  printf("Merging trees...\n");
  TTree::MergeTrees(&tree_list);
  printf("...done!\n");
  f->Write();
  f->Close();//close the output file
    
  return 0; //great success
}
