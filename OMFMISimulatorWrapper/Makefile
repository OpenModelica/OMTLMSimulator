#################################################################################
#   Makefile for the OMFMISimulatorWrapper
#   Date: 2017/09
#   Author: Robert Braun
#

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

BNAME=OMFMISimulatorWrapper$(FEXT)
MISCHOME=../3rdParty/misc

INCLUDES=  -I. \
	-I"${OMFMISimulatorPath}/install/include" \
	-I"${OMFMISimulatorPath}/src/OMSimulatorLib" \
	-I"../common" -I"../3rdParty/misc/include"
    
LIBS = 	-L${OMFMISimulatorPath}/install/lib \
	-lOMSimulatorLib \
	-lfmilib_shared

$(ABI)/%.o: %.cpp
	$(CXX) $(INCLUDES) -c $< -o $@

$(ABI)/%.o: ../common/%.cc
	$(CXX) $(INCLUDES) -c $< -o $@ 

$(ABI)/%.o: $(MISCHOME)/src/%.cc
	$(CXX) $(INCLUDES) -c $< -o $@ 

$(ABI)/%.o: $(MISCHOME)/src/%.cc
	$(CXX) $(INCLUDES) -c $< -o $@
    
OBJS=  main.o 

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
	tostr.o \
	ErrorLog.o \
	double33s.o \
	dsyevq3.o \
	dsyevv3.o \
	Bstring.o \
	dsytrd3.o \
	dsyevc3.o

ABIOBJS=$(OBJS:%.o= $(ABI)/%.o) $(EXT_OBJS:%.o= $(ABI)/%.o)


default: dirs $(ABIOBJS)
	$(LINK) $(ABIOBJS$) -o $(ABI)/$(BNAME) $(LIBS)

install: default
	cp $(ABI)/$(BNAME) $(BINDIR)
	cp StartTLMOMFMISimulatorWrapper* $(BINDIR)

.PHONY: dirs clean

dirs:
	-mkdir -p $(ABI)
	-mkdir -p $(ABI)/Plugin
	-mkdir -p $(ABI)/Parameters
	-mkdir -p $(ABI)/Interfaces
	-mkdir -p $(ABI)/Logging
	-mkdir -p $(ABI)/Communication

# Problem that MingW contains no shell to perform the "if" command.	
dirs_unix:	
	-if [ ! -d $(ABI) ] ; then \
	mkdir $(ABI) ; fi ;

clean:
	rm -rf $(ABI)
	rm -rf $(ABI)/Plugin
	rm -rf $(ABI)/Parameters
	rm -rf $(ABI)/Logging
	rm -rf $(ABI)/Interfaces
	rm -rf $(ABI)/Communication
	rm -rf $(BINDIR)/OMFMISimulatorWrapper$(FEXT)
	rm -rf $(BINDIR)/StartTLMOMFMISimulatorWrapper*

# DO NOT DELETE
