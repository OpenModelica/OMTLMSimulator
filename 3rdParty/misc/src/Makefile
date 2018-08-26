#################################################################################
#

SHELL=/bin/sh

ifeq ($(MAKEFILEHEADHOME),)
# In OpenModelica Team, MAKEFILEHEADHOME is not defined.
# Any other indicator can be used for this branch if you wish 

# These files are not needed in OpenModelica Team, we skip them
CCSRC_FOR_BEAST=
INCLUDE_FOR_BEAST=
# These symbols are defined in Makefile.head, we need them for generic make
override CXX += -std=c++11 -fPIC
override CC += -fPIC

else

# In Beast Team
include $(MAKEFILEHEADHOME)/Makefile.head
CCSRC_FOR_BEAST=PrivPack.cc MathUtilityFunc.cc
UP=../../../src
INCLUDE_FOR_BEAST=-I$(NMATHOME)

endif

# Architectures ends here, DO NOT delete this line config depends on it.
#
#################################################################################

B_INCLUDES = -I../include $(INCLUDE_FOR_BEAST)

MINCLUDE= $(B_INCLUDES) 
CCSRC= double3.cc realN.cc float2.cc double2.cc float3.cc double33.cc coordTransform.cc doubleN.cc doubleN3.cc doubleN33.cc intN.cc tostr.cc ErrorLog.cc portability.cc Bstring.cc Btokenizer.cc \
        double33s.cc doubleN33s.cc float33s.cc dsyevv3.cc  dsyevq3.cc dsyevc3.cc dsytrd3.cc $(CCSRC_FOR_BEAST)

CSRC=g2c_w32.c  

OBJS=$(CCSRC:%.cc=$(ABI)/%.o)
OBJS+=$(CSRC:%.c=$(ABI)/%.o)

$(ABI)/%.o: %.cc
	$(CXX) $(SPECFLAGS) $(CXXFLAGS) $(OPTFLAGS4) $(MINCLUDE) -c $< -o $@ 

$(ABI)/%.o: %.c
	$(CC) $(SPECFLAGS) $(CFLAGS) $(CLIBFLAGS) $(MINCLUDE) -c $< -o $@ 

lib: dirs ../lib/$(ABI)/libmisc.a

GTEST=../../gtest/googletest-master/googletest/
ifeq ($(ABI),LINUX64)
LPTHREAD = -lpthread
endif

%_unittest: %_unittest.cc ../lib/$(ABI)/libmisc.a $(GTEST)
	$(CXX) $(SPECFLAGS) $(CLIBFLAGS) $(MINCLUDE) -I$(GTEST)/include -c $< -o $(ABI)/$@.o
	$(LINK) -o  $(ABI)/$@  $(LPTHREAD) $(ABI)/$@.o $(GTEST)/make/$(ABI)/gtest-all.o $(GTEST)/make/$(ABI)/gtest_main.o ../lib/$(ABI)/libmisc.a
	$(ABI)/$@


unittest:Bstring_unittest

$(GTEST):
	cd ../../gtest;$(MAKE)

clean:
	-rm -f $(ABI)/*
	-rm -f ../lib/$(ABI)/libmisc.a 

testpp: testpp.cc PrivPack.cc ../include/PrivPack.h
	$(CXX) -o testpp $(MINCLUDE) testpp.cc \
		../lib/$(ABI)/libmisc.a \
		-lm

testbp: testbp.cc boolPack.cc ../include/boolPack.h
	$(CXX) -o testbp $(MINCLUDE) testbp.cc \
		../lib/$(ABI)/libmisc.a \
		-lm


testdouble33s: testdouble33s.cc double33s.cc ../include/double33s.h lib
	$(CXX) -o testdouble33s  $(SPECFLAGS) $(CCFLAGS) $(OPTFLAGS4) $(MINCLUDE) testdouble33s.cc \
		../lib/$(ABI)/libmisc.a \
		-lm

testlightmat: testlightmat.cc lib
	$(CXX) -o testlightmat $(MINCLUDE) testlightmat.cc \
		../lib/$(ABI)/libmisc.a \
		-lm

testString: lib testString.cc
	$(CXX) -g -o testString $(MINCLUDE) testString.cc \
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


-include $(ABI)/makedepend.tail
