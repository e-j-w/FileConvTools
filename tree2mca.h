#include <stdlib.h>
#include <stdio.h>
#include <math.h>

//ROOT stuff
#include "TRandom.h"
#include "TTree.h"
#include "TBranch.h"
#include "TFile.h"
#include "TApplication.h"

#define S32K   32768
#define NSPECT 100

//forward declarations
void addTreeDataToOutHist();
double FWHM_response(double);

bool same_branches;
int outHist[NSPECT][S32K];
Double_t data[NSPECT];
TTree *stree;
TApplication *theApp;
TRandom *randGen;




