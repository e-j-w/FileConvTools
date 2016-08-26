CFLAGS = -O -Wall -fPIC -ansi
ROOT = $(shell $(ROOTSYS)/bin/root-config --glibs) $(shell $(ROOTSYS)/bin/root-config --cflags)

all: tree_gate_intensity.c tree_gate_intensity.h read_config.c
	g++ tree_gate_intensity.c $(CFLAGS) $(ROOT) -o ../bin/tree_gate_intensity
