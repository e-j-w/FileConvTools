# File Conversion Tools for NUSC

Maintainers: Jonathan Williams, Aaron Chester


## Description

Codes for converting between and processing different file types commonly used for data analysis at the SFU NSL and TRIUMF.

Running any code with no parameters will show a list of required parameters and/or parameter files.

### Parameter files

Some codes require parameter files to run.  Sample parameter files are included containing examples and listings of valid parameters, and are named based on the code that they are useful for.  For instance, the 'tree2doppler_map_sample_parameters' file is a valid parameter file for the `tree2doppler_map` code.


## Instructions

Use 'make' to compile.  Many of the codes require CERN's ROOT package to be installed (available at https://root.cern.ch/).  Codes that do not depend on ROOT may be compiled using 'make no_root'.  gf3 is recommended for viewing .mca files, or the code mca2txt can be used to convert them to plaintext (for viewing in Excel, etc.).

To make codes which depend on ROOT, environment variables related to ROOT must be set up properly.  This can be done by adding to your .bashrc (and then reloading the terminal):

```
#ROOT configuration
export ROOTSYS=/path/to/root
export ROOTINC=$ROOTSYS/include
export ROOTLIB=$ROOTSYS/lib
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$ROOTLIB
```

To run the programs from anywhere, move the resulting executables (in the 'bin' directory) to any directory under your $PATH environment variable.

Tested using Ubuntu 14.04 and Scientific Linux/CentOS 7 with ROOT v6.x.  The codes not based on ROOT are self-contained and should work on more or less any Linux distro.
