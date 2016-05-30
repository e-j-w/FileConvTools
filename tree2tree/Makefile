CFLAGS = -O -Wall -fPIC -ansi
ROOT = $(shell $(ROOTSYS)/bin/root-config --glibs) $(shell $(ROOTSYS)/bin/root-config --cflags)

all: tree2tree.c tree2tree.h read_config.c common.h
	g++ tree2tree.c $(CFLAGS) $(ROOT) -o ../bin/tree2tree
