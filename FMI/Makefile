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

RESOURCESDIR=$(INSTDIR)/Resources
MISCHOME=../extralibs/misc

INCLUDES=  -I. \
	-I../common \
	-I$(MISCHOME)/include \
  -I../extralibs/libxml2/include



CFLAGS=-fPIC -Iinclude $(INCLUDES)
LFLAGS=-w -shared -static-libgcc -static-libstdc++ -Wl,--rpath,'$ORIGIN/.'






#todo: Archdir should not be hard coded to linux64
BNAME=TLMPlugin.a
INSTDIR=$(BINDIR)/OpenModelica
BINARIESDIR=$(INSTDIR)/binaries
ARCHDIR=$BINARIESDIR/linux64
RESOURCESDIR=$(INSTDIR)/Resources


CPP_FILES = fmi_wrapper.cc \
	../common/PluginImplementer.cc \
	../common/TLMClientComm.cc\
	../common/TLMCommUtil.cc\
	../common/TLMInterface.cc\
	../common/TLMErrorLog.cc\
	../common/TLMPlugin.cc\
	../extralibs/misc/src/coordTrans.cc\
	../extralibs/misc/src/double3Vec.cc\
	../extralibs/misc/src/double33Mat.cc\
	../extralibs/misc/src/strConv.cc

C_FILES = fmu2_tlmplugin_cs.c

OBJ_FILES = fmi_wrapper.o\
	fmu2_tlmplugin_cs.o \
	PluginImplementer.o \
	TLMClientComm.o \
	TLMCommUtil.o \
	TLMInterface.o \
	TLMErrorLog.o \
	TLMPlugin.o \
	coordTrans.o \
	double3Vec.o \
	double33Mat.o \
	strConv.o



fmu2_tlmplugin_cs: 
	$(CC) -c $(CPP_FILES) $(CFLAGS)
	$(cc) -c $(C_FILES) $(CFLAGS)
	$(CC) $(LFLAGS) $(OBJ_FILES) -o TLMPlugin.so $(CFLAGS)
	mkdir -p $(BINDIR)/FMU
	mkdir -p $(BINDIR)/FMU/binaries
	mkdir -p $(BINDIR)/FMU/binaries/linux64
	cp -f TLMPlugin.so $(BINDIR)/FMU/binaries/linux64
	cp -f modelDescription.xml $(BINDIR)/FMU

clean :
	rm -f *.o *.so $(BINDIR)/FMU/modelDescription.xml $(BINDIR)/FMU/binaries/linux64/*.so

# DO NOT DELETE
