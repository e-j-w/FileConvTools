CFLAGS   = -O -Wall -fPIC -ansi $(shell $(ROOTSYS)/bin/root-config --cflags)
ROOT =  $(shell $(ROOTSYS)/bin/root-config --glibs)

all: tree2mca treelist2mca tree2mca_gated add_mca
no_root: add_mca

tree2mca: tree2mca.c
	g++ tree2mca.c $(CFLAGS) $(ROOT) -o tree2mca

treelist2mca: treelist2mca.c
	g++ treelist2mca.c $(CFLAGS) $(ROOT) -o treelist2mca
	
tree2mca_gated: tree2mca_gated.c tree2mca.h read_config.c
	g++ tree2mca_gated.c $(CFLAGS) $(ROOT) -o tree2mca_gated

add_mca: add_mca.c
	g++ add_mca.c $(CFLAGS) -o add_mca

clean:
	rm -rf *~ tree2mca treelist2mca tree2mca_gated add_mca
