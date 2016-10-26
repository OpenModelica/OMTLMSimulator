//
// File: ManagerMain.cc
// 
// TLM manager main program file.
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <map>
#include <sstream>
#include "TLMErrorLog.h"
#include "MetaModel.h"
#include "MetaModelReader.h"
#include "ManagerCommHandler.h"
#include "MonitoringPluginImplementer.h"
#include "double3Vec.h"
#include "double33Mat.h"
#include "timing.h"

#ifdef _MSC_VER
#include "mygetopt.h"
#else
#include <getopt.h>
#endif

using std::string;
using namespace tlmMisc;

void usage(){
    string usageStr = "Usage: tlmmonitor [-d] [-n num-seps | -t time-step-size] <server:port> <metamodel>, where metamodel is an XML file.";
    TLMErrorLog::SetDebugOut(true);
    TLMErrorLog::Log(usageStr);
    std::cout << usageStr << std::endl;
    exit(1);
}

TLMPlugin* initializeTLMConnection(MetaModel& model, std::string& serverName)
{
    TLMPlugin* TLMlink = MonitoringPluginImplementer::CreateInstance();

    TLMErrorLog::Log("Trying to register TLM monitor on host " + serverName);

    if(! TLMlink->Init("monitor",
                       model.GetSimParams().GetStartTime(),
                       model.GetSimParams().GetEndTime(),
                       model.GetSimParams().GetWriteTimeStep(),
                       serverName) )
    {
        TLMErrorLog::FatalError("Cannot initialize MonitoringPluginImplementer.");
        return 0;
    }

    int nTLMInterfaces = model.GetInterfacesNum();
    for( int i=0 ; i<nTLMInterfaces ; i++ ){
        TLMInterfaceProxy& interfaceProxy = model.GetTLMInterfaceProxy(i);
        TLMComponentProxy& component = model.GetTLMComponentProxy(interfaceProxy.GetComponentID());

        TLMErrorLog::Log( "Trying to register monitoring interface " + interfaceProxy.GetName() );
        int TLMInterfaceID = TLMlink->RegisteTLMInterface( component.GetName() + "." + interfaceProxy.GetName(), interfaceProxy.GetType() );

        if(TLMInterfaceID >= 0) {
            TLMErrorLog::Log("Registration was successful");
        }
        else{
            TLMErrorLog::Log("Interface not connected in Meta-Model: " + component.GetName() + "." + interfaceProxy.GetName() );
        }
    }

    return TLMlink;
}

