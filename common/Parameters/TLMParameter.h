//!
//! \file TLMInterface.h
//!
//! Provides a definition for the TLMParameter class
//!
//!
//! \author Robert Braun
//!

#ifndef TLMPARAMETER_H
#define TLMPARAMETER_H


#include <queue>
#include <vector>
#include <string>
#include "Communication/TLMCommUtil.h"
#include "Communication/TLMClientComm.h"
#include "common.h"


//!
//! TLMParameter provides the client side functionality for a single sub-model parameter
//! (not to be confused with TLM connection parameters)
//!
class TLMParameter {

 public:

    //! TLMParameter constructor. Sends a registration message to the TLM manager
    //! and prepares the object for simulation.
    TLMParameter(TLMClientComm& theComm, std::string& aName, std::string& aDefaultValue);

    //! Get name of the parameter
    const std::string& GetName() const { return Name; }

    //! Get parameter ID of this parameter
    int GetParameterID() const { return  ParameterID; }

    //! Get the current value of this parameter
    std::string GetValue() const { return Value; }

 protected:
    //! Name of this TLM parameter
    std::string Name;

    //! Value of this parameter
    std::string Value;

    //! TLM ID of this interface as received from the TLM manager
    int ParameterID;

    //! The communication object of the client
    TLMClientComm& Comm;

    //! Message buffer used to transfer information between different methods
    TLMMessage Message;
};

#endif // TLMPARAMETER_H
