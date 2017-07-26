//ROOT stuff
#include "TRandom2.h"
#include "TTree.h"
#include "TLeaf.h"
#include "TBranch.h"
#include "TFile.h"
#include "TApplication.h"
#include "TH1.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "Math/Minimizer.h"
#include "Math/Factory.h"
#include "Math/Functor.h"
#include "TROOT.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define S32K   32768
#define NSPECT 10

//
// global for config file reading
//
//
// File I/O
FILE *config,*customFile;
char cfgstr[256],str[256],str1[256],str2[256];

// bootstrap parameters
char sort_path[256],weight_path[256],gate_path[256],inp_filename[256],sort_tree_name[256],gate_tree_name[256],out_filename[256],file_handler[256];
char group_file[256],pos_path[256],col_path[256],csi_path[256]; // group sorting stuff
double custom_gates[NSPECT][4];
double gate_weight[NSPECT];
int num_custom_gates=0;
double sort_scaling=1.0;//data scaling factor
double sort_shift=0.0;//data shift in bins
double gate_scaling=1.0;//gate scaling factor
double gate_shift=0.0;//gate shift in bins
bool listMode=false; //whether to sort tree files from a list 
bool fwhmResponse=false; //whether to do energy convolution
double fwhmF,fwhmG,fwhmH,fwhmTauH,fwhmTauL,wH,wL,wG; //energy convolution parameters
bool output_specified=false;
bool file_handler_specified=false;
int use_custom_gates=0;//0=no gates,1=1D gates,2=2D gates, etc.
bool use_gate_weights=false;

//binnedavgtxt parameters
char x_branch[256],y_branch[256];
int x_leaf, y_leaf;
double val_scaling=1.0;
double binSize=1.0;
double max_x;
bool use_max_x=false;

// peak comp paramters
char expDataName[256],simDataName[256],statsDataName[256],resultsDataName[256],bgDataName[256]; // file names
int spectrum[NSPECT],startCh[NSPECT],endCh[NSPECT],numSpectra,endSpectrum,maxNumCh,numSimData;
double sfc[NSPECT],sfw[NSPECT];
int addBackground; // 0=no, 1=constant background
int plotOutput;    // 0=no, 1=yes, 2=detailed

//general output
int outHist[NSPECT][S32K];
float dOutHist[NSPECT][S32K];

//
// bootstrap
//
//
void addEventToOutHist(UInt_t evt);
int numEntries,numCompare;
TTree *stree,*btree;
TRandom2 *randGen;
TLeaf *sortLeaf, *weightLeaf, *gateLeaf;
TBranch *sortBranch, *weightBranch, *gateBranch;

// for sorting into groups
void readGroupMap();
double FWHM_response(double);
int  group_map[17][5][25];
TLeaf *posLeaf,*colLeaf,*csiLeaf,*csiELeaf;
TBranch *posBranch,*colBranch,*csiBranch;
int groupHist[NSPECT][S32K];

// histogram fitting and analysis
double lrchisq(const double *par); // likelihood ratio chisq for spectrum sp
void   find_chisqMin(); // ROOT minimization class
void   plotSpectra();   // ROOT canvas
int    readMCA(FILE* inp,char* filename,float inpHist[NSPECT][S32K]);
int    readFMCA(FILE* inp,char* filename,float inpHist[NSPECT][S32K]);
float  expHist[NSPECT][S32K];
float  simHist[NSPECT][S32K];
float  likelihoodHist[NSPECT][S32K];
double chisq;
double scaledSimHist[NSPECT][S32K];
float  resultsHist[NSPECT][S32K];
TApplication *theApp;
TCanvas *c;
TH1D *data[NSPECT],*sim[NSPECT];
double expCurrent[S32K],simCurrent[S32K];
int spCurrent;

// fit parameters
double aFinal[3][NSPECT]; // final parameters