//! Evaluate the data needed for the current time step.
void MonitorTimeStep(TLMPlugin* TLMlink,
                     MetaModel& model,
                     double SimTime,
                     std::map<int, TLMTimeDataSignal>& dataStorageSignal,
                     std::map<int, TLMTimeData1D>& dataStorage1D,
                     std::map<int, TLMTimeData3D>& dataStorage3D) {
    if( TLMlink != 0 ){
        // Get data from TLM-Manager here!
        int nTLMInterfaces = model.GetInterfacesNum();

        for( int i=0 ; i<nTLMInterfaces ; i++ ){
            TLMInterfaceProxy& interfaceProxy = model.GetTLMInterfaceProxy(i);
            int interfaceID = interfaceProxy.GetID();
            int connectionID = interfaceProxy.GetConnectionID();
            std::string type = interfaceProxy.GetType();

            TLMErrorLog::Log("Data request for " + interfaceProxy.GetName() + " for time " + ToStr(SimTime) + ", id: " + ToStr(interfaceID));

            if( connectionID >= 0 ){
#define LOGGEDFORCEFIX
#ifdef  LOGGEDFORCEFIX
              if(type == "3D") {
                  TLMTimeData3D& PrevTimeData = dataStorage3D[interfaceID];
                  TLMTimeData3D& CurTimeData = dataStorage3D[interfaceID];

                  TLMlink->GetTimeData3D(interfaceID, SimTime, CurTimeData);

                  double delay = model.GetTLMConnection(interfaceProxy.GetConnectionID()).GetParams().Delay;
                  double alpha = model.GetTLMConnection(interfaceProxy.GetConnectionID()).GetParams().alpha;
                  TLMlink->GetTimeData3D(interfaceID, SimTime-delay, PrevTimeData);

                  //Apply damping factor, since this can not be done in GetTimeData (DampedTimeData is not available for monitor)
                  for(int i = 0; i < 6; i++) {
                    CurTimeData.GenForce[i] =
                        CurTimeData.GenForce[i] * (1 - alpha)
                        + PrevTimeData.GenForce[i] * alpha;
                  }
              }
              else if(type == "1D"){
                TLMTimeData1D& PrevTimeData = dataStorage1D[interfaceID];
                TLMTimeData1D& CurTimeData = dataStorage1D[interfaceID];

                TLMlink->GetTimeData1D(interfaceID, SimTime, CurTimeData);

                double delay = model.GetTLMConnection(interfaceProxy.GetConnectionID()).GetParams().Delay;
                double alpha = model.GetTLMConnection(interfaceProxy.GetConnectionID()).GetParams().alpha;
                TLMlink->GetTimeData1D(interfaceID, SimTime-delay, PrevTimeData);

                //Apply damping factor, since this can not be done in GetTimeData (DampedTimeData is not available for monitor)
                CurTimeData.GenForce = CurTimeData.GenForce*(1-alpha) + PrevTimeData.GenForce*alpha;
              }
              else if(type == "SignalOutput"){
                TLMTimeDataSignal& CurTimeData = dataStorageSignal[interfaceID];
                TLMlink->GetTimeDataSignal(interfaceID, SimTime, CurTimeData);
              }
              //No need to check for erroneous interface type, we can simply log nothing instead /robbr
#else
                TLMTimeData& CurTimeData = dataStorage[interfaceID];
                TLMlink->GetTimeData(interfaceID, SimTime, CurTimeData);
#endif
            }
        }
    }
}

void printHeader(MetaModel& model, std::ofstream& dataFile)
{
    // Get data from TLM-Manager here!
    int nTLMInterfaces = model.GetInterfacesNum();

    // First variable written is time.
    dataFile << "\"" << "time\",";

    int nActiveInterfaces = 0;
    for( int i=0 ; i<nTLMInterfaces ; i++ ){
        TLMInterfaceProxy& interfaceProxy = model.GetTLMInterfaceProxy(i);
        TLMComponentProxy& component = model.GetTLMComponentProxy(interfaceProxy.GetComponentID());
        if( interfaceProxy.GetConnectionID() >= 0 ){
            if(interfaceProxy.GetType() == "3D") {
                // Comma between interfaces
                if(nActiveInterfaces > 0) dataFile << ",";

                // Add all TLM variable names for all active interfaces
                std::string name = component.GetName() + "." + interfaceProxy.GetName();
                dataFile << "\"" << name << ".R[cG][cG](1)\",\"" << name << ".R[cG][cG](2)\",\"" << name << ".R[cG][cG](3)\","; // Position vector
                dataFile << "\"" << name << ".phi[cG](1)\",\"" << name << ".phi[cG](2)\",\"" << name << ".phi[cG](3)\","; // Orientation vector (three angles)
                dataFile << "\"" << name << ".vR[cG][cG,cG](1)\",\"" << name << ".vR[cG][cG,cG](2)\",\"" << name << ".vR[cG][cG,cG](3)\","; // velocity
                dataFile << "\"" << name << ".Omega[cG][cG](1)\",\"" << name << ".Omega[cG][cG](2)\",\"" << name << ".Omega[cG][cG](3)\","; // angular velocity
                dataFile << "\"" << name << ".F_tie[cG](1)\",\"" << name << ".F_tie[cG](2)\",\"" << name << ".F_tie[cG](3)\","; // force vector
                dataFile << "\"" << name << ".M_tie[cG][cG](1)\",\"" << name << ".M_tie[cG][cG](2)\",\"" << name << ".M_tie[cG][cG](3)\""; // torque vector

                nActiveInterfaces++;
            }
            else if(interfaceProxy.GetType() == "1D") {
              // Comma between interfaces
              if(nActiveInterfaces > 0) dataFile << ",";

              // Add all TLM variable names for all active interfaces
              std::string name = component.GetName() + "." + interfaceProxy.GetName();
              dataFile << "\"" << name << ".x\","; // Position
              dataFile << "\"" << name << ".v\","; // Speed
              dataFile << "\"" << name << ".F\""; // Force

              nActiveInterfaces++;
            }
            else if(interfaceProxy.GetType() == "SignalOutput") {
              // Comma between interfaces
              if(nActiveInterfaces > 0) dataFile << ",";

              // Add variable names for all active interfaces
              std::string name = component.GetName() + "." + interfaceProxy.GetName();
              dataFile << "\"" << name << "\""; // Value

              nActiveInterfaces++;
            }
        }
    }

    dataFile << std::endl;
}

