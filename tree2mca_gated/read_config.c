#include <string.h>

void readConfigFile(const char * fileName) 
{

  if((config=fopen(fileName,"r"))==NULL)
    {
      printf("ERROR: Cannot open the parameter file %s!\n",fileName);
      exit(-1);
    }
  
  while(!(feof(config)))//go until the end of file is reached
    {
      if(fgets(cfgstr,256,config)!=NULL)
        {
          if(sscanf(cfgstr,"%s %s",str1,str2)==2) //single parameter data
            {
              if(strcmp(str1,"SORT_PATH")==0)
                strcpy(sort_path,str2);
              if(strcmp(str1,"GATE_PATH")==0)
                strcpy(gate_path,str2);
              if(strcmp(str1,"INPUT_FILE")==0)
                {
                  listMode=false;
                  strcpy(inp_filename,str2);
                }
              if(strcmp(str1,"INPUT_FILE_LIST")==0)
                {
                  listMode=true;
                  strcpy(inp_filename,str2);
                }
              if(strcmp(str1,"TREE_NAME")==0) //use this to put the game tree for sort and gate
                {
                  strcpy(sort_tree_name,str2);
                  strcpy(gate_tree_name,str2);
                }
              if(strcmp(str1,"SORT_TREE_NAME")==0)
                strcpy(sort_tree_name,str2);
              if(strcmp(str1,"GATE_TREE_NAME")==0)
                strcpy(gate_tree_name,str2);
              if(strcmp(str1,"SORT_DATA_SCALING_FACTOR")==0)
                sort_scaling=atof(str2);
              if(strcmp(str1,"SORT_DATA_SHIFT")==0)
                sort_shift=atof(str2);
              if(strcmp(str1,"GATE_DATA_SCALING_FACTOR")==0)
                gate_scaling=atof(str2);
              if(strcmp(str1,"GATE_DATA_SHIFT")==0)
                gate_shift=atof(str2);
              if(strcmp(str1,"CUSTOM_GATE_FILE")==0)
                {
                  if((customFile=fopen(str2,"r"))==NULL)
                    {
                      printf("ERROR: Cannot open the custom gate file %s specified in the parameter file!\n",str2);
                      exit(-1);
                    }
                  use_custom_gates=1;
                  num_custom_gates=0;
                  while(!(feof(customFile)))//go until the end of file is reached
                    {
                      if(num_custom_gates<NSPECT)
                        if(fgets(str2,256,customFile)!=NULL)
                          if(sscanf(str2,"%lf %lf",&custom_gates[num_custom_gates][0],&custom_gates[num_custom_gates][1])==2)
                            num_custom_gates++;
                    }
                  fclose(customFile);
                }
              if(strcmp(str1,"CUSTOM_GATE_WEIGHT_FILE")==0)
                {
                  if((customFile=fopen(str2,"r"))==NULL)
                    {
                      printf("ERROR: Cannot open the custom gate file %s specified in the parameter file!\n",str2);
                      exit(-1);
                    }
                  use_custom_gates=1;
                  use_gate_weights=true;
                  num_custom_gates=0;
                  while(!(feof(customFile)))//go until the end of file is reached
                    {
                      if(num_custom_gates<NSPECT){
                        if(fgets(str2,256,customFile)!=NULL)
                          if(sscanf(str2,"%lf %lf %lf",&custom_gates[num_custom_gates][0],&custom_gates[num_custom_gates][1],&gate_weight[num_custom_gates])==3)
                            num_custom_gates++;
                      }else{
                        break;
                      }
                    }
                  fclose(customFile);
                }
              if(strcmp(str1,"CUSTOM_2D_GATE_WEIGHT_FILE")==0)
                {
                  if((customFile=fopen(str2,"r"))==NULL)
                    {
                      printf("ERROR: Cannot open the custom 2D gate file %s specified in the parameter file!\n",str2);
                      exit(-1);
                    }
                  use_custom_gates=2;
                  use_gate_weights=true;
                  num_custom_gates=0;
                  while(!(feof(customFile)))//go until the end of file is reached
                    {
                      if(num_custom_gates<NSPECT){
                        if(fgets(str2,256,customFile)!=NULL)
                          if(sscanf(str2,"%lf %lf %lf %lf %lf",&custom_gates[num_custom_gates][0],&custom_gates[num_custom_gates][1],&custom_gates[num_custom_gates][2],&custom_gates[num_custom_gates][3],&gate_weight[num_custom_gates])==5)
                            num_custom_gates++;
                      }else{
                        break;
                      }
                    }
                  fclose(customFile);
                }
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
              if(strcmp(str1,"FWHM_TAU_HIGH")==0)
                fwhmTauH=atof(str2);
              if(strcmp(str1,"FWHM_TAU_LOW")==0)
                fwhmTauL=atof(str2);
              if(strcmp(str1,"EXP_HIGH_WEIGHT")==0)
                wH=atof(str2);
              if(strcmp(str1,"EXP_LOW_WEIGHT")==0)
                wL=atof(str2);
              if(strcmp(str1,"GAUS_WEIGHT")==0)
                wG=atof(str2);
              if(strcmp(str1,"OUTPUT_FILE")==0)
                {
                  output_specified=true;
                  strcpy(out_filename,str2);
                }
              if(strcmp(str1,"BIN_SCALING_FACTOR")==0)
                sort_scaling=atof(str2);
              if(strcmp(str1,"FILE_TYPE_HANDLER")==0)
                {
                  file_handler_specified=true;
                  strcpy(file_handler,str2);
                }
            }
          if(sscanf(cfgstr,"%s %s",str1,str2)==1) //only one item on line
            if(strcmp(str1,"<---END_OF_PARAMETERS--->")==0)
              break;  
        }
    }
  fclose(config);
  
  //Report parameters
  if(listMode==false)
    printf("Input tree file: %s\n",inp_filename);
  if(listMode==true)
    printf("Input list file: %s\n",inp_filename);
  printf("Sorting from leaf with path: %s in tree: %s\n",sort_path,sort_tree_name);
  printf("Taking gate data from leaf with path: %s in tree: %s\n",gate_path,gate_tree_name);
  if(fwhmResponse==true)
    {
      printf("Will apply FWHM response function to sorted data.\n");
      printf("FWHM response function paremeters: F=%f, G=%f, H=%f.\n",fwhmF,fwhmG,fwhmH);
      if(fwhmTauH>0.0)
        printf("A high energy exponential tail will be added, with tau=%0.3f channels and weight=%0.3f.\n",fwhmTauH,wH);
      if(fwhmTauL>0.0)
        printf("A low energy exponential tail will be added, with tau=%0.3f channels and weight=%0.3f.\n",fwhmTauL,wL);
    }
  if(sort_scaling!=1.0)
    printf("Will scale sorted data by a factor of %f\n",sort_scaling);
  if(sort_shift!=0.0)
    printf("Will shift sorted data by %0.2f bins.\n",sort_shift);
  if(use_custom_gates>0)
    {
      printf("Using custom gates. %i gates total.\n",num_custom_gates);
      if(use_gate_weights==false)
        {
          printf("First gate corresponds to gate data with values ranging from %f to %f\n",custom_gates[0][0],custom_gates[0][1]);
          printf("Last gate corresponds to gate data with values ranging from %f to %f\n",custom_gates[num_custom_gates-1][0],custom_gates[num_custom_gates-1][1]);
        }
      else
        {
          printf("First gate corresponds to gate data with values ranging from %f to %f with weight %f\n",custom_gates[0][0],custom_gates[0][1],gate_weight[0]);
          printf("Last gate corresponds to gate data with values ranging from %f to %f with weight %f\n",custom_gates[num_custom_gates-1][0],custom_gates[num_custom_gates-1][1],gate_weight[num_custom_gates-1]);
        }
    }
  else
    {
      if(gate_scaling!=1.0)
        printf("Will scale gate data by a factor of %f\n",gate_scaling);
      if(gate_shift!=0.0)
        printf("Will shift gate data by %0.2f bins.\n",gate_shift);
    }
  if(output_specified==true)
    printf("Will save output data to file: %s\n",out_filename);
  if(output_specified==false)
    printf("Will save output data to individual files (input filenames + '.mca').\n");
  
  printf("Finished reading parameter file...\n");
  
}
