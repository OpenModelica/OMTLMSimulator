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

# For microsoft and Dymola 
#CC=mycl cl -DWIN32 -D_WIN32 -DXMSTATIC -DYY_NEVER_INTERACTIVE  /EHsc
#cc=mycl cl -DWIN32 -D_WIN32 -DXMSTATIC -DYY_NEVER_INTERACTIVE  /EHsc

CC += -std=c++11 -DYY_NEVER_INTERACTIVE -fPIC -DINTERFACE_TYPES
cc += -DYY_NEVER_INTERACTIVE -fPIC -DINTERFACE_TYPES

#################################################################################
#
# includes, try to do it good this time. previously everything
# included everything. even though the slave doesn't need the solver
# it needs the definition of the matrix, hence the slaves includes the
# solver. 


BNAME=libtlmopenmodelica.a
INSTDIR=$(BINDIR)/OpenModelica
RESOURCESDIR=$(INSTDIR)/Resources
MISCHOME=../3rdParty/misc

INCLUDES=  -I. \
	-I../common \
	-I$(MISCHOME)/include

$(ABI)/%.o: %.c
	$(cc) $(INCLUDES) -c $< -o $@

$(ABI)/%.o: %.cc
	$(CC) $(INCLUDES) -c $< -o $@ 

$(ABI)/%.o: ../common/%.cc
	$(CC) $(INCLUDES) -c $< -o $@ 

$(ABI)/%.o: $(MISCHOME)/src/%.cc
	$(CC) $(INCLUDES) -c $< -o $@

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
	Parameters/ComponentParameter.o \
	Logging/TLMErrorLog.o \
	Plugin/TLMPlugin.o \
	coordTransform.o \
	double3.o \
	double33.o \
	ErrorLog.o \
	double33s.o \
	dsyevq3.o \
	dsyevv3.o \
	dsyevc3.o \
	dsytrd3.o \
	Bstring.o \
	tostr.o \

ABIOBJS=$(OBJS:%.o= $(ABI)/%.o) $(EXT_OBJS:%.o= $(ABI)/%.o)


default: dirs $(ABIOBJS)
	$(AR) ruv $(ABI)/$(BNAME) $(ABIOBJS) 

install: default
	cp $(ABI)/$(BNAME) $(RESOURCESDIR)/Library
	cp tlmforce.h $(RESOURCESDIR)/Include
	cp OM_TLM.mo $(INSTDIR)
	cp StartTLMOpenModelica* $(BINDIR)
	chmod +x $(BINDIR)/StartTLMOpenModelica*

.PHONY: dirs clean

dirs:
	-mkdir -p $(ABI)
	-mkdir -p $(ABI)/Plugin
	-mkdir -p $(ABI)/Parameters
	-mkdir -p $(ABI)/Communication
	-mkdir -p $(ABI)/Interfaces
	-mkdir -p $(ABI)/Logging
	-mkdir -p $(INSTDIR)
	-mkdir -p $(RESOURCESDIR)/Include
	-mkdir -p $(RESOURCESDIR)/Library

clean:
	rm -rf $(ABI)
	rm -rf $(INSTDIR) $(BINDIR)/StartTLMOpenModelica*

# DO NOT DELETE
