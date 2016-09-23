/*-----------------------------------------------------------------------------
 This source file is a part of Hopsan

 Copyright (c) 2009 to present year, Hopsan Group

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

 For license details and information about the Hopsan Group see the files
 GPLv3 and HOPSANGROUP in the Hopsan source code root directory

 For author and contributor information see the AUTHORS file
-----------------------------------------------------------------------------*/

#ifndef TLMPLUGININTERFACE_HPP_INCLUDED
#define TLMPLUGININTERFACE_HPP_INCLUDED

static const char* TLM_CONFIG_FILE_NAME = "tlm.config";
static const char* TLM_DEBUG_FILE_NAME = "tlmmodelica.log";

#include "ComponentEssentials.h"
#include "ComponentUtilities.h"
#include "ComponentSystem.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <vector>
#include <fstream>

// TLMPlugin includes
#include "TLMPlugin.h"
#include "TLMErrorLog.h"

// TLM config data
struct tlmConfig_t {
    std::string model;
    std::string server;
    double tstart;
    double tend;
    double hmax;
};


tlmConfig_t readTlmConfigFile(std::string path)
{
    tlmConfig_t tlmConfig;
    std::ifstream tlmConfigFile(path.c_str());

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
    TLMErrorLog::Log("---"+std::string(TLM_CONFIG_FILE_NAME)+"---");
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


namespace hopsan {

    class TLMPluginInterface : public ComponentC
    {
    private:
        //Constants
        bool mDebug;
        //Power port pointers
        Port *mpP1;
        //Power port node data pointers
        double *mpP1_x, *mpP1_v, *mpP1_F, *mpP1_me, *mpP1_c, *mpP1_Zc;
        TLMPlugin* mpPlugin;
        std::ofstream mDebugOutFile;
        size_t mInterfaceId;

    public:
        static Component *Creator()
        {
            return new TLMPluginInterface();
        }

        void configure()
        {
            //Register constant parameters
            addConstant("Debug", "", "", false, mDebug);

            //Add power ports
            mpP1 = addPowerPort("P1", "NodeMechanic", "");
        }


        void initialize()
        {
            // Get node data pointers from ports
            mpP1_x = getSafeNodeDataPtr(mpP1, NodeMechanic::Position);
            mpP1_v = getSafeNodeDataPtr(mpP1, NodeMechanic::Velocity);
            mpP1_F = getSafeNodeDataPtr(mpP1, NodeMechanic::Force);
            mpP1_me = getSafeNodeDataPtr(mpP1, NodeMechanic::EquivalentMass);
            mpP1_c = getSafeNodeDataPtr(mpP1, NodeMechanic::WaveVariable);
            mpP1_Zc = getSafeNodeDataPtr(mpP1, NodeMechanic::CharImpedance);

            // Read from TLM configuration file
            tlmConfig_t tlmConfig = readTlmConfigFile(findFilePath(HString(TLM_CONFIG_FILE_NAME)).c_str());

            // Open debug file if debug is enabled
            if(mDebug && !mDebugOutFile.is_open() ){
                mDebugOutFile.open(TLM_DEBUG_FILE_NAME);

                if( mDebugOutFile.good()){
                    TLMErrorLog::SetOutStream(mDebugOutFile);
                }

                TLMErrorLog::SetDebugOut(true);
            }

            // Instnatiate TLM Plugin
            mpPlugin = TLMPlugin::CreateInstance();

            // Initialize TLM Plugin
            mpPlugin->Init(tlmConfig.model,
                           tlmConfig.tstart,
                           tlmConfig.tend,
                           tlmConfig.hmax,
                           tlmConfig.server);

            // Register TLM Interface
            mInterfaceId = mpPlugin->RegisteTLMInterface(this->getName().c_str());
        }


        void simulateOneTimestep()
        {
            // Read input variables (position and speed only)
            double x[3], T[9], v[3],w[3], f[6];

            // Position
            x[0] = (*mpP1_x);   x[1] = 0;       x[2] = 0;

            // Orientation
            T[0] = 1;           T[1] = 0;       T[2] = 0;
            T[3] = 0;           T[4] = 1;       T[5] = 0;
            T[6] = 0;           T[7] = 0;       T[8] = 1;

            // Speed
            v[0] = (*mpP1_v);   v[1] = 0;       v[2] = 0;

            // Angular speed
            w[0] = 0;           w[1] = 0;       w[2] = 0;

            f[0] = 0;           f[1] = 0;       f[2] = 0;
            f[3] = 0;           f[4] = 0;       f[5] = 0;

            // Get force from TLM interface
            mpPlugin->GetForce(mInterfaceId,mTime,x,T,v,w,f);

            // Write output variables
            (*mpP1_c) = -f[0];
            (*mpP1_Zc) = 0;     //Not needed, since already included in f

            // Set motion in TLM interface
            mpPlugin->SetMotion(mInterfaceId,mTime,x,T,v,w);
        }


        void finalize()
        {
            delete mpPlugin;
        }
    };
}

#endif //TLMPLUGININTERFACE_HPP_INCLUDED


