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

#include "common.h"
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

namespace hopsan {

    class TLMPluginInterface1D : public ComponentC
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
            return new TLMPluginInterface1D();
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
            mInterfaceId = mpPlugin->RegisteTLMInterface(this->getName().c_str(), "1D");

//            TLMTimeData1D data;
//            mpPlugin->GetTimeData1D(mInterfaceId,mTime,data);
//            (*mpP1_x) = data.Position;
//            (*mpP1_v) = data.Velocity;
//            TLMErrorLog::Log("Starting position: "+TLMErrorLog::ToStdStr(*mpP1_x));
        }


        void simulateOneTimestep()
        {
            // Read input variables (position and speed only)
            double x,v,f;

            x = (*mpP1_x);  // Position
            v = (*mpP1_v);  // Speed

            // Get force from TLM interface
            mpPlugin->GetForce1D(mInterfaceId,mTime,x,v,&f);

            // Write output variables
            (*mpP1_c) = -f;
            (*mpP1_Zc) = 0;     //Not needed, since already included in f

            // Set motion in TLM interface
            mpPlugin->SetMotion1D(mInterfaceId,mTime,x,v);
        }


        void finalize()
        {
            delete mpPlugin;
        }
    };
}

#endif //TLMPLUGININTERFACE_HPP_INCLUDED


