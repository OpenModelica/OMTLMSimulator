# This is for BEAST compatibility
ifeq ($(MAKEFILEHEADHOME),)
  UP=$(PWD)/..
  MAKEFILEHEADHOME=$(UP)
  buildlibs=misc/src rtime
else
  # This is for BEAST
  UP=$(MAKEFILEHEADHOME)/src
  buildlibs=misc/src
endif

include $(MAKEFILEHEADHOME)/Makefile.head

all:
	$(CP-LIBXML)
	for i in ${buildlibs} ; do \
		( cd $$i ; $(MAKE) UP=$(UP) MAKEFILEHEADHOME=$(MAKEFILEHEADHOME) lib ) \
	done

clean:
	rm -rf $(UP)/bin/libxml2*
	for i in ${buildlibs} ; do \
		( cd $$i ; $(MAKE) UP=$(UP) MAKEFILEHEADHOME=$(MAKEFILEHEADHOME) clean ) \
	done
