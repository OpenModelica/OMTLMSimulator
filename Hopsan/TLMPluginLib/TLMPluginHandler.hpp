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

#ifndef TLMPLUGINHANDLER_HPP_INCLUDED
#define TLMPLUGINHANDLER_HPP_INCLUDED

#include "common.h"
#include "ComponentEssentials.h"
#include "ComponentUtilities.h"
#include "ComponentSystem.h"

// TLMPlugin includes
#include "TLMPlugin.h"

namespace hopsan {

    class TLMPluginHandler : public ComponentC
    {
    private:
        //Constants
        bool mDebug;

        std::ofstream mDebugOutFile;

        TLMPlugin *mpPlugin;

    public:
        static Component *Creator()
        {
            return new TLMPluginHandler();
        }

        void configure()
        {
            //Register constant parameters
            addConstant("Debug", "", "", false, mDebug);
        }


        bool preInitialize()
        {
            // Read from TLM configuration file
            tlmConfig_t tlmConfig = readTlmConfigFile(findFilePath(HString(TLM_CONFIG_FILE_NAME)).c_str());


            // Instnatiate TLM Plugin
            mpPlugin = TLMPlugin::CreateInstance();

            // Initialize TLM Plugin
            mpPlugin->Init(tlmConfig.model,
                           tlmConfig.tstart,
                           tlmConfig.tend,
                           tlmConfig.hmax,
                           tlmConfig.server);

            // Open debug file if debug is enabled
            if(mDebug && !mDebugOutFile.is_open() ){
                mDebugOutFile.open(TLM_DEBUG_FILE_NAME);

                if( mDebugOutFile.good()){
                    TLMErrorLog::SetOutStream(mDebugOutFile);
                }

                TLMErrorLog::SetDebugOut(true);
            }

            return (mpPlugin != 0);
        }

        void initialize()
        {
         }


        void simulateOneTimestep() {}


        void finalize()
        {
            //delete mpPlugin;
        }

        TLMPlugin *getPlugin()
        {
            return mpPlugin;
        }
    };
}

#endif //TLMPLUGININTERFACE_HPP_INCLUDED


