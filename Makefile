.PHONY: all lib default depend clean

# The following is need for BEAST compatibility
ifeq ($(MAKEFILEHEADHOME),)
  UP=$(PWD)
  MAKEFILEHEADHOME=$(UP)
  BINDIR=$(UP)/bin
else
  # This is for BEAST
  UP=$(MAKEFILEHEADHOME)/src
  BINDIR=$(UP)/../bin/$(ABI)
endif

allplugins=ADAMS Simulink Modelica OpenModelica FMIWrapper
plugins=Modelica OpenModelica FMIWrapper

all default: lib
	$(MAKE) -C common all

lib:
# Note: lib will not make the manager!
	cd 3rdParty; $(MAKE)
	$(MAKE) -C common lib

install: lib
	$(MAKE) -C common install
	for i in ${plugins} ; do \
		$(MAKE) -C $$i install \
	done

omtlmlib:
	$(MAKE) -C common omtlmlib

depend:
	$(MAKE) -C common depend

clean:
	rm -rf $(BINDIR)/*.dll
	$(MAKE) -C common clean
	cd 3rdParty; $(MAKE) clean
	for i in ${allplugins} ; do \
		$(MAKE) -C $$i clean \
	done
