// C++ includes
#include <iostream>
#include <string>
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <vector>
#include <fstream>

#define Ith(v,i)    NV_Ith_S(v,i-1)       /* Ith numbers components 1..NEQ */
#define IJth(A,i,j) DENSE_ELEM(A,i-1,j-1) /* IJth numbers rows,cols 1..NEQ */

#include "cvode/cvode.h"             /* prototypes for CVODE fcts., consts. */
#include "nvector/nvector_serial.h"  /* serial N_Vector types, fcts., macros */
#include "cvode/cvode_dense.h"       /* prototype for CVDense */
#include "sundials/sundials_dense.h" /* definitions DlsMat DENSE_ELEM */
#include "sundials/sundials_types.h" /* definition of type realtype */

#include "ida/ida.h"
#include "ida/ida_dense.h"
#include "nvector/nvector_serial.h"
#include "sundials/sundials_math.h"
#include "sundials/sundials_types.h"

// FMILibrary includes
#include "FMI/fmi_import_context.h"
#include "FMI1/fmi1_import.h"
#include "FMI2/fmi2_import.h"
#include "JM/jm_portability.h"

// TLMPlugin includes
#include "TLMPlugin.h"
#include "TLMErrorLog.h"

using namespace std;

// FMI config data
struct fmiConfig_t {
  size_t nSubSteps;
  size_t nInterfaces;
  std::vector<TLMPlugin*> plugins;
  std::vector<std::string> interfaceNames;
  std::vector<int> interfaceIds;
  std::vector<fmi2_value_reference_t*> position_vr;
  std::vector<fmi2_value_reference_t*> orientation_vr;
  std::vector<fmi2_value_reference_t*> speed_vr;
  std::vector<fmi2_value_reference_t*> ang_speed_vr;
  std::vector<fmi2_value_reference_t*> force_vr;
};

// TLM config data
struct tlmConfig_t {
  std::string model;
  std::string server;
  double tstart;
  double tend;
  double hmax;
};

enum solver_t { ExplicitEuler, CVODE, IDA };

struct simConfig_t {
  solver_t solver;
  double reltol;
  std::vector<double> abstol;
};

static const char* TEMP_DIR_NAME = "temp";
static const char* TLM_CONFIG_FILE_NAME = "tlm.config";
static const char* FMI_CONFIG_FILE_NAME = "fmi.config";

static size_t n_states = 0;
static fmi2_status_t fmistatus = fmi2_status_t();
static fmi2_real_t* states = 0;
static fmi2_real_t* states_nominal = 0;
static fmi2_real_t* states_abstol = 0;
static fmi2_real_t* states_der = 0;
static fmi2_import_t* fmu = 0;

static fmiConfig_t fmiConfig = fmiConfig_t();
static tlmConfig_t tlmConfig = tlmConfig_t();
static simConfig_t simConfig = simConfig_t();

//Read force from TLMPlugin and write it to FMU
void forceFromTlmToFmu(double tcur)
{
  //Write interpolated force to FMU
  for(size_t j=0; j<fmiConfig.plugins.size(); ++j) {
    double position[3],orientation[9],speed[3],ang_speed[3],force[6];

    //Read position and speed from FMU
    fmistatus = fmi2_import_get_real(fmu,fmiConfig.position_vr[j],3,position);
    fmistatus = fmi2_import_get_real(fmu,fmiConfig.orientation_vr[j],9,orientation);
    fmistatus = fmi2_import_get_real(fmu,fmiConfig.speed_vr[j],3,speed);
    fmistatus = fmi2_import_get_real(fmu,fmiConfig.ang_speed_vr[j],3,ang_speed);

    //Get interpolated force
    fmiConfig.plugins.at(j)->GetForce(fmiConfig.interfaceIds[j], tcur, position,orientation,speed,ang_speed,force);

    for(size_t k=0; k<6; ++k) {
      force[k] = -force[k];
    }

    // Write force to FMU
    fmistatus = fmi2_import_set_real(fmu,fmiConfig.force_vr[j],6,force);
  }
}


static int rhs(realtype t, N_Vector y, N_Vector ydot, void *user_data)
{
  // Update states in FMU
  for(size_t i=0; i<n_states; ++i) {
    states[i] = Ith(y,i+1);
  }
  fmistatus = fmi2_import_set_continuous_states(fmu, states, n_states);

  // Write interpolated force to FMU
  forceFromTlmToFmu(t);

  // Read derivatives
  fmistatus = fmi2_import_get_derivatives(fmu, states_der, n_states);

  for(size_t i=0; i<n_states; ++i) {
    Ith(ydot,i+1) = states_der[i];
  }

  return(0);
}



static int rhs_ida(realtype t, N_Vector yy, N_Vector yp, N_Vector rr, void *user_data)
{
  // Update states in FMU
  for(size_t i=0; i<n_states; ++i) {
    states[i] = Ith(yy,i+1);
  }
  fmistatus = fmi2_import_set_continuous_states(fmu, states, n_states);

  // Write interpolated force to FMU
  forceFromTlmToFmu(t);

  // Read derivatives
  fmistatus = fmi2_import_get_derivatives(fmu, states_der, n_states);

  for(size_t i=0; i<n_states; ++i) {
    Ith(rr,i+1) = Ith(yp,i+1)-states_der[i];
  }

  return(0);
}

