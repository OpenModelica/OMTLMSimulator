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

# For microsoft and Dymola 
#CC=mycl cl -DWIN32 -D_WIN32 -DXMSTATIC -DYY_NEVER_INTERACTIVE  /EHsc
#cc=mycl cl -DWIN32 -D_WIN32 -DXMSTATIC -DYY_NEVER_INTERACTIVE  /EHsc

CC += -pipe -O2 -frtti -Wall -Wextra -fexceptions -DUNICODE -DINTERFACE_TYPES -std=c++11
cc += -pipe -O2 -frtti -Wall -Wextra -fexceptions -DUNICODE -DINTERFACE_TYPES -std=c++11

BNAME=FMIWrapper$(FEXT)
MISCHOME=../3rdParty/misc

INCLUDES=  -I. \
	-I"../FMIWrapper" \
	-I"../FMIWrapper/FMILibrary-2.0.1/install/include" \
	-I"../common" -I"../3rdParty/misc/include" \
	-I"cvode-2.9.0/include" \
	-I"ida-2.9.0/include"
    
ifeq ($(OS),Windows_NT)
	LIBS = 	-lws2_32 \
	        -LFMILibrary-2.0.1/install/lib \
		-llibfmilib_shared 
else
	LIBS = 	-LFMILibrary-2.0.1/install/lib \
		-lfmilib_shared 
endif

$(ABI)/%.o: %.cpp
	$(CC) $(INCLUDES) -c $< -o $@

$(ABI)/%.o: ../common/%.cc
	$(CC) $(INCLUDES) -c $< -o $@ 

$(ABI)/%.o: $(MISCHOME)/src/%.cc
	$(CC) $(INCLUDES) -c $< -o $@ 

$(ABI)/%.o: $(MISCHOME)/src/%.cc
	$(CC) $(INCLUDES) -c $< -o $@
    
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
	$(MAKE) fmil 
	$(MAKE) $(ABIOBJS)
	$(LINK) $(ABIOBJS$) -o $(ABI)/$(BNAME) $(LIBS)

copyfiles:
	cp $(ABI)/$(BNAME) $(BINDIR)
	cp StartTLMFmiWrapper* $(BINDIR)

install: default copyfiles

wrapper: dirs $(ABIOBJS)
	$(LINK) $(ABIOBJS$) -o $(ABI)/$(BNAME) $(LIBS)

wrapper_install: wrapper copyfiles


.PHONY: dirs clean

dirs:
	-mkdir -p $(ABI)
	-mkdir -p $(ABI)/Plugin
	-mkdir -p $(ABI)/Parameters
	-mkdir -p $(ABI)/Interfaces
	-mkdir -p $(ABI)/Logging
	-mkdir -p $(ABI)/Communication

fmil:
	mkdir -p FMILibrary-2.0.1/build
	(cd FMILibrary-2.0.1/build && $(CMAKE) -D CMAKE_AR:String="$(AR)" $(FMIL_FLAGS) -D FMILIB_BUILD_TESTS:Bool=OFF -D FMILIB_GENERATE_DOXYGEN_DOC:Bool=OFF -DFMILIB_INSTALL_PREFIX:String=../install .. -G $(CMAKE_TARGET))
	$(MAKE) -C FMILibrary-2.0.1/build
	$(MAKE) -C FMILibrary-2.0.1/build install
	cp -p FMILibrary-2.0.1/install/lib/libfmilib_shared$(SHREXT) $(BINDIR)

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
	rm -rf FMILibrary-2.0.1/build
	rm -rf FMILibrary-2.0.1/install

# DO NOT DELETE
