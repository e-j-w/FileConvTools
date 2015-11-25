#include <stdlib.h>
#include <stdio.h>
#include <math.h>

//ROOT stuff
#include "TRandom.h"
#include "TTree.h"
#include "TBranch.h"
#include "TFile.h"
#include "TApplication.h"

//maximum number of lines and columns that can be input/output to/from text files
#define MAXFILELENGTH   1000000
#define MAXNUMCOLUMNS   12

//maximum number of leaves in a given branch of a ROOT tree
#define MAXLEAVES 10

int maxBin;
int numLines;

bool same_branches;
Double_t xVal[MAXLEAVES],yVal[MAXLEAVES];
TTree *stree;
TApplication *theApp;
TRandom *randGen;




