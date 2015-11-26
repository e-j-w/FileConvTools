#include <stdlib.h>
#include <stdio.h>
#include <math.h>

//maximum number of lines and columns that can be input/output to/from text files
#define MAXFILELENGTH   1000000
#define MAXNUMCOLUMNS   12

//forward declarations
bool readFileData(const char *);
void setupFileRead();

//file I/O
FILE *input;
char str[2][256];
double fileValues[MAXNUMCOLUMNS];
int maxBin;
int numLines;
bool skipHeader=true;
bool negBin=false;
double val;
int bin;

//histogram bin calculation
double avg[MAXFILELENGTH],stdev[MAXFILELENGTH];
int numEntriesPerBin[MAXFILELENGTH];
