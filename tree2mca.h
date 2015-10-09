#include <stdlib.h>
#include <stdio.h>

//ROOT stuff
#include "TTree.h"
#include "TBranch.h"
#include "TFile.h"
#include "TApplication.h"

#define S32K   32768
#define NSPECT 100

int outHist[NSPECT][S32K];
Double_t data[NSPECT];
TApplication *theApp;


