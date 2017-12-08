/*
 * tlmforce.c: Based on 'C' template for a level 2 S-function.
 *
 *  -------------------------------------------------------------------------
 *  | See matlabroot/simulink/src/sfuntmpl_doc.c for a more detailed template |
 *  -------------------------------------------------------------------------
 *
 * Copyright 1990-2000 The MathWorks, Inc.
 * $Revision$
 */

#include "tlmforce.h"

#include "Plugin/TLMPlugin.h"
#include "Logging/TLMErrorLog.h"
#include "tostr.h"

#include <iostream>
#include <fstream>
#include <map>
#include <cmath>
#include <stdlib.h>

// The wrapper expect TLM parameters in this file.
static const char* TLM_CONFIG_FILE_NAME = "tlm.config";


TLM_InterfaceReg* TLM_InterfaceReg::instance_= 0;

TLM_InterfaceReg::TLM_InterfaceReg(bool debugFlg):
    // instance_ // static variable, used as singleton
    InterfaceIDmap(),
    NumInterfaces(0),
    Plugin(TLMPlugin::CreateInstance()),
    DebugOutFlg(debugFlg),
    timeStart(0.0),
    timeEnd(0.0),
    maxStep(0.0)
{
    if(instance_ != 0) {
        TLMErrorLog::FatalError( "Singleton pattern violated in Simulink TLM_InterfaceReg constructor" );
        exit(1);
    }

    if(debugFlg) SetDebugOut();

    instance_ = this;

    // Read parameters from a file

    std::ifstream tlmConfigFile(TLM_CONFIG_FILE_NAME);

    std::string model;
    std::string serverName;

    tlmConfigFile >> model;
    tlmConfigFile >> serverName;
    tlmConfigFile >> timeStart;
    tlmConfigFile >> timeEnd;
    tlmConfigFile >> maxStep;

    if( timeStart < 0.0 || timeEnd <= timeStart ){ //|| maxStep <= 0.0 ){
	TLMErrorLog::FatalError( "Bad data in Simulink TLM configuration file " 
                                 + std::string(TLM_CONFIG_FILE_NAME) 
                                 + ", please check your meta-model simulation time setup!" );
	exit(1);
    }

    if(!tlmConfigFile.good()) {
	TLMErrorLog::FatalError( "Error reading Simulink TLM configuration data from " 
                                 + std::string(TLM_CONFIG_FILE_NAME) );
	exit(1);
    }

    if( maxStep <= 0.0 ){
	TLMErrorLog::Warning( "Simulink TLM plugin, got negative or zero max time step:" 
                          + TLMErrorLog::ToStdStr(maxStep) + ". Set max time step to 1.0e-10" );
        TLMErrorLog::Warning( "There might be no connected TLM interfaces." );
        TLMErrorLog::Warning( "Note: This might happen and is OK during interface registration in meta-modeller." );

        maxStep = 1.0e-10;
    }

    TLMErrorLog::Info( "Try to initialize Simulink plugin." );

    if(! Plugin->Init( model,
		       timeStart,
		       timeEnd,
		       maxStep,
		       serverName)) {
	TLMErrorLog::FatalError( "Error initializing the Simulink TLM plugin" );
	exit(1);
    }

    TLMErrorLog::Info( "TLM Simulink plugin was initialized" );   
}

TLM_InterfaceReg::~TLM_InterfaceReg() {
}

void TLM_InterfaceReg::SetDebugOut(){
    TLMErrorLog::SetLogLevel(TLMLogLevel::Debug);
}

TLM_InterfaceReg* TLM_InterfaceReg::GetInstance(bool debugFlg)
{
    if(!instance_) {
	instance_ = new TLM_InterfaceReg( debugFlg);
    }
    return instance_;
}


void TLM_InterfaceReg::RegisterInterface(std::string ifID, int dimensions, std::string causality, std::string domain) {
    // No way to get the real marker name from the solver - using "M<ID>"
    TLMErrorLog::Info( "Trying to register interface " + ifID );

    if( InterfaceIDmap.count(ifID) > 0 ){
	TLMErrorLog::FatalError( "Try to register same interface twice " + ifID );
    }
  
    int interfaceID = Plugin->RegisteTLMInterface(ifID, dimensions, causality, domain);
    
    InterfaceIDmap[ifID] = interfaceID;
    NumInterfaces++;
}

int TLM_InterfaceReg::GetInterfaceID(std::string ifID)
{
    if( InterfaceIDmap.count(ifID) <= 0 ){
	TLMErrorLog::FatalError( "Request for non existend interface " + ifID);
        //return -1;
    }
    return InterfaceIDmap[ifID];
}



