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

INSTDIR=$(BINDIR)/Hopsan
LIBDIR=$(INSTDIR)/TLMPluginLib

default:
	cd TLMPluginLib; qmake TLMPluginLib.pro "CONFIG+=release"
	cd TLMPluginLib; $(MAKE)

install: default
	-mkdir -p $(INSTDIR)
	-mkdir -p $(LIBDIR)
	cp TLMPluginLib/TLMPluginLib.xml $(LIBDIR)
	cp TLMPluginLib/TLMPluginLib.cc $(LIBDIR)
	cp TLMPluginLib/common.h $(LIBDIR)
	cp TLMPluginLib/TLMPluginHandler.xml $(LIBDIR)
	cp TLMPluginLib/TLMPluginHandler.hpp $(LIBDIR)
	cp TLMPluginLib/TLMPluginHandler.svg $(LIBDIR)
	cp TLMPluginLib/TLMPluginInterfaceMechanical1D.xml $(LIBDIR)
	cp TLMPluginLib/TLMPluginInterfaceMechanical1D.hpp $(LIBDIR)
	cp TLMPluginLib/TLMPluginInterfaceHydraulic1D.xml $(LIBDIR)
	cp TLMPluginLib/TLMPluginInterfaceHydraulic1D.hpp $(LIBDIR)
	cp TLMPluginLib/TLMPluginInterfaceRotational1D.xml $(LIBDIR)
	cp TLMPluginLib/TLMPluginInterfaceRotational1D.hpp $(LIBDIR)
	cp TLMPluginLib/TLMPluginInterface1D.svg $(LIBDIR)    
	cp TLMPluginLib/TLMPluginInterfaceSignalInput.svg $(LIBDIR)
	cp TLMPluginLib/TLMPluginInterfaceSignalInput.hpp $(LIBDIR)
	cp TLMPluginLib/TLMPluginInterfaceSignalInput.xml $(LIBDIR)
	cp TLMPluginLib/TLMPluginInterfaceSignalOutput.svg $(LIBDIR)
	cp TLMPluginLib/TLMPluginInterfaceSignalOutput.hpp $(LIBDIR)
	cp TLMPluginLib/TLMPluginInterfaceSignalOutput.xml $(LIBDIR)

	cp StartTLMHopsan.bat $(BINDIR)
	cp StartTLMHopsan $(BINDIR)
    
clean:
	rm -rf $(INSTDIR)
	- rm $(BINDIR)/StartTLMHopsan.bat
	- rm $(BINDIR)/StartTLMHopsan
	cd TLMPluginLib; $(MAKE) clean

# DO NOT DELETE