/*
 * Jacobian routine. Compute J(t,y) = df/dy. *
 */

static int Jac(long int N, realtype t,
               N_Vector y, N_Vector fy, DlsMat J, void *user_data,
               N_Vector tmp1, N_Vector tmp2, N_Vector tmp3)
{
  realtype y1, y2, y3;

  y1 = Ith(y,1); y2 = Ith(y,2); y3 = Ith(y,3);

  IJth(J,1,1) = RCONST(-0.04);
  IJth(J,1,2) = RCONST(1.0e4)*y3;
  IJth(J,1,3) = RCONST(1.0e4)*y2;
  IJth(J,2,1) = RCONST(0.04);
  IJth(J,2,2) = RCONST(-1.0e4)*y3-RCONST(6.0e7)*y2;
  IJth(J,2,3) = RCONST(-1.0e4)*y2;
  IJth(J,3,2) = RCONST(6.0e7)*y2;

  return(0);
}

static int check_flag(void *flagvalue, const char *funcname, int opt)
{
  int *errflag;

  /* Check if SUNDIALS function returned NULL pointer - no memory allocated */
  if (opt == 0 && flagvalue == NULL) {
    fprintf(stderr, "\nSUNDIALS_ERROR: %s() failed - returned NULL pointer\n\n",
            funcname);
    return(1); }

  /* Check if flag < 0 */
  else if (opt == 1) {
    errflag = (int *) flagvalue;
    if (*errflag < 0) {
      fprintf(stderr, "\nSUNDIALS_ERROR: %s() failed with flag = %d\n\n",
              funcname, *errflag);
      return(1); }}

  /* Check if function returned NULL pointer - no memory allocated */
  else if (opt == 2 && flagvalue == NULL) {
    fprintf(stderr, "\nMEMORY_ERROR: %s() failed - returned NULL pointer\n\n",
            funcname);
    return(1); }

  return(0);
}


// FMILibrary logger
void fmiLogger(jm_callbacks* c, jm_string module, jm_log_level_enu_t log_level, jm_string message)
{
  std::stringstream ss;
  ss << "FMI: module = " << module << ", log level = " << jm_log_level_to_string(log_level) << ": " << message;
  switch(log_level)
  {
    case jm_log_level_fatal:
      TLMErrorLog::FatalError(ss.str());
      break;
    case jm_log_level_error:
      TLMErrorLog::Warning(ss.str());     //Non-fatal errors is no suppored by TLMErrorLog, so we use warnings
      break;
    case jm_log_level_warning:
      TLMErrorLog::Warning(ss.str());
      break;
    default:
      TLMErrorLog::Log(ss.str());
      break;
  }
}


