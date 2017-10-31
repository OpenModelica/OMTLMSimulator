# QT -= core gui, means that we should not link the default qt libs into the component
# Template = lib, means that we want to build a library (.dll or .so)
QT -= core gui
TEMPLATE = lib

# TARGET is the name of the compiled lib, (.dll or .so will be added automatically)
# Change this to the name of YOUR lib
TARGET = TLMPluginLib

# Destination for the compiled dll. $${PWD}/ means the same directory as this .pro file, even if you use shadow build
DESTDIR = $${PWD}/../../bin/Hopsan/TLMPluginLib

#TLMPLugin include paths
INCLUDEPATH += ../../common
INCLUDEPATH += ../../3rdParty/misc/include

unix {
LIBS += -ldl
}
win32 {
LIBS += -lws2_32
}

# The location to search for the Hopsan include files, by specifying the path here, you dont need to do this everywhere in all of your component .hpp files
# See myLocalPathTemplate.prf, copy this file (do not change it, change the copy to suit your needs)
!exists(myLocalPaths.prf){
    error("myLocalPaths.prf does not exist. You need to COPY the template file. DO NOT commit myLocalPaths.prf")
}
include(myLocalPaths.prf)

# Special options for deug and release mode. This will link the correct HopsanCore .dll or .so
# In debug mode HopsanCore has the debug extension _d
include(hopsanDebugReleaseCompile.prf)

# Reduce compile output clutter, but show warnings
#CONFIG += silent warn_on

# The compiler should be pedantic to catch all errors (optional)
#QMAKE_CXXFLAGS += -pedantic

DEFINES += INTERFACE_TYPES

CONFIG += c++11

# -------------------------------------------------
# Project files
# -------------------------------------------------
SOURCES += \
    TLMPluginLib.cc \
    ../../common/Plugin/PluginImplementer.cc \
    ../../common/Communication/TLMClientComm.cc \
    ../../common/Communication/TLMCommUtil.cc \
    ../../common/Logging/TLMErrorLog.cc \
    ../../common/Interfaces/TLMInterface.cc \
    ../../common/Plugin/TLMPlugin.cc \
    ../../3rdParty/misc/src/Bstring.cc \
    ../../3rdParty/misc/src/ErrorLog.cc \
    ../../3rdParty/misc/src/strConv.cc \
    ../../3rdParty/misc/src/coordTransform.cc \
    ../../3rdParty/misc/src/double3.cc \
    ../../3rdParty/misc/src/double33.cc \
    ../../3rdParty/misc/src/double33s.cc \
    ../../3rdParty/misc/src/dsyevq3.cc \
    ../../3rdParty/misc/src/dsyevv3.cc \
    ../../3rdParty/misc/src/dsytrd3.cc \
    ../../3rdParty/misc/src/dsyevc3.cc \
    ../../common/Interfaces/TLMInterfaceSignal.cc \
    ../../common/Interfaces/TLMInterfaceSignalInput.cc \
    ../../common/Interfaces/TLMInterfaceSignalOutput.cc \
    ../../common/Interfaces/TLMInterface1D.cc \
    ../../common/Interfaces/TLMInterface3D.cc \
    ../../common/Parameters/ComponentParameter.cc

HEADERS += \
    TLMPluginInterfaceSignalInput.hpp \
    TLMPluginInterfaceSignalOutput.hpp \
    common.h \
    TLMPluginHandler.hpp \
    TLMPluginInterfaceMechanical1D.hpp \
    TLMPluginInterfaceHydraulic1D.hpp \
    TLMPluginInterfaceRotational1D.hpp

OTHER_FILES += \
    hopsanDebugReleaseCompile.prf \
    myLocalPaths.prf

DISTFILES += \
    TLMPluginInterface1D.xml \
    TLMPluginInterface1D.svg \
    TLMPluginInterfaceSignalInput.xml \
    TLMPluginInterfaceSignalOutput.xml \
    TLMPluginInterfaceSignalInput.svg \
    TLMPluginInterfaceSignalOutput.svg \
    TLMPluginLib.xml \
    TLMPluginHandler.xml \
    TLMPluginHandler.svg \
    TLMPluginInterfaceHydraulic1D.xml \
    TLMPluginInterfaceMechanical1D.xml \
    TLMPluginInterfaceRotational1D.xml


