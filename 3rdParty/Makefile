detected_OS := $(shell uname -s)

# This is for BEAST compatibility
ifeq ($(MAKEFILEHEADHOME),)
  UP=$(PWD)/..
  MAKEFILEHEADHOME=$(UP)
ifeq ($(detected_OS),Darwin)
  #Dont build rtime on Mac
  buildlibs=misc/src
else
  buildlibs=misc/src rtime
endif
else
  # This is for BEAST
  UP=$(MAKEFILEHEADHOME)/src
  buildlibs=misc/src
endif

include $(MAKEFILEHEADHOME)/Makefile.head

all:
	$(CP-LIBXML)
	for i in ${buildlibs} ; do \
		( cd $$i ; $(MAKE) UP=$(UP) lib ) \
	done

clean:
	rm -rf $(UP)/bin/libxml2*
	for i in ${buildlibs} ; do \
		( cd $$i ; $(MAKE) UP=$(UP) clean ) \
	done
