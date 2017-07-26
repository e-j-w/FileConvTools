CFLAGS = -O -Wall -fPIC -ansi
ROOT = $(shell $(ROOTSYS)/bin/root-config --glibs) $(shell $(ROOTSYS)/bin/root-config --cflags)

all: tree2doppler_map.c tree2doppler_map.h read_config.c
	g++ tree2doppler_map.c $(CFLAGS) $(ROOT) -o ../bin/tree2doppler_map