// Simulate function
int simulate_fmi2_cs()
{
  fmi2_status_t fmistatus;
  jm_status_enu_t jmstatus;

  fmi2_string_t instanceName = "TLM-FMI CS model instance";
  fmi2_string_t fmuGUID;
  fmi2_string_t fmuLocation = "";
  fmi2_boolean_t visible = fmi2_false;
  fmi2_real_t relativeTol = 1e-4;

  fmi2_real_t tcur = tlmConfig.tstart;
  fmi2_boolean_t StopTimeDefined = fmi2_false;

  fmi2_callback_functions_t callBackFunctions;
  callBackFunctions.logger = fmi2_log_forwarding;
  callBackFunctions.allocateMemory = calloc;
  callBackFunctions.freeMemory = free;
  callBackFunctions.componentEnvironment = fmu;

  //Load the FMU shared library
  jmstatus = fmi2_import_create_dllfmu(fmu, fmi2_fmu_kind_cs, &callBackFunctions);
  if (jmstatus == jm_status_error) {
    TLMErrorLog::FatalError("Could not create the DLL loading mechanism(C-API). Error: "+string(fmi2_import_get_last_error(fmu)));
  }

  TLMErrorLog::Log("Version returned from FMU: "+string(fmi2_import_get_version(fmu)));
  TLMErrorLog::Log("Platform type returned: "+string(fmi2_import_get_types_platform(fmu)));

  fmuGUID = fmi2_import_get_GUID(fmu);
  TLMErrorLog::Log("GUID: "+string(fmuGUID));


  jmstatus = fmi2_import_instantiate(fmu, instanceName, fmi2_cosimulation, fmuLocation, visible);
  if (jmstatus == jm_status_error) {
    TLMErrorLog::FatalError("fmi2_import_instantiate failed");
  }

  fmistatus = fmi2_import_setup_experiment(fmu, fmi2_true,
                                           relativeTol, tlmConfig.tstart, StopTimeDefined, tlmConfig.tend);
  if(fmistatus != fmi2_status_ok) {
    TLMErrorLog::FatalError("fmi2_import_setup_experiment failed");
  }

  fmistatus = fmi2_import_enter_initialization_mode(fmu);
  if(fmistatus != fmi2_status_ok) {
    TLMErrorLog::FatalError("fmi2_import_enter_initialization_mode failed");
  }

  fmistatus = fmi2_import_exit_initialization_mode(fmu);
  if(fmistatus != fmi2_status_ok) {
    TLMErrorLog::FatalError("fmi2_import_exit_initialization_mode failed");
  }

  tcur = tlmConfig.tstart;

  while (tcur < tlmConfig.tend) {
    fmi2_real_t hsub = tlmConfig.hmax/fmiConfig.nSubSteps;
    for(size_t i=0; i<fmiConfig.nSubSteps; ++i) {
      for(size_t j=0; j<fmiConfig.plugins.size(); ++j)
      {
        double position[3],orientation[9],speed[3],ang_speed[3],force[6];

        //Read position and speed from FMU
        fmistatus = fmi2_import_get_real(fmu,fmiConfig.position_vr[j],3,position);
        fmistatus = fmi2_import_get_real(fmu,fmiConfig.orientation_vr[j],9,orientation);
        fmistatus = fmi2_import_get_real(fmu,fmiConfig.speed_vr[j],3,speed);
        fmistatus = fmi2_import_get_real(fmu,fmiConfig.ang_speed_vr[j],3,ang_speed);

        //Get interpolated force
        fmiConfig.plugins.at(j)->GetForce(fmiConfig.interfaceIds[j], tcur, position,orientation,speed,ang_speed,force);

        for(size_t k=0; k<6; ++k) {
          force[k] = -force[k];
        }

        //Write force to FMU
        fmistatus = fmi2_import_set_real(fmu,fmiConfig.force_vr[j],6,force);
      }

      //Take one sub step
      fmistatus = fmi2_import_do_step(fmu,tcur,hsub,fmi2_true);

      //Increment time
      tcur+=hsub;

      for(size_t j=0; j<fmiConfig.plugins.size(); ++j) {

        double force[6], position[3],orientation[9],speed[3],ang_speed[3];

        //Read position and speed from FMU
        fmistatus = fmi2_import_get_real(fmu,fmiConfig.position_vr[j],3,position);
        fmistatus = fmi2_import_get_real(fmu,fmiConfig.orientation_vr[j],9,orientation);
        fmistatus = fmi2_import_get_real(fmu,fmiConfig.speed_vr[j],3,speed);
        fmistatus = fmi2_import_get_real(fmu,fmiConfig.ang_speed_vr[j],3,ang_speed);

        //Get interpolated force
        fmiConfig.plugins.at(j)->GetForce(fmiConfig.interfaceIds[j], tcur, position,orientation,speed,ang_speed,force);

        //Write back motion for sub step
        fmiConfig.plugins.at(j)->SetMotion(fmiConfig.interfaceIds[j], tcur, position, orientation, speed, ang_speed);
      }
    }
  }

  TLMErrorLog::Log("Simulation finished.");

  fmistatus = fmi2_import_terminate(fmu);

  fmi2_import_free_instance(fmu);

  return 0;
}



// Event iteration auxiliary function
void do_event_iteration(fmi2_import_t *fmu, fmi2_event_info_t *eventInfo)
{
  eventInfo->newDiscreteStatesNeeded = fmi2_true;
  eventInfo->terminateSimulation     = fmi2_false;
  while (eventInfo->newDiscreteStatesNeeded && !eventInfo->terminateSimulation) {
    fmi2_import_new_discrete_states(fmu, eventInfo);
  }
}


//Read motion from FMU and write it to TLMPlugin
void motionFromFmuToTlm(double tcur)
{
  for(size_t j=0; j<fmiConfig.plugins.size(); ++j) {
    double position[3],orientation[9],speed[3],ang_speed[3];

    fmistatus = fmi2_import_get_real(fmu,fmiConfig.position_vr[j],3,position);
    fmistatus = fmi2_import_get_real(fmu,fmiConfig.orientation_vr[j],9,orientation);
    fmistatus = fmi2_import_get_real(fmu,fmiConfig.speed_vr[j],3,speed);
    fmistatus = fmi2_import_get_real(fmu,fmiConfig.ang_speed_vr[j],3,ang_speed);

    fmiConfig.plugins.at(j)->SetMotion(fmiConfig.interfaceIds[j], tcur, position, orientation, speed, ang_speed);
  }
}





