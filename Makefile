CFLAGS = -O -Wall -fPIC -ansi
ROOT = $(shell $(ROOTSYS)/bin/root-config --glibs) $(shell $(ROOTSYS)/bin/root-config --cflags)

all: tree2mca tree2mca_gated tree2binnedavgtxt analyze_mca add_mca contract_mca sum_mca mca2txt txt2mca txt2binnedavgtxt chn2mca spx2mca tree2tree
no_root: analyze_mca add_mca contract_mca sum_mca mca2txt txt2mca txt2binnedavgtxt chn2mca spx2mca
	
spx2mca: spx2mca.c common.h
	g++ spx2mca.c $(CFLAGS) -o spx2mca
	
chn2mca: chn2mca.c common.h
	g++ chn2mca.c $(CFLAGS) -o chn2mca

tree2mca: tree2mca.c tree2mca.h read_config.c
	g++ tree2mca.c $(CFLAGS) $(ROOT) -o tree2mca
	
tree2mca_gated: tree2mca_gated.c tree2mca.h read_config.c
	g++ tree2mca_gated.c $(CFLAGS) $(ROOT) -o tree2mca_gated

analyze_mca: analyze_mca.c mca.h
	g++ analyze_mca.c $(CFLAGS) -o analyze_mca

add_mca: add_mca.c mca.h
	g++ add_mca.c $(CFLAGS) -o add_mca
	
contract_mca: contract_mca.c mca.h
	g++ contract_mca.c $(CFLAGS) -o contract_mca

sum_mca: sum_mca.c mca.h
	g++ sum_mca.c $(CFLAGS) -o sum_mca
	
mca2txt: mca2txt.c mca.h
	g++ mca2txt.c $(CFLAGS) -o mca2txt
	
txt2mca: txt2mca.c mca.h read_config.c
	g++ txt2mca.c $(CFLAGS) -o txt2mca

txt2binnedavgtxt: txt2binnedavgtxt.c txt2binnedavgtxt.h read_config.c
	g++ txt2binnedavgtxt.c $(CFLAGS) -o txt2binnedavgtxt

tree2binnedavgtxt: tree2binnedavgtxt.c tree2binnedavgtxt.h read_config.c
	g++ tree2binnedavgtxt.c $(CFLAGS) $(ROOT) -o tree2binnedavgtxt

tree2tree: tree2tree.c tree2tree.h read_config.c
	g++ tree2tree.c $(CFLAGS) $(ROOT) -o tree2tree

clean:
	rm -rf *~ tree2mca tree2mca_gated analyze_mca add_mca contract_mca sum_mca mca2txt txt2mca txt2binnedavgtxt tree2binnedavgtxt chn2mca spx2mca tree2tree
