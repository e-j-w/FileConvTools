#ifndef TREE2MCA
#define TREE2MCA

#include "common.h"

//ROOT stuff
#include "TRandom3.h"
#include "TMath.h"
#include "TTree.h"
#include "TLeaf.h"
#include "TBranch.h"
#include "TFile.h"
#include "TApplication.h"

//forward declarations
void addTreeDataToOutHist();
double FWHM_response(double);
bool valueInRange(double, double, double);

int outHist[NSPECT][S32K];

TTree *stree,*gtree;
TLeaf *sortLeaf, *gateLeaf;
TBranch *sortBranch, *gateBranch;
TApplication *theApp;
TRandom3 *randGen;

#endif