void printData(MetaModel& model,
               std::ofstream& dataFile,
               std::map<int, TLMTimeDataSignal> &dataStorageSignal,
               std::map<int, TLMTimeData1D>& dataStorage1D,
               std::map<int, TLMTimeData3D> &dataStorage3D)
{
    // Get data from TLM-Manager here!
    int nTLMInterfaces = model.GetInterfacesNum();

    bool printTimeFlg = true;
    int nActiveInterfaces = 0;

    for( int i=0 ; i<nTLMInterfaces ; i++ ){
        TLMInterfaceProxy& interfaceProxy = model.GetTLMInterfaceProxy(i);
        if( interfaceProxy.GetConnectionID() >= 0 ){
            if(interfaceProxy.GetType() == "3D") {
                TLMTimeData3D& timeData = dataStorage3D.at(interfaceProxy.GetID());

                // Print time only once, that is, for the first entry.
                if( printTimeFlg ){
                    dataFile << timeData.time << ",";
                    printTimeFlg = false;
                }

                // Comma between interfaces
                if(nActiveInterfaces > 0) dataFile << ",";

                // Convert orientation matrix to angles

                // first convert the matrices into double33 format
                double33Mat A(timeData.RotMatrix[0], timeData.RotMatrix[1], timeData.RotMatrix[2],
                        timeData.RotMatrix[3], timeData.RotMatrix[4], timeData.RotMatrix[5],
                        timeData.RotMatrix[6], timeData.RotMatrix[7], timeData.RotMatrix[8]);

                // Then convert to angles
                double3Vec phi = ATophi321(A);

                // Backward calculation of force from TLM wave.
                // This is done because the actual force send is the delayed force.
                // The wave is: C = - Force + Impedance * Velocity -> F = -(C - Imp*Vel)
                double3Vec force(0.0);
                double3Vec torque(0.0);
                TLMConnection& connection = model.GetTLMConnection(interfaceProxy.GetConnectionID());
                for(int i = 0; i < 3; i++) {
    #if 1
                    force(i+1) =  -timeData.GenForce[i] + connection.GetParams().Zf * timeData.Velocity[i];
                    torque(i+1) = -timeData.GenForce[i+3] + connection.GetParams().Zfr * timeData.Velocity[i+3];
    #else
                    force(i+1) =  timeData.GenForce[i];
                    torque(i+1) = timeData.GenForce[i+3];
    #endif
                }

                dataFile << timeData.Position[0] << "," << timeData.Position[1] << "," << timeData.Position[2] << ",";
                dataFile << phi(1)               << "," << phi(2)               << "," << phi(3)               << ",";
                dataFile << timeData.Velocity[0] << "," << timeData.Velocity[1] << "," << timeData.Velocity[2] << ",";
                dataFile << timeData.Velocity[3] << "," << timeData.Velocity[4] << "," << timeData.Velocity[5] << ",";
                dataFile << force(1)             << "," << force(2)             << "," << force(3)             << ",";
                dataFile << torque(1)            << "," << torque(2)            << "," << torque(3);

                nActiveInterfaces++;
            }
            else if(interfaceProxy.GetType() == "1D") {
              TLMTimeData1D& timeData = dataStorage1D.at(interfaceProxy.GetID());

              // Print time only once, that is, for the first entry.
              if( printTimeFlg ){
                  dataFile << timeData.time << ",";
                  printTimeFlg = false;
              }

              // Comma between interfaces
              if(nActiveInterfaces > 0) dataFile << ",";

              // Backward calculation of force from TLM wave.
              // This is done because the actual force send is the delayed force.
              // The wave is: C = - Force + Impedance * Velocity -> F = -(C - Imp*Vel)

              TLMConnection& connection = model.GetTLMConnection(interfaceProxy.GetConnectionID());

              double force =  -timeData.GenForce + connection.GetParams().Zf * timeData.Velocity;

              dataFile << timeData.Position << ",";
              dataFile << timeData.Velocity << ",";
              dataFile << force;

              nActiveInterfaces++;
            }
            else if(interfaceProxy.GetType() == "SignalOutput") {
              TLMTimeDataSignal& timeData = dataStorageSignal.at(interfaceProxy.GetID());

              // Print time only once, that is, for the first entry.
              if( printTimeFlg ){
                  dataFile << timeData.time << ",";
                  printTimeFlg = false;
              }

              // Comma between interfaces
              if(nActiveInterfaces > 0) dataFile << ",";

              dataFile << timeData.Value;

              nActiveInterfaces++;
            }
        }
    }
    dataFile << std::endl;
}

