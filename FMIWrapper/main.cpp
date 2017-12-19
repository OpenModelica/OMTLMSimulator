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
#include <map>
#include <regex>

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
#include "Plugin/TLMPlugin.h"
#include "Logging/TLMErrorLog.h"
#include "common.h"

using namespace std;

// FMI config data
struct fmiConfig_t {
  size_t nSubSteps;
  size_t nInterfaces;
  std::vector<int> dimensions;
  std::vector<std::string> causalities;
  std::vector<std::string> domains;
  std::vector<std::string> interfaceNames;
  std::vector<int> interfaceIds;
  std::vector<fmi2_value_reference_t*> position_vr;
  std::vector<fmi2_value_reference_t*> orientation_vr;
  std::vector<fmi2_value_reference_t*> speed_vr;
  std::vector<fmi2_value_reference_t*> ang_speed_vr;
  std::vector<fmi2_value_reference_t*> force_vr;
  std::vector<fmi2_value_reference_t*> value_vr;
};

// TLM config data
struct tlmConfig_t {
  std::string model;
  std::string server;
  double tstart;
  double tend;
  double hmax;
};

enum solver_t { ExplicitEuler, RungeKutta, CVODE, IDA };

struct simConfig_t {
  solver_t solver;
  double reltol;
  std::vector<double> abstol;
  int logLevel;
  std::string variableFilter = ".*";
};

static const char* TEMP_DIR_NAME = "temp";
static const char* TLM_CONFIG_FILE_NAME = "tlm.config";
static const char* FMI_CONFIG_FILE_NAME = "fmi.config";
static const char* LOG_FILE_NAME = "logdata.csv";

static TLMPlugin* plugin;
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

static std::map<fmi2_value_reference_t,std::string> parameterMap;

static std::vector<fmi2_value_reference_t> logVariables;
static std::ofstream logStream;
bool logStreamOpen = false;


void initializeLogging() {
  logStream.open(LOG_FILE_NAME);
  if(logStream.is_open()) {
    std::regex exp(simConfig.variableFilter);
    fmi2_import_variable_list_t *list = fmi2_import_get_variable_list(fmu,0);
    size_t nVar = fmi2_import_get_variable_list_size(list);
    for(size_t i=0; i<nVar; ++i) {
      fmi2_import_variable_t* var = fmi2_import_get_variable(list,i);
      std::string name = fmi2_import_get_variable_name(var);
      if (std::regex_match(name, exp)) {
        logVariables.push_back(fmi2_import_get_variable_vr(var));
      }
    }
    if(logVariables.empty()) {
      logStreamOpen = false;
      logStream.close();
      return;
    }
  }

  logStreamOpen = true;
  logStream << "\"time\"";

  for(size_t i=0; i<logVariables.size(); ++i) {
    fmi2_value_reference_t vr = logVariables[i];
    fmi2_import_variable_t* var = fmi2_import_get_variable_by_vr(fmu,fmi2_base_type_real,vr);
    logStream << ",\"" << fmi2_import_get_variable_name(var) << "\"";
  }
  logStream << "\n";
}

void logAllVariables(double time) {
  if(logStream.is_open()) {
    logStream << time;
    for(size_t i=0; i<logVariables.size(); ++i) {
      double value;
      fmi2_value_reference_t vr = logVariables[i];
      fmi2_import_get_real(fmu,&vr,1,&value);
      logStream << "," << value;
    }
    logStream << "\n";
  }
}


void setParameters()
{
    //Todo: Support other types than real
    std::map<fmi2_value_reference_t, std::string >::iterator it;
    for(it=parameterMap.begin(); it!=parameterMap.end(); ++it) {
        fmi2_value_reference_t vr = it->first;
        std::string value = it->second;
        double value_real = atof(value.c_str());
        std::stringstream ss;
        ss << "Setting parameter: " << vr << " to " << value_real;
        TLMErrorLog::Info(ss.str());
        fmi2_import_set_real(fmu,&vr,1,&value_real);
    }
}


