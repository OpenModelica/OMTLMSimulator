#ifndef TLMPLUGININTERFACEMECHANICAL1D_HPP_INCLUDED
#define TLMPLUGININTERFACEMECHANICAL1D_HPP_INCLUDED

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

    class TLMPluginInterfaceMechanical1D : public ComponentC
    {
    private:
        //Constants
        bool mDebug;
        //Power port pointers
        Port *mpP1;
        //Power port node data pointers
        double *mpP1_x, *mpP1_v, *mpP1_F, *mpP1_me, *mpP1_c, *mpP1_Zc;
        std::ofstream mDebugOutFile;
        size_t mInterfaceId;
        TLMPlugin *mpPlugin;
        bool mInvert;

    public:
        static Component *Creator()
        {
            return new TLMPluginInterfaceMechanical1D();
        }

        void configure()
        {
            //Register constant parameters
            addConstant("Debug", "", "", false, mDebug);
            addConstant("Invert", "", "", false, mInvert);
            //Add power ports
            mpP1 = addPowerPort("P1", "NodeMechanic", "");
        }


        void initialize()
        {
            TLMErrorLog::Log("Initializing!");
            // Get node data pointers from ports
            mpP1_x = getSafeNodeDataPtr(mpP1, NodeMechanic::Position);
            mpP1_v = getSafeNodeDataPtr(mpP1, NodeMechanic::Velocity);
            mpP1_F = getSafeNodeDataPtr(mpP1, NodeMechanic::Force);
            mpP1_me = getSafeNodeDataPtr(mpP1, NodeMechanic::EquivalentMass);
            mpP1_c = getSafeNodeDataPtr(mpP1, NodeMechanic::WaveVariable);
            mpP1_Zc = getSafeNodeDataPtr(mpP1, NodeMechanic::CharImpedance);

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
            mInterfaceId = mpPlugin->RegisteTLMInterface(this->getName().c_str(),1,"Bidirectional","Mechanical");
            TLMErrorLog::Log("Finished initializing!");
        }


        void simulateOneTimestep()
        {
            if(mInvert)
            {
                TLMErrorLog::Log("Mechanical1D: Taking inverted step!");
            }
            else
            {
                TLMErrorLog::Log("Mechanical1D: Taking step!");
            }
            // Read input variables (position and speed only)
            double x,v,f;

            x = (*mpP1_x);  // Position
            v = (*mpP1_v);  // Speed

            if(mInvert)
            {
                x = -x;
                v = -v;
            }

            // Get force from TLM interface
            mpPlugin->GetForce1D(mInterfaceId,mTime,v,&f);

            if(mInvert)
            {
                f = -f;
            }

            // Write output variables
            (*mpP1_c) = -f;
            (*mpP1_Zc) = 0;     //Not needed, since already included in f

            // Set motion in TLM interface
            mpPlugin->SetMotion1D(mInterfaceId,mTime,x,v);

            TLMErrorLog::Log("Mechanical1D: Took step!");
        }
    };
}

#endif //TLMPLUGININTERFACEMECHANICAL1D_HPP_INCLUDED


