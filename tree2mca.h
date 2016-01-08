#ifndef TREE2MCA
#define TREE2MCA

#include "common.h"

//ROOT stuff
#include "TRandom.h"
#include "TTree.h"
#include "TLeaf.h"
#include "TBranch.h"
#include "TFile.h"
#include "TApplication.h"

#define MAXLEAVES 10

//forward declarations
void addTreeDataToOutHist();
double FWHM_response(double);
bool valueInRange(double, double, double);

TTree *stree,*gtree;
TLeaf *sortLeaf, *gateLeaf;
TBranch *sortBranch, *gateBranch;
TApplication *theApp;
TRandom *randGen;

#endif




