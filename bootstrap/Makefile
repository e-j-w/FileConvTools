CFLAGS = -O -g -Wall -fPIC -ansi
ROOT = $(shell $(ROOTSYS)/bin/root-config --glibs) $(shell $(ROOTSYS)/bin/root-config --cflags)

all: bootstrap.c bootstrap.h read_config.c
	g++ bootstrap.c $(CFLAGS) $(ROOT) -o ../bin/bootstrap