// Simulate function for model exchange
int simulate_fmi2_me()
{
  TLMErrorLog::Log("Starting simulation using FMI for Model Exchange.");
  switch (simConfig.solver) {
    case CVODE:
      TLMErrorLog::Log("Using CVODE solver.");
      break;
    case IDA:
      TLMErrorLog::Log("Using IDA solver.");
      break;
    case ExplicitEuler:
      TLMErrorLog::Log("Using explicit Euler solver.");
      break;
  }

  jm_status_enu_t jmstatus;
  fmi2_real_t tstart = 0.0;
  fmi2_real_t tcur = tlmConfig.tstart;
  fmi2_real_t hcur;
  fmi2_real_t hdef = tlmConfig.hmax;
  fmi2_real_t tend = tlmConfig.tend;
  size_t n_event_indicators;
  fmi2_real_t* event_indicators;
  fmi2_real_t* event_indicators_prev;
  fmi2_boolean_t callEventUpdate;
  fmi2_boolean_t terminateSimulation = fmi2_false;
  fmi2_boolean_t toleranceControlled = fmi2_true;
  fmi2_real_t relativeTolerance = 0.0001;
  fmi2_event_info_t eventInfo;
  fmi2_string_t instanceName = "TLM-FMI ME model instance";
  fmi2_string_t fmuGUID;
  fmi2_string_t fmuLocation = "";
  fmi2_boolean_t visible = fmi2_false;
  fmi2_boolean_t StopTimeDefined = fmi2_false;

  fmi2_callback_functions_t callBackFunctions;
  callBackFunctions.logger = fmi2_log_forwarding;
  callBackFunctions.allocateMemory = calloc;
  callBackFunctions.freeMemory = free;
  callBackFunctions.componentEnvironment = fmu;

  //Load the FMU shared library
  jmstatus = fmi2_import_create_dllfmu(fmu, fmi2_fmu_kind_me, &callBackFunctions);
  if (jmstatus == jm_status_error) {
    TLMErrorLog::FatalError("Could not create the DLL loading mechanism(C-API). Error: "+string(fmi2_import_get_last_error(fmu)));
  }

  TLMErrorLog::Log("Version returned from FMU: "+string(fmi2_import_get_version(fmu)));
  TLMErrorLog::Log("Platform type returned: "+string(fmi2_import_get_types_platform(fmu)));

  fmuGUID = fmi2_import_get_GUID(fmu);
  TLMErrorLog::Log("GUID: "+string(fmuGUID));

  n_states = fmi2_import_get_number_of_continuous_states(fmu);
  n_event_indicators = fmi2_import_get_number_of_event_indicators(fmu);

  states = (fmi2_real_t*)calloc(n_states, sizeof(double));
  states_nominal = (fmi2_real_t*)calloc(n_states, sizeof(double));
  states_abstol = (fmi2_real_t*)calloc(n_states, sizeof(double));
  states_der = (fmi2_real_t*)calloc(n_states, sizeof(double));
  event_indicators = (fmi2_real_t*)calloc(n_event_indicators, sizeof(double));
  event_indicators_prev = (fmi2_real_t*)calloc(n_event_indicators, sizeof(double));

  jmstatus = fmi2_import_instantiate(fmu, instanceName, fmi2_model_exchange, fmuLocation, visible);
  if (jmstatus == jm_status_error) {
    TLMErrorLog::FatalError("fmi2_import_instantiate failed");
  }

  fmi2_import_set_debug_logging(fmu, fmi2_false, 0, 0);
  std::stringstream ss;
  ss << "fmi2_import_set_debug_logging: " << fmi2_status_to_string(fmistatus);
  TLMErrorLog::Log(ss.str());
  fmi2_import_set_debug_logging(fmu, fmi2_true, 0, 0);

  relativeTolerance = fmi2_import_get_default_experiment_tolerance(fmu);

  fmistatus = fmi2_import_setup_experiment(fmu, toleranceControlled,
                                           relativeTolerance, tlmConfig.tstart, StopTimeDefined, tlmConfig.tend);
  if(fmistatus != fmi2_status_ok) {
    TLMErrorLog::FatalError("fmi2_import_setup_experiment failed");
  }

  fmistatus = fmi2_import_enter_initialization_mode(fmu);
  if(fmistatus != fmi2_status_ok) {
    TLMErrorLog::FatalError("fmi2_import_enter_initialization_mode failed");
  }

  fmistatus = fmi2_import_exit_initialization_mode(fmu);
  if(fmistatus != fmi2_status_ok) {
    TLMErrorLog::FatalError("fmi2_import_exit_initialization_mode failed");
  }

  tcur = tstart;
  hcur = hdef;
  callEventUpdate = fmi2_false;

  eventInfo.newDiscreteStatesNeeded           = fmi2_false;
  eventInfo.terminateSimulation               = fmi2_false;
  eventInfo.nominalsOfContinuousStatesChanged = fmi2_false;
  eventInfo.valuesOfContinuousStatesChanged   = fmi2_true;
  eventInfo.nextEventTimeDefined              = fmi2_false;
  eventInfo.nextEventTime                     = -0.0;

  /* fmiExitInitializationMode leaves FMU in event mode */
  do_event_iteration(fmu, &eventInfo);
  fmi2_import_enter_continuous_time_mode(fmu);

  fmistatus = fmi2_import_get_continuous_states(fmu, states, n_states);
  fmistatus = fmi2_import_get_derivatives(fmu,states_der, n_states);
  fmistatus = fmi2_import_get_nominals_of_continuous_states(fmu,states_nominal, n_states);
  fmistatus = fmi2_import_get_event_indicators(fmu, event_indicators, n_event_indicators);

  realtype reltol = relativeTolerance;
  for(size_t i=0; i<n_states; ++i) {
    states_abstol[i] = 0.01*reltol*states_nominal[i];
  }
  N_Vector y, yp, abstol;
  void *mem;
  int flag;
  if(simConfig.solver == CVODE) {
    y = abstol = NULL;
    mem = NULL;

    /* Create serial vector of length NEQ for I.C. and abstol */
    y = N_VNew_Serial(n_states);
    if (check_flag((void *)y, "N_VNew_Serial", 0)) return(1);
    abstol = N_VNew_Serial(n_states);
    if (check_flag((void *)abstol, "N_VNew_Serial", 0)) return(1);

    /* Initialize y */
    for(size_t i=0; i<n_states; ++i) {
      Ith(y,i+1) = states[i];
    }

    /* Set the vector absolute tolerance */
    for(size_t i=0; i<n_states; ++i) {
      Ith(abstol,i+1) = states_abstol[i];
    }

    /* Call CVodeCreate to create the solver memory and specify the
   * Backward Differentiation Formula and the use of a Newton iteration */
    mem = CVodeCreate(CV_BDF, CV_NEWTON);
    if (check_flag((void *)mem, "CVodeCreate", 0)) return(1);

    /* Call CVodeInit to initialize the integrator memory and specify the
   * user's right hand side function in y'=f(t,y), the inital time T0, and
   * the initial dependent variable vector y. */
    flag = CVodeInit(mem, rhs, tstart, y);
    if (check_flag(&flag, "CVodeInit", 1)) return(1);

    /* Call CVodeSVtolerances to specify the scalar relative tolerance
   * and vector absolute tolerances */
    flag = CVodeSVtolerances(mem, reltol, abstol);
    if (check_flag(&flag, "CVodeSVtolerances", 1)) return(1);

    /* Call CVDense to specify the CVDENSE dense linear solver */
    flag = CVDense(mem, n_states);
    if (check_flag(&flag, "CVDense", 1)) return(1);

    flag = CVodeSetMaxStep(mem, tlmConfig.hmax);
    if (check_flag(&flag, "CVodeSetMaxStep", 1)) return(1);

    /* Set the Jacobian routine to Jac (user-supplied) */
    //flag = CVDlsSetDenseJacFn(cvode_mem, Jac);        //TODO: Supply with jacobian somehow
    //if (check_flag(&flag, "CVDlsSetDenseJacFn", 1)) return(1);
  }
  else if(simConfig.solver == IDA) {
    y = yp = abstol = NULL;
    mem = NULL;

    /* Create serial vector of length NEQ for I.C. and abstol */
    y = N_VNew_Serial(n_states);
    if (check_flag((void *)y, "N_VNew_Serial", 0)) return(1);
    yp = N_VNew_Serial(n_states);
    if (check_flag((void *)yp, "N_VNew_Serial", 0)) return(1);
    abstol = N_VNew_Serial(n_states);
    if (check_flag((void *)abstol, "N_VNew_Serial", 0)) return(1);

    /* Initialize y */
    for(size_t i=0; i<n_states; ++i) {
      Ith(y,i+1) = states[i];
    }

    /* Initialize yp */
    for(size_t i=0; i<n_states; ++i) {
      Ith(yp,i+1) = states_der[i];
    }

    /* Set the vector absolute tolerance */
    for(size_t i=0; i<n_states; ++i) {
      Ith(abstol,i+1) = states_abstol[i];
    }

    /* Call CVodeCreate to create the solver memory and specify the
     * Backward Differentiation Formula and the use of a Newton iteration */
    mem = IDACreate();
    if (check_flag((void *)mem, "IDACreate", 0)) return(1);

    /* Call IDACreate and IDAInit to initialize IDA memory */
    flag = IDAInit(mem, rhs_ida, tstart, y, yp);
    if (check_flag(&flag, "IDAInit", 1)) return(1);

    /* Call CVodeSVtolerances to specify the scalar relative tolerance
   * and vector absolute tolerances */
    flag = IDASVtolerances(mem, reltol, abstol);
    if (check_flag(&flag, "IDASVtolerances", 1)) return(1);

    /* Call CVDense to specify the CVDENSE dense linear solver */
    flag = IDADense(mem, n_states);
    if (check_flag(&flag, "IDADense", 1)) return(1);

    flag = IDASetMaxStep(mem, tlmConfig.hmax);
    if (check_flag(&flag, "IDASetMaxStep", 1)) return(1);

    /* Set the Jacobian routine to Jac (user-supplied) */
    //flag = CVDlsSetDenseJacFn(cvode_mem, Jac);        //TODO: Supply with jacobian somehow
    //if (check_flag(&flag, "CVDlsSetDenseJacFn", 1)) return(1);
  }

  double tc=tstart; //Cvode time
  while ((tcur < tend) && (!(eventInfo.terminateSimulation || terminateSimulation))) {
    size_t k;
    fmi2_real_t tlast;
    int zero_crossing_event = 0;

    fmistatus = fmi2_import_set_time(fmu, tcur);

    { /* Swap event_indicators and event_indicators_prev so that we can get new indicators */
      fmi2_real_t *temp = event_indicators;
      event_indicators = event_indicators_prev;
      event_indicators_prev = temp;
    }
    fmistatus = fmi2_import_get_event_indicators(fmu, event_indicators, n_event_indicators);

    /* Check if an event indicator has triggered */
    for (k = 0; k < n_event_indicators; k++) {
      if ((event_indicators[k] > 0) != (event_indicators_prev[k] > 0)) {
        zero_crossing_event = 1;
        break;
      }
    }

    /* Handle any events */
    if (callEventUpdate || zero_crossing_event ||
        (eventInfo.nextEventTimeDefined && tcur == eventInfo.nextEventTime)) {
      fmistatus = fmi2_import_enter_event_mode(fmu);
      do_event_iteration(fmu, &eventInfo);
      fmistatus = fmi2_import_enter_continuous_time_mode(fmu);

      fmistatus = fmi2_import_get_continuous_states(fmu, states, n_states);
      fmistatus = fmi2_import_get_event_indicators(fmu, event_indicators, n_event_indicators);
    }


    if(simConfig.solver == CVODE) {
      /* Calculate next time step */
      tlast = tcur;
      tcur += hdef;
      if (eventInfo.nextEventTimeDefined && (tcur >= eventInfo.nextEventTime)) {
        tcur = eventInfo.nextEventTime;
      }
      hcur = tcur - tlast;
      if(tcur > tend - hcur/1e16) {
        tcur = tend;
        hcur = tcur - tlast;
      }

      //Write interpolated force to FMU
      forceFromTlmToFmu(tlast);

      fmistatus = fmi2_import_get_continuous_states(fmu,states,n_states);

      //Read states from FMU
      for(size_t i=0; i<n_states; ++i) {
        Ith(y,i+1) = states[i];
      }

      //Take one step
      while(tc < tcur){
        flag = CVode(mem, tcur, y, &tc, CV_ONE_STEP);
        if (check_flag(&flag, "CVode", 1)) {
          TLMErrorLog::FatalError("CVODE solver failed!");
          exit(1);
        }
      }

      /* Set states */
      for(size_t i=0; i<n_states; ++i) {
        states[i] = Ith(y,i+1);
      }
      fmistatus = fmi2_import_set_continuous_states(fmu, states, n_states);
      /* Step is complete */
      fmistatus = fmi2_import_completed_integrator_step(fmu, fmi2_true, &callEventUpdate,
                                                        &terminateSimulation);
    }
    else if(simConfig.solver == IDA) {
      /* Calculate next time step */
      tlast = tcur;
      tcur += hdef;
      if (eventInfo.nextEventTimeDefined && (tcur >= eventInfo.nextEventTime)) {
        tcur = eventInfo.nextEventTime;
      }
      hcur = tcur - tlast;
      if(tcur > tend - hcur/1e16) {
        tcur = tend;
        hcur = tcur - tlast;
      }

      //Write interpolated force to FMU
      forceFromTlmToFmu(tlast);

      fmistatus = fmi2_import_get_continuous_states(fmu,states,n_states);
      fmistatus = fmi2_import_get_derivatives(fmu,states_der,n_states);

      //Read states from FMU
      for(size_t i=0; i<n_states; ++i) {
        Ith(y,i+1) = states[i];
        Ith(yp,i+1) = states_der[i];
      }

      //Take one step
      while(tc < tcur){
        flag = IDASolve(mem, tcur, &tc, y, yp, IDA_ONE_STEP);
        if (check_flag(&flag, "IDASolve", 1)) {
          TLMErrorLog::FatalError("IDA solver failed!");
          exit(1);
        }
      }

      /* Set states */
      for(size_t i=0; i<n_states; ++i) {
        states[i] = Ith(y,i+1);
      }
      fmistatus = fmi2_import_set_continuous_states(fmu, states, n_states);
      /* Step is complete */
      fmistatus = fmi2_import_completed_integrator_step(fmu, fmi2_true, &callEventUpdate,
                                                        &terminateSimulation);
    }
    else if(simConfig.solver == ExplicitEuler) {
      /* Calculate next time step */
      tlast = tcur;
      tcur += hdef;
      if (eventInfo.nextEventTimeDefined && (tcur >= eventInfo.nextEventTime)) {
        tcur = eventInfo.nextEventTime;
      }
      hcur = tcur - tlast;
      if(tcur > tend - hcur/1e16) {
        tcur = tend;
        hcur = tcur - tlast;
      }

      // Integrate one step (Euler forward)
      fmistatus = fmi2_import_get_derivatives(fmu, states_der, n_states);
      for (k = 0; k < n_states; k++) {
        states[k] = states[k] + hcur*states_der[k];
      }

      /* Set states */
      fmistatus = fmi2_import_set_continuous_states(fmu, states, n_states);
      /* Step is complete */
      fmistatus = fmi2_import_completed_integrator_step(fmu, fmi2_true, &callEventUpdate,
                                                        &terminateSimulation);
      //Write interpolated force to FMU
      forceFromTlmToFmu(tlast);
    }

    // Read motion from FMU
    forceFromTlmToFmu(tcur);
    motionFromFmuToTlm(tcur);
  }

  fmistatus = fmi2_import_terminate(fmu);

  fmi2_import_free_instance(fmu);

  free(states);
  free(states_der);
  free(event_indicators);
  free(event_indicators_prev);
  fmi2_import_free_instance(fmu);

  return 0;
}


