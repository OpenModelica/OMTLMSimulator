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

install:
	-mkdir -p $(INSTDIR)
	-mkdir -p $(LIBDIR)
	cp TLMPluginLib/TLMPluginLib.dll $(LIBDIR)
	cp TLMPluginLib/TLMPluginLib.xml $(LIBDIR)
	cp TLMPluginLib/TLMPluginLib.cc $(LIBDIR)
	cp TLMPluginLib/TLMPluginInterface.xml $(LIBDIR)
	cp TLMPluginLib/TLMPluginInterface.hpp $(LIBDIR)
	cp TLMPluginLib/TLMPluginInterface.svg $(LIBDIR)
	cp StartTLMHopsan.bat $(BINDIR)
    
clean:
	rm -rf $(INSTDIR)
	rm $(BINDIR)/StartTLMHopsan.bat

# DO NOT DELETE