void printRunStatus(MetaModel& model, std::ofstream& runFile, tTM_Info& tInfo, double SimTime)
{
    double startTime = model.GetSimParams().GetStartTime();
    double endTime = model.GetSimParams().GetEndTime();
    double timeStep = model.GetSimParams().GetWriteTimeStep();
    int numSteps = (endTime-startTime)/timeStep;
    int curStep = (SimTime-startTime)/timeStep;
    double progress = ((curStep*1.0)/(numSteps*1.0))*100.0;
    std::string statusStr = (curStep == numSteps ? "Done" : "Running");

    // Calculate average wall clock time for a single logging time step.
    timespec_t avg;
    TM_Avg(&tInfo, &avg);

    double avgStepTime = avg.tv_sec + (avg.tv_nsec/1.0e9);
    double timeLeft = static_cast<double>(numSteps-curStep)*avgStepTime;
    int hLeft = timeLeft/3600;
    int mLeft = (timeLeft - static_cast<double>(hLeft))/60;
    int sLeft = (timeLeft - static_cast<double>(hLeft) - static_cast<double>(mLeft));

    // Always write from beginning of file, that is, overwrite old data.
    runFile.seekp(0);
    runFile << "Status    : " << statusStr << std::endl;
    runFile << "Sim. time : " << SimTime   << std::endl;
    runFile << "Step      : " << curStep   << " of " << numSteps << std::endl;
    runFile << "Progress  : " << progress   << "%" << std::endl;
    runFile << "            " << std::endl;
    runFile << "Estimated time left: " << hLeft << ":" << mLeft << ":" << sLeft << std::endl;
    //runFile << "average time per step: " << avgStepTime << " sec." << endl;
    //runFile << "time left            : " << timeLeft << " sec." << endl;
    runFile << "                                                              " << std::endl;
}