// Convert a CSV string to an array of integers
void csvToIntArray(std::string csv, int length, fmi2_value_reference_t *array[])
{
  *array = (fmi2_value_reference_t*)calloc(length, sizeof(fmi2_value_reference_t*));
  std::string word;
  std::stringstream ss(csv);
  getline(ss,word,',');
  for(int i=0; i<length; ++i) {
    getline(ss, word,',');
    int number;
    std::istringstream(word) >> number;
    (*array)[i] = number;
  }
}



// Reads interface data (Value references for FMI mapped to TLM connections) from FMI configuration file
// Todo: Add error handling
void readFmiConfigFile()
{
  fmiConfig.nInterfaces=0;
  std::ifstream infile(FMI_CONFIG_FILE_NAME);
  if(infile.is_open()) {
    for( std::string line; getline( infile, line ); ) {
      std::stringstream ss(line);

      std::string word;
      getline(ss, word, ',');
      if(word == "substeps") {
        getline(ss, word, ',');
        fmiConfig.nSubSteps = atoi(word.c_str());
      }
      else if(word == "name") {
        fmiConfig.nInterfaces++;
        fmiConfig.interfaceIds.resize(fmiConfig.nInterfaces);
        getline(ss, word, ',');
        fmiConfig.interfaceNames.push_back(word);
      }
      else if(word == "position") {
        fmiConfig.position_vr.push_back(new fmi2_value_reference_t);
        csvToIntArray(ss.str(),3,&(fmiConfig.position_vr.back()));
      }
      else if(word == "orientation") {
        fmiConfig.orientation_vr.push_back(new fmi2_value_reference_t);
        csvToIntArray(ss.str(),9,&(fmiConfig.orientation_vr.back()));
      }
      else if(word == "speed") {
        fmiConfig.speed_vr.push_back(new fmi2_value_reference_t);
        csvToIntArray(ss.str(),3,&(fmiConfig.speed_vr.back()));
      }
      else if(word == "ang_speed") {
        fmiConfig.ang_speed_vr.push_back(new fmi2_value_reference_t);
        csvToIntArray(ss.str(),3,&(fmiConfig.ang_speed_vr.back()));
      }
      else if(word == "force") {
        fmiConfig.force_vr.push_back(new fmi2_value_reference_t);
        csvToIntArray(ss.str(),6,&(fmiConfig.force_vr.back()));
      }
    }

    // Print log output
    TLMErrorLog::Log("---"+string(FMI_CONFIG_FILE_NAME)+"---");
    std::stringstream output;
    output << "Number of interfaces: " << fmiConfig.nInterfaces;
    TLMErrorLog::Log(output.str());
    for(size_t i=0; i<fmiConfig.nInterfaces; ++i) {
      TLMErrorLog::Log("Name: "+fmiConfig.interfaceNames[i]);
      output.str("");
      output << "Position:";
      for(int j=0; j<3; ++j) {
        output << " " << fmiConfig.position_vr[i][j];
      }
      TLMErrorLog::Log(output.str());
      output.str("");
      output << "Orientation:";
      for(int j=0; j<9; ++j) {
        output << " " << fmiConfig.orientation_vr[i][j];
      }
      TLMErrorLog::Log(output.str());
      output.str("");
      output << "Speed:";
      for(int j=0; j<3; ++j) {
        output << " " << fmiConfig.speed_vr[i][j];
      }
      TLMErrorLog::Log(output.str());
      output.str("");
      output << "Angular speed:";
      for(int j=0; j<3; ++j) {
        output << " " << fmiConfig.ang_speed_vr[i][j];
      }
      TLMErrorLog::Log(output.str());
      output.str("");
      output << "Force:";
      for(int j=0; j<6; ++j) {
        output << " " << fmiConfig.force_vr[i][j];
      }
      TLMErrorLog::Log(output.str());
    }
  }
  else {
    TLMErrorLog::FatalError("Unable to read "+string(FMI_CONFIG_FILE_NAME));
    exit(1);
  }
}


