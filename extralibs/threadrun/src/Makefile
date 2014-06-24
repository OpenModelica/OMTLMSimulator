#################################################################################
#

SHELL=/bin/sh

include $(MAKEFILEHEADHOME)/Makefile.head
UP=$(MAKEFILEHEADHOME)


# Architectures ends here, DO NOT delete this line config depends on it.
#
#################################################################################

ifneq (,$(findstring -DUSE_PARALLEL_INTERFACE_OFF,$(commdef)))
	MINCLUDE_SCHEDULERHOME=-I$(SCHEDULERHOME)/src
else
	MINCLUDE_SCHEDULERHOME=-I$(SCHEDULERHOME)/include
endif


MINCLUDE= -I../include -I$(MISCHOME)/include -I$(NMATHOME) -I$(SCHEDULERHOME)/src $(MINCLUDE_SCHEDULER) -I$(TIMEHOME)/include -I$(PVM_ROOT)/include

CCSRC=TaskQueue.cc TaskQueueNode.cc 

.cc.o:
	$(CC) $(SPECFLAGS) $(CLIBFLAGS) $(MINCLUDE) -c $< -o $@ 


OBJS=$(CCSRC:%.cc=$(ABI)/%.o)


$(ABI)/%.o: %.cc
	$(CC) $(SPECFLAGS) $(CLIBFLAGS)  $(commdef)  $(MINCLUDE) -c $< -o $@ 

lib: 
	$(MAKE) dirs
	$(MAKE)	../lib/$(ABI)/libthreadrun.a  

clean:
	rm -f $(OBJS) testTaskQueue 
#	rm -f ../lib/$(ABI)/libthreadrun.a 


test: testTaskQueue

testTaskQueue: testTaskQueue.cc LoopScheduler.cc TaskQueue.cc ../include/TaskQueue.h lib

testTaskQueue:  testTaskQueue.cc LoopScheduler.cc TaskQueue.cc ../include/TaskQueue.h lib
	$(MAKE)
	$(CC) $(OPTFLAGS1) $(commdef) -o $(ABI)/testTaskQueue $(MINCLUDE) testTaskQueue.cc \
		../lib/$(ABI)/libthreadrun.a -lpthread -L$(MISCHOME)/lib/$(ABI) -lmisc \
	-L$(NMATHOME)/lib/$(ABI)  -lnewmat -L$(SCHEDULERHOME)//lib/$(ABI) -lSchedGR \
	-L$(TIMEHOME)/lib/$(ABI) -lrtime -lrt

.PHONY: dirs spec clean

dirs:
	@if [ ! -d $(ABI) ] ; then \
        mkdir $(ABI) ; fi ; \
	if [ ! -d ../lib/$(ABI) ] ; then  mkdir -p ../lib/$(ABI) ; fi

../lib/$(ABI)/libthreadrun.a: $(OBJS)
	(rm -f ../lib/$(ABI)/libthreadrun.a)
	($(AR) ruv ../lib/$(ABI)/libthreadrun.a $(OBJS); )

spec:
	/bin/sh ../../../src/setuplibs.sh threadrun