//Read start values for interface variablesfrom
//modelDescription.xml and apply them in OMTLMSimulator.
void applyStartValues()
{
    fmi2_import_variable_list_t *list = fmi2_import_get_variable_list(fmu,0);
    size_t nVar = fmi2_import_get_variable_list_size(list);
    for(size_t i=0; i<nVar; ++i) {
        fmi2_import_variable_t* var = fmi2_import_get_variable(list,i);
        if(!fmi2_import_get_variable_has_start(var)) {
          continue;
        }
        fmi2_base_type_enu_t baseType = fmi2_import_get_variable_base_type(var);
        fmi2_value_reference_t vr = fmi2_import_get_variable_vr(var);
        if(baseType == fmi2_base_type_real) {
            fmi2_import_real_variable_t* realVar = fmi2_import_get_variable_as_real(var);
            fmi2_real_t sv = fmi2_import_get_real_variable_start(realVar);
            fmi2_import_set_real(fmu,&vr,1,&sv);
        }
        else if(baseType == fmi2_base_type_int) {
            fmi2_import_integer_variable_t* intVar = fmi2_import_get_variable_as_integer(var);
            fmi2_integer_t sv = fmi2_import_get_integer_variable_start(intVar);
            fmi2_import_set_integer(fmu,&vr,1,&sv);
        }
        else if(baseType == fmi2_base_type_bool) {
            fmi2_import_bool_variable_t* boolVar = fmi2_import_get_variable_as_boolean(var);
            fmi2_boolean_t sv = fmi2_import_get_boolean_variable_start(boolVar);
            fmi2_import_set_boolean(fmu,&vr,1,&sv);
        }
        else if(baseType == fmi2_base_type_str) {
            fmi2_import_string_variable_t* strVar = fmi2_import_get_variable_as_string(var);
            fmi2_string_t sv = fmi2_import_get_string_variable_start(strVar);
            fmi2_import_set_string(fmu,&vr,1,&sv);
        }
    }

    for(size_t j=0; j<fmiConfig.nInterfaces; ++j) {
        if(fmiConfig.dimensions[j] == 6  &&
           fmiConfig.causalities[j] == "Bidirectional") {

            double force[6];
            for(int k=0; k<6; ++k) {
                fmi2_import_variable_t* var = fmi2_import_get_variable_by_vr(fmu,fmi2_base_type_real,fmiConfig.force_vr[j][k]);
                if(fmi2_import_get_variable_has_start(var)) {
                    fmi2_import_real_variable_t* realVar = fmi2_import_get_variable_as_real(var);
                    force[k] = fmi2_import_get_real_variable_start(realVar);
                }
                else {
                    force[k] = 0;
                }
            }

            for(size_t k=0; k<6; ++k) {
                force[k] = -force[k];
            }

            plugin->SetInitialForce3D(fmiConfig.interfaceIds[j],
                                      force[0], force[1], force[2],
                                      force[3], force[4], force[5]);
        }
        else if(fmiConfig.dimensions[j] == 1  &&
                fmiConfig.causalities[j] == "Bidirectional") {
            double force;

            fmi2_import_variable_t* var = fmi2_import_get_variable_by_vr(fmu,fmi2_base_type_real,(*fmiConfig.force_vr[j]));
            if(fmi2_import_get_variable_has_start(var)) {
                fmi2_value_reference_t vr = fmi2_import_get_variable_vr(var);
                fmi2_import_real_variable_t* realVar = fmi2_import_get_variable_as_real(var);
                force = fmi2_import_get_real_variable_start(realVar);
            }
            else {
                force = 0;
            }

            if(fmiConfig.domains[j] != "Hydraulic") {
                force = -force;
            }

            plugin->SetInitialForce1D(fmiConfig.interfaceIds[j], force);
        }
        else if(fmiConfig.dimensions[j] == 1 &&
                fmiConfig.causalities[j] == "Input" ) {
            double value;

            fmi2_import_variable_t* var = fmi2_import_get_variable_by_vr(fmu,fmi2_base_type_real,(*fmiConfig.value_vr[j]));
            if(fmi2_import_get_variable_has_start(var)) {
                fmi2_import_real_variable_t* realVar = fmi2_import_get_variable_as_real(var);
                value = fmi2_import_get_real_variable_start(realVar);
            }
            else {
                value = 0;
            }

            plugin->SetInitialValue(fmiConfig.interfaceIds[j], value);
        }
    }
}

