#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define S32K   32768
#define NSPECT 100

//forward declarations
void readFileToArrays(const char *);

int outHist[NSPECT][S32K];

