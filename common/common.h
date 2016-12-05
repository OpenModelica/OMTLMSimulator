#ifndef TLM_COMMON_H
#define TLM_COMMON_H

enum InterfaceType { Interface3D, Interface1D, InterfaceSignalInput, InterfaceSignalOutput };


inline std::string type2str(InterfaceType ifType) {
    std::string strType = "3D";
    if(ifType == Interface1D) strType = "1D";
    else if(ifType == InterfaceSignalInput) strType = "SignalInput";
    else if(ifType == InterfaceSignalOutput) strType = "SignalOutput";
    return strType;
}

inline InterfaceType str2type(std::string strType) {
    InterfaceType ifType = Interface3D;
    if(strType == "1D") ifType = Interface1D;
    else if(strType == "SignalInput") ifType = InterfaceSignalInput;
    else if(strType == "SignalOutput") ifType = InterfaceSignalOutput;
    return ifType;
}

#endif // TLM_COMMON_H
