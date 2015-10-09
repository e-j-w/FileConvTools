CFLAGS   = -O -Wall -fPIC -ansi $(shell $(ROOTSYS)/bin/root-config --cflags)
ROOT =  $(shell $(ROOTSYS)/bin/root-config --glibs)

all: tree2mca treelist2mca tree2mca_gated

tree2mca: tree2mca.c
	g++ tree2mca.c $(CFLAGS) $(ROOT) -o tree2mca

treelist2mca: treelist2mca.c
	g++ treelist2mca.c $(CFLAGS) $(ROOT) -o treelist2mca

tree2mca_gated: tree2mca_gated.c
	g++ tree2mca_gated.c $(CFLAGS) $(ROOT) -o tree2mca_gated

clean:
	rm -rf *~ tree2mca treelist2mca tree2mca_gated
