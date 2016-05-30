#ifndef TREE2TREE
#define TREE2TREE

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
double FWHM_response(double);
bool valueInRange(double, double, double);

TFile *f;
TTree *gtree;
TLeaf *sortLeaf, *gateLeaf;
TBranch *sortBranch, *gateBranch;
//TApplication *theApp;
TRandom3 *randGen;

#endif




