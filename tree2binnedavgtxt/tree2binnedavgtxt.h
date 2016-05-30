#include <stdlib.h>
#include <stdio.h>
#include <math.h>

//ROOT stuff
#include "TTree.h"
#include "TBranch.h"
#include "TFile.h"
#include "TApplication.h"

//maximum number of lines(bins) that can be output to text files
#define MAXNUMBINS   100

//maximum number of leaves in a given branch of a ROOT tree
#define MAXLEAVES 10

//forward declarations
void getXYTreedata(int);

//ROOT
bool same_branches;
Double_t xVal[MAXLEAVES],yVal[MAXLEAVES];
TTree *stree;
TApplication *theApp;

//file I/O
FILE *input;
char str[2][256];
int maxBin;
int numLines;
bool negBin;
double val;
int bin;
bool skipLine;

//histogram bin calculation
double avg[MAXNUMBINS],stdev[MAXNUMBINS];
int numEntriesPerBin[MAXNUMBINS];






