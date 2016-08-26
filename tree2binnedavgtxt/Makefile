CFLAGS = -O -Wall -fPIC -ansi
ROOT = $(shell $(ROOTSYS)/bin/root-config --glibs) $(shell $(ROOTSYS)/bin/root-config --cflags)

all: tree2binnedavgtxt.c tree2binnedavgtxt.h read_config.c
	g++ tree2binnedavgtxt.c $(CFLAGS) $(ROOT) -o ../bin/tree2binnedavgtxt
