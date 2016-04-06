################################################
#                                              #
# Robert Braun. Makefile for a generic FMU     #
# for co-simualtion in TLMPlugin library.      #
#                                              #
# FMI requires a plain C interface. Hence,     #
# the compilation is divided into a C++ part   #
# and a C part. Objective files are linked     #
# together at the end.                         #
#                                              #
# C++ files:                                   #
#   tlmforce.cc - TLMPlugin interface          #
# C files:                                     #
#   fmu_cs.c - Main FMU file                   #
#   fmu_wrapper.c - C wrapper for tlmforce.cc  #
#                                              #
# (fmu_wrapper.c is included from cmu_cs.c)    #
#                                              #
# After compilation, files must be compressed  #
# to a zip-file with file extension "fmu".		 #
#                                              #
################################################

# The following is need for BEAST compatibility
ifeq ($(MAKEFILEHEADHOME),)
  UP=..
  MAKEFILEHEADHOME=$(UP)
  BINDIR=$(UP)/bin/
else
  # This is for BEAST
  UP=$(MAKEFILEHEADHOME)/src
  BINDIR=$(UP)/../bin/
endif

include $(MAKEFILEHEADHOME)/Makefile.head

RESOURCESDIR=$(INSTDIR)/Resources
MISCHOME=../extralibs/misc

INCLUDES=  -I. \
	-I../common \
	-I$(MISCHOME)/include \
  -I../extralibs/libxml2/include



CFLAGS=-fPIC -Iinclude $(INCLUDES)
LFLAGS=-w -shared -static-libgcc -static-libstdc++ -Wl,--rpath,'$ORIGIN/.'






#todo: Archdir should not be hard coded to linux64
BNAME=TLMPlugin.a
INSTDIR=$(BINDIR)/OpenModelica
BINARIESDIR=$(INSTDIR)/binaries
ARCHDIR=$BINARIESDIR/linux64
RESOURCESDIR=$(INSTDIR)/Resources


CPP_FILES = tlmforce.cc \
	../common/PluginImplementer.cc \
	../common/TLMClientComm.cc\
	../common/TLMCommUtil.cc\
	../common/TLMInterface.cc\
	../common/TLMErrorLog.cc\
	../common/TLMPlugin.cc\
	../extralibs/misc/src/coordTrans.cc\
	../extralibs/misc/src/double3Vec.cc\
	../extralibs/misc/src/double33Mat.cc\
	../extralibs/misc/src/strConv.cc

C_FILES = fmu_cs.c

OBJ_FILES = tlmforce.o \
	fmu_cs.o \
	PluginImplementer.o \
	TLMClientComm.o \
	TLMCommUtil.o \
	TLMInterface.o \
	TLMErrorLog.o \
	TLMPlugin.o \
	coordTrans.o \
	double3Vec.o \
	double33Mat.o \
	strConv.o

cc=clang
CC=clang++

fmu2_tlmplugin_cs: 
	$(CC) -c $(CPP_FILES) $(CFLAGS)
	$(cc) -c $(C_FILES) $(CFLAGS)
	$(CC) $(LFLAGS) $(OBJ_FILES) -o TLMPlugin.so $(CFLAGS)
	mkdir -p $(BINDIR)/FMU
	mkdir -p $(BINDIR)/FMU/binaries
	mkdir -p $(BINDIR)/FMU/binaries/linux64
	cp -f TLMPlugin.so $(BINDIR)/FMU/binaries/linux64
	cp -f modelDescription.xml $(BINDIR)/FMU

clean :
	rm -f *.o *.so $(BINDIR)/FMU/modelDescription.xml $(BINDIR)/FMU/binaries/linux64/*.so

# DO NOT DELETE
