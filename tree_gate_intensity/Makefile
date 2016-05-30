CFLAGS = -O -Wall -fPIC -ansi
ROOT = $(shell $(ROOTSYS)/bin/root-config --glibs) $(shell $(ROOTSYS)/bin/root-config --cflags)

all: tree_gate_intensity.c tree2tree.h read_config.c common.h
	g++ tree_gate_intensity.c $(CFLAGS) $(ROOT) -o ../bin/tree_gate_intensity
