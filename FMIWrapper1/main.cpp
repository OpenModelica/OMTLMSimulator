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

// FMILibrary includes
#include "FMI/fmi_import_context.h"
#include "FMI1/fmi1_import.h"
#include "FMI2/fmi2_import.h"
#include "JM/jm_portability.h"

// TLMPlugin includes
#include "TLMPlugin.h"
#include "TLMErrorLog.h"

using namespace std;

static const char* TLM_CONFIG_FILE_NAME = "tlm.config";
static const char* FMI_CONFIG_FILE_NAME = "fmi.config";

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

// FMILibrary logger
void importlogger(jm_callbacks* c, jm_string module, jm_log_level_enu_t log_level, jm_string message) {
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

// Exit function
void do_exit(int code) {
  printf("Press 'Enter' to exit\n");
  /* getchar(); */
  exit(code);
}

// Simulate function
int simulate(fmi2_import_t* fmu, tlmConfig_t tlmConfig, fmiConfig_t fmiConfig)
{
  fmi2_status_t fmistatus;
  jm_status_enu_t jmstatus;

  fmi2_string_t instanceName = "Test CS model instance";
  fmi2_string_t fmuGUID;
  fmi2_string_t fmuLocation = "";
  fmi2_boolean_t visible = fmi2_false;
  fmi2_real_t relativeTol = 1e-4;

  fmi2_real_t tcur = tlmConfig.tstart;
  fmi2_boolean_t StopTimeDefined = fmi2_false;

  TLMErrorLog::Log("Version returned from FMU: "+string(fmi2_import_get_version(fmu)));
  TLMErrorLog::Log("Platform type returned: "+string(fmi2_import_get_types_platform(fmu)));

  fmuGUID = fmi2_import_get_GUID(fmu);
  TLMErrorLog::Log("GUID: "+string(fmuGUID));


  jmstatus = fmi2_import_instantiate(fmu, instanceName, fmi2_cosimulation, fmuLocation, visible);
  if (jmstatus == jm_status_error) {
    TLMErrorLog::FatalError("fmi2_import_instantiate failed");
    do_exit(-1);
  }

  fmistatus = fmi2_import_setup_experiment(fmu, fmi2_true,
                                           relativeTol, tlmConfig.tstart, StopTimeDefined, tlmConfig.tend);
  if(fmistatus != fmi2_status_ok) {
    TLMErrorLog::FatalError("fmi2_import_setup_experiment failed");
    do_exit(-1);
  }

  fmistatus = fmi2_import_enter_initialization_mode(fmu);
  if(fmistatus != fmi2_status_ok) {
    TLMErrorLog::FatalError("fmi2_import_enter_initialization_mode failed");
    do_exit(-1);
  }

  fmistatus = fmi2_import_exit_initialization_mode(fmu);
  if(fmistatus != fmi2_status_ok) {
    TLMErrorLog::FatalError("fmi2_import_exit_initialization_mode failed");
    do_exit(-1);
  }

  tcur = tlmConfig.tstart;

  while (tcur < tlmConfig.tend) {
    fmi2_real_t hsub = tlmConfig.hmax/fmiConfig.nSubSteps;
    for(size_t i=0; i<fmiConfig.nSubSteps; ++i) {
      for(size_t j=0; j<fmiConfig.plugins.size(); ++j)
      {
        double position[3],orientation[3],speed[3],ang_speed[3],force[6];

        //Read position and speed from FMU
        fmistatus = fmi2_import_get_real(fmu,fmiConfig.position_vr[j],3,position);
        fmistatus = fmi2_import_get_real(fmu,fmiConfig.orientation_vr[j],3,orientation);
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

      for(size_t j=0; j<fmiConfig.plugins.size(); ++j)
      {
        double position[3],orientation[3],speed[3],ang_speed[3];

        //Read position and speed from FMU
        fmistatus = fmi2_import_get_real(fmu,fmiConfig.position_vr[j],3,position);
        fmistatus = fmi2_import_get_real(fmu,fmiConfig.orientation_vr[j],3,orientation);
        fmistatus = fmi2_import_get_real(fmu,fmiConfig.speed_vr[j],3,speed);
        fmistatus = fmi2_import_get_real(fmu,fmiConfig.ang_speed_vr[j],3,ang_speed);

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


void csvToIntArray(std::string csv, int length, fmi2_value_reference_t array[]){
  std::string word;
  std::stringstream ss(csv);
  getline(ss,word,',');
  for(int i=0; i<length; ++i) {
    getline(ss, word,',');
    int number;
    std::istringstream(word) >> number;
    array[i] = number;
  }
}



//Reads interface data (Value references for FMI mapped to TLM connecitons) from fmi_interfaces.csv
//Todo: Add error handling
fmiConfig_t readFmiConfigFile(std::string path)
{
  fmiConfig_t fmiConfig;
  fmiConfig.nInterfaces=0;
  std::ifstream infile(path.c_str());
  if(infile.is_open())
  {
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
        csvToIntArray(ss.str(),3,fmiConfig.position_vr.back());
      }
      else if(word == "orientation") {
        fmiConfig.orientation_vr.push_back(new fmi2_value_reference_t);
        csvToIntArray(ss.str(),3,fmiConfig.orientation_vr.back());
      }
      else if(word == "speed") {
        fmiConfig.speed_vr.push_back(new fmi2_value_reference_t);
        csvToIntArray(ss.str(),3,fmiConfig.speed_vr.back());
      }
      else if(word == "ang_speed") {
        fmiConfig.ang_speed_vr.push_back(new fmi2_value_reference_t);
        csvToIntArray(ss.str(),3,fmiConfig.ang_speed_vr.back());
      }
      else if(word == "force") {
        fmiConfig.force_vr.push_back(new fmi2_value_reference_t);
        csvToIntArray(ss.str(),6,fmiConfig.force_vr.back());
      }
    }

    TLMErrorLog::Log("---"+string(FMI_CONFIG_FILE_NAME)+"---");
    TLMErrorLog::Log("Number of interfaces: "+fmiConfig.nInterfaces);
    for(size_t i=0; i<fmiConfig.nInterfaces; ++i) {
      TLMErrorLog::Log("Name: "+fmiConfig.interfaceNames[i]);
      std::stringstream output;
      output << "Position:";
      for(int j=0; j<3; ++j) {
        output << " " << fmiConfig.position_vr[i][j];
      }
      TLMErrorLog::Log(output.str());
      output.str("");
      output << "Orientation:";
      for(int j=0; j<3; ++j) {
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
  else
  {
    TLMErrorLog::FatalError("Unable to read "+string(FMI_CONFIG_FILE_NAME));
    exit(1);
  }

  return fmiConfig;
}


tlmConfig_t readTlmConfigFile(std::string path)
{
    tlmConfig_t tlmConfig;
    ifstream tlmConfigFile(path.c_str());

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

    return tlmConfig;
}

void createAndClearTempDirectory(std::string path)
{
    //Create temp directory, or clear contents if it already exists
    struct stat info;
    if( stat( path.c_str(), &info ) != 0 ) {            //cannot access temp directory
      TLMErrorLog::FatalError("Unable to access temp directory!");
    }
    else if( info.st_mode & S_IFDIR ) {           //temp directory already exists
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



int main(int argc, char* argv[]) {

  if(argc < 2) {
    TLMErrorLog::FatalError("Too few arguments!");
    return -1;
  }

  std::string path = argv[1];
  std::string FMUPath = path+"/"+argv[2];
  std::string tmpPath = path+"/temp";
  std::string fmiConfigPath = path+"/"+FMI_CONFIG_FILE_NAME;
  std::string tlmConfigPath = path+"/"+TLM_CONFIG_FILE_NAME;

  if(argc > 3 && !strcmp(argv[3],"-d")) {
      TLMErrorLog::SetDebugOut(true);
  }
  TLMErrorLog::SetNormalErrorLogOn(true);
  TLMErrorLog::SetWarningOut(true);

  TLMErrorLog::Log("---Arguments---");
  TLMErrorLog::Log("FMU file: "+FMUPath+"");
  TLMErrorLog::Log("Temp path: "+tmpPath+"");
  TLMErrorLog::Log("Interfaces file: "+fmiConfigPath+"");
  TLMErrorLog::Log("TLM config file: "+tlmConfigPath+"");

  // Create and clear temporary directory
  createAndClearTempDirectory(tmpPath);

  // Read TLM configuration
  fmiConfig_t fmiConfig = readFmiConfigFile(fmiConfigPath);

  // Read FMI configuration
  tlmConfig_t tlmConfig = readTlmConfigFile(tlmConfigPath);

  // Instantiate each TLMPlugin
  for(size_t i=0; i<fmiConfig.nInterfaces; ++i)
  {
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

  // Register each TLMPlugin
  for(size_t i=0; i<fmiConfig.plugins.size(); ++i)
  {
    fmiConfig.interfaceIds[i] = fmiConfig.plugins[i]->RegisteTLMInterface(fmiConfig.interfaceNames[i]);
  }

  fmi2_callback_functions_t callBackFunctions;
  jm_callbacks callbacks;
  fmi_import_context_t* context;
  fmi_version_enu_t version;
  jm_status_enu_t status;
  fmi2_import_t* fmu;

  callbacks.malloc = malloc;
  callbacks.calloc = calloc;
  callbacks.realloc = realloc;
  callbacks.free = free;
  callbacks.logger = importlogger;
  callbacks.log_level = jm_log_level_warning;
  callbacks.context = 0;

  context = fmi_import_allocate_context(&callbacks);

  version = fmi_import_get_fmi_version(context, FMUPath.c_str(), tmpPath.c_str());

  if(version != fmi_version_2_0_enu) {
    TLMErrorLog::FatalError("The code only supports version 2.0");
    do_exit(-1);
  }

  fmu = fmi2_import_parse_xml(context, tmpPath.c_str(), 0);

  if(!fmu) {
    TLMErrorLog::FatalError("Error parsing XML, exiting");
    do_exit(-1);
  }

  if(fmi2_import_get_fmu_kind(fmu) == fmi2_fmu_kind_me) {
    TLMErrorLog::FatalError("Only CS 2.0 is supported by this code");
    do_exit(-1);
  }

  callBackFunctions.logger = fmi2_log_forwarding;
  callBackFunctions.allocateMemory = calloc;
  callBackFunctions.freeMemory = free;
  callBackFunctions.componentEnvironment = fmu;

  //Load the FMU shared library
  status = fmi2_import_create_dllfmu(fmu, fmi2_fmu_kind_cs, &callBackFunctions);
  if (status == jm_status_error) {
    TLMErrorLog::FatalError("Could not create the DLL loading mechanism(C-API). Error: "+string(fmi2_import_get_last_error(fmu)));
    do_exit(-1);
  }

  //Start simulation
  simulate(fmu, tlmConfig, fmiConfig);

  //Clean up
  fmi2_import_destroy_dllfmu(fmu);
  fmi2_import_free(fmu);
  fmi_import_free_context(context);

  TLMErrorLog::Log("FMIWrapper1 completed successfully!");

  do_exit(0);

  return 0;
}

