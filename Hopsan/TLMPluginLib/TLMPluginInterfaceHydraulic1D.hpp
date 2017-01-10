#ifndef TLMPLUGININTERFACEHYDRAULIC1D_HPP_INCLUDED
#define TLMPLUGININTERFACEHYDRAULIC1D_HPP_INCLUDED

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

    class TLMPluginInterfaceHydraulic1D : public ComponentC
    {
    private:
        //Constants
        bool mDebug;
        //Power port pointers
        Port *mpP1;
        //Power port node data pointers
        double *mpP1_x, *mpP1_q, *mpP1_p, *mpP1_me, *mpP1_c, *mpP1_Zc;
        std::ofstream mDebugOutFile;
        size_t mInterfaceId;
        TLMPlugin *mpPlugin;

    public:
        static Component *Creator()
        {
            return new TLMPluginInterfaceHydraulic1D();
        }

        void configure()
        {
            //Register constant parameters
            addConstant("Debug", "", "", false, mDebug);

            //Add power ports
            mpP1 = addPowerPort("P1", "NodeHydraulic", "");
        }


        void initialize()
        {
            // Get node data pointers from ports
            mpP1_q = getSafeNodeDataPtr(mpP1, NodeHydraulic::Flow);
            mpP1_p = getSafeNodeDataPtr(mpP1, NodeHydraulic::Pressure);
            mpP1_c = getSafeNodeDataPtr(mpP1, NodeHydraulic::WaveVariable);
            mpP1_Zc = getSafeNodeDataPtr(mpP1, NodeHydraulic::CharImpedance);

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
            mInterfaceId = mpPlugin->RegisteTLMInterface(this->getName().c_str(),1,"Bidirectional","Hydraulic");
        }


        void simulateOneTimestep()
        {
            TLMErrorLog::Log("Hydraulic1D: Taking step!");
            // Read input variables (position and speed only)
            double x,q,p;

            q = (*mpP1_q);  // Speed

            // Get force from TLM interface
            mpPlugin->GetForce1D(mInterfaceId,mTime,q,&p);

            // Write output variables
            (*mpP1_c) = -p;
            (*mpP1_Zc) = 0;     //Not needed, since already included in f

            // Set motion in TLM interface
            x = 0;  //Dummy variable (no position in hydraulics)
            mpPlugin->SetMotion1D(mInterfaceId,mTime,x,q);
            TLMErrorLog::Log("Hydraulic1D: Took step!");
        }
    };
}

#endif //TLMPLUGININTERFACEHYDRAULIC1D_HPP_INCLUDED