void readTlmConfigFile()
{
  ifstream tlmConfigFile(TLM_CONFIG_FILE_NAME);

  tlmConfigFile >> tlmConfig.model;
  tlmConfigFile >> tlmConfig.server;
  tlmConfigFile >> tlmConfig.tstart;
  tlmConfigFile >> tlmConfig.tend;
  tlmConfigFile >> tlmConfig.hmax;

  if(!tlmConfigFile.good()) {
    TLMErrorLog::FatalError("Error reading TLM configuration data from tlm.config");
    exit(1);
  }

  //Print results to log file
  TLMErrorLog::Log("---"+string(TLM_CONFIG_FILE_NAME)+"---");
  TLMErrorLog::Log("model: "+tlmConfig.model);
  TLMErrorLog::Log("server: "+tlmConfig.server);
  std::stringstream ss1;
  ss1 << "tstart: " << tlmConfig.tstart;
  TLMErrorLog::Log(ss1.str());
  std::stringstream ss2;
  ss2 << "tend: " << tlmConfig.tend;
  TLMErrorLog::Log(ss2.str());
  std::stringstream ss3;
  ss3 << "hmax: " << tlmConfig.hmax;
  TLMErrorLog::Log(ss3.str());
}


//Create temp directory, or clear contents if it already exists
void createAndClearTempDirectory(std::string path)
{
  struct stat info;
  if( info.st_mode & S_IFDIR ) {           //temp directory already exists
#ifdef WIN32
    std::string command = "rd /s /q \""+path+"\"";
#else
    std::string command = "rm -rf "+tmpPath;
#endif
    TLMErrorLog::Log("Calling: "+command);
    system(command.c_str());
  }
  std::string command = "mkdir \""+path+"\"";
  TLMErrorLog::Log("Calling: "+command);
  system(command.c_str());
}



