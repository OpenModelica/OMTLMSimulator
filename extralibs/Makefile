# This is for BEAST compatibility
ifeq ($(MAKEFILEHEADHOME),)
  UP=$(PWD)/..
  MAKEFILEHEADHOME=$(UP)
else
  # This is for BEAST
  UP=$(MAKEFILEHEADHOME)/src
endif

include $(MAKEFILEHEADHOME)/Makefile.head

buildlibs=misc/src threadrun rtime 


all:
	for i in ${buildlibs} ; do \
		( cd $$i ; $(MAKE) UP=$(UP) MAKEFILEHEADHOME=$(MAKEFILEHEADHOME) lib ) \
	done

clean:
	for i in ${buildlibs} ; do \
		( cd $$i ; $(MAKE) UP=$(UP) MAKEFILEHEADHOME=$(MAKEFILEHEADHOME) clean ) \
	done
