.PHONY: all lib default depend clean

# The following is need for BEAST compatibility
ifeq ($(MAKEFILEHEADHOME),"")
  # This is for BEAST
  UP=$(MAKEFILEHEADHOME)/src
else
  UP=$(shell pwd)
  MAKEFILEHEADHOME=$(UP)	
endif


plugins=ADAMS Simulink
extralibs=misc/src threadrun rtime 

all  default:
	cd common; $(MAKE) all

lib:
# Note: lib will not make the manager!
	cd extralibs; $(MAKE) 
	cd common; $(MAKE) lib

install: default
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
