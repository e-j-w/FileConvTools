#include <stdlib.h>
#include <stdio.h>
#include <math.h>

//ROOT stuff
#include "TRandom.h"
#include "TTree.h"
#include "TLeaf.h"
#include "TBranch.h"
#include "TFile.h"
#include "TApplication.h"

#define S32K   32768
#define NSPECT 100
#define MAXLEAVES 10

//forward declarations
void addTreeDataToOutHist();
double FWHM_response(double);

int outHist[NSPECT][S32K];
TTree *stree;
TLeaf *sortLeaf, *gateLeaf;
TBranch *sortBranch, *gateBranch;
TApplication *theApp;
TRandom *randGen;




