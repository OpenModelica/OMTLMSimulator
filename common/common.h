#ifndef TLM_COMMON_H
#define TLM_COMMON_H

enum InterfaceDomain { DomainMechanical, DomainElectric, DomainHydraulic, DomainPneumatic, DomainUnspecified };

inline std::string domain2str(InterfaceDomain ifDomain) {
    std::string strDomain = "Mechanical";
    if(ifDomain == DomainElectric) strDomain = "Electric";
    else if(ifDomain == DomainHydraulic) strDomain = "Hydraulic";
    else if(ifDomain == DomainPneumatic) strDomain = "Pneumatic";
    else if(ifDomain == DomainUnspecified) strDomain = "Unspecified";
    return strDomain;
}

inline InterfaceDomain str2domain(std::string strDomain) {
    InterfaceDomain ifDomain = DomainMechanical;
    if(strDomain == "Electric") ifDomain = DomainElectric;
    else if(strDomain == "Hyraulic") ifDomain = DomainHydraulic;
    else if(strDomain == "Pneumatic") ifDomain = DomainPneumatic;
    else if(strDomain == "Unspecified") ifDomain = DomainUnspecified;
    return ifDomain;
}


#endif // TLM_COMMON_H
