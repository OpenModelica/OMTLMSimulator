#ifndef TLM_COMMON_H
#define TLM_COMMON_H

enum InterfaceDimensionality { Dimensionality3D, Dimensionality1D, DimensionalitySignal};
enum InterfaceDomain { DomainMechanical, DomainElectric, DomainHydraulic, DomainPneumatic, DomainUnspecified };
enum InterfaceCausality { CausalityAcausal, CausalityInput, CausalityOutput };

inline std::string dimensionality2str(InterfaceDimensionality ifDim) {
    std::string strDim = "3D";
    if(ifDim == Dimensionality1D) strDim = "1D";
    else if(ifDim == DimensionalitySignal) strDim = "Signal";
    return strDim;
}

inline InterfaceDimensionality str2dimensionality(std::string strDim) {
    InterfaceDimensionality ifDim = Dimensionality3D;
    if(strDim == "1D") ifDim = Dimensionality1D;
    else if(strDim == "Signal") ifDim = DimensionalitySignal;
    return ifDim;
}

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


inline std::string causality2str(InterfaceCausality ifCausality) {
    std::string strCaulsality = "Acaulsal";
    if(ifCausality == CausalityInput) strCaulsality = "Input";
    else if(ifCausality == CausalityOutput) strCaulsality = "Output";
    return strCaulsality;
}

inline InterfaceCausality str2causality(std::string strCausality) {
    InterfaceCausality ifCausality = CausalityAcausal;
    if(strCausality == "Input") ifCausality = CausalityInput;
    else if(strCausality == "Output") ifCausality = CausalityOutput;
    return ifCausality;
}

#endif // TLM_COMMON_H