//Read force from TLMPlugin and write it to FMU
void forceFromTlmToFmu(double tcur)
{
    //Write interpolated force to FMU
    for(size_t j=0; j<fmiConfig.nInterfaces; ++j) {
        if(fmiConfig.dimensions[j] == 6 &&
           fmiConfig.domains[j] == "Mechanical" &&
           fmiConfig.causalities[j] == "Bidirectional") {
            double position[3],orientation[9],speed[3],ang_speed[3],force[6];
            //Read position and speed from FMU
            fmistatus = fmi2_import_get_real(fmu,fmiConfig.position_vr[j],3,position);
            fmistatus = fmi2_import_get_real(fmu,fmiConfig.orientation_vr[j],9,orientation);
            fmistatus = fmi2_import_get_real(fmu,fmiConfig.speed_vr[j],3,speed);
            fmistatus = fmi2_import_get_real(fmu,fmiConfig.ang_speed_vr[j],3,ang_speed);

            //Get interpolated force
            plugin->GetForce3D(fmiConfig.interfaceIds[j], tcur, position,orientation,speed,ang_speed,force);

            for(size_t k=0; k<6; ++k) {
              force[k] = -force[k];
            }

            // Write force to FMU
            fmistatus = fmi2_import_set_real(fmu,fmiConfig.force_vr[j],6,force);
        }
        else if(fmiConfig.dimensions[j] == 1  &&
                fmiConfig.causalities[j] == "Bidirectional") {
          double position,speed,force;

          //Read position and speed from FMU
          fmistatus = fmi2_import_get_real(fmu,fmiConfig.position_vr[j],1,&position);
          fmistatus = fmi2_import_get_real(fmu,fmiConfig.speed_vr[j],1,&speed);

          //Get interpolated force
          plugin->GetForce1D(fmiConfig.interfaceIds[j], tcur, speed,&force);

          if(fmiConfig.domains[j] != "Hydraulic") {
              force = -force;
          }

          // Write force to FMU
          fmistatus = fmi2_import_set_real(fmu,fmiConfig.force_vr[j],1,&force);
        }
        else if(fmiConfig.dimensions[j] == 1 &&
                fmiConfig.causalities[j] == "Input" ) {
            double value;

            plugin->GetValueSignal(fmiConfig.interfaceIds[j], tcur, &value);

            //Write value from FMU
            fmistatus = fmi2_import_set_real(fmu,fmiConfig.value_vr[j],1,&value);
        }
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
      TLMErrorLog::Info(ss.str());
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

  TLMErrorLog::Info("Version returned from FMU: "+string(fmi2_import_get_version(fmu)));
  TLMErrorLog::Info("Platform type returned: "+string(fmi2_import_get_types_platform(fmu)));

  fmuGUID = fmi2_import_get_GUID(fmu);
  TLMErrorLog::Info("GUID: "+string(fmuGUID));


  jmstatus = fmi2_import_instantiate(fmu, instanceName, fmi2_cosimulation, fmuLocation, visible);
  if (jmstatus == jm_status_error) {
    TLMErrorLog::FatalError("fmi2_import_instantiate failed");
  }

  applyStartValues();
  setParameters();

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
    logAllVariables(tcur);

    fmi2_real_t hsub = tlmConfig.hmax/fmiConfig.nSubSteps;
    for(size_t i=0; i<fmiConfig.nSubSteps; ++i) {
      for(size_t j=0; j<fmiConfig.nInterfaces; ++j) {
          if(fmiConfig.dimensions[j] == 6 &&
             fmiConfig.causalities[j] == "Bidirectional") {
              double position[3],orientation[9],speed[3],ang_speed[3],force[6];

              //Read position and speed from FMU
              fmistatus = fmi2_import_get_real(fmu,fmiConfig.position_vr[j],3,position);
              fmistatus = fmi2_import_get_real(fmu,fmiConfig.orientation_vr[j],9,orientation);
              fmistatus = fmi2_import_get_real(fmu,fmiConfig.speed_vr[j],3,speed);
              fmistatus = fmi2_import_get_real(fmu,fmiConfig.ang_speed_vr[j],3,ang_speed);

              //Get interpolated force
              plugin->GetForce3D(fmiConfig.interfaceIds[j], tcur, position,orientation,speed,ang_speed,force);

              for(size_t k=0; k<6; ++k) {
                force[k] = -force[k];
              }

              //Write force to FMU
              fmistatus = fmi2_import_set_real(fmu,fmiConfig.force_vr[j],6,force);
          }
          else if(fmiConfig.dimensions[j] == 1 &&
                  fmiConfig.causalities[j] == "Bidirectional") {
            double position,speed,force;

            //Read position and speed from FMU
            fmistatus = fmi2_import_get_real(fmu,fmiConfig.position_vr[j],1,&position);
            fmistatus = fmi2_import_get_real(fmu,fmiConfig.speed_vr[j],1,&speed);

            //Get interpolated force
            plugin->GetForce1D(fmiConfig.interfaceIds[j], tcur, speed,&force);

            if(fmiConfig.domains[j] != "Hydraulic") {
                force = -force;
            }

            //Write force to FMU
            fmistatus = fmi2_import_set_real(fmu,fmiConfig.force_vr[j],1,&force);
          }
          else if(fmiConfig.dimensions[j] == 1 &&
                  fmiConfig.causalities[j] == "Input") {
              double value;

              //Get value
              plugin->GetValueSignal(fmiConfig.interfaceIds[j], tcur, &value);

              fmistatus = fmi2_import_set_real(fmu,fmiConfig.value_vr[j],1,&value);
          }
      }

      //Take one sub step
      TLMErrorLog::Info("Taking step!");
      fmistatus = fmi2_import_do_step(fmu,tcur,hsub,fmi2_true);

      //Increment time
      tcur+=hsub;

      for(size_t j=0; j<fmiConfig.nInterfaces; ++j) {
          if(fmiConfig.dimensions[j] == 6 &&
             fmiConfig.causalities[j] == "Bidirectional") {
              double force[6], position[3],orientation[9],speed[3],ang_speed[3];

              //Read position and speed from FMU
              fmistatus = fmi2_import_get_real(fmu,fmiConfig.position_vr[j],3,position);
              fmistatus = fmi2_import_get_real(fmu,fmiConfig.orientation_vr[j],9,orientation);
              fmistatus = fmi2_import_get_real(fmu,fmiConfig.speed_vr[j],3,speed);
              fmistatus = fmi2_import_get_real(fmu,fmiConfig.ang_speed_vr[j],3,ang_speed);

              //Get interpolated force
              plugin->GetForce3D(fmiConfig.interfaceIds[j], tcur, position,orientation,speed,ang_speed,force);

              //Write back motion for sub step
              plugin->SetMotion3D(fmiConfig.interfaceIds[j], tcur, position, orientation, speed, ang_speed);
          }
          else if(fmiConfig.dimensions[j] == 1 &&
                  fmiConfig.causalities[j] == "Bidirectional") {
              double force, position, speed;

              //Read position and speed from FMU
              fmistatus = fmi2_import_get_real(fmu,fmiConfig.position_vr[j],1,&position);
              fmistatus = fmi2_import_get_real(fmu,fmiConfig.speed_vr[j],1,&speed);

              //Get interpolated force
              plugin->GetForce1D(fmiConfig.interfaceIds[j], tcur, speed,&force);

              //Write back motion for sub step
              plugin->SetMotion1D(fmiConfig.interfaceIds[j], tcur, position, speed);
          }
          else if(fmiConfig.dimensions[j] == 1 &&
                  fmiConfig.causalities[j] == "Output") {
              double value;

              //Read value from FMU
              fmistatus = fmi2_import_get_real(fmu,fmiConfig.value_vr[j],1,&value);

              //Write back value for sub step
              plugin->SetValueSignal(fmiConfig.interfaceIds[j], tcur, value);
          }
      }
    }
  }

  TLMErrorLog::Info("Simulation finished.");

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
  for(size_t j=0; j<fmiConfig.nInterfaces; ++j) {
    if(fmiConfig.dimensions[j] == 6 &&
       fmiConfig.causalities[j] == "Bidirectional") {
      double position[3],orientation[9],speed[3],ang_speed[3];

      fmistatus = fmi2_import_get_real(fmu,fmiConfig.position_vr[j],3,position);
      fmistatus = fmi2_import_get_real(fmu,fmiConfig.orientation_vr[j],9,orientation);
      fmistatus = fmi2_import_get_real(fmu,fmiConfig.speed_vr[j],3,speed);
      fmistatus = fmi2_import_get_real(fmu,fmiConfig.ang_speed_vr[j],3,ang_speed);

       plugin->SetMotion3D(fmiConfig.interfaceIds[j], tcur, position, orientation, speed, ang_speed);
    }
    else if(fmiConfig.dimensions[j] == 1 &&
            fmiConfig.causalities[j] == "Bidirectional") {
      double position,speed;

      fmistatus = fmi2_import_get_real(fmu,fmiConfig.position_vr[j],1,&position);
      fmistatus = fmi2_import_get_real(fmu,fmiConfig.speed_vr[j],1,&speed);

      plugin->SetMotion1D(fmiConfig.interfaceIds[j], tcur, position, speed);
    }
    else if(fmiConfig.dimensions[j] == 1 &&
            fmiConfig.causalities[j] == "Output") {
        double value;

        fmistatus = fmi2_import_get_real(fmu,fmiConfig.value_vr[j],1,&value);

        plugin->SetValueSignal(fmiConfig.interfaceIds[j], tcur, value);
    }
  }
}





