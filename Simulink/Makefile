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

MISCHOME=../extralibs/misc
MEX_DEBUGFLG=
CC += -fPIC

$(ABI)/%.o: %.c
	$(cc) $(DEFINES) $(CFLAGS) $(OPTFLAGS4) $(INCLUDES) -c $< -o $@

$(ABI)/%.o: %.cc
	$(CC) $(DEFINES) $(CCFLAGS) $(OPTFLAGS4) $(INCLUDES) -c $< -o $@ 

$(ABI)/%.o: ../common/%.cc
	$(CC) $(DEFINES) $(CCFLAGS) $(OPTFLAGS4) $(INCLUDES) -c $< -o $@ 

OBJS=   

EXT_OBJS = PluginImplementer.o \
	TLMClientComm.o \
	TLMCommUtil.o \
	TLMInterface.o \
	TLMErrorLog.o \
	TLMPlugin.o

EXT_LIBS = $(MISCHOME)/lib/$(ABI)/libtlmmisc.a

ABIOBJS=$(OBJS:%.o= $(ABI)/%.o) $(EXT_OBJS:%.o= $(ABI)/%.o) $(EXT_LIBS)


default:  dirs $(ABIOBJS)
	$(MEX) $(MEX_DEBUGFLG) $(INCLUDES) tlmforce.cc $(ABIOBJS)

install: default
	cp tlmforce.mex* $(INSTDIR)
	cp TLMLib.mdl $(INSTDIR)
	chmod ug+w $(INSTDIR)/TLMLib.mdl

.PHONY: dirs clean

dirs:
	-if [ ! -d $(ABI) ] ; then \
	mkdir $(ABI) ; fi ;
	-if [ ! -d $(INSTDIR) ] ; then \
	mkdir -p $(INSTDIR) ; fi ;

clean:
	rm -rf $(ABI)

# DO NOT DELETE
