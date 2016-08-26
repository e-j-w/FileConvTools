CFLAGS = -O -Wall -fPIC -ansi
ROOT = $(shell $(ROOTSYS)/bin/root-config --glibs) $(shell $(ROOTSYS)/bin/root-config --cflags)

all: tree2mca_gated.c tree2mca_gated.h read_config.c
	g++ tree2mca_gated.c $(CFLAGS) $(ROOT) -o ../bin/tree2mca_gated
