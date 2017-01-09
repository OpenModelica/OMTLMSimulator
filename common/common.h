#ifndef TLM_COMMON_H
#define TLM_COMMON_H

enum InterfaceType { Type3D, Type1D, TypeInput, TypeOutput };
enum InterfaceDomain { DomainMechanical, DomainElectric, DomainHydraulic, DomainPneumatic, DomainSignal };


inline std::string type2str(InterfaceType ifType) {
    std::string strType = "3D";
    if(ifType == Type1D) strType = "1D";
    else if(ifType == TypeInput) strType = "SignalInput";
    else if(ifType == TypeOutput) strType = "SignalOutput";
    return strType;
}

inline InterfaceType str2type(std::string strType) {
    InterfaceType ifType = Type3D;
    if(strType == "1D") ifType = Type1D;
    else if(strType == "SignalInput") ifType = TypeInput;
    else if(strType == "SignalOutput") ifType = TypeOutput;
    return ifType;
}

inline std::string domain2str(InterfaceDomain ifDomain) {
    std::string strDomain = "Mechanical";
    if(ifDomain == DomainElectric) strDomain = "Electric";
    else if(ifDomain == DomainHydraulic) strDomain = "Hydraulic";
    else if(ifDomain == DomainPneumatic) strDomain = "Pneumatic";
    else if(ifDomain == DomainSignal) strDomain = "Signal";
    return strDomain;
}

inline InterfaceDomain str2domain(std::string strDomain) {
    InterfaceDomain ifDomain = DomainMechanical;
    if(strDomain == "Electric") ifDomain = DomainElectric;
    else if(strDomain == "Hyraulic") ifDomain = DomainHydraulic;
    else if(strDomain == "Pneumatic") ifDomain = DomainPneumatic;
    else if(strDomain == "Signal") ifDomain = DomainSignal;
    return ifDomain;
}

#endif // TLM_COMMON_H