#ifdef __cplusplus
extern "C" {
#endif

/*
 * You must specify the S_FUNCTION_NAME as the name of your S-function
 * (i.e. replace sfuntmpl_basic with the name of your S-function).
 */

#define S_FUNCTION_NAME  tlmforce
#define S_FUNCTION_LEVEL 2

/*
 * Need to include simstruc.h for the definition of the SimStruct and
 * its associated macro definitions.
 */
#include "simstruc.h"



/* Error handling
 * --------------
 *
 * You should use the following technique to report errors encountered within
 * an S-function:
 *
 *       ssSetErrorStatus(S,"Error encountered due to ...");
 *       return;
 *
 * Note that the 2nd argument to ssSetErrorStatus must be persistent memory.
 * It cannot be a local variable. For example the following will cause
 * unpredictable errors:
 *
 *      mdlOutputs()
 *      {
 *         char msg[256];         {ILLEGAL: to fix use "static char msg[256];"}
 *         sprintf(msg,"Error due to %s", std::string);
 *         ssSetErrorStatus(S,msg);
 *         return;
 *      }
 *
 * See matlabroot/simulink/src/sfuntmpl_doc.c for more details.
 */

/*====================*
 * S-function methods *
 *====================*/

/* Function: mdlInitializeSizes ===============================================
 * Abstract:
 *    The sizes information is used by Simulink to determine the S-function
 *    block's characteristics (number of inputs, outputs, states, etc.).
 */
static void mdlInitializeSizes(SimStruct *S)
{
    /* See sfuntmpl_doc.c for more details on the macros below */

    ssSetNumSFcnParams(S, 0);  /* Number of expected parameters */
    if (ssGetNumSFcnParams(S) != ssGetSFcnParamsCount(S)) {
        /* Return if number of expected != number of actual parameters */
        return;
    }

    /* no internal states to be handeled by simulink */
    ssSetNumContStates(S, 0);
    ssSetNumDiscStates(S, 1); /* We define one discrete state to force a call to mdlUpdate(...) */

    /* input ports are: */
    /* position[3]      Interface position data */
    /* orientation[3x3] Interface rotation matrix */
    /* speed[3]         Interface translational velocity */
    /* ang_speed[3]     Interface angular velocity */
    if (!ssSetNumInputPorts(S, 4)) return;

    /* Set size of input arrays */
    ssSetInputPortWidth(S, 0, 3); 
    ssSetInputPortWidth(S, 1, 9); 
    //ssSetInputPortMatrixDimensions(S, 1, 3, 3); 
    ssSetInputPortWidth(S, 2, 3); 
    ssSetInputPortWidth(S, 3, 3); 

    /* Yes, we want contiguous memory allocation for the data arrays */
    ssSetInputPortRequiredContiguous(S, 0, true); /*direct input signal access*/
    ssSetInputPortRequiredContiguous(S, 1, true); /*direct input signal access*/
    ssSetInputPortRequiredContiguous(S, 2, true); /*direct input signal access*/
    ssSetInputPortRequiredContiguous(S, 3, true); /*direct input signal access*/

    /*
     * Set direct feedthrough flag (1=yes, 0=no).
     * A port has direct feedthrough if the input is used in either
     * the mdlOutputs or mdlGetTimeOfNextVarHit functions.
     * See matlabroot/simulink/src/sfuntmpl_directfeed.txt.
     */
    ssSetInputPortDirectFeedThrough(S, 0, 1);
    ssSetInputPortDirectFeedThrough(S, 1, 1);
    ssSetInputPortDirectFeedThrough(S, 2, 1);
    ssSetInputPortDirectFeedThrough(S, 3, 1);

    /* output ports are: */
    /* force[3]  Output force */
    /* moment[3] Output moment */
    if (!ssSetNumOutputPorts(S, 4)) return;
    ssSetOutputPortWidth(S, 0, 3);
    ssSetOutputPortWidth(S, 1, 3);
    ssSetOutputPortWidth(S, 2, 3);
    ssSetOutputPortWidth(S, 3, 9);

    /* Some default stuff ??? */
    ssSetNumSampleTimes(S, 2);
    ssSetNumRWork(S, 0);
    ssSetNumIWork(S, 0);
    ssSetNumPWork(S, 0);
    ssSetNumModes(S, 0);
    ssSetNumNonsampledZCs(S, 0);

    ssSetOptions(S, 0);
}



/* Function: mdlInitializeSampleTimes =========================================
 * Abstract:
 *    This function is used to specify the sample time(s) for your
 *    S-function. You must register the same number of sample times as
 *    specified in ssSetNumSampleTimes.
 */
static void mdlInitializeSampleTimes(SimStruct *S)
{
    double sTime, eTime, timeStep;
    TLM_InterfaceReg::GetInstance(false)->GetSimParameters(sTime, eTime, timeStep);
    // true or false in GetInstance(...) enables/disables debug output

    TLMErrorLog::Info("Set sample time to " + ToStr(timeStep));

    /* Set TLM delay here! */
    ssSetSampleTime(S, 0, CONTINUOUS_SAMPLE_TIME);
    ssSetOffsetTime(S, 0, 0.0);

    /* Set TLM delay here! */
    ssSetSampleTime(S, 1, timeStep);
    ssSetOffsetTime(S, 1, 0.0);
}



#define MDL_INITIALIZE_CONDITIONS   /* Change to #undef to remove function */
#if defined(MDL_INITIALIZE_CONDITIONS)
  /* Function: mdlInitializeConditions ========================================
   * Abstract:
   *    In this function, you should initialize the continuous and discrete
   *    states for your S-function block.  The initial states are placed
   *    in the state vector, ssGetContStates(S) or ssGetRealDiscStates(S).
   *    You can also perform any other initialization activities that your
   *    S-function may require. Note, this routine will be called at the
   *    start of simulation and if it is present in an enabled subsystem
   *    configured to reset states, it will be call when the enabled subsystem
   *    restarts execution to reset the states.
   */
  static void mdlInitializeConditions(SimStruct *S)
  {       
  }
#endif /* MDL_INITIALIZE_CONDITIONS */



#define MDL_START  /* Change to #undef to remove function */
#if defined(MDL_START) 
  /* Function: mdlStart =======================================================
   * Abstract:
   *    This function is called once at start of model execution. If you
   *    have states that should be initialized once, this is the place
   *    to do it.
   */
  static void mdlStart(SimStruct *S)
  {
      //char* name = ssGetModelName(S);
      const char* name = ssGetPath(S);      
      TLM_InterfaceReg::GetInstance()->RegisterInterface(name, 6, "Bidirectional", "Mechanical");
  }
#endif /*  MDL_START */



/* Function: mdlOutputs =======================================================
 * Abstract:
 *    In this function, you compute the outputs of your S-function
 *    block. Generally outputs are placed in the output vector, ssGetY(S).
 */
static void mdlOutputs(SimStruct *S, int_T tid)
{
    /* inputs: */
    /* position[3]      Interface position data */
    /* orientation[3x3] Interface rotation matrix */
    /* speed[3]         Interface translational velocity */
    /* ang_speed[3]     Interface angular velocity */
    /*
      InputRealPtrsType R = ssGetInputPortRealSignalPtrs(S,0);
      InputRealPtrsType A = ssGetInputPortRealSignalPtrs(S,1);
      InputRealPtrsType vR = ssGetInputPortRealSignalPtrs(S,2);
      InputRealPtrsType Omega = ssGetInputPortRealSignalPtrs(S,3);
    */
    double *R = (double*)ssGetInputPortSignal(S,0);
    double *A = (double*)ssGetInputPortSignal(S,1);
    double *vR = (double*)ssGetInputPortSignal(S,2);
    double *Omega = (double*)ssGetInputPortSignal(S,3);

    const char* name = ssGetPath(S);          
    real_T time = ssGetT(S);

    int ifID = TLM_InterfaceReg::GetInstance()->GetInterfaceID(name);

    /* output */
    double force[6];
    TLMTimeData3D CurTimeData;

    if( ifID >= 0 ) {
        TLM_InterfaceReg::GetInstance()->GetPlugin()->GetForce3D(ifID,
                                                               time,
                                                               R,
                                                               A,
                                                               vR,
                                                               Omega,
                                                               force);
        
        
        TLMErrorLog::Info("Got force for: " + std::string(name) );
        TLMErrorLog::Info("time: " + ToStr(time) );
        TLMErrorLog::Info("F: " + ToStr(force[0]) + " " +  ToStr(force[1]) + " " + ToStr(force[2]) );
        TLMErrorLog::Info("M: " + ToStr(force[3]) + " " +  ToStr(force[4]) + " " + ToStr(force[5]) );
        
#ifdef DEBUGFLG
        if( isnan(force[0]) || isnan(force[1]) || isnan(force[2]) ){
            TLMErrorLog::FatalError("Got not-a-number in reaction force.");
            abort();
        }
#endif
        
        /* Get Position and Orientation */
        TLM_InterfaceReg::GetInstance()->GetPlugin()->GetTimeData3D(ifID, time, CurTimeData);        

        TLMErrorLog::Info("Got position for: " + std::string(name) );
        TLMErrorLog::Info("R: " + ToStr(CurTimeData.Position[0]) + " " +  ToStr(CurTimeData.Position[1]) + " " + ToStr(CurTimeData.Position[2]) );
        
    }
    else {
        /* Not connected */
        for( int i=0 ; i<6 ; i++ ) {
            force[i] = 0.0;        
        }

        /* CurTimeData is initialized in constructor */
    }


    /* ------- store the result ------- */

    /* Force & Moment */
    real_T  *f = ssGetOutputPortRealSignal(S,0);
    real_T  *m = ssGetOutputPortRealSignal(S,1);
    
    for( int i=0 ; i<3 ; i++ ){
        f[i] = force[i];
        m[i] = force[i+3];
    }

    /* Position & Orientation */
    real_T  *R_TLM = ssGetOutputPortRealSignal(S,2);
    real_T  *A_TLM = ssGetOutputPortRealSignal(S,3);
    
    for( int i=0 ; i<3 ; i++ ){
        R_TLM[i] = CurTimeData.Position[i];
        A_TLM[i] = CurTimeData.RotMatrix[i];
        A_TLM[i+3] = CurTimeData.RotMatrix[i+3];
        A_TLM[i+6] = CurTimeData.RotMatrix[i+6];
    }

}



#define MDL_UPDATE  /* Change to #undef to remove function */
#if defined(MDL_UPDATE)
  /* Function: mdlUpdate ======================================================
   * Abstract:
   *    This function is called once for every major integration time step.
   *    Discrete states are typically updated here, but this function is useful
   *    for performing any tasks that should only take place once per
   *    integration step.
   */
  static void mdlUpdate(SimStruct *S, int_T tid)
  {
      if( ssIsSampleHit(S, 1, tid) ){
          /*
            InputRealPtrsType R = ssGetInputPortRealSignalPtrs(S,0);
            InputRealPtrsType A = ssGetInputPortRealSignalPtrs(S,1);
            InputRealPtrsType vR = ssGetInputPortRealSignalPtrs(S,2);
            InputRealPtrsType Omega = ssGetInputPortRealSignalPtrs(S,3);
          */
          
          double *R = (double*)ssGetInputPortSignal(S,0);
          double *A = (double*)ssGetInputPortSignal(S,1);
          double *vR = (double*)ssGetInputPortSignal(S,2);
          double *Omega = (double*)ssGetInputPortSignal(S,3);
          
          const char* name = ssGetPath(S);          
          real_T time = ssGetT(S);
          
          int ifID = TLM_InterfaceReg::GetInstance()->GetInterfaceID(name);
          
          if( ifID >= 0 ){          
              TLMErrorLog::Info("Call SetMotion for: " + std::string(name) );
              TLMErrorLog::Info("time: " + ToStr(time) );
              TLMErrorLog::Info("R: " + ToStr(R[0]) + " " +  ToStr(R[1]) + " " + ToStr(R[2]) );
              //TLMErrorLog::Info("A0: " + ToStr(A[0]) + " " +  ToStr(A[1]) + " " + ToStr(A[2]) );
              //TLMErrorLog::Info("A1: " + ToStr(A[3]) + " " +  ToStr(A[4]) + " " + ToStr(A[5]) );
              //TLMErrorLog::Info("A2: " + ToStr(A[6]) + " " +  ToStr(A[7]) + " " + ToStr(A[8]) );
              TLMErrorLog::Info("vR: " + ToStr(vR[0]) + " " +  ToStr(vR[1]) + " " + ToStr(vR[2]) );
              //TLMErrorLog::Info("Omega: " + ToStr(Omega[0]) + " " +  ToStr(Omega[1]) + " " + ToStr(Omega[2]) );
              
              TLM_InterfaceReg::GetInstance()->GetPlugin()->SetMotion3D(ifID,          // Send data to the Plugin
                                                                      time,
                                                                      R,
                                                                      A,
                                                                      vR,
                                                                      Omega);
          }
      }
  }
#endif /* MDL_UPDATE */



#define MDL_DERIVATIVES  /* Change to #undef to remove function */
#if defined(MDL_DERIVATIVES)
  /* Function: mdlDerivatives =================================================
   * Abstract:
   *    In this function, you compute the S-function block's derivatives.
   *    The derivatives are placed in the derivative vector, ssGetdX(S).
   */
  static void mdlDerivatives(SimStruct *S)
  {
  }
#endif /* MDL_DERIVATIVES */



/* Function: mdlTerminate =====================================================
 * Abstract:
 *    In this function, you should perform any actions that are necessary
 *    at the termination of a simulation.  For example, if memory was
 *    allocated in mdlStart, this is the place to free it.
 */
static void mdlTerminate(SimStruct *S)
{
}


/*======================================================*
 * See sfuntmpl_doc.c for the optional S-function methods *
 *======================================================*/

/*=============================*
 * Required S-function trailer *
 *=============================*/

#ifdef  MATLAB_MEX_FILE    /* Is this file being compiled as a MEX-file? */
#include "simulink.c"      /* MEX-file interface mechanism */
#else
#include "cg_sfun.h"       /* Code generation registration function */
#endif

#ifdef __cplusplus
} // extern "C"
#endif
