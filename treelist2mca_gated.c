#include "tree2mca.h"

Int_t gate_data[NSPECT];
char str[256];

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
  double scaling=1.0; //factor to scale data by

  bool output_specified = false;

  FILE *list;
  TFile *inp;
  TTree *stree;

  if((argc!=9)&&(argc!=8))
    {
      printf("\ntreelist2mca_gated root_file_list tree_name sort_branch_name sort_leaf_number scaling gate_branch_name gate_leaf_number output_file\n");
      printf("\nTakes the data in the specified branch and leaf of the specified ROOT trees and sorts it to .mca files with spectra gated on the data in the specified gate branch and leaf.\n");
      printf("Eg. the sort data could refer to gamma ray energy, while the gate data could refer to detector number.  Output is then an .mca file containing gamma-ray spectra, where the spectrum number corresponds to the detector number.  The gate data should contain integer values.\n");
      printf("\nscaling specifies a scaling factor for the sort data, which can be used to get different binning in the output spectra.\n");
      printf("\nThe output_file parameter is optional - if used, all data will be sorted into a single .mca file with the filename specified by output_file.  Otherwise, individual .mca files for each root tree will be generated with matching filenames.\n\n");
      exit(-1);
    }
    
  printf("Here!\n");
    
  if(argc==9)
    {
      output_specified = true;
      printf("Sorting data to single output file %s.\n", argv[7]);
      printf("Remove the last argument if you want to sort data to individual .mca files.\n");
    }
  else
    {
      printf("Sorting data to individual .mca files.\n");
      printf("Add an output_file argument if you want to sort all data to a single .mca file.\n");
    }

  //read in command line arguments
  sort_branch=argv[3];
  sort_leaf=atoi(argv[4]);
  scaling=atof(argv[5]);
  gate_branch=argv[6];
  gate_leaf=atoi(argv[7]);
  if(strcmp(sort_branch,gate_branch)==0)
    same_branches=true;
  
  //initialize the output histogram
  for (int i=0;i<NSPECT;i++)
    for (int j=0;j<S32K;j++)
      outHist[i][j]=0;

  //read in tree list file
  if((list=fopen(argv[1],"r"))==NULL)
    {
      printf("ERROR: Cannot open the input list file %s!\n",argv[1]);
      exit(-1);
    }
    
  //scan the list file for ROOT files and put their data into the output hitogram
  while(fscanf(list,"%s",str)!=EOF)
    {
  
      inp = new TFile(str,"read");
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

      stree->ResetBranchAddresses();
      if (!same_branches)
        stree->SetBranchAddress(sort_branch,data);
      stree->SetBranchAddress(gate_branch,gate_data);
      printf("Branch address set.\n");
      printf("Number of tree entries: %Ld\n",stree->GetEntries());

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
                outHist[j][(int)(data[sort_leaf]*scaling)]++; //fill the output histogram
        }
        
      //save results to individual .mca files if applicable
      if(output_specified==false)
        {
          if((output=fopen(strcat(str,".mca"),"w"))==NULL)
          {
            printf("ERROR: Cannot open the output .mca file %s!\n",strcat(str,".mca"));
            exit(-1);
          }
          for (int i=0;i<NSPECT;i++)
            fwrite(outHist[i],S32K*sizeof(int),1,output);
          fclose(output);
          
          //reset the output histogram
          for (int i=0;i<NSPECT;i++)
            for (int j=0;j<S32K;j++)
              outHist[i][j]=0;
        }

    }
    
  fclose(list);

  //save results to a single .mca file if applicable
  if(output_specified==true)
    {
      //write the output histogram to disk
      if((output=fopen(argv[8],"w"))==NULL)
        {
          printf("ERROR: Cannot open the output .mca file!\n");
          exit(-1);
        }
      for (int i=0;i<NSPECT;i++)
        fwrite(outHist[i],S32K*sizeof(int),1,output);
      fclose(output);
    }
    
  return(0); //great success
}
