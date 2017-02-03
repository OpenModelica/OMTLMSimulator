#ifndef TLMPLUGININTERFACESIGNALINPUT_HPP_INCLUDED
#define TLMPLUGININTERFACESIGNALINPUT_HPP_INCLUDED

#include "common.h"
#include "ComponentEssentials.h"
#include "ComponentUtilities.h"
#include "ComponentSystem.h"
#include "TLMPluginHandler.hpp"

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

    class TLMPluginInterfaceSignalInput : public ComponentC
    {
    private:
        //Constants
        bool mDebug;

        //Power port node data pointers
        double *mpP1_x;
        TLMPlugin *mpPlugin;
        std::ofstream mDebugOutFile;
        size_t mInterfaceId;

    public:
        static Component *Creator()
        {
            return new TLMPluginInterfaceSignalInput();
        }

        void configure()
        {
            //Register constant parameters
            addConstant("Debug", "", "", false, mDebug);

            //Add power ports
            addOutputVariable("out", "Output", "", 0, &mpP1_x);
        }


        void initialize()
        {
            bool foundHandler = false;
            for(size_t i=0; i<mpSystemParent->getSubComponents().size(); ++i)
            {
                if(mpSystemParent->getSubComponents()[i]->getTypeName() == "TLMPluginHandler")
                {
                    mpPlugin = dynamic_cast<TLMPluginHandler*>(mpSystemParent->getSubComponents()[i])->getPlugin();
                    foundHandler = true;
                }
            }

            if(!foundHandler)
            {
                this->stopSimulation("No TLMHandler component found!");
                return;
            }

            // Register TLM Interface
            mInterfaceId = mpPlugin->RegisteTLMInterface(this->getName().c_str(), 1, "Input", "Signal");
        }


        void simulateOneTimestep()
        {
            // Get force from TLM interface
            mpPlugin->GetValueSignal(mInterfaceId,mTime,mpP1_x);
        }


        void finalize()
        {
            //delete mpPlugin;
        }
    };
}

#endif //TLMPluginInterfaceSignalInput_HPP_INCLUDED


