#################################################################################
#
#   @(#)Makefile	05/01
#
# Alexander Siemers. Makefile for the Simulink TLM plugin

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

INSTDIR=$(BINDIR)/Simulink

INCLUDES=  -I. \
	-I../common \
	-I$(MISCHOME)/include

MISCHOME=../3rdParty/misc
MEX_DEBUGFLG=

$(ABI)/%.o: %.c
	$(CC) $(DEFINES) $(CFLAGS) -DWIN32 $(OPTFLAGS4) $(INCLUDES) -c $< -o $@

$(ABI)/%.o: %.cc
	$(CXX) $(DEFINES) $(CXXFLAGS) -DWIN32 $(OPTFLAGS4) $(INCLUDES) -c $< -o $@ 

$(ABI)/%.o: ../common/%.cc
	$(CXX) $(DEFINES) $(CXXFLAGS) -DWIN32 $(OPTFLAGS4) $(INCLUDES) -c $< -o $@ 

$(ABI)/%.o: $(MISCHOME)/src/%.cc
	$(CXX) $(DEFINES) $(CXXFLAGS) -DWIN32 $(OPTFLAGS4) $(INCLUDES) -c $< -o $@

OBJS=   

EXT_OBJS = Plugin/PluginImplementer.o \
	Communication/TLMClientComm.o \
	Communication/TLMCommUtil.o \
	Interfaces/TLMInterface.o \
	Interfaces/TLMInterface1D.o \
	Interfaces/TLMInterface3D.o \
	Interfaces/TLMInterfaceSignal.o \
	Interfaces/TLMInterfaceSignalInput.o \
	Interfaces/TLMInterfaceSignalOutput.o \
	Parameters/ComponentParameter.o \
	Logging/TLMErrorLog.o \
	Plugin/TLMPlugin.o \
	coordTransform.o \
	double3.o \
	double33.o \
        tostr.o

ABIOBJS=$(OBJS:%.o= $(ABI)/%.o) $(EXT_OBJS:%.o= $(ABI)/%.o)


default:  dirs $(ABIOBJS)
	$(MEX) $(MEX_DEBUGFLG) $(INCLUDES) tlmforce.cc $(ABIOBJS)

install: default
	cp tlmforce.mex* $(INSTDIR)
	cp tlmsignal.mex* $(INSTDIR)
	cp TLMLib.mdl $(INSTDIR)
	chmod ug+w $(INSTDIR)/TLMLib.mdl

.PHONY: dirs clean

dirs:
	mkdir -p $(ABI)
	mkdir -p $(ABI)/Plugin
	mkdir -p $(ABI)/Communication
	mkdir -p $(ABI)/Interfaces
	mkdir -p $(ABI)/Parameters
	mkdir -p $(ABI)/Logging
	-if [ ! -d $(INSTDIR) ] ; then \
	mkdir -p $(INSTDIR) ; fi ;

clean:
	rm -rf $(ABI)

# DO NOT DELETE
