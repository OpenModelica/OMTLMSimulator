#################################################################################
#   Makefile for the FMIWrapper
#   Date: 2016/09
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

override CC += -pipe -O2 -frtti -Wall -Wextra -fexceptions -DUNICODE -DINTERFACE_TYPES
override CXX += -pipe -O2 -frtti -Wall -Wextra -fexceptions -DUNICODE -DINTERFACE_TYPES

# deal with regex crap
STD_REGEX=$(shell ../../3rdParty/RegEx/OMSRegEx)
ifeq ("$(STD_REGEX)","1")
	EXTRA_LIBS=
else
	EXTRA_LIBS=-lboost_regex
endif


BNAME=FMIWrapper$(FEXT)
MISCHOME=../3rdParty/misc

INCLUDES=  -I. \
	-I"../FMIWrapper" \
	-I"../../3rdParty/FMIL/$(INSTALL_DIR)/include" \
	-I"../../3rdParty/RegEx" \
	-I"../common" -I"../3rdParty/misc/include" \
	-I"cvode-2.9.0/include" \
	-I"ida-2.9.0/include"

ifeq (MINGW,$(findstring MINGW, $(detected_OS)))
  ISWINDOWS = 1
endif
ifeq (Windows_NT,$(OS))
  ISWINDOWS = 1
endif

ifeq (1,$(ISWINDOWS))
	LIBS = 	-lws2_32 \
	        -L../../3rdParty/FMIL/$(INSTALL_DIR)/lib \
		-lfmilib -lshlwapi
else
	LIBS = 	-L../../3rdParty/FMIL/$(INSTALL_DIR)/lib \
		-lfmilib -ldl $(EXTRA_LIBS)
endif

$(ABI)/%.o: %.cpp
	$(CXX) $(INCLUDES) -c $< -o $@

$(ABI)/%.o: ../common/%.cc
	$(CXX) $(INCLUDES) -c $< -o $@

$(ABI)/%.o: $(MISCHOME)/src/%.cc
	$(CXX) $(INCLUDES) -c $< -o $@

$(ABI)/%.o: $(MISCHOME)/src/%.cc
	$(CXX) $(INCLUDES) -c $< -o $@

$(ABI)/%.o: cvode-2.9.0/src/nvec_ser/%.c
	$(CC) $(INCLUDES) -c $< -o $@

$(ABI)/%.o: cvode-2.9.0/src/sundials/%.c
	$(CC) $(INCLUDES) -c $< -o $@

$(ABI)/%.o: cvode-2.9.0/src/cvode/%.c
	$(CC) $(INCLUDES) -c $< -o $@

$(ABI)/%.o: ida-2.9.0/src/ida/%.c
	$(CC) $(INCLUDES) -c $< -o $@

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
	ErrorLog.o \
	double33s.o \
	dsyevq3.o \
	dsyevv3.o \
	dsyevc3.o \
	dsytrd3.o \
	Bstring.o \
	tostr.o \
	nvector_serial.o \
	sundials_math.o \
	sundials_nvector.o \
	sundials_direct.o \
	sundials_dense.o \
	cvode.o \
	cvode_dense.o \
	cvode_direct.o \
	cvode_io.o \
	ida.o \
	ida_dense.o \
	ida_direct.o \
	ida_io.o


ABIOBJS=$(OBJS:%.o= $(ABI)/%.o) $(EXT_OBJS:%.o= $(ABI)/%.o)

default: dirs
	echo Using std::regex: $(STD_REGEX)
	$(MAKE) $(ABIOBJS)
	$(LINK) $(ABIOBJS) -o $(ABI)/$(BNAME) $(LIBS)


install: default
	cp $(ABI)/$(BNAME) $(BINDIR)
	cp StartTLMFmiWrapper* $(BINDIR)


wrapper: dirs $(ABIOBJS)
	$(LINK) $(ABIOBJS) -o $(ABI)/$(BNAME) $(LIBS)

wrapper_install: wrapper
	cp $(ABI)/$(BNAME) $(BINDIR)
	cp StartTLMFmiWrapper* $(BINDIR)


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
	rm -rf $(BINDIR)/FMIWrapper$(FEXT)
	rm -rf $(BINDIR)/StartTLMFmiWrapper*

# DO NOT DELETE
