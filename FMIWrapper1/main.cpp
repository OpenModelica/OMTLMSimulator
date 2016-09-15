#include <iostream>
#include <string>
#include "config_test.h"
#include "fmilib.h"
#include <JM/jm_portability.h>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <vector>
#include <fstream>
#include "TLMPlugin.h"
#include "TLMErrorLog.h"
#include <sstream>

using namespace std;


static const char* TLM_CONFIG_FILE_NAME = "tlm.config";
//static const char* TLM_DEBUG_FILE_NAME = "tlmfmi.log";    //Should this be used? /robbr


//static bool debugFlg = false; //Should this variable be used? /robbr
std::ofstream debugOutFile;

//TLM-FMI interface data
static int NumInterfaces;
static std::vector<TLMPlugin*> Plugins;
static std::vector<std::string> InterfaceNames;
static std::vector<int> InterfaceIds;
static std::vector<fmi2_value_reference_t*> position_vr;
static std::vector<fmi2_value_reference_t*> orientation_vr;
static std::vector<fmi2_value_reference_t*> speed_vr;
static std::vector<fmi2_value_reference_t*> ang_speed_vr;
static std::vector<fmi2_value_reference_t*> force_vr;




void importlogger(jm_callbacks* c, jm_string module, jm_log_level_enu_t log_level, jm_string message) {
  printf("module = %s, log level = %s: %s\n", module, jm_log_level_to_string(log_level), message);
}

void do_exit(int code) {
  printf("Press 'Enter' to exit\n");
  /* getchar(); */
  exit(code);
}



