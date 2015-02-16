.PHONY: all lib default depend clean

# The following is need for BEAST compatibility
ifeq ($(MAKEFILEHEADHOME),)
  UP=$(PWD)
  MAKEFILEHEADHOME=$(UP)
else
  # This is for BEAST
  UP=$(MAKEFILEHEADHOME)/src
endif


plugins=ADAMS Simulink Modelica OpenModelica

all default: lib
	cd common; $(MAKE) all

lib:
# Note: lib will not make the manager!
	cd extralibs; $(MAKE) 
	cd common; $(MAKE) lib

install:
	cd common ; $(MAKE) install
	for i in ${plugins} ; do \
		( cd $$i ; $(MAKE) UP=$(UP) MAKEFILEHEADHOME=$(MAKEFILEHEADHOME) install ) \
	done


depend:
	cd common; $(MAKE) depend

clean:
	cd common; $(MAKE) clean
	cd extralibs; $(MAKE) clean
	for i in ${plugins} ; do \
		( cd $$i ; $(MAKE) UP=$(UP) MAKEFILEHEADHOME=$(MAKEFILEHEADHOME) clean ) \
	done
