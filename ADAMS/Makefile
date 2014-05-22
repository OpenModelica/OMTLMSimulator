#################################################################################
#
#   @(#)Makefile	05/01
#
# Iakov Nakhimovski. Makefile for the tlmadams dynamic library

include $(MAKEFILEHEADHOME)/Makefile.head
UP=../..

#include $(UP)/Versions/Format.mak

BINDIR=$(UP)/../bin/$(ABI)

#################################################################################
#
# includes, try to do it good this time. previously everything
# included everything. even though the slave doesn't need the solver
# it needs the definition of the matrix, hence the slaves includes the
# solver. 


BNAME=tlmadams
INSTDIR=../../../bin/$(ABI)/MSC.ADAMS

INCLUDES=  -I. \
	-I../common 

$(ABI)/%.o: %.c
	echo Compiling with ADAMSDEFS = $(ADAMSDEFS)
	$(CC) $(INCLUDES) $(ADAMSDEFS) -c $< -o $@

$(ABI)/%.o: %.cc
	echo Compiling with ADAMSDEFS = $(ADAMSDEFS)
	$(CC) $(INCLUDES) $(ADAMSDEFS) -c $< -o $@ 

OBJS=  tlmforce.o gfo_wrapper.o

OBJS_TO_LINK=$(OBJS:%.o= $(ABI)/%.o)


tlmadams default:  $(OBJS_TO_LINK)
	$(MAKE) dirs
	$(LINK) -o $(ABI)/$(BNAME)$(DLLEXT) $(OBJS_TO_LINK) $(ADAMSLIBS) $(XTRLIBS) -L../common/$(ABI)/ -lTLM_m


install: tlmadams 
	@ if [ -f $(ABI)/$(BNAME)$(DLLEXT).manifest ] ; then \
		(cd $(ABI) ; mt.exe -manifest $(BNAME)$(DLLEXT).manifest -outputresource:$(BNAME)$(DLLEXT)\;2); fi
	cp $(ABI)/$(BNAME)$(DLLEXT) $(INSTDIR)

.PHONY: dirs clean

dirs:
	-if [ ! -d $(ABI) ] ; then \
	mkdir $(ABI) ; fi ;
	-if [ ! -d $(INSTDIR) ] ; then \
	mkdir -p $(INSTDIR) ; fi ;

clean:
	rm -fr $(ABI)/*

# DO NOT DELETE