int simulate_cs(fmi2_import_t* fmu)
{
  fmi2_status_t fmistatus;
  jm_status_enu_t jmstatus;

  fmi2_string_t instanceName = "Test CS model instance";
  fmi2_string_t fmuGUID;
  fmi2_string_t fmuLocation = "";
  fmi2_boolean_t visible = fmi2_false;
  fmi2_real_t relativeTol = 1e-4;
  /*	fmi2_boolean_t loggingOn = fmi2_true; */

  /* fmi2_real_t simulation_results[] = {-0.001878, -1.722275}; */
  fmi2_real_t simulation_results[] = {0.0143633,   -1.62417};
  fmi2_value_reference_t compare_real_variables_vr[] = {0, 1};
  //size_t k; //What does this do? (comes from FMILibrary test code) /robbr

  fmi2_real_t tstart = 0.0;
  fmi2_real_t tcur = tstart;
  fmi2_real_t hstep = 0.1;
  fmi2_real_t tend = 2.0;
  fmi2_boolean_t StopTimeDefined = fmi2_false;

  if (sizeof(compare_real_variables_vr)/sizeof(fmi2_value_reference_t) != sizeof(simulation_results)/sizeof(fmi2_real_t)) {
    printf("Number of simulation values and reference values are different\n");
    do_exit(CTEST_RETURN_FAIL);
  }

  printf("Version returned from FMU:   %s\n", fmi2_import_get_version(fmu));
  printf("Platform type returned:      %s\n", fmi2_import_get_types_platform(fmu));

  fmuGUID = fmi2_import_get_GUID(fmu);
  printf("GUID:      %s\n", fmuGUID);


  jmstatus = fmi2_import_instantiate(fmu, instanceName, fmi2_cosimulation, fmuLocation, visible);
  if (jmstatus == jm_status_error) {
    printf("fmi2_import_instantiate failed\n");
    do_exit(CTEST_RETURN_FAIL);
  }

  fmistatus = fmi2_import_setup_experiment(fmu, fmi2_true,
                                           relativeTol, tstart, StopTimeDefined, tend);
  if(fmistatus != fmi2_status_ok) {
    printf("fmi2_import_setup_experiment failed\n");
    do_exit(CTEST_RETURN_FAIL);
  }

  fmistatus = fmi2_import_enter_initialization_mode(fmu);
  if(fmistatus != fmi2_status_ok) {
    printf("fmi2_import_enter_initialization_mode failed\n");
    do_exit(CTEST_RETURN_FAIL);
  }

  fmistatus = fmi2_import_exit_initialization_mode(fmu);
  if(fmistatus != fmi2_status_ok) {
    printf("fmi2_import_exit_initialization_mode failed\n");
    do_exit(CTEST_RETURN_FAIL);
  }

  tcur = tstart;

  while (tcur < tend) {
    //fmi2_boolean_t newStep = fmi2_true;
    //fmistatus = fmi2_import_do_step(fmu, tcur, hstep, newStep);
    //fmi2_import_get_real(fmu, compare_real_variables_vr, 2, val);

    size_t nSubSteps=10;
    fmi2_real_t hsub = (tend-tcur)/nSubSteps;
    for(size_t i=0; i<nSubSteps; ++i) {
      for(size_t j=0; j<Plugins.size(); ++j)
      {
        double position[3],orientation[3],speed[3],ang_speed[3],force[6];

        //Read position and speed from FMU
        fmistatus = fmi2_import_get_real(fmu,position_vr[j],3,position);
        fmistatus = fmi2_import_get_real(fmu,orientation_vr[j],3,orientation);
        fmistatus = fmi2_import_get_real(fmu,speed_vr[j],3,speed);
        fmistatus = fmi2_import_get_real(fmu,ang_speed_vr[j],3,ang_speed);

        //Get interpolated force
        Plugins.at(j)->GetForce(InterfaceIds[j], tcur, position,orientation,speed,ang_speed,force);

        //Write force to FMU
        fmistatus = fmi2_import_set_real(fmu,force_vr[j],6,force);
      }

      //Take one sub step
      fmistatus = fmi2_import_do_step(fmu,tcur,hsub,fmi2_true);

      //Increment time
      tcur+=hsub;

      for(size_t j=0; j<Plugins.size(); ++j)
      {
        double position[3],orientation[3],speed[3],ang_speed[3];

        //Read position and speed from FMU
        fmistatus = fmi2_import_get_real(fmu,position_vr[j],3,position);
        fmistatus = fmi2_import_get_real(fmu,orientation_vr[j],3,orientation);
        fmistatus = fmi2_import_get_real(fmu,speed_vr[j],3,speed);
        fmistatus = fmi2_import_get_real(fmu,ang_speed_vr[j],3,ang_speed);

        //Write back motion for sub step
        Plugins.at(j)->SetMotion(InterfaceIds[j], tcur, position, orientation, speed, ang_speed);
      }
    }

    tcur += hstep;
  }

  printf("Simulation finished.");

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
void readInterfaceData()
{


  std::ifstream infile("fmi_interfaces.csv");
  if(infile.is_open())
  {
    for( std::string line; getline( infile, line ); )
    {
      //cout << "Line: " << line << "\n";
      std::stringstream ss(line);

      std::string word;
      getline(ss, word, ',');
      if(word == "name") {
        NumInterfaces++;
        InterfaceIds.resize(NumInterfaces);
        //osition_vr.resize(NumInterfaces);
        //orientation_vr.resize(NumInterfaces);
//        speed_vr.resize(NumInterfaces);
//        ang_speed_vr.resize(NumInterfaces);
//        force_vr.resize(NumInterfaces);
        getline(ss, word, ',');
        InterfaceNames.push_back(word);
      }
      else if(word == "position") {
        position_vr.push_back(new fmi2_value_reference_t);
        csvToIntArray(ss.str(),3,position_vr.back());
      }
      else if(word == "orientation") {
        orientation_vr.push_back(new fmi2_value_reference_t);
        csvToIntArray(ss.str(),3,orientation_vr.back());
      }
      else if(word == "speed") {
        speed_vr.push_back(new fmi2_value_reference_t);
        csvToIntArray(ss.str(),3,speed_vr.back());
      }
      else if(word == "ang_speed") {
        ang_speed_vr.push_back(new fmi2_value_reference_t);
        csvToIntArray(ss.str(),3,ang_speed_vr.back());
      }
      else if(word == "force") {
        force_vr.push_back(new fmi2_value_reference_t);
        csvToIntArray(ss.str(),6,force_vr.back());
      }
    }
  }
  else
  {
    std::cout << "Unable to read fmi_interfaces.csv.";
    exit(1);
  }
}






int main(int argc, char* argv[]) {

  readInterfaceData();

  // TODO: Read this data from an auxiliary file

  //InterfaceNames[0] = "tlm1";
  //InterfaceNames[1] = "tlm2";


  cout << "Number of interfaces: " << NumInterfaces << "\n";
  for(int i=0; i<NumInterfaces; ++i)
  {
    cout << "Name: " << InterfaceNames[i];
    cout << "\nPosition:";
    for(int j=0; j<3; ++j) {
      cout << " " << position_vr[i][j];
    }
    cout << "\nOrientation:";
    for(int j=0; j<3; ++j) {
      cout << " " << orientation_vr[i][j];
    }
    cout << "\nSpeed:";
    for(int j=0; j<3; ++j) {
      cout << " " << speed_vr[i][j];
    }
    cout << "\nAngular speed:";
    for(int j=0; j<3; ++j) {
      cout << " " << ang_speed_vr[i][j];
    }
    cout << "\nForce:";
    for(int j=0; j<6; ++j) {
      cout << " " << force_vr[i][j];
    }
    cout << "\n";

  }


  // Instantiate the plugins
  for(int i=0; i<NumInterfaces; ++i)
  {
    Plugins.push_back(TLMPlugin::CreateInstance());
  }

  // Read parameters from a file
  ifstream tlmConfigFile(TLM_CONFIG_FILE_NAME);

  std::string model;
  std::string serverName;

  double timeStart;
  double timeEnd;
  double maxStep;

  tlmConfigFile >> model;
  tlmConfigFile >> serverName;
  tlmConfigFile >> timeStart;
  tlmConfigFile >> timeEnd;
  tlmConfigFile >> maxStep;

  if(!tlmConfigFile.good()) {
    debugOutFile << "Error reading TLM configuration data from tlm.config" << endl;
    exit(1);
  }

  for(int i=0; i<NumInterfaces; ++i) {
    if(!Plugins[i]->Init(model,
                         timeStart,
                         timeEnd,
                         maxStep,
                         serverName)) {
      debugOutFile << "Error initializing the TLM plugin" << endl;
      exit(1);
    }
  }

  for(size_t i=0; i<Plugins.size(); ++i)
  {
    InterfaceIds[i] = Plugins[i]->RegisteTLMInterface(InterfaceNames[i]);
  }

  fmi2_callback_functions_t callBackFunctions;
  jm_callbacks callbacks;
  fmi_import_context_t* context;
  fmi_version_enu_t version;
  jm_status_enu_t status;
  //int k;  //What does this do? (from FMILibrary test code...) /robbr
  fmi2_import_t* fmu;

  if(argc < 2) {
    cout << "Too few arguments!\n";
    return -1;
  }

  std::string FMUPath = getcwd(NULL,0); //MEMORY LEAK
  std::string tmpPath = getcwd(NULL,0); //MEMORY LEAK
  FMUPath.append("/");
  FMUPath.append(argv[1]);
  tmpPath.append("/temp");

  cout << "FMU file: " << FMUPath << "\n";
  cout << "Temp path: " << tmpPath << "\n";


  callbacks.malloc = malloc;
  callbacks.calloc = calloc;
  callbacks.realloc = realloc;
  callbacks.free = free;
  callbacks.logger = importlogger;
  callbacks.log_level = jm_log_level_debug;
  callbacks.context = 0;

#ifdef FMILIB_GENERATE_BUILD_STAMP
  printf("Library build stamp:\n%s\n", fmilib_get_build_stamp());
#endif

  context = fmi_import_allocate_context(&callbacks);

  version = fmi_import_get_fmi_version(context, FMUPath.c_str(), tmpPath.c_str());

  if(version != fmi_version_2_0_enu) {
    printf("The code only supports version 2.0\n");
    do_exit(CTEST_RETURN_FAIL);
  }

  fmu = fmi2_import_parse_xml(context, tmpPath.c_str(), 0);

  if(!fmu) {
    printf("Error parsing XML, exiting\n");
    do_exit(CTEST_RETURN_FAIL);
  }

  if(fmi2_import_get_fmu_kind(fmu) == fmi2_fmu_kind_me) {
    printf("Only CS 2.0 is supported by this code\n");
    do_exit(CTEST_RETURN_FAIL);
  }

  callBackFunctions.logger = fmi2_log_forwarding;
  callBackFunctions.allocateMemory = calloc;
  callBackFunctions.freeMemory = free;
  callBackFunctions.componentEnvironment = fmu;

  status = fmi2_import_create_dllfmu(fmu, fmi2_fmu_kind_cs, &callBackFunctions);
  if (status == jm_status_error) {
    printf("Could not create the DLL loading mechanism(C-API) (error: %s).\n", fmi2_import_get_last_error(fmu));
    do_exit(CTEST_RETURN_FAIL);
  }

  simulate_cs(fmu);

  fmi2_import_destroy_dllfmu(fmu);

  fmi2_import_free(fmu);
  fmi_import_free_context(context);

  printf("Everything seems to be OK since you got this far=)!\n");

  do_exit(CTEST_RETURN_SUCCESS);

  return 0;


  return 0;
}

