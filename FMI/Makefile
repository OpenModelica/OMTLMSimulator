#################################################################################
#
# Robert Braun. Makefile for the generic FMU in TLMPlugin library

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

CC=gcc
CFLAGS=-I. -Iinclude

fmu2_tlmplugin_cs: fmu2_tlmplugin_cs.o
	$(CC) -fPIC -c fmu2_tlmplugin_cs.c -I. -Iinclude
	$(CC) -fPIC -w -shared -static-libgcc fmu2_tlmplugin_cs.o -o TLMPlugin.so
	mkdir -p $(BINDIR)/FMU
	mkdir -p $(BINDIR)/FMU/binaries
	mkdir -p $(BINDIR)/FMU/binaries/linux64
	cp -f TLMPlugin.so $(BINDIR)/FMU/binaries/linux64
	cp -f modelDescription.xml $(BINDIR)/FMU

clean :
	rm fmu2_tlmplugin_cs.o TLMPlugin.so $(BINDIR)/FMU/modelDescription.xml $(BINDIR)/FMU/binaries/linux64/TLMPlugin.so

# DO NOT DELETE
