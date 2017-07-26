CFLAGS = -O -Wall -fPIC -ansi
ROOT = $(shell $(ROOTSYS)/bin/root-config --glibs) $(shell $(ROOTSYS)/bin/root-config --cflags)

all: tree2mca_group.c tree2mca_group.h read_config.c
	g++ tree2mca_group.c $(CFLAGS) $(ROOT) -o ../bin/tree2mca_group
