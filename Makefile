CFLAGS = -O -Wall -fPIC -ansi
ROOT = $(shell $(ROOTSYS)/bin/root-config --glibs) $(shell $(ROOTSYS)/bin/root-config --cflags)

all: tree2mca tree2mca_gated add_mca mca2txt txt2mca txt2binnedavgtxt
no_root: add_mca mca2txt txt2mca txt2binnedavgtxt

tree2mca: tree2mca.c tree2mca.h read_config.c
	g++ tree2mca.c $(CFLAGS) $(ROOT) -o tree2mca
	
tree2mca_gated: tree2mca_gated.c tree2mca.h read_config.c
	g++ tree2mca_gated.c $(CFLAGS) $(ROOT) -o tree2mca_gated

add_mca: add_mca.c mca.h
	g++ add_mca.c $(CFLAGS) -o add_mca
	
mca2txt: mca2txt.c mca.h
	g++ mca2txt.c $(CFLAGS) -o mca2txt
	
txt2mca: txt2mca.c mca.h
	g++ txt2mca.c $(CFLAGS) -o txt2mca

txt2binnedavgtxt: txt2binnedavgtxt.c binnedavgtxt.h
	g++ txt2binnedavgtxt.c $(CFLAGS) -o txt2binnedavgtxt

clean:
	rm -rf *~ tree2mca tree2mca_gated add_mca mca2txt txt2mca txt2binnedavgtxt
