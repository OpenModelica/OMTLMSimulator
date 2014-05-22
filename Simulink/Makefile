#################################################################################
#
#   @(#)Makefile	05/01
#
# Alexander Siemers. Makefile for the Simulink TLM plugin

include $(MAKEFILEHEADHOME)/Makefile.head
UP=../..

#include $(UP)/Versions/Format.mak

BINDIR=$(UP)/../bin/$(ABI)
INSTDIR=../../../bin/$(ABI)/Simulink

INCLUDES=  -I. \
	-I../common \
	-I$(MISCHOME)/include

#MHOME=`$(MISCHOME)`
#MHOME=$(BEAST)/lib/misc
MHOME=../../../lib/misc
#MEX_DEBUGFLG=-g
MEX_DEBUGFLG=
#CC=g++ -fPIC $(DEBUGFLGS)
CC += -fPIC

$(ABI)/%.o: %.c
	$(cc) $(DEFINES) $(CFLAGS) $(OPTFLAGS4) $(INCLUDES) -c $< -o $@

$(ABI)/%.o: %.cc
	$(CC) $(DEFINES) $(CCFLAGS) $(OPTFLAGS4) $(INCLUDES) -c $< -o $@ 

$(ABI)/%.o: ../common/%.cc
	$(CC) $(DEFINES) $(CCFLAGS) $(OPTFLAGS4) $(INCLUDES) -c $< -o $@ 

$(ABI)/%.o: $(MHOME)/src/%.cc
	$(CC) $(DEFINES) $(CCFLAGS) $(OPTFLAGS4) $(INCLUDES) -c $< -o $@ 

OBJS=   

EXT_OBJS = PluginImplementer.o \
	TLMClientComm.o \
	TLMCommUtil.o \
	TLMInterface.o \
	TLMErrorLog.o \
	TLMPlugin.o \
	lightmat.o \
	ErrorLog.o \
	tostr.o \
	Bstring.o

ABIOBJS=$(OBJS:%.o= $(ABI)/%.o) $(EXT_OBJS:%.o= $(ABI)/%.o)


default:  dirs $(ABIOBJS)
	$(MEX) $(MEX_DEBUGFLG) $(INCLUDES) tlmforce.cc $(ABIOBJS) 
#$(XTRLIBS)

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
	rm -fr $(ABI)/*

# DO NOT DELETE
