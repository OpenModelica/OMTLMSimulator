#################################################################################
#

SHELL=/bin/sh

include $(MAKEFILEHEADHOME)/Makefile.head
UP=../../../src


# Architectures ends here, DO NOT delete this line config depends on it.
#
#################################################################################

B_INCLUDES = -I../include
MINCLUDE= $(B_INCLUDES) 


CCSRC= lightmat.cc tostr.cc ErrorLog.cc portability.cc Bstring.cc Btokenizer.cc 
#	double33s.cc dsyevv3.cc  dsyevq3.cc dsyevc3.cc dsytrd3.cc 
#	MathUtilityFunc.cc

CSRC=g2c_w32.c  

.cc.o:
	$(CC) $(SPECFLAGS) $(CLIBFLAGS) $(MINCLUDE) -c $< -o $@ 

.c.o:
	$(cc) $(SPECFLAGS) $(CLIBFLAGS) $(MINCLUDE) -c $< -o $@ 

OBJS=$(CCSRC:%.cc=$(ABI)/%.o)
OBJS+=$(CSRC:%.c=$(ABI)/%.o)

$(ABI)/%.o: %.cc
	$(CC) $(SPECFLAGS) $(CLIBFLAGS) $(MINCLUDE) -c $< -o $@ 

$(ABI)/%.o: %.c
	$(cc) $(SPECFLAGS) $(CLIBFLAGS) $(MINCLUDE) -c $< -o $@ 

lib: dirs ../lib/$(ABI)/libmisc.a  

clean:
	rm -f $(OBJS) 
#	../lib/$(ABI)/libmisc.a 

testpp: testpp.cc PrivPack.cc ../include/PrivPack.h
	$(CC) -o testpp $(MINCLUDE) testpp.cc \
		../lib/$(ABI)/libmisc.a \
		-lm

testdouble33s: testdouble33s.cc double33s.cc ../include/double33s.h lib
	$(CC) -o testdouble33s $(CLIBFLAGS)  $(MINCLUDE) testdouble33s.cc \
		../lib/$(ABI)/libmisc.a \
		-lm

testlightmat: testlightmat.cc lib
	$(CC) -o testlightmat $(MINCLUDE) testlightmat.cc \
		../lib/$(ABI)/libmisc.a \
		-lm

testString: lib testString.cc
	$(CC) -g -o testString $(MINCLUDE) testString.cc \
		../lib/$(ABI)/libmisc.a \
		-lm

dirs : $(ABI) ../lib/$(ABI)

../lib/$(ABI) : 
	mkdir -p ../lib/$(ABI)


../lib/$(ABI)/libmisc.a :  $(OBJS)
#	(rm -f ../lib/$(ABI)/libmisc.a)
	($(AR) ruv ../lib/$(ABI)/libmisc.a $(OBJS); )

spec:
	/bin/sh ../../../src/setuplibs.sh fida
	echo $(MISCHOME)
	echo $(CVODEHOME)


# Change 080701: $ABI is used for *.o files and .tail files

$(ABI)/makedepend.tail : $(ABI)
	touch $@
$(ABI) :	
	mkdir -p $@
depend: 
	@echo > $(ABI)/makedepend.tail
	makedepend -Y -p"\$$(ABI)/" -f $(ABI)/makedepend.tail \
			$(B_INCLUDES) $(CCSRC) $(CSRC) $(FORDEPEND) 2> /dev/null


include $(ABI)/makedepend.tail
