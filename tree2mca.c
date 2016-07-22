#include "tree2mca.h"
#include "read_config.c"

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

  FILE *list, *output;
  TFile *inp;
  randGen = new TRandom3();

  if(argc!=2)
    {
      printf("\ntree2mca parameter_file\n");
      printf("-----------------------\nTakes the specified branch/leaf of the specified ROOT tree file(s) and converts to a single spectrum .mca file.\n\n");
      exit(-1);
    }
    
  readConfigFile(argv[1],"tree2mca"); //grab data from the parameter file
  
  
  for (int i=0;i<S32K;i++)
    outHist[0][i]=0; //initialize all elements to 0

  //sort list of ROOT files
  if(listMode==true)
    { 
      //read in tree list file
      if((list=fopen(inp_filename,"r"))==NULL)
        {
          printf("ERROR: Cannot open the input list file %s!\n",inp_filename);
          exit(-1);
        } 
      //scan the list file for ROOT files and put their data into the output hitogram
      while(fscanf(list,"%s",str)!=EOF)
        {
  
          inp = new TFile(str,"read");
          if((stree = (TTree*)inp->Get(sort_tree_name))==NULL)
            {
              printf("The specified tree named %s doesn't exist, trying default name 'tree'.\n",sort_tree_name);
              if((stree = (TTree*)inp->Get("tree"))==NULL)//try the default tree name
                {
                  printf("ERROR: The specified tree named %s (within the ROOT file) cannot be opened!\n",sort_tree_name);
                  exit(-1);
                }
            }
          printf("Tree in %s read out.\n",str);
          
          if((sortLeaf = stree->GetLeaf(sort_path))==NULL)
            if((sortBranch = stree->GetBranch(sort_path))==NULL)
              {
                printf("ERROR: Sort data path '%s' doesn't correspond to a branch or leaf in the tree!\n",sort_path);
                exit(-1);
              }
          if(sortLeaf==NULL)
            sortLeaf = (TLeaf*)sortBranch->GetListOfLeaves()->First(); //get the first leaf from the specified branch       
          printf("Paths to sort data set.\n");
          printf("Number of tree entries: %Ld\n",stree->GetEntries());          

          addTreeDataToOutHist();
          
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
    }
  
  //sort a single ROOT file  
  if(listMode==false)
    {
      //read in tree file
      inp = new TFile(inp_filename,"read");
      if((stree = (TTree*)inp->Get(sort_tree_name))==NULL)
        {
          printf("The specified tree named %s doesn't exist, trying default name 'tree'.\n",sort_tree_name);
          if((stree = (TTree*)inp->Get("tree"))==NULL)//try the default tree name
            {
               printf("ERROR: The specified tree named %s (within the ROOT file) cannot be opened!\n",sort_tree_name);
               exit(-1);
            }
        }
      printf("Tree in %s read out.\n",inp_filename);

      if((sortLeaf = stree->GetLeaf(sort_path))==NULL)
        if((sortBranch = stree->GetBranch(sort_path))==NULL)
          {
            printf("ERROR: Sort data path '%s' doesn't correspond to a branch or leaf in the tree!\n",sort_path);
            exit(-1);
          }
      if(sortLeaf==NULL)
        sortLeaf = (TLeaf*)sortBranch->GetListOfLeaves()->First(); //get the first leaf from the specified branch       
      printf("Paths to sort data set.\n");
      printf("Number of tree entries: %Ld\n",stree->GetEntries()); 
      
      addTreeDataToOutHist();
    }



  //write the output histogram to disk
  if((output=fopen(out_filename,"w"))==NULL)
    {
      printf("ERROR: Cannot open the output .mca file!\n");
      exit(-1);
    }
  fwrite(outHist[0],S32K*sizeof(int),1,output);
  fclose(output);

  return 0; //great success
}

//This function extracts data from the tree after 
//the tree addresses have been set and puts it in 
//the output histogram.
void addTreeDataToOutHist()
{
  Double_t sort_value;
  double histVal;
  
  for (int i=0;i<stree->GetEntries();i++)
    {
      stree->GetEntry(i);
      
      for(int j=0; j<sortLeaf->GetNdata(); j++) //deal with multiple fold events
        {
          sort_value = sortLeaf->GetValue(j);
          
          if(fwhmResponse==false)
            histVal=sort_value*sort_scaling + sort_shift;
          else
            histVal=FWHM_response(sort_value*sort_scaling + sort_shift);
          
          if(histVal>=0.0)
            if(histVal<S32K)
              outHist[0][(int)(histVal)]++; //fill the output histogram
        }
    }

}

double FWHM_response(double ch_in)
{
  if(ch_in==0.)
    return ch_in;

  double ch_out,fwhm,sigma,ch;
  double roll=randGen->Uniform();
  
  if((wL>0.0)||(wH>0.0))//gaussian with exponential tail(s)
    {
      if(roll<wG)//gaussian response
        {
          ch=ch_in/1000.;
          fwhm=sqrt(fwhmF*fwhmF + fwhmG*fwhmG*ch + fwhmH*fwhmH*ch*ch);
          sigma=fwhm/2.35482;
          if(sigma>0)
            ch_out=randGen->Gaus(ch_in,sigma);
          else
            ch_out=ch_in;
        }
      else if(roll<(wG+wH))//high energy exponential response
        {
          ch_out = ch_in + randGen->Exp(fwhmTauH);
        }
      else//low energy exponential response
        {
          ch_out = ch_in - randGen->Exp(fwhmTauL);
        }
    }
  else//gaussian only
    {
      ch=ch_in/1000.;
      fwhm=sqrt(fwhmF*fwhmF + fwhmG*fwhmG*ch + fwhmH*fwhmH*ch*ch);
      sigma=fwhm/2.35482;
      if(sigma>0)
        ch_out=randGen->Gaus(ch_in,sigma);
      else
        ch_out=ch_in;
    }

  return ch_out;
}
