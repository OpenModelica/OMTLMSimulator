#################################################################################
#
#   @(#)Makefile	05/01
#
# Iakov Nakhimovski. Makefile for the tlmmodelica dynamic library

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

override CC += -DYY_NEVER_INTERACTIVE 
override CXX += -DYY_NEVER_INTERACTIVE

#################################################################################
#
# includes, try to do it good this time. previously everything
# included everything. even though the slave doesn't need the solver
# it needs the definition of the matrix, hence the slaves includes the
# solver. 


BNAME=libtlmmodelica.a
INSTDIR=$(BINDIR)/Modelica
RESOURCESDIR=$(INSTDIR)/Resources
MISCHOME=../3rdParty/misc

INCLUDES=  -I. \
	-I../common \
	-I$(MISCHOME)/include

$(ABI)/%.o: %.c
	$(CC) $(INCLUDES) -c $< -o $@

$(ABI)/%.o: %.cc
	$(CXX) $(INCLUDES) -c $< -o $@ 

$(ABI)/%.o: ../common/%.cc
	$(CXX) $(INCLUDES) -c $< -o $@ 

$(ABI)/%.o: $(MISCHOME)/src/%.cc
	$(CXX) $(INCLUDES) -c $< -o $@ 

$(ABI)/%.o: $(MISCHOME)/src/%.cc
	$(CXX) $(INCLUDES) -c $< -o $@

OBJS=  tlmforce.o 

EXT_OBJS = Plugin/PluginImplementer.o \
	Communication/TLMClientComm.o \
	Communication/TLMCommUtil.o \
	Interfaces/TLMInterface.o \
	Interfaces/TLMInterfaceSignal.o \
	Interfaces/TLMInterfaceSignalInput.o \
	Interfaces/TLMInterfaceSignalOutput.o \
	Interfaces/TLMInterface1D.o \
	Interfaces/TLMInterface3D.o \
	Logging/TLMErrorLog.o \
	Plugin/TLMPlugin.o \
	coordTransform.o \
	double3.o \
	double33.o \
	tostr.o

ABIOBJS=$(OBJS:%.o= $(ABI)/%.o) $(EXT_OBJS:%.o= $(ABI)/%.o)


default: dirs $(ABIOBJS)
	$(AR) ruv $(ABI)/$(BNAME) $(ABIOBJS)

install: default
	cp $(ABI)/$(BNAME) $(RESOURCESDIR)/Library
	cp tlmforce.h $(RESOURCESDIR)/Include
	cp TLM.mo $(INSTDIR)


.PHONY: dirs clean

dirs:
	-mkdir -p $(ABI)
	-mkdir -p $(ABI)/Communication
	-mkdir -p $(ABI)/Interfaces
	-mkdir -p $(ABI)/Logging
	-mkdir -p $(ABI)/Plugin
	-mkdir -p $(INSTDIR)
	-mkdir -p $(RESOURCESDIR)/Include
	-mkdir -p $(RESOURCESDIR)/Library

# Problem that MingW contains no shell to perform the "if" command.	
dirs_unix:	
	-if [ ! -d $(ABI) ] ; then \
	mkdir $(ABI) ; fi ;

clean:
	rm -rf $(ABI)
	rm -rf $(INSTDIR)

# DO NOT DELETE
