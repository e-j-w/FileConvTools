CFLAGS   = -O -Wall -fPIC -ansi $(shell $(ROOTSYS)/bin/root-config --cflags)
ROOT =  $(shell $(ROOTSYS)/bin/root-config --glibs)

all: tree2mca treelist2mca

tree2mca: tree2mca.c
	g++ tree2mca.c $(CFLAGS) $(ROOT) -o tree2mca

treelist2mca: treelist2mca.c
	g++ treelist2mca.c $(CFLAGS) $(ROOT) -o treelist2mca
clean:
	rm -rf *~ tree2mca