int main(int argc, char* argv[]) {

  TLMErrorLog::Log("Starting monitor...");

#ifndef USE_THREADS
#warning TLM manager requires pthreads to be compiled in. Use -DUSE_THREADS in the Makefile.head if neeeded.    
	TLMErrorLog::Error("tlmmanger was compiled without threads and is not usable.");
	exit(1);    
#endif

    bool debugFlg = false;
    double timeStep = 0.0;
    double nSteps = 0;
    char c;
    while ((c = getopt (argc, argv, "dt:n:")) != -1){
        switch (c) {
        case 'd':
            debugFlg = true;
            break;
        case 't':
            timeStep = atof(optarg);
            break;
        case 'n':
            nSteps = atoi(optarg);
            break;
        default:
            usage();
            break;
        }
    }

    // We exspect two arguments
    // tlmmonitor server-ip:port metamodel.xml
    if( optind+1 >= argc ){
        usage();
    }

    // Enable debug?
    std::ofstream logfile;
    logfile.open("monitor.log");
    TLMErrorLog::SetOutStream(logfile);
    if(debugFlg) {
        TLMErrorLog::SetDebugOut(true);
    }

    // Get input strings, server name and meta-model XML file.
    std::string serverStr(argv[optind]);
    std::string inFile(argv[optind+1]);
    std::string baseFileName = inFile.substr(0, inFile.rfind('.'));

    // Create the meta model object
    MetaModel theModel;

    {
        // Create model reader for the model
        MetaModelReader modelReader(theModel);

        // read the XML file and build the model
        modelReader.ReadModel(inFile);
    }
    
    // Open file for data logging, that is, storing the co-simulation data.
    std::ofstream outdataFile((baseFileName + ".csv").c_str());
    if( !outdataFile.good() ){
        TLMErrorLog::FatalError("Failed to open outfile " + baseFileName + ".csv, give up.");
        exit(1);
    }

    // Open run file for logging of simulation progress.
    std::ofstream runFile((baseFileName + ".run").c_str());
    if( !runFile.good() ){
        TLMErrorLog::FatalError("Failed to open runfile " + baseFileName + ".run, give up.");
        exit(1);
    }

    // Initialize TLM
    TLMPlugin* thePlugin = initializeTLMConnection(theModel, serverStr);
    if( !thePlugin ){
        TLMErrorLog::FatalError("Failed to initialize TLM interface, give up.");
        exit(1);
    }

    // Setup simulation time for logging.
    double simTime = theModel.GetSimParams().GetStartTime();
    double endTime  = theModel.GetSimParams().GetEndTime();

    // Setup time step for output logging according to priority:
    // 1. User input specified in -t option
    // 2. User input specified in -s option
    // 3. Time step from MetaModel
    if( timeStep == 0.0 ) {
        if( nSteps > 0 ){
            timeStep = (endTime-simTime)/static_cast<double>(nSteps);
        }
        else {
            timeStep = theModel.GetSimParams().GetWriteTimeStep();
        }
    }

    // Print/log the header information
    printHeader(theModel, outdataFile);

    // Setup timer for run-time estimation.
    tTM_Info tInfo;
    TM_Init(&tInfo);
    TM_Clear(&tInfo);

    do {
        // Next time step (yes I know, we miss the first step)
        simTime += timeStep;

        // Adjust to meet end-time step.
        if( simTime > endTime ) simTime = endTime;

        // Data structure for data logging
        std::map<int, TLMTimeDataSignal> dataSignal;
        std::map<int, TLMTimeData1D> data1D;
        std::map<int, TLMTimeData3D> data3D;

        // Get data for next time step.
        TM_Start(&tInfo);
        MonitorTimeStep(thePlugin, theModel, simTime, dataSignal, data1D, data3D);
        TM_Stop(&tInfo);

        // Print data row
        printData(theModel, outdataFile, dataSignal, data1D, data3D);

        // Update run status
        printRunStatus(theModel, runFile, tInfo, simTime);

    } while(simTime < endTime);

    return 0;
}


