#include "tree2tree.h"
#include "read_config.c"

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

  //FILE *list;
  TFile *inp;
  randGen = new TRandom3();

  if(argc!=2)
    {
      printf("\ntree2tree parameter_file\n");
      printf("-----------------------------\nSeparates the ROOT tree specified in the parameter file into a ROOT tree containing a subset of the original data, according to the sort scheme specified in the parameter file.\n\n");
      exit(-1);
    }

  readConfigFile(argv[1],"tree2tree"); //grab data from the parameter file
  
  //WILL WANT TO IMPLEMENT LIST SORTING LATER
  //sort a single ROOT file  
  if(listMode==false)
    {
      //read in tree file
      inp = new TFile(inp_filename,"read");
      if((gtree = (TTree*)inp->Get(gate_tree_name))==NULL)
        {
          printf("The specified tree named %s doesn't exist, trying default name 'tree'.\n",gate_tree_name);
          if((gtree = (TTree*)inp->Get("tree"))==NULL)//try the default tree name
            {
              printf("ERROR: The specified tree named %s (within the ROOT file) cannot be opened!\n",gate_tree_name);
              exit(-1);
            }
        }
      printf("Tree in %s read out.\n",inp_filename);

      if((gateLeaf = gtree->GetLeaf(gate_path))==NULL)
        if((gateBranch = gtree->GetBranch(gate_path))==NULL)
          {
            printf("ERROR: Gate data path '%s' doesn't correspond to a branch or leaf in the tree!\n",gate_path);
            exit(-1);
          }
      if(gateLeaf==NULL)
        gateLeaf = (TLeaf*)gateBranch->GetListOfLeaves()->First(); //get the first leaf from the specified branch
      printf("Path to gate data set.\n");
      
      //set up the output file (must be done before setting up the output tree)
      TFile *f;
      if(output_specified==true)
        f = new TFile(out_filename,"recreate");
      else
        f = new TFile(strcat(inp_filename,"_out.root"),"recreate");
      TTree *sortedTree = gtree->CloneTree(0);
      printf("Output tree set up.\n");
      
      //sort the tree data
      Double_t gate_value[2];
      long long int entries;
      bool dropFlag=true;
      
      entries=gtree->GetEntries();
      printf("Number of entries in input tree: %Ld\n",entries);
      
      for (int i=0;i<entries;i++)
        {
          gtree->GetEntry(i);
          
          //printf("Values in gate leaf: %i\n",gateLeaf->GetNdata()); 
          
          dropFlag=true;
          if(use_gate_weights==true)//custom gates used with weights
            {
              if(use_custom_gates==1)//1D gate
                for(int j=0; j<gateLeaf->GetNdata(); j++) //deal with multiple fold events
                  {
                    gate_value[0] = gateLeaf->GetValue(j);
                    for (int k=0;k<num_custom_gates;k++)
                      if(valueInRange(gate_value[0],custom_gates[k][0],custom_gates[k][1]))
                        if(randGen->Uniform()<gate_weight[k])
                          dropFlag=false;
                  }
              
              if(use_custom_gates==2)//2D gate
                for(int j=0; j<gateLeaf->GetNdata(); j++) //deal with multiple fold events
                  for(int k=j+1; k<gateLeaf->GetNdata(); k++)
                    {
                      gate_value[0] = gateLeaf->GetValue(j);
                      gate_value[1] = gateLeaf->GetValue(k);
                      for (int l=0;l<num_custom_gates;l++)
                        if((valueInRange(gate_value[0],custom_gates[l][0],custom_gates[l][1]))&&(valueInRange(gate_value[1],custom_gates[l][2],custom_gates[l][3])))
                          {
                            if(randGen->Uniform()<gate_weight[l])
                              dropFlag=false;
                            break;
                          }
                        else if((valueInRange(gate_value[0],custom_gates[l][2],custom_gates[l][3]))&&(valueInRange(gate_value[1],custom_gates[l][0],custom_gates[l][1])))
                          {
                            if(randGen->Uniform()<gate_weight[l])
                              dropFlag=false;
                            break;
                          }
                    }
                
            }
          
          if(dropFlag==false)
            sortedTree->Fill();//keep the event
        }
        
      printf("Number of entries retained in output tree: %Ld\n",sortedTree->GetEntries());
      f->Write();
      delete f;//close the output file
    }
    
  return 0; //great success
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

//checks whether a value falls within the range of two bounds (lower bound inclusive)
//the order of the bounds doesn't matter
bool valueInRange(double value, double bound1, double bound2)
{

  if(bound1>bound2)
    {
      if((value>=bound2)&&(value<bound1))
        return true;
      else
        return false;
    }
  else if(bound2>bound1)
    {
      if((value>=bound1)&&(value<bound2))
        return true;
      else
        return false;
    }
  else if(bound1==bound2)
    {
      if(value==bound1)
        return true;
      else
        return false;
    }
    
  return false;
}
