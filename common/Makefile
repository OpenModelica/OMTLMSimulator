# Makefile for TLMManager & Plugin

ifeq ($(MAKEFILEHEADHOME),)
  UP=..
  MAKEFILEHEADHOME=$(UP)
  BINDIR=$(UP)/bin
else
  # This is for BEAST
  UP=$(MAKEFILEHEADHOME)/src
  BINDIR=$(UP)/../bin/$(ABI)
endif

include $(MAKEFILEHEADHOME)/Makefile.head

MISCHOME = ../3rdParty/misc
TIMEHOME = ../3rdParty/rtime

TARGET_TRIPLE	:= $(subst -, ,$(shell $(CC) -dumpmachine))
TARGET_ARCH		:= $(word 1,$(TARGET_TRIPLE))
TARGET_OS		:= $(word 3,$(TARGET_TRIPLE))

EXTRAFLAGS =
ifeq ($TARGET_OS,mingw32)
else ifeq ($TARGET_OS,cygwin)
else
EXTRAFLAGS = -fPIC
endif

CC += -std=c++11 $(EXTRAFLAGS)

SRCCLT= Plugin/PluginImplementer.cc \
	Plugin/MonitoringPluginImplementer.cc \
	Communication/TLMClientComm.cc \
	Communication/TLMCommUtil.cc \
	Interfaces/TLMInterface.cc \
	Interfaces/TLMInterfaceSignal.cc \
	Interfaces/TLMInterfaceSignalInput.cc \
	Interfaces/TLMInterfaceSignalOutput.cc \
	Interfaces/TLMInterface1D.cc \
	Interfaces/TLMInterface3D.cc \
	Parameters/ComponentParameter.cc \
	Logging/TLMErrorLog.cc \
	Plugin/TLMPlugin.cc 


SRCMGR= Communication/ManagerCommHandler.cc \
	ManagerMain.cc	\
	CompositeModels/CompositeModel.cc \
	CompositeModels/CompositeModelReader.cc \
	Communication/TLMCommUtil.cc \
	Communication/TLMManagerComm.cc \
	Communication/TLMMessageQueue.cc \
	Logging/TLMErrorLog.cc

SRCSRVLIB= Communication/ManagerCommHandler.cc \
	CompositeModels/CompositeModel.cc \
	Communication/TLMCommUtil.cc \
	Communication/TLMManagerComm.cc \
	Communication/TLMMessageQueue.cc \
	Logging/TLMErrorLog.cc

SRCMONITOR= $(SRCCLT) \
	CompositeModels/CompositeModel.cc \
	CompositeModels/CompositeModelReader.cc \
	MonitorMain.cc

SRCMSTLIB=  $(SRCCLT) \
	CompositeModels/CompositeModel.cc \
	CompositeModels/CompositeModelReader.cc \
	Communication/ManagerCommHandler.cc \
	Communication/TLMManagerComm.cc \
	Communication/TLMMessageQueue.cc \
	OMTLMSimulatorLib/OMTLMSimulatorLib.cc

SRCMSTMAIN= OMTLMSimulatorMain.cc

CP=cp

SRC= $($(SRCTYPE))

SRCTST=	TLMTestApp.cc

OBJS = $(SRC:%.cc=$(ABI)/%.o)

INCLUDES= -I. \
	-I$(MISCHOME)/include \
	-I$(TIMEHOME) \
	$(INCLPTHREAD)

LIBS = 	-L$(MISCHOME)/lib/$(ABI) \
	-L$(TIMEHOME)/lib/$(ABI) \
	-L$(ABI) \
	-lmisc -lrtime

INCXTR=
LIBXTR=

default: help all

help:
	@echo Makefile for TLMPlugin related files.
	@echo Possible targets are:
	@echo lib - creates the libTLM.a and libTLM_m.a libraries - the client side of the plugin
	@echo manager - creates the tlmmanager application
	@echo all, defaulf: build everything.


all: lib manager monitor mstlib test

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

omtlmlib:
	$(MAKE) dir
	$(MAKE) SRCTYPE=SRCMSTLIB $(ABI)/libomtlmsimulator$(SHREXT)
	$(MAKE) SRCTYPE=SRCMSTMAIN $(ABI)/omtlmsimulator$(FEXT)

test:
	$(MAKE) dir
	$(MAKE) $(ABI)/testapp$(FEXT)

install: manager monitor mstlib
	cp $(ABI)/tlmmonitor$(FEXT) $(ABI)/tlmmanager$(FEXT) ../bin

