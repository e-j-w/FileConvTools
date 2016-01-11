#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "common.h"

#define CHN    -1
#define CHN_DIM    16384

int main(int argc, char *argv[])
{
  size_t elementsRead;
  char           acq_time[8],   /* buffer for time, date */
                    month[4];      /* buffer for month string */
  short          f_type;        /* .CHN file type */
  unsigned short chan_offset,   /* beginning channel number */
                    count,         /* loop counter */
                    mca_num,       /* MCA number */
                    num_chans,     /* no. of data channels */
                    segment;       /* segment number */
  int            livetime,      /* 20ms tics of livetime */    
                    realtime,      /* 20ms tics of realtime */
                    chan_data;     /* stores channel data */
  FILE           *input,*output;
  int hist[S32K];
  memset(hist,0,sizeof(hist));
  if (argc!=3 )
    {
      printf("\nchn2mca input_chn output_filename\n");
      printf("---------------------------------\nConverts the data in the specified .chn file into a single spectrum .mca file.\n\n");
	    exit(1);
    }
    
  if((input = fopen(argv[1],"r"))==NULL)
    {
      printf("ERROR: Cannot open the input file %s!\n",argv[1]);
      exit(-1);
    }

  elementsRead=fread( &f_type, sizeof(f_type), 1, input );
  if ( f_type != CHN) /*changed from .CHN to CHN*/
    {
      printf("ERROR: Input file %s is not a valid .chn file!\n",argv[1]);
      exit(1);
    }
    
  printf("Here!\n");  
     
  //read in the .chn file header and do absolutely nothing with it
  elementsRead=0;
  elementsRead+=fread(&mca_num, sizeof(mca_num), 1, input); /* MCA # */
  elementsRead+=fread(&segment, sizeof(segment), 1, input); /* seg # */
  elementsRead+=fread(acq_time, sizeof(char), 2, input);    /* start time */
  elementsRead+=fread(&realtime, sizeof(realtime), 1, input); /* real time, 20 ms tics */
  elementsRead+=fread(&livetime, sizeof(livetime), 1, input); /* live time, 20 ms tics */
  elementsRead+=fread(acq_time, sizeof(char), 2, input);      /* start day */
  elementsRead+=fread(month, sizeof(char), 3, input);         /* start month */
  elementsRead+=fread(acq_time, sizeof(char), 2, input);       /* start year */
  elementsRead+=fread(acq_time, sizeof(char), 1, input);      /* century */
  elementsRead+=fread(acq_time, sizeof(char), 2, input);      /* hour */
  elementsRead+=fread(acq_time, sizeof(char), 2, input);      /* minute */
  elementsRead+=fread(&chan_offset, sizeof(chan_offset), 1, input); /* offset */
  elementsRead+=fread(&num_chans, sizeof(num_chans), 1, input);     /* # chans */
     
  printf("%lu elements read from header of input file %s\n",elementsRead,argv[1]);
  elementsRead=0;

  //read in histogram data from the .chn file
  for (count = 0; count < num_chans; count++ )
    if(count<S32K)
      {
        elementsRead+=fread(&chan_data, sizeof(chan_data), 1, input );
	      hist[count]=chan_data;
      }       
  printf("%lu elements of data read from input file %s\n",elementsRead,argv[1]);
  
  fclose(input);
  
  if((output=fopen(argv[2],"w"))==NULL)
    {
      printf("ERROR: Cannot open the output .mca file!\n");
      exit(-1);
    }
  fwrite(hist,S32K*sizeof(int),1,output);
  
  fclose(output);
  printf("Output file %s written.\n",argv[2]);
  
  return 0;//great success
}
