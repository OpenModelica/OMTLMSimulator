# Makefile for TLMManager & Plugin

ifeq ($(MAKEFILEHEADHOME),)
  UP=$(PWD)/..
  MAKEFILEHEADHOME=$(UP)
  BINDIR=$(UP)/bin
else
  # This is for BEAST
  UP=$(MAKEFILEHEADHOME)/src
  BINDIR=$(UP)/../bin/$(ABI)
endif

include $(MAKEFILEHEADHOME)/Makefile.head


SRCCLT= PluginImplementer.cc \
	MonitoringPluginImplementer.cc \
	TLMClientComm.cc \
	TLMCommUtil.cc \
	TLMInterface.cc \
	TLMErrorLog.cc \
	TLMPlugin.cc 


SRCMGR= ManagerCommHandler.cc \
	ManagerMain.cc	\
	MetaModel.cc \
	MetaModelReader.cc \
	TLMCommUtil.cc \
	TLMManagerComm.cc \
	TLMMessageQueue.cc \
	TLMErrorLog.cc

SRCSRVLIB= ManagerCommHandler.cc \
	MetaModel.cc \
	TLMCommUtil.cc \
	TLMManagerComm.cc \
	TLMMessageQueue.cc \
	TLMErrorLog.cc

SRCMONITOR= $(SRCCLT) \
	MetaModel.cc \
	MetaModelReader.cc \
	MonitorManagerMain.cc

CP=cp

SRC= $($(SRCTYPE))

SRCTST=	TLMTestApp.cc

OBJS = $(SRC:%.cc=$(ABI)/%.o)

INCLUDES= -I. \
	-I$(MISCHOME)/include \
	-I$(THREADRUNHOME)/include \
	-I$(TIMEHOME) 

LIBS = 	-L$(MISCHOME)/lib/$(ABI) \
	-L$(THREADRUNHOME)/lib/$(ABI) \
	-L$(TIMEHOME)/lib/$(ABI) \
	-lmisc -lthreadrun -lrtime

INCXTR=

LIBXTR=

default: help all

help:
	@echo Makefile for TLMPlugin related files.
	@echo Possible targets are:
	@echo lib - creates the libTLM.a and libTLM_m.a libraries - the client side of the plugin
	@echo manager - creates the tlmmanager application
	@echo all, defaulf: build everything.


all: lib manager monitor test

lib: lib_s
	$(MAKE) dir
	$(MAKE) SRCTYPE=SRCCLT $(ABI)/libTLM.a

lib_s:
	$(MAKE) dir
	$(MAKE) SRCTYPE=SRCSRVLIB $(ABI)/libTLM_s.a

manager: 
	$(MAKE) dir
	$(MAKE) SRCTYPE=SRCMGR $(ABI)/tlmmanager$(FEXT)

monitor:
	$(MAKE) dir
	$(MAKE) SRCTYPE=SRCMONITOR $(ABI)/tlmmonitor$(FEXT)

test:
	$(MAKE) dir
	$(MAKE) $(ABI)/testapp$(FEXT)


$(ABI)/libTLM.a $(ABI)/libTLM_m.a: $(OBJS)
	$(MAKE) dir
	@echo Use TLM-library for client applications using misc library.
	@echo Use TLM_m library otherwise.
	$(AR) ruv $(ABI)/libTLM.a $(OBJS) 
	$(AR) ruv $(ABI)/libTLM_m.a $(OBJS) \
		$(MISCHOME)/src/$(ABI)/lightmat.o \
		$(MISCHOME)/src/$(ABI)/tostr.o \
		$(MISCHOME)/src/$(ABI)/Bstring.o \
		$(MISCHOME)/src/$(ABI)/ErrorLog.o 

$(ABI)/libTLM_s.a: $(OBJS)
	$(AR) ruv $(ABI)/libTLM_s.a $(OBJS) 

$(ABI)/tlmmanager$(FEXT): $(OBJS)
	$(MAKE) dir
	$(LINK) -o $(ABI)/tlmmanager$(FEXT) $(OBJS) $(LIBS) $(XTRLIBS) $(LIBXML)
	if [ -f $(ABI)/tlmmanager.exe.manifest ] ; then \
		(cd $(ABI) ; mt.exe -manifest tlmmanager.exe.manifest -outputresource:tlmmanager.exe\;1); fi
	$(CP) $(ABI)/tlmmanager$(FEXT) $(BINDIR)/tlmmanager$(FEXT)

$(ABI)/tlmmonitor$(FEXT): $(OBJS)
	$(MAKE) dir
	$(LINK) -o $(ABI)/tlmmonitor$(FEXT) $(OBJS) $(LIBS) $(XTRLIBS) $(LIBXML)
	if [ -f $(ABI)/tlmmonitor.exe.manifest ] ; then \
		(cd $(ABI) ; mt.exe -manifest tlmmonitor.exe.manifest -outputresource:tlmmonitor.exe\;1); fi
	$(CP) $(ABI)/tlmmonitor$(FEXT) $(BINDIR)/tlmmonitor$(FEXT)

$(ABI)/testapp$(FEXT): $(ABI)/TLMTestApp.o $(ABI)/libTLM_m.a
	$(LINK) $(ABI)/TLMTestApp.o -o $(ABI)/testapp$(FEXT) -L$(ABI) -lTLM_m $(XTRLIBS)

$(ABI)/%.o: %.cc
	$(CC) $(DEFINES) $(CCFLAGS) $(OPTFLAGS4) $(INCLUDES) $(INCLXML) -c $< -o $@

.PHONY: clean dir depend lib manager test

clean:
	rm -f $(ABI)/*.o  $(ABI)/*.a $(ABI)/tlmmanager$(FEXT)

# Change 080701: $ABI is used for *.o files and .tail files

dir:
	@ ( if [ ! -d ./$(ABI) ] ; then mkdir -p ./$(ABI) ; fi)

$(ABI)/makedepend.tail: $(ABI)
	touch $@
$(ABI):
	mkdir -p $@
depend: 
	@echo > $(ABI)/makedepend.tail
	makedepend -Y -p"\$$(ABI)/" -f $(ABI)/makedepend.tail \
			$(INCLUDES) $(SRCCLT) $(SRCMGR) $(FORDEPEND) 2> /dev/null

# if this file doesn't exist, a touch may be enough.
-include $(ABI)/makedepend.tail