$(ABI)/libTLM.a: $(OBJS)
	$(MAKE) dir
	@echo Use TLM-library for client applications using misc library.
	@echo Use TLM_m library otherwise.
	$(AR) ruv $(ABI)/libTLM.a $(OBJS) 

$(ABI)/libTLM_s.a: $(OBJS)
	$(AR) ruv $(ABI)/libTLM_s.a $(OBJS) 

$(ABI)/tlmmanager$(FEXT): $(OBJS)
	$(MAKE) dir
	$(LINK) -o $(ABI)/tlmmanager$(FEXT) $(OBJS) $(LIBS) $(XTRLIBS) $(LIBXML) $(LIBPTHREAD)
	if [ -f $(ABI)/tlmmanager.exe.manifest ] ; then \
		(cd $(ABI) ; mt.exe -manifest tlmmanager.exe.manifest -outputresource:tlmmanager.exe\;1); fi
	$(CP) $(ABI)/tlmmanager$(FEXT) $(BINDIR)/tlmmanager$(FEXT)

$(ABI)/tlmmonitor$(FEXT): $(OBJS)
	$(MAKE) dir
	$(LINK) -o $(ABI)/tlmmonitor$(FEXT) $(OBJS) $(LIBS) $(XTRLIBS) $(LIBXML) $(LIBPTHREAD)
	if [ -f $(ABI)/tlmmonitor.exe.manifest ] ; then \
		(cd $(ABI) ; mt.exe -manifest tlmmonitor.exe.manifest -outputresource:tlmmonitor.exe\;1); fi
	$(CP) $(ABI)/tlmmonitor$(FEXT) $(BINDIR)/tlmmonitor$(FEXT)

$(ABI)/omtlmsimulator$(FEXT): $(OBJS)
	$(MAKE) dir
	$(LINK) -o $(ABI)/omtlmsimulator$(FEXT) $(OBJS) $(LIBPTHREAD) -L$(ABI) -lomtlmsimulator
	if [ -f $(ABI)/omtlmsimulator.exe.manifest ] ; then \
		(cd $(ABI) ; mt.exe -manifest omtlmsimulator.exe.manifest -outputresource:omtlmsimulator.exe\;1); fi
	$(CP) $(ABI)/omtlmsimulator$(FEXT) $(BINDIR)/omtlmsimulator$(FEXT)

$(ABI)/libomtlmsimulator$(SHREXT): $(OBJS)
	$(LINK) -shared -fPIC -o $(ABI)/libomtlmsimulator$(SHREXT) $(OBJS) $(LIBS) $(XTRLIBS) $(LIBXML) $(LIBPTHREAD)
	$(CP) $(ABI)/libomtlmsimulator$(SHREXT) $(BINDIR)/libomtlmsimulator$(SHREXT)

$(ABI)/testapp$(FEXT): $(ABI)/TLMTestApp.o
	$(LINK) $(ABI)/TLMTestApp.o -o $(ABI)/testapp$(FEXT) -L$(ABI) -lTLM $(LIBS) $(XTRLIBS) $(LIBPTHREAD)

$(ABI)/%.o: %.cc
	$(CC) $(DEFINES) $(CCFLAGS) $(OPTFLAGS4) $(INCLUDES) $(INCLXML) -c $< -o $@

.PHONY: clean dir depend lib manager test

clean:
	rm -rf $(ABI)
	rm -rf $(BINDIR)/tlmmanager$(FEXT) $(BINDIR)/tlmmonitor$(FEXT) $(BINDIR)/libomtlmsimulator$(SHREXT) $(BINDIR)/omtlmsimulator$(FEXT)

# Change 080701: $ABI is used for *.o files and .tail files

dir:
	@ ( if [ ! -d ./$(ABI) ] ; then mkdir -p ./$(ABI) ; fi)

$(ABI)/makedepend.tail: $(ABI)
	touch $@
$(ABI):
	mkdir -p $@
	mkdir -p $@/Interfaces
	mkdir -p $@/CompositeModels
	mkdir -p $@/Plugin
	mkdir -p $@/Communication
	mkdir -p $@/Parameters
	mkdir -p $@/Logging
	mkdir -p $@/OMTLMSimulatorLib
depend: 
	@echo > $(ABI)/makedepend.tail
	makedepend -Y -p"\$$(ABI)/" -f $(ABI)/makedepend.tail \
			$(INCLUDES) $(SRCCLT) $(SRCMGR) $(FORDEPEND) 2> /dev/null

# if this file doesn't exist, a touch may be enough.
-include $(ABI)/makedepend.tail
