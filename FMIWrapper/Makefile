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

CC += -pipe -fno-keep-inline-dllexport -O2 -frtti -Wall -Wextra -fexceptions -mthreads -DUNICODE
cc += -pipe -fno-keep-inline-dllexport -O2 -frtti -Wall -Wextra -fexceptions -mthreads -DUNICODE


BNAME=FMIWrapper.exe
INSTDIR=$(BINDIR)
#RESOURCESDIR=$(INSTDIR)/Resources
MISCHOME=../extralibs/misc

INCLUDES=  -I. \
	-I"..\FMIWrapper" \
    -I"..\FMIWrapper\FMILibrary-2.0.1\install\include" \
    -I"..\common" -I"..\extralibs\misc\include" \
    -I"cvode-2.9.0/include"
    

LIBS = 	-lws2_32 \
        -LFMILibrary-2.0.1/install/lib \
        -llibfmilib_shared 
    
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
    
OBJS=  main.o 

EXT_OBJS = PluginImplementer.o \
	TLMClientComm.o \
	TLMCommUtil.o \
	TLMInterface.o \
	TLMErrorLog.o \
	TLMPlugin.o \
	coordTrans.o \
	double3Vec.o \
	double33Mat.o \
	strConv.o \
    nvector_serial.o \
    sundials_math.o \
    sundials_nvector.o \
    sundials_direct.o \
    sundials_dense.o \
    cvode.o \
    cvode_dense.o \
    cvode_direct.o \
    cvode_io.o

ABIOBJS=$(OBJS:%.o= $(ABI)/%.o) $(EXT_OBJS:%.o= $(ABI)/%.o)


default: dirs $(ABIOBJS)
	$(LINK) $(ABIOBJS$) -o $(ABI)/$(BNAME) $(LIBS)

install: default
	cp $(ABI)/$(BNAME) $(INSTDIR)
	cp FMILibrary-2.0.1/install/lib/libfmilib_shared.dll $(INSTDIR)


.PHONY: dirs clean

dirs:
	-mkdir -p $(ABI)
	-mkdir -p $(INSTDIR)

# Problem that MingW contains no shell to perform the "if" command.	
dirs_unix:	
	-if [ ! -d $(ABI) ] ; then \
	mkdir $(ABI) ; fi ;

clean:
	rm -rf $(ABI)
	rm -rf $(INSTDIR)

# DO NOT DELETE
