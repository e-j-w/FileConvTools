#include <string.h>

//file I/O
FILE *config;
char str1[256],str2[256];

//tree2mca parameters
char sort_branch[256],gate_branch[256],inp_filename[256],tree_name[256],out_filename[256],file_handler[256];
int sort_leaf, gate_leaf, numLeaf;
double scaling=1.0;
double val_scaling=1.0;
bool listMode=false; //whether to sort tree files from a list 
bool fwhmResponse=false; //whether to do energy convolution
double fwhmF,fwhmG,fwhmH; //energy convolution parameters
bool output_specified=false;
bool file_handler_specified=false;

void readConfigFile(const char * fileName,const char *configType) 
{

  if((config=fopen(fileName,"r"))==NULL)
    {
      printf("ERROR: Cannot open the parameter file %s!\n",fileName);
      exit(-1);
    }
  while(fscanf(config,"%s %s",str1,str2)!=EOF)
    {
      //printf("%s %s.\n",str1,str2);
      if(strcmp(str1,"SORT_BRANCH")==0)
        strcpy(sort_branch,str2);
      if(strcmp(str1,"GATE_BRANCH")==0)
        strcpy(gate_branch,str2);
      if(strcmp(str1,"SORT_LEAF")==0)
        sort_leaf=atoi(str2);
      if(strcmp(str1,"GATE_LEAF")==0)
        sort_leaf=atoi(str2);
      if(strcmp(str1,"NUMBER_OF_LEAVES")==0)
        numLeaf=atoi(str2);
      if((strcmp(str1,"INPUT_TREE")==0)||(strcmp(str1,"INPUT_FILE")==0))
        {
          listMode=false;
          strcpy(inp_filename,str2);
        }
      if(strcmp(str1,"INPUT_TREE_LIST")==0)
        {
          listMode=true;
          strcpy(inp_filename,str2);
        }
      if(strcmp(str1,"TREE_NAME")==0)
        strcpy(tree_name,str2);
      if(strcmp(str1,"SORT_DATA_SCALING_FACTOR")==0)
        scaling=atof(str2);
      if(strcmp(str1,"SORT_DATA_FWHM_RESPONSE")==0)
        {
          if(strcmp(str2,"yes")==0)
            fwhmResponse=true;
          else
            fwhmResponse=false;
        }
      if(strcmp(str1,"FWHM_F")==0)
        fwhmF=atof(str2);
      if(strcmp(str1,"FWHM_G")==0)
        fwhmG=atof(str2);
      if(strcmp(str1,"FWHM_H")==0)
        fwhmH=atof(str2);
      if(strcmp(str1,"OUTPUT_FILE")==0)
        {
          output_specified=true;
          strcpy(out_filename,str2);
        }
      if(strcmp(str1,"BIN_SCALING_FACTOR")==0)
        scaling=atof(str2);
      if(strcmp(str1,"VALUE_SCALING_FACTOR")==0)
        val_scaling=atof(str2);
      if(strcmp(str1,"FILE_TYPE_HANDLER")==0)
        {
          file_handler_specified=true;
          strcpy(file_handler,str2);
        }
    }
  fclose(config);
  
  //Report parameters based on the config file type used
  if(strcmp(configType,"tree2mca")==0)
    {
      if(listMode==false)
        printf("Input tree file: %s\n",inp_filename);
      if(listMode==true)
        printf("Input list file: %s\n",inp_filename);
      printf("Name of tree in the file(s) to be used: %s\n",tree_name);
      printf("Sorting branch with name: %s\n",sort_branch);
      printf("Number of leaves to sort into seperate spectra in the output file: %i\n",numLeaf);
      if(fwhmResponse==true)
        {
          printf("Will apply FWHM response function to sorted data.\n");
          printf("FWHM response function paremeters: F=%f, G=%f, H=%f.\n",fwhmF,fwhmG,fwhmH);
        }
      if(scaling!=1.0)
        printf("Will scale sorted data by a factor of %f\n",scaling);
      if(output_specified==true)
        printf("Will save output data to file: %s\n",out_filename);
      if(output_specified==false)
        printf("Will save output data to individual files (input filenames + '.mca').\n");
    }
  if(strcmp(configType,"tree2mca_gated")==0)
    {
      if(listMode==false)
        printf("Input tree file: %s\n",inp_filename);
      if(listMode==true)
        printf("Input list file: %s\n",inp_filename);
      printf("Name of tree in the file(s) to be used: %s\n",tree_name);
      printf("Sorting branch with name: %s, leaf #%i.\n",sort_branch,sort_leaf);
      printf("Taking gate data taken from branch with name: %s, leaf #%i.\n",gate_branch,gate_leaf);
      if(fwhmResponse==true)
        {
          printf("Will apply FWHM response function to sorted data.\n");
          printf("FWHM response function paremeters: F=%f, G=%f, H=%f.\n",fwhmF,fwhmG,fwhmH);
        }
      if(scaling!=1.0)
        printf("Will scale sorted data by a factor of %f\n",scaling);
      if(output_specified==true)
        printf("Will save output data to file: %s\n",out_filename);
      if(output_specified==false)
        printf("Will save output data to individual files (input filenames + '.mca').\n");
    }
  if(strcmp(configType,"txt2mca")==0)
    {
      printf("Input text file: %s\n",inp_filename);
      if(file_handler_specified==true)
        printf("Assuming input file format: %s\n",file_handler);
      else
        printf("Assuming regular file formatting (two columns).\n");
      if(scaling!=1.0)
        printf("Will scale bins by a factor of %f\n",scaling);
      if(val_scaling!=1.0)
        printf("Will scale bins by a factor of %f\n",val_scaling);
      printf("Will save output data to .mca file: %s\n",out_filename);
    }
  
  printf("Finished reading parameter file...\n");
  
}
