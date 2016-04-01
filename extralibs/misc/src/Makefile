#################################################################################
#

SHELL=/bin/sh

# The following is need for BEAST compatibility
ifeq ($(MAKEFILEHEADHOME),)
  UP=$(PWD)/../../..
  MAKEFILEHEADHOME=$(UP)
else
  # This is for BEAST
  UP=$(MAKEFILEHEADHOME)/src
endif

include $(MAKEFILEHEADHOME)/Makefile.head

# Architectures ends here, DO NOT delete this line config depends on it.
#
#################################################################################

INCLUDES = -I../include
SPECFLAGS = -fPIC

CCSRC= double3Vec.cc double33Mat.cc coordTrans.cc strConv.cc

OBJS=$(CCSRC:%.cc=$(ABI)/%.o)
OBJS+=$(CSRC:%.c=$(ABI)/%.o)

$(ABI)/%.o: %.cc
	$(CC) $(SPECFLAGS) $(CLIBFLAGS) $(INCLUDES) -c $< -o $@ 

$(ABI)/%.o: %.c
	$(cc) $(SPECFLAGS) $(CLIBFLAGS) $(INCLUDES) -c $< -o $@ 

lib: dirs ../lib/$(ABI)/libtlmmisc.a

clean:
	-rm -rf $(ABI)
	-rm -rf ../lib


dirs : $(ABI) ../lib/$(ABI)

../lib/$(ABI) : 
	mkdir -p ../lib/$(ABI)


../lib/$(ABI)/libtlmmisc.a :  $(OBJS)
	($(AR) ruv ../lib/$(ABI)/libtlmmisc.a $(OBJS); )


# Change 080701: $ABI is used for *.o files and .tail files

$(ABI)/makedepend.tail : $(ABI)
	touch $@
$(ABI) :	
	mkdir -p $@
depend: 
	@echo > $(ABI)/makedepend.tail
	makedepend -Y -p"\$$(ABI)/" -f $(ABI)/makedepend.tail \
			$(INCLUDES) $(CCSRC) $(CSRC) $(FORDEPEND) 2> /dev/null


-include $(ABI)/makedepend.tail
