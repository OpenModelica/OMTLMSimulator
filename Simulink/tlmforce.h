//! \file tlmforce.h
//!
//! The file provides declarations for the Simulink specific wrapper 
//! function and classes to interface Matlab/Simulink S-Function and TLMPlugin

#ifndef TLM_SIMULINK
#define TLM_SIMULINK

#ifdef __cplusplus
// This part is compiled in C++ mode only

#include <map>
#include <string>

// Forward declarations
class TLMPlugin;


//! TLM_InterfaceReg is a class for keeping track of TLM interfac id's
//! There is just one instance of the class.
class TLM_InterfaceReg {

    //! Singleton instance
    static TLM_InterfaceReg* instance_;  

    //! InterfaceIDmap maps the Simulink ID to TLM force ID
    std::map<std::string, int> InterfaceIDmap;
    
    //! Number of registered TLM interfaces
    int NumInterfaces;

    //! Plugin is the reference to TLM plugin instance
    TLMPlugin* Plugin;

    //! DebugOutFlg if true enables debug output
    bool DebugOutFlg;

    //! Meta-Model simulation start time.
    double timeStart;

    //! Meta-Model simulation end time.
    double timeEnd;

    //! Max time step for this component.
    //! Defined by minimum TLM delay of this component.
    double maxStep;

 public: 

    //! Constructor takes debugFlg as input which is used to set DebugOutFlg
    TLM_InterfaceReg(bool debugFlg);

    //! Destructor handles memory deallocation and graceful exit
    ~TLM_InterfaceReg();

    //! GetInstance creates this class instance if necessary and enables
    //! debug output when  debugFlg is true.
    static TLM_InterfaceReg* GetInstance(bool debugFlg=true);

    //! RegisterInterface registers an TLM interface with name ifID.
    //! Adds marker to internal map of ID's 
    void RegisterInterface(std::string ifID, int dimensions=6);

    //! Return the TLM interface ID for the specified interface.
    int GetInterfaceID(std::string ifID);

    //! Set DebugOutFlg to true
    void SetDebugOut();

    //! Returns the simulation time parameters as defined in the meta-model. 
    void GetSimParameters(double &startTimeOut, double &endTimeOut, double &maxTimeStepOut)
    {
        startTimeOut = timeStart;
        endTimeOut = timeEnd;
        maxTimeStepOut = maxStep;
    }

    //! Get the TLM-Plugin implementer.
    TLMPlugin* GetPlugin(){ return Plugin; }

};
#endif
#endif
