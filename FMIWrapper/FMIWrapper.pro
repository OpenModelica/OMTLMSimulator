TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

DEFINES += INTERFACE_TYPES

TARGET = ../../bin/FMIWrapper

unix {
LIBS += -ldl
}
win32 {
LIBS += -lws2_32
}

LIBS += -L$${PWD}/FMILibrary-2.0.1/install/lib
LIBS += -llibfmilib_shared

#FMILibrary include path
INCLUDEPATH += FMILibrary-2.0.1/install/include

#CVODE include path
INCLUDEPATH += cvode-2.9.0/include

#IDA include path
INCLUDEPATH += ida-2.9.0/include

#DEFINES += SUNDIALS_EXPORT=""

#TLMPLugin include paths
INCLUDEPATH += ../common
INCLUDEPATH += ../extralibs/misc/include

SOURCES += main.cpp \
    ../common/PluginImplementer.cc \
    ../common/TLMClientComm.cc \
    ../common/TLMCommUtil.cc \
    ../common/TLMErrorLog.cc \
    ../common/TLMInterface.cc \
    ../common/TLMPlugin.cc \
    ../extralibs/misc/src/strConv.cc \
    ../extralibs/misc/src/coordTrans.cc \
    ../extralibs/misc/src/double3Vec.cc \
    ../extralibs/misc/src/double33Mat.cc \
    cvode-2.9.0/src/nvec_ser/nvector_serial.c \
    cvode-2.9.0/src/sundials/sundials_math.c \
    cvode-2.9.0/src/cvode/cvode.c \
    cvode-2.9.0/src/sundials/sundials_nvector.c \
    cvode-2.9.0/src/cvode/cvode_dense.c \
    cvode-2.9.0/src/cvode/cvode_direct.c \
    cvode-2.9.0/src/sundials/sundials_direct.c \
    cvode-2.9.0/src/sundials/sundials_dense.c \
    cvode-2.9.0/src/cvode/cvode_io.c \
    ida-2.9.0/src/ida/ida.c \
    ida-2.9.0/src/ida/ida_dense.c \
    ida-2.9.0/src/ida/ida_direct.c \
    ida-2.9.0/src/ida/ida_io.c \
    ../common/TLMInterface1D.cc \
    ../common/TLMInterface3D.cc \
    ../common/TLMInterfaceSignal.cc \
    ../common/TLMInterfaceSignalInput.cc \
    ../common/TLMInterfaceSignalOutput.cc \
    ../common/Parameters/ComponentParameter.cc

HEADERS += \
    FMILibrary-2.0.1/install/include/JM/jm_portability.h \
    FMILibrary-2.0.1/install/include/FMI/fmi_import_context.h \
    FMILibrary-2.0.1/install/include/FMI1/fmi1_import.h \
    FMILibrary-2.0.1/install/include/FMI2/fmi2_import.h \
    FMILibrary-2.0.1/install/include/fmilib_config.h