int main(int argc, char* argv[])
{
  TLMErrorLog::SetNormalErrorLogOn(true);
  TLMErrorLog::SetWarningOut(true);

  if(argc < 2) {
    cout << "Too few arguments to FMIWrapper (should be at least 3)." << endl;
    TLMErrorLog::FatalError("Too few arguments!");
    return -1;
  }

  std::string path = argv[1];
  std::string FMUPath = path+"\\"+argv[2];
  std::string tmpPath = path+"\\"+TEMP_DIR_NAME;
  std::string fmiConfigPath = path+"\\"+FMI_CONFIG_FILE_NAME;
  std::string tlmConfigPath = path+"\\"+TLM_CONFIG_FILE_NAME;

  simConfig.solver = ExplicitEuler;

  // Check additional arguments (solver and debug settings)
  // Not so nice to test all cases, but it works for now
  for(int i=3; i<argc; ++i) {
    if(!strcmp(argv[i],"solver=Euler"))
      simConfig.solver = ExplicitEuler;
    else if(!strcmp(argv[i],"solver=CVODE"))
      simConfig.solver = CVODE;
    else if(!strcmp(argv[i],"solver=IDA"))
      simConfig.solver = IDA;
    else if(!strcmp(argv[i],"-d")) {
      TLMErrorLog::SetDebugOut(true);
      cout << "Activating debug output" << endl;
    }
  }

  for(int i=0; i<argc; ++i) {
    TLMErrorLog::Log("Hello!");
    TLMErrorLog::Log(argv[i]);
  }

  TLMErrorLog::Log("---Arguments---");
  TLMErrorLog::Log("FMU file: "+FMUPath+"");
  TLMErrorLog::Log("Temp path: "+tmpPath+"");
  TLMErrorLog::Log("Interfaces file: "+fmiConfigPath+"");
  TLMErrorLog::Log("TLM config file: "+tlmConfigPath+"");

  // Create and clear temporary directory
  createAndClearTempDirectory(tmpPath);

  // Read TLM configuration
  readFmiConfigFile();

  // Read FMI configuration
  readTlmConfigFile();

  // Instantiate each TLMPlugin
  for(size_t i=0; i<fmiConfig.nInterfaces; ++i) {
    fmiConfig.plugins.push_back(TLMPlugin::CreateInstance());
  }

  // Initialize each TLMPlugin
  for(size_t i=0; i<fmiConfig.nInterfaces; ++i) {
    if(!fmiConfig.plugins[i]->Init(tlmConfig.model,
                                   tlmConfig.tstart,
                                   tlmConfig.tend,
                                   tlmConfig.hmax,
                                   tlmConfig.server)) {
      TLMErrorLog::FatalError("Error initializing the TLM plugin.");
      exit(1);
    }
  }

  // Register TLM interfaces
  for(size_t i=0; i<fmiConfig.plugins.size(); ++i) {
    fmiConfig.interfaceIds[i] = fmiConfig.plugins[i]->RegisteTLMInterface(fmiConfig.interfaceNames[i]);
  }

  jm_callbacks callbacks;
  fmi_import_context_t* context;
  fmi_version_enu_t version;

  callbacks.malloc = malloc;
  callbacks.calloc = calloc;
  callbacks.realloc = realloc;
  callbacks.free = free;
  callbacks.logger = fmiLogger;
  callbacks.log_level = jm_log_level_warning;   //Log level
  callbacks.context = 0;

  context = fmi_import_allocate_context(&callbacks);

  // Check version of FMU
  version = fmi_import_get_fmi_version(context, FMUPath.c_str(), tmpPath.c_str());
  if(version != fmi_version_2_0_enu) {
    TLMErrorLog::FatalError("The code only supports version 2.0");
  }

  // Parse modelDescription.xml
  fmu = fmi2_import_parse_xml(context, tmpPath.c_str(), 0);

  if(!fmu) {
    TLMErrorLog::FatalError("Error parsing XML, exiting");
  }

  // Check FMU kind (CS or ME)
  fmi2_fmu_kind_enu_t kind = fmi2_import_get_fmu_kind(fmu);

  // Start simulation
  switch(kind) {
    case fmi2_fmu_kind_cs:
      simulate_fmi2_cs();
      break;
    case fmi2_fmu_kind_me:
      simulate_fmi2_me();
      break;
    case fmi2_fmu_kind_me_and_cs:         //Not sure how to handle FMUs that can be both kinds, guess ME better than CS
      simulate_fmi2_me();
      break;
    case fmi2_fmu_kind_unknown:
      TLMErrorLog::FatalError("Unknown FMU kind.");
      break;
  }

  //Clean up
  fmi2_import_destroy_dllfmu(fmu);
  fmi2_import_free(fmu);
  fmi_import_free_context(context);

  TLMErrorLog::Log("FMIWrapper completed successfully!");

  return 0;
}

