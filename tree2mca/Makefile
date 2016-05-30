CFLAGS = -O -Wall -fPIC -ansi
ROOT = $(shell $(ROOTSYS)/bin/root-config --glibs) $(shell $(ROOTSYS)/bin/root-config --cflags)

all: tree2mca.c tree2mca.h read_config.c
	g++ tree2mca.c $(CFLAGS) $(ROOT) -o ../bin/tree2mca