// Simulate function for model exchange
int simulate_fmi2_me()
{
  TLMErrorLog::Info("Starting simulation using FMI for Model Exchange.");
  switch (simConfig.solver) {
    case CVODE:
      TLMErrorLog::Info("Using CVODE solver.");
      break;
    case IDA:
      TLMErrorLog::Info("Using IDA solver.");
      break;
    case ExplicitEuler:
      TLMErrorLog::Info("Using explicit Euler solver.");
      break;
    case RungeKutta:
      TLMErrorLog::Info("Using 4th order explicit Runge-Kutta solver.");
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

  TLMErrorLog::Info("Version returned from FMU: "+string(fmi2_import_get_version(fmu)));
  TLMErrorLog::Info("Platform type returned: "+string(fmi2_import_get_types_platform(fmu)));

  fmuGUID = fmi2_import_get_GUID(fmu);
  TLMErrorLog::Info("GUID: "+string(fmuGUID));

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

  applyStartValues();
  setParameters();

  fmi2_import_set_debug_logging(fmu, fmi2_false, 0, 0);
  TLMErrorLog::Info("fmi2_import_set_debug_logging: " + string(fmi2_status_to_string(fmistatus)));
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
  mem = NULL;
  y = yp = abstol = NULL;

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
  TLMErrorLog::Info("n_states = "+TLMErrorLog::ToStdStr(int(n_states)));
  for(size_t i=0; i<n_states; ++i) {
    Ith(abstol,i+1) = states_abstol[i];
    TLMErrorLog::Info("abstol["+TLMErrorLog::ToStdStr(int(i))+"] = "+TLMErrorLog::ToStdStr(Ith(abstol,i+1)));
  }

  if(simConfig.solver == CVODE) {
    TLMErrorLog::Info("Initializing CVODE solver.");

    if(n_states == 0) {
      TLMErrorLog::FatalError("At least one state variable is required for model exchange (n_states == 0)");
    }

    /* Call CVodeCreate to create the solver memory and specify the
   * Backward Differentiation Formula and the use of a Newton iteration */
    TLMErrorLog::Info("Creating solver.");
    mem = CVodeCreate(CV_BDF, CV_NEWTON);
    if (check_flag((void *)mem, "CVodeCreate", 0)) return(1);

    /* Call CVodeInit to initialize the integrator memory and specify the
   * user's right hand side function in y'=f(t,y), the inital time T0, and
   * the initial dependent variable vector y. */
    TLMErrorLog::Info("Initializing solver memory.");
    flag = CVodeInit(mem, rhs, tstart, y);
    if (check_flag(&flag, "CVodeInit", 1)) return(1);

    /* Call CVodeSVtolerances to specify the scalar relative tolerance
   * and vector absolute tolerances */
    TLMErrorLog::Info("Specifying tolerances.");
    //TLMErrorLog::Info("abstol = "+TLMErrorLog::ToStdStr(abstol));
    flag = CVodeSVtolerances(mem, reltol, abstol);
    if (check_flag(&flag, "CVodeSVtolerances", 1)) return(1);

    /* Call CVDense to specify the CVDENSE dense linear solver */
    TLMErrorLog::Info("Specifying linear solver (dense).");
    flag = CVDense(mem, n_states);
    if (check_flag(&flag, "CVDense", 1)) return(1);

    TLMErrorLog::Info("Specifying maximum step size.");
    flag = CVodeSetMaxStep(mem, tlmConfig.hmax);
    if (check_flag(&flag, "CVodeSetMaxStep", 1)) return(1);

    /* Set the Jacobian routine to Jac (user-supplied) */
    //flag = CVDlsSetDenseJacFn(cvode_mem, Jac);        //TODO: Supply with jacobian somehow
    //if (check_flag(&flag, "CVDlsSetDenseJacFn", 1)) return(1);
  }
  else if(simConfig.solver == IDA) {
    TLMErrorLog::Info("Initializing IDA solver.");

    if(n_states == 0) {
      TLMErrorLog::FatalError("At least one state variable is required for model exchange (n_states == 0)");
    }

      /* Call IDACreate to create the solver memory and specify the
     * Backward Differentiation Formula and the use of a Newton iteration */
    TLMErrorLog::Info("Creating solver.");
    mem = IDACreate();
    if (check_flag((void *)mem, "IDACreate", 0)) return(1);

    /* Call IDAInit and IDAInit to initialize IDA memory */
    TLMErrorLog::Info("Initializing solver memory.");
    flag = IDAInit(mem, rhs_ida, tstart, y, yp);
    if (check_flag(&flag, "IDAInit", 1)) return(1);

    /* Call IDASVtolerances to specify the scalar relative tolerance
   * and vector absolute tolerances */
    TLMErrorLog::Info("Spcifying tolerances.");
    flag = IDASVtolerances(mem, reltol, abstol);
    if (check_flag(&flag, "IDASVtolerances", 1)) return(1);

    /* Call IDADense to specify the CVDENSE dense linear solver */
    TLMErrorLog::Info("Specifying linear solver (dense).");
    flag = IDADense(mem, n_states);
    if (check_flag(&flag, "IDADense", 1)) return(1);

    TLMErrorLog::Info("Specifying maximum step size.");
    flag = IDASetMaxStep(mem, tlmConfig.hmax);
    if (check_flag(&flag, "IDASetMaxStep", 1)) return(1);

    /* Set the Jacobian routine to Jac (user-supplied) */
    //flag = CVDlsSetDenseJacFn(cvode_mem, Jac);        //TODO: Supply with jacobian somehow
    //if (check_flag(&flag, "CVDlsSetDenseJacFn", 1)) return(1);
  }

  double tc=tstart; //Cvode time
  TLMErrorLog::Info("Starting simulation loop.");
  while ((tcur < tend) && (!(eventInfo.terminateSimulation || terminateSimulation))) {
    logAllVariables(tcur);
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

    // Write interpolated force to FMU
    forceFromTlmToFmu(tlast);

    // Read motion from FMU
    motionFromFmuToTlm(tlast);

    // Read states and derivatives from FMU
    fmistatus = fmi2_import_get_continuous_states(fmu,states,n_states);
    fmistatus = fmi2_import_get_derivatives(fmu, states_der, n_states);
    for(size_t i=0; i<n_states; ++i) {
      Ith(y,i+1) = states[i];
      Ith(yp,i+1) = states_der[i];
    }

    //Integrate using specified solver
    if(simConfig.solver == CVODE) {
      while(tc < tcur){
        flag = CVode(mem, tcur, y, &tc, CV_ONE_STEP);
        if (check_flag(&flag, "CVode", 1)) {
          TLMErrorLog::FatalError("CVODE solver failed!");
          exit(1);
        }
      }
    }
    else if(simConfig.solver == IDA) {
      while(tc < tcur){
        flag = IDASolve(mem, tcur, &tc, y, yp, IDA_ONE_STEP);
        if (check_flag(&flag, "IDASolve", 1)) {
          TLMErrorLog::FatalError("IDA solver failed!");
          exit(1);
        }
      }
    }
    else if(simConfig.solver == ExplicitEuler) {
      for (k = 0; k < n_states; k++) {
        Ith(y,k+1) = Ith(y,k+1) + hcur*Ith(yp,k+1);
      }
    }
    else if(simConfig.solver == RungeKutta) {
      N_Vector k1,k2,k3,k4;
      k1 = N_VNew_Serial(n_states);
      if (check_flag((void *)y, "N_VNew_Serial", 0)) return(1);
      k2 = N_VNew_Serial(n_states);
      if (check_flag((void *)y, "N_VNew_Serial", 0)) return(1);
      k3 = N_VNew_Serial(n_states);
      if (check_flag((void *)y, "N_VNew_Serial", 0)) return(1);
      k4 = N_VNew_Serial(n_states);
      if (check_flag((void *)y, "N_VNew_Serial", 0)) return(1);

      k1 = yp;
      for(k=0; k<n_states; ++k) {
        Ith(y,k+1) = Ith(y,k+1) + hcur/2.0*Ith(k1,k+1);
      }
      rhs(tcur+hcur/2.0,y,yp,0);
      k2 = yp;
      for(k=0; k<n_states; ++k) {
        Ith(y,k+1) = Ith(y,k+1) + hcur/2.0*Ith(k2,k+1);
      }
      rhs(tcur+hcur/2.0,y,yp,0);
      k3 = yp;
      for(k=0; k<n_states; ++k) {
        Ith(y,k+1) = Ith(y,k+1) + hcur*Ith(k3,k+1);
      }
      rhs(tcur+hcur/2.0,y,yp,0);
      k4 = yp;

      for(k=0; k<n_states; ++k) {
        Ith(y,k+1) = Ith(y,k+1)+hcur*(Ith(k1,k+1)+2*Ith(k2,k+1)+2*Ith(k3,k+1)+Ith(k4,k+1))/6.0;
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
  TLMErrorLog::Info("Simulation ended.");

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


//Generate fmi.config if it does not exist (using only inputs and outputs)
void generateFmiConfigFile()
{
  TLMErrorLog::Info("Generating fmi.config...");

  std::ofstream fmiConfigStream;
  fmiConfigStream.open("fmi.config");
  if(fmiConfigStream.is_open()) {
    fmiConfigStream << "substeps,1\n";

    fmi2_import_variable_list_t *list = fmi2_import_get_variable_list(fmu,0);
    size_t nVar = fmi2_import_get_variable_list_size(list);
    for(size_t i=0; i<nVar; ++i) {
      fmi2_import_variable_t* var = fmi2_import_get_variable(list,i);
      if(fmi2_import_get_variable_base_type(var) == fmi2_base_type_real) {
        std::string name = fmi2_import_get_variable_name(var);
        if(fmi2_import_get_causality(var) == fmi2_causality_enu_input) {
          fmiConfigStream << "name," << name << "\n";
          fmiConfigStream << "domain,Signal\n";
          fmiConfigStream << "dimensions,1\n";
          fmiConfigStream << "causality,Input\n";
          fmiConfigStream << "value," << fmi2_import_get_variable_vr(var) << "\n";
        }
        else if(fmi2_import_get_causality(var) == fmi2_causality_enu_output) {
          fmiConfigStream << "name," << name << "\n";
          fmiConfigStream << "domain,Signal\n";
          fmiConfigStream << "dimensions,1\n";
          fmiConfigStream << "causality,Output\n";
          fmiConfigStream << "value," << fmi2_import_get_variable_vr(var) << "\n";
        }
      }
    }
  }
  else {
    TLMErrorLog::FatalError("Unable to write to fmi.config");
  }
  fmiConfigStream.close();
}


// Reads interface data (Value references for FMI mapped to TLM connections) from FMI configuration file
// Todo: Add error handling
void readFmiConfigFile()
{
  fmiConfig.nInterfaces=0;

  struct stat buffer;
  if(stat (FMI_CONFIG_FILE_NAME, &buffer) != 0) {
    TLMErrorLog::Info("File fmi.config does not exist.");
    generateFmiConfigFile();
  }

  TLMErrorLog::Info("Reading fmi.config...");

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
        fmiConfig.dimensions.push_back(6);                  //Default to 6D interface
        fmiConfig.causalities.push_back("Bidirectional");   //Default to bidirectional interface
        fmiConfig.domains.push_back("Mechanical");          //Default to mechanical domain
        fmiConfig.position_vr.push_back(new fmi2_value_reference_t);
        fmiConfig.orientation_vr.push_back(new fmi2_value_reference_t);
        fmiConfig.speed_vr.push_back(new fmi2_value_reference_t);
        fmiConfig.ang_speed_vr.push_back(new fmi2_value_reference_t);
        fmiConfig.force_vr.push_back(new fmi2_value_reference_t);
        fmiConfig.value_vr.push_back(new fmi2_value_reference_t);
      }
      else if(word == "dimensions") {
        getline(ss, word, ',');
        fmiConfig.dimensions[fmiConfig.dimensions.size()-1] = std::atoi(word.c_str());
      }
      else if(word == "causality") {
          getline(ss, word, ',');
          fmiConfig.causalities[fmiConfig.causalities.size()-1] = word.c_str();
      }
      else if(word == "domain") {
          getline(ss, word, ',');
          fmiConfig.domains[fmiConfig.domains.size()-1] = word.c_str();
      }
      else if(word == "position" &&
              fmiConfig.dimensions[fmiConfig.dimensions.size()-1] == 6 &&
              fmiConfig.causalities[fmiConfig.dimensions.size()-1] == "Bidirectional") {
        csvToIntArray(ss.str(),3,&(fmiConfig.position_vr.back()));
      }
      else if(word == "position" &&
              fmiConfig.dimensions[fmiConfig.dimensions.size()-1] == 1 &&
              fmiConfig.causalities[fmiConfig.dimensions.size()-1] == "Bidirectional") {
        csvToIntArray(ss.str(),1,&(fmiConfig.position_vr.back()));
        std::stringstream ss2;
        ss2 << "position_vr = " << fmiConfig.position_vr.back();
        TLMErrorLog::Info(ss2.str());
      }
      else if(word == "orientation") {
        csvToIntArray(ss.str(),9,&(fmiConfig.orientation_vr.back()));
      }
      else if(word == "speed" &&
              fmiConfig.dimensions[fmiConfig.dimensions.size()-1] == 6 &&
              fmiConfig.causalities[fmiConfig.dimensions.size()-1] == "Bidirectional") {
        csvToIntArray(ss.str(),3,&(fmiConfig.speed_vr.back()));
      }
      else if(word == "speed" &&
              fmiConfig.dimensions[fmiConfig.dimensions.size()-1] == 1 &&
              fmiConfig.causalities[fmiConfig.dimensions.size()-1] == "Bidirectional") {
        csvToIntArray(ss.str(),1,&(fmiConfig.speed_vr.back()));
        std::stringstream ss2;
        ss2 << "speed_vr = " << fmiConfig.speed_vr.back();
        TLMErrorLog::Info(ss2.str());
      }
      else if(word == "ang_speed") {
        csvToIntArray(ss.str(),3,&(fmiConfig.ang_speed_vr.back()));
      }
      else if(word == "force" &&
              fmiConfig.dimensions[fmiConfig.dimensions.size()-1] == 6 &&
              fmiConfig.causalities[fmiConfig.dimensions.size()-1] == "Bidirectional") {
        csvToIntArray(ss.str(),6,&(fmiConfig.force_vr.back()));
        std::stringstream ss2;
        ss2 << "force_vr = " << fmiConfig.force_vr.back();
        TLMErrorLog::Info(ss2.str());
      }
      else if(word == "force" &&
              fmiConfig.dimensions[fmiConfig.dimensions.size()-1] == 1 &&
              fmiConfig.causalities[fmiConfig.dimensions.size()-1] == "Bidirectional") {
        csvToIntArray(ss.str(),1,&(fmiConfig.force_vr.back()));
      }
      else if(word == "value" &&
              fmiConfig.dimensions[fmiConfig.dimensions.size()-1] == 1 &&
              fmiConfig.causalities[fmiConfig.dimensions.size()-1] == "Input") {
          csvToIntArray(ss.str(),1,&(fmiConfig.value_vr.back()));
      }
      else if(word == "value" &&
              fmiConfig.dimensions[fmiConfig.dimensions.size()-1] == 1 &&
              fmiConfig.causalities[fmiConfig.dimensions.size()-1] == "Output") {
          csvToIntArray(ss.str(),1,&(fmiConfig.value_vr.back()));
      }
    }

    // Print log output
    TLMErrorLog::Info("---"+string(FMI_CONFIG_FILE_NAME)+"---");
    TLMErrorLog::Info("Number of interfaces: "+TLMErrorLog::ToStdStr(int(fmiConfig.nInterfaces)));
    for(size_t i=0; i<fmiConfig.nInterfaces; ++i) {
      TLMErrorLog::Info("Name: "+fmiConfig.interfaceNames[i]);
      TLMErrorLog::Info("Causality: "+fmiConfig.causalities[i]);
      TLMErrorLog::Info("Dimensions: "+TLMErrorLog::ToStdStr(fmiConfig.dimensions[i]));
      TLMErrorLog::Info("Domain: "+fmiConfig.domains[i]);
      int nP,nO,nS,nA,nF,nV;
      if(fmiConfig.dimensions[i] == 6 &&
         fmiConfig.causalities[i] == "Bidirectional") {
          nP = nS = nA = 3;
          nO = 9;
          nF = 6;
          nV = 0;
      }
      else if(fmiConfig.dimensions[i] == 1 &&
              fmiConfig.causalities[i] == "Bidirectional") {
          nP = nS = nF = 1;
          nA = nO = 0;
          nV = 0;
      }
      else /*if(fmiConfig.causalities[i] == "Input" || fmiConfig.causalities[i] == "Output")*/ {
          nP = nS = nF = nA = nO = 0;
          nV = 1;
      }

      std::stringstream output;
      output << "Position:";
      for(int j=0; j<nP; ++j) {
        output << " " << fmiConfig.position_vr[i][j];
      }
      TLMErrorLog::Info(output.str());
      output.str("");
      output << "Orientation:";
      for(int j=0; j<nO; ++j) {
        output << " " << fmiConfig.orientation_vr[i][j];
      }
      TLMErrorLog::Info(output.str());
      output.str("");
      output << "Speed:";
      for(int j=0; j<nS; ++j) {
        output << " " << fmiConfig.speed_vr[i][j];
      }
      TLMErrorLog::Info(output.str());
      output.str("");
      output << "Angular speed:";
      for(int j=0; j<nA; ++j) {
        output << " " << fmiConfig.ang_speed_vr[i][j];
      }
      TLMErrorLog::Info(output.str());
      output.str("");
      output << "Force:";
      for(int j=0; j<nF; ++j) {
        output << " " << fmiConfig.force_vr[i][j];
      }
      TLMErrorLog::Info(output.str());
      output.str("");
      output << "Value:";
      for(int j=0; j<nV; ++j) {
        output << " " << fmiConfig.value_vr[i][j];
      }
      TLMErrorLog::Info(output.str());

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
  TLMErrorLog::Info("---"+string(TLM_CONFIG_FILE_NAME)+"---");
  TLMErrorLog::Info("model: "+tlmConfig.model);
  TLMErrorLog::Info("server: "+tlmConfig.server);
  TLMErrorLog::Info("tstart: "+TLMErrorLog::ToStdStr(tlmConfig.tstart));
  TLMErrorLog::Info("tend: "+TLMErrorLog::ToStdStr(tlmConfig.tend));
  TLMErrorLog::Info("hmax: "+TLMErrorLog::ToStdStr(tlmConfig.hmax));
}


//Create temp directory, or clear contents if it already exists
void createAndClearTempDirectory(std::string path)
{
  struct stat info;
  stat(path.c_str(), &info);
  if( info.st_mode & S_IFDIR ) {           //temp directory already exists
#ifdef WIN32
    std::string command = "rd /s /q \""+path+"\"";
#else
    std::string command = "rm -rf "+path;
#endif
    system(command.c_str());
  }
  std::string command = "mkdir \""+path+"\"";
  system(command.c_str());
}



int main(int argc, char* argv[])
{
  if(argc < 2) {
    cout << "Too few arguments." << endl << endl;
    cout << "Usage:" << endl;
    cout << "  FMIWrapper [path] [fmu file] [additional arguments]" << endl << endl;
    cout << "Additional arguments:" << endl;
    cout << "  solver=[solver]    Set numerical solver (Euler, RungeKutta, CVODE or IDA)" << endl;
    cout << "  -d                 Enable additional debug output" << endl << endl;
    cout << "  -l X               Logging level for FMU" << endl << endl;
    cout << "                     (0 = nothing, 1 = fatal,   2 = error, 3 = warning," << endl << endl;
    cout << "                      4 = info,    5 = verbose, 6 = debug, 7 = all)" << endl << endl;
    cout << "Example:" << endl;
    cout << "  FMIWrapper c:\\path\\to\\fmu model.fmu solver=CVODE -d -l 3" << endl;
    TLMErrorLog::FatalError("Too few arguments!");
    return -1;
  }

  std::string path = argv[1];
  std::string FMUPath = path+"/"+argv[2];
  std::string tmpPath = path+"/"+TEMP_DIR_NAME;
  std::string fmiConfigPath = path+"/"+FMI_CONFIG_FILE_NAME;
  std::string tlmConfigPath = path+"/"+TLM_CONFIG_FILE_NAME;

  simConfig.solver = ExplicitEuler;

  // Check additional arguments (solver and debug settings)
  // Not so nice to test all cases, but it works for now
  for(int i=3; i<argc; ++i) {
    if(!strcmp(argv[i],"solver=Euler"))
      simConfig.solver = ExplicitEuler;
    else if(!strcmp(argv[i],"solver=RungeKutta"))
      simConfig.solver = RungeKutta;
    else if(!strcmp(argv[i],"solver=CVODE"))
      simConfig.solver = CVODE;
    else if(!strcmp(argv[i],"solver=IDA"))
      simConfig.solver = IDA;
    else if(!strcmp(argv[i],"-d")) {
      TLMErrorLog::SetLogLevel(TLMLogLevel::Debug);
      cout << "Activating debug output" << endl;
    }
    else if(!strcmp(argv[i],"-l") && argc > i+1) {
      int logLevel = atoi(argv[i+1]);
      if(logLevel >= 0 && logLevel <= 7)
        simConfig.logLevel = atoi(argv[i+1]);
      else {
        cout << "Invalid logging level: " << logLevel << "\n";
        TLMErrorLog::FatalError("Invalid logging level: "+TLMErrorLog::ToStdStr(logLevel));
      }
    }
    else if(!strcmp(argv[i],"-v") && argc > i+1) {
      simConfig.variableFilter = argv[i+1];
    }
  }

  cout << "Starting FMIWrapper. Debug output will be written to \"TLMlogfile.log\"." << endl;

//  for(int i=0; i<argc; ++i) {
//    TLMErrorLog::Info("Hello!");
//    TLMErrorLog::Info(argv[i]);
//  }

  TLMErrorLog::Info("---Arguments---");
  TLMErrorLog::Info("FMU file: "+FMUPath+"");
  TLMErrorLog::Info("Temp path: "+tmpPath+"");
  TLMErrorLog::Info("Interfaces file: "+fmiConfigPath+"");
  TLMErrorLog::Info("TLM config file: "+tlmConfigPath+"");

  // Create and clear temporary directory
  createAndClearTempDirectory(tmpPath);


  jm_callbacks callbacks;
  fmi_import_context_t* context;
  fmi_version_enu_t version;

  callbacks.malloc = malloc;
  callbacks.calloc = calloc;
  callbacks.realloc = realloc;
  callbacks.free = free;
  callbacks.logger = fmiLogger;
  callbacks.log_level = jm_log_level_enu_t(simConfig.logLevel);   //Log level
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


  // Read TLM configuration
  readFmiConfigFile();

  // Read FMI configuration
  readTlmConfigFile();

  // Instantiate the TLMPlugin
  plugin = TLMPlugin::CreateInstance();


  // Initialize the TLMPlugin
    if(!plugin->Init(tlmConfig.model,
                                   tlmConfig.tstart,
                                   tlmConfig.tend,
                                   tlmConfig.hmax,
                                   tlmConfig.server)) {
      TLMErrorLog::FatalError("Error initializing the TLM plugin.");
      exit(1);
    }

  // Register TLM interfaces
  for(size_t i=0; i<fmiConfig.interfaceNames.size(); ++i) {
    std::stringstream ss;
    ss << "Registers interface " <<
          fmiConfig.interfaceNames[i] <<
          " of type " <<
          fmiConfig.dimensions[i];
    TLMErrorLog::Info(ss.str());
    fmiConfig.interfaceIds[i] = plugin->RegisteTLMInterface(fmiConfig.interfaceNames[i],
                                                            fmiConfig.dimensions[i],
                                                            fmiConfig.causalities[i],
                                                            fmiConfig.domains[i]);
  }



  // Check FMU kind (CS or ME)
  fmi2_fmu_kind_enu_t kind = fmi2_import_get_fmu_kind(fmu);

  fmi2_import_variable_list_t *list = fmi2_import_get_variable_list(fmu,0);
  size_t nVar = fmi2_import_get_variable_list_size(list);
  for(size_t i=0; i<nVar; ++i) {
      fmi2_import_variable_t* var = fmi2_import_get_variable(list,i);
      if(fmi2_import_get_causality(var) == fmi2_causality_enu_parameter) {
        std::string name = fmi2_import_get_variable_name(var);
        std::string value;
        if(fmi2_import_get_variable_base_type(var) == fmi2_base_type_real) {
            double value_real;
            fmi2_import_real_variable_t *var_real = fmi2_import_get_variable_as_real(var);
            value_real = fmi2_import_get_real_variable_start(var_real);
            std::stringstream ss;
            ss << value_real;
            value = ss.str();
        }
        else if(fmi2_import_get_variable_base_type(var) == fmi2_base_type_int) {
            double value_int;
            fmi2_import_integer_variable_t *var_int = fmi2_import_get_variable_as_integer(var);
            value_int = fmi2_import_get_integer_variable_start(var_int);
            std::stringstream ss;
            ss << value_int;
            value = ss.str();
        }
        else if(fmi2_import_get_variable_base_type(var) == fmi2_base_type_bool) {
            int value_bool;
            fmi2_import_bool_variable_t *var_bool = fmi2_import_get_variable_as_boolean(var);
            value_bool = fmi2_import_get_boolean_variable_start(var_bool);
            std::stringstream ss;
            ss << value_bool;
            value = ss.str();
        }
        else if(fmi2_import_get_variable_base_type(var) == fmi2_base_type_str) {
            const char* value_str;
            fmi2_import_string_variable_t *var_str = fmi2_import_get_variable_as_string(var);
            value_str = fmi2_import_get_string_variable_start(var_str);
            value = value_str;
        }

        int parId = plugin->RegisterComponentParameter(name,value);

        plugin->GetParameterValue(parId, name, value);
        TLMErrorLog::Info("Received value: "+value+" for parameter "+name);
        fmi2_value_reference_t vr = fmi2_import_get_variable_vr(var);
        parameterMap.insert(std::pair<fmi2_value_reference_t,std::string>(vr,value));
      }
  }

  initializeLogging();

  // Start simulation
  switch(kind) {
    case fmi2_fmu_kind_cs:
      TLMErrorLog::Info("FMU kind is co-simulation.");
      simulate_fmi2_cs();
      break;
    case fmi2_fmu_kind_me:
      TLMErrorLog::Info("FMU kind is model exchange.");
      simulate_fmi2_me();
      break;
    case fmi2_fmu_kind_me_and_cs:         //Not sure how to handle FMUs that can be both kinds, guess ME better than CS
      TLMErrorLog::Info("FMU kind is either co-simulation or model exchange.");
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

  plugin->AwaitClosePermission();

  TLMErrorLog::Info("FMIWrapper completed successfully!");

  return 0;
}



