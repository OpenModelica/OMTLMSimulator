# Makefile for rtime
# A. Siemers 00.01.25
UP=../../src

#include $(MAKEFILEHEADHOME)/Makefile.head

HOST_OFILES =

srcdir = .



INSTALL = ./install-sh

INSTALLDIR = ./lib/$(ABI)
LIBNAME = $(INSTALLDIR)/librtime.a 
LDLIBS = $(LIBS) $(LIBNAME)

# Pthread include is needed for WINDOWS 
INCLUDES=-I../pthread/$(ABI)/include -I../include

OFILES=rtime.o timing.o $(HOST_OFILES)
CFILES=$(OFILES:%.o=%.c)
ABIOBJS=$(OFILES:%.o=$(ABI)/%.o)

RM = rm -f
MV = mv -f



lib: dir 
	$(MAKE) $(LIBNAME) 

dir:
	@ ( if [ ! -d lib ] ; then  mkdir lib ; fi )
	@ ( if [ ! -d $(ABI) ] ; then  mkdir $(ABI) ; fi )
	@ ( if [ ! -d lib/$(ABI) ] ; then  mkdir lib/$(ABI) ; fi )

$(LIBNAME): $(ABIOBJS)
	($(AR) ruv $(LIBNAME) $(ABIOBJS); )

all: lib $(ABI)/test $(ABI)/timing_test


$(ABI)/test: $(ABI)/test.o $(LIBNAME)
	$(LINK) -o $@ $(ABI)/test.o $(LDLIBS)

$(ABI)/timing_test: $(ABI)/timing_test.o $(LIBNAME)
	$(LINK) -o $@ $(ABI)/timing_test.o $(LDLIBS)

$(ABI)/%.o: %.c
	$(CC) -fPIC -c $< $(INCLUDES) -o $@ 

install: rtime.h $(LIBNAME)
	$(INSTALL) -d $(INSTALLDIR)/include
	$(INSTALL) -c -m 444 rtime.h $(INSTALLDIR)/include
	$(INSTALL) -c -m 444 timing.h $(INSTALLDIR)/include
	$(INSTALL) -d $(INSTALLDIR)/lib
	$(INSTALL) -c -m 444 $(LIBNAME) $(INSTALLDIR)/lib
	$(INSTALL) -d $(INSTALLDIR)/man/man3
	$(INSTALL) -c -m 444 rtime.man $(INSTALLDIR)/man/man3/rtime.3x
	$(INSTALL) -c -m 444 timing.man $(INSTALLDIR)/man/man3/timing.3x

clean:
	rm -f $(ABIOBJS) $(ABI)/*.o *~ core a.out $(ABI)/timing_test $(ABI)/test
#	rm -f $(LIBNAME)

distclean: clean
	rm -f Makefile *.man config.cache config.log config.status

# Change 080701: $ABI is used for *.o files and .tail files	

$(ABI)/makedepend.tail :
	if [ ! -d $(ABI) ] ; then mkdir -p $(ABI) ; fi ;
	touch $(ABI)/makedepend.tail


depend:
	@echo > $(ABI)/makedepend.tail
	makedepend -Y -p"\$$(ABI)/" -f $(ABI)/makedepend.tail \
			$(B_INCLUDES) $(CFILES)  test.c test2.c timing_test.c $(FORDEPEND) 2> /dev/null



tar: distclean
	find . \( -name *~ -o -name core \) -exec rm -f '{}' \;
	dir=`basename $$PWD`; \
	tar cvFFf - -C .. $$dir | gzip > ../$$dir.tar.gz

include $(ABI)/makedepend.tail